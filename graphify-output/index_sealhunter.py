#!/usr/bin/env python3
"""Index demo-sealhunter into graphify-output/nolf2-index.json (same schema).
Schema: nodes {id,path,ext,size,lines,windows_tokens}, edges {from,to,include}.
windows_tokens = Windows-only API markers (port-priority signal, as in base index).
Merges with new ids; backs up the original index first.
"""
import json, os, re, sys

ROOT = 'demo-sealhunter/sealhunter'
INDEX = 'graphify-output/nolf2-index.json'
SRC_EXTS = {'.h', '.hpp', '.c', '.cpp', '.cxx', '.rc', '.lta', '.sln',
            '.vcproj', '.cfg', '.bat', '.txt', '.ico'}
SKIP_DIRS = {'.git'}
SKIP_FILES = {'.zip', '.exe', '.dll', '.rez', '.dat', '.dtx', '.ltb', '.ltc',
              '.lta', '.raw', '.ttf', '.cur'}

WIN_PAT = re.compile(
    r'HWND|HINSTANCE|HMODULE|WNDPROC|FARPROC|LRESULT|WPARAM|LPARAM|DWORD|WORD0|'
    r'\bBYTE\b|\bBOOL\b|HANDLE|HICON|HCURSOR|CRITICAL_SECTION|GUID|HRESULT|'
    r'__declspec|__stdcall|__fastcall|__asm\b|_MSC_VER|windows\.h|'
    r'#pragma\s+comment|DirectX|d3d8|d3d9|D3DX|IDirect3D|MAKEINTRESOURCE|'
    r'LoadLibrary|GetProcAddress|RegOpenKey|Ws2_32|winsock|MFC|afxwin|'
    r'VirtualAlloc|CreateThread|WaitForSingleObject|EnterCriticalSection|'
    r'GetSystemMetrics|SystemParametersInfo|ClipCursor|ShowWindow|'
    r'SetWindowLong|GetWindowLong|CallWindowProc|DefWindowProc|'
    r'RegisterClass|CreateWindow|strcmpi\b|stricmp\b|_stricmp|strupr\b|'
    r'strlwr\b|_snprintf|_vsnprintf|_strdup|_fdopen|_O_BINARY|\\\\ServerApp\\\\',
)
INCL_PAT = re.compile(r'#include\s+"([^"]+)"')


def main():
    with open(INDEX, encoding='utf-8') as f:
        idx = json.load(f)
    nodes = idx['nodes']
    edges = idx['edges']
    all_files = idx['all_files']
    have = set(all_files)
    by_base = {}
    for n in nodes:
        by_base.setdefault(os.path.basename(n['path']).lower(), []).append(n['id'])
    nid = max(n['id'] for n in nodes) + 1

    files = []
    for dp, dns, fns in os.walk(ROOT):
        dns[:] = [x for x in dns if x not in SKIP_DIRS]
        for fn in sorted(fns):
            p = os.path.join(dp, fn)
            _, ext = os.path.splitext(fn)
            if ext.lower() in SKIP_FILES:
                continue
            if ext.lower() not in SRC_EXTS:
                continue
            files.append(p)

    new_nodes = []
    for p in files:
        if p in have:
            continue
        try:
            with open(p, 'rb') as f:
                raw = f.read()
            text = raw.decode('latin1')
        except OSError:
            continue
        lines = text.count('\n') + 1
        wt = len(WIN_PAT.findall(text))
        node = {'id': nid, 'path': p, 'ext': os.path.splitext(p)[1].lower(),
                'size': len(raw), 'lines': lines, 'windows_tokens': wt}
        nodes.append(node)
        all_files.append(p)
        by_base.setdefault(os.path.basename(p).lower(), []).append(nid)
        new_nodes.append((nid, text))
        nid += 1

    new_edges = 0
    for nid_, text in new_nodes:
        for inc in INCL_PAT.findall(text):
            base = os.path.basename(inc).lower()
            for tgt in by_base.get(base, []):
                if tgt != nid_:
                    edges.append({'from': nid_, 'to': tgt, 'include': inc})
                    new_edges += 1

    idx.setdefault('stats', {})['sealhunter_nodes'] = len(new_nodes)
    idx['stats']['sealhunter_edges'] = new_edges
    with open(INDEX + '.bak', 'w', encoding='utf-8') as f:
        pass
    import shutil
    shutil.copy(INDEX, INDEX + '.bak')
    with open(INDEX, 'w', encoding='utf-8') as f:
        json.dump(idx, f)
    top = sorted([n for n in nodes if n['path'].startswith(ROOT)],
                 key=lambda n: -n['windows_tokens'])[:10]
    print('indexed files: %d  edges: %d' % (len(new_nodes), new_edges))
    print('top windows_tokens:')
    for n in top:
        print('  %d %s' % (n['windows_tokens'], n['path']))


main()
