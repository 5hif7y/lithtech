#!/usr/bin/env python3
"""Generate null-object subclass overrides for LithTech abstract interfaces.
Usage: gen_host.py IFaceName header1.h header2.h ... > overrides.inc
Parses `virtual ... = 0;` declarations (multiline ok) and emits bodies with
neutral default returns. Compiler-verified afterwards; hand-tune as needed.
"""
import re, sys

def strip_comments(s):
    s = re.sub(r'/\*.*?\*/', ' ', s, flags=re.S)
    s = re.sub(r'//[^\n]*', ' ', s)
    return s

def split_params(p):
    depth = 0
    cur = ''
    for ch in p:
        if ch in '(<': depth += 1
        elif ch in ')>': depth -= 1
        if ch == ',' and depth == 0:
            yield cur; cur = ''
        else:
            cur += ch
    yield cur

def strip_default(param):
    depth = 0
    for i, ch in enumerate(param):
        if ch in '(<[{': depth += 1
        elif ch in ')>]}': depth -= 1
        elif ch == '=' and depth == 0:
            return param[:i].rstrip()
    return param

def default_body(ret):
    r = ret.strip()
    if r == 'void':
        return '{}'
    if r.endswith('&'):
        return '{ static std::remove_reference_t<%s> d{}; return d; }' % r[:-1].strip()
    if r in ('LTRESULT',):
        return '{ return LT_OK; }'
    if r in ('bool',):
        return '{ return false; }'
    if r in ('LTBOOL',):
        return '{ return LTFALSE; }'
    if r in ('float', 'LTFLOAT'):
        return '{ return 0.0f; }'
    if r in ('double',):
        return '{ return 0.0; }'
    if r in ('char', 'wchar_t', 'int8', 'uint8'):
        return '{ return 0; }'
    if 'char*' in r or 'wchar_t*' in r:
        return '{ return ""; }' if 'const' in r else '{ return nullptr; }'
    if '*' in r:
        return '{ return nullptr; }'
    return '{ return %s(); }' % r

def class_bodies(text):
    # yield (classname, body) for top-level class/struct definitions
    out = []
    for m in re.finditer(r'(?:class|struct)\s+(\w+)', text):
        name = m.group(1)
        i = text.find('{', m.end())
        if i < 0:
            continue
        # skip forward declarations (a ';' before the brace)
        if text.find(';', m.end(), i) >= 0:
            continue
        depth = 0
        j = i
        while j < len(text):
            if text[j] == '{':
                depth += 1
            elif text[j] == '}':
                depth -= 1
                if depth == 0:
                    break
            j += 1
        out.append((name, text[i:j]))
    return out

def main():
    iface = sys.argv[1]
    decls = []
    for spec in sys.argv[2:]:
        if ':' in spec:
            path, want = spec.split(':', 1)
            want = set(want.split(','))
        else:
            path, want = spec, None
        with open(path, 'rb') as f:
            text = f.read().decode('utf-8', 'replace')
        text = strip_comments(text)
        chunks = []
        for name, body in class_bodies(text):
            if want is None or name in want:
                chunks.append(body)
        text = ' '.join(chunks)
        text = re.sub(r'\s+', ' ', text)
        for m in re.finditer(r'virtual\s+([^{}]*?)\s*;', text):
            d = m.group(1).strip()
            # pure specifier '= 0' must sit at paren depth 0 (not a default arg)
            depth = 0
            pure = False
            for i, ch in enumerate(d):
                if ch == '(':
                    depth += 1
                elif ch == ')':
                    depth -= 1
                elif ch == '=' and depth == 0 and d[i+1:].strip() == '0':
                    d = d[:i].strip()
                    break
            if '(' not in d:
                continue
            # split name/params, keep trailing const
            pre, post = d.split('(', 1)
            pre = pre.strip()
            # find matching close paren
            depth = 1
            idx = 0
            for i, ch in enumerate(post):
                if ch == '(': depth += 1
                elif ch == ')':
                    depth -= 1
                    if depth == 0:
                        idx = i
                        break
            params = post[:idx]
            trail = post[idx+1:].strip()
            const = ' const' if trail.startswith('const') else ''
            parts = pre.rsplit(None, 1)
            if len(parts) != 2:
                continue
            ret, name = parts
            while name and name[0] in '*&':
                ret = ret + name[0]
                name = name[1:]
            if 'operator' in name or name.startswith('~'):
                continue
            pnames = []
            for p in split_params(params):
                p = strip_default(p.strip())
                if p in ('', 'void'):
                    continue
                pnames.append(p)
            sig = '%s %s(%s)%s' % (ret, name, ', '.join(pnames), const)
            decls.append((name, ret, sig))
    seen = set()
    print('// Auto-generated null overrides for %s' % iface)
    for name, ret, sig in decls:
        if (name, sig) in seen:
            continue
        seen.add((name, sig))
        print('%s override %s' % (sig, default_body(ret)))


def table_funcs(body, prefix):
    # find Ret (*Name)(params); fields at any depth (no braces inside decl)
    out = []
    for m in re.finditer(r'([A-Za-z_][\w:<>\s\*,&]*?)\(\*(\w+)\)\s*\(([^;{}]*?)\)\s*;', body):
        ret, name, params = m.group(1).strip(), m.group(2), m.group(3).strip()
        if ret.startswith('typedef') or 'return' in ret:
            continue
        # normalize params: drop default values
        ps = []
        for prm in split_params(params):
            prm = strip_default(prm.strip())
            if prm == '':
                continue
            ps.append(prm)
        pstr = ', '.join(ps) if ps else 'void'
        out.append((ret, name, pstr))
    return out

def table_main():
    # usage: gen_host.py --table PREFIX ClassName file[:Cls,Cls]...
    prefix = sys.argv[2]
    clsname = sys.argv[3]
    fields = []
    seen = set()
    for spec in sys.argv[4:]:
        if ':' in spec:
            path, want = spec.split(':', 1)
            want = set(want.split(','))
        else:
            path, want = spec, None
        with open(path, 'rb') as f:
            text = f.read().decode('utf-8', 'replace')
        text = strip_comments(text)
        for name, body in class_bodies(text):
            if want is None or name in want:
                for ret, fname, pstr in table_funcs(body, prefix):
                    if fname in seen:
                        continue
                    seen.add(fname)
                    fields.append((ret, fname, pstr))
    print('// Auto-generated table fill for %s' % clsname)
    for ret, fname, pstr in fields:
        is_void = (ret.strip() == 'void')
        # varargs: keep ... but ignore
        print('static %s %s_%s(%s) %s' % (ret, prefix, fname, pstr, '{}' if is_void else default_body(ret)))
    print('inline void %s_Fill(%s* o) {' % (prefix, clsname))
    for ret, fname, pstr in fields:
        print('    o->%s = &%s_%s;' % (fname, prefix, fname))
    print('}')

if sys.argv[1] == '--table':
    table_main()
    sys.exit(0)

if __name__ == '__main__':
    main()
