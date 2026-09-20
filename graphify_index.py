#!/usr/bin/env python3
"""Indexacion y regeneracion DETERMINISTA del grafo graphify.

Sin LLMs, sin subagentes, sin tokens: todo es AST + reglas fijas.
Uso:
    python graphify_index.py --update      # incremental (default)
    python graphify_index.py --full        # rebuild completo
    python graphify_index.py --update --no-html
Los docs se extraen con reglas (headers/links/menciones), no con IA:
menos rico que la extraccion semantica, pero gratis y reproducible.
"""
import glob
import json
import os
import re
import subprocess
import sys
from collections import Counter
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path('.').resolve()
OUT = ROOT / 'graphify-out'

# ---------------------------------------------------------------- util
def load(p):
    return json.loads((OUT / p).read_text(encoding='utf-8'))


def save(p, obj):
    (OUT / p).write_text(json.dumps(obj, ensure_ascii=False), encoding='utf-8')


def norm(s):
    return re.sub(r'[^a-z0-9]+', '_', s.lower()).strip('_')


def stem_of(abs_path):
    try:
        rel = str(Path(abs_path).resolve().relative_to(ROOT))
    except ValueError:
        rel = os.path.basename(abs_path)
    rel = rel.replace('\\', '/')
    if '.' in rel.rsplit('/', 1)[-1]:
        rel = rel.rsplit('.', 1)[0]
    return '_'.join(norm(seg) for seg in rel.split('/'))


# ------------------------------------------------- extraccion determinista
def extract_docs(files, corpus_paths):
    """Docs -> nodos de concepto (headers) + aristas a ficheros citados."""
    nodes, edges = [], []
    seen = set()

    def add_node(nid, label, ftype, src):
        if nid in seen:
            return
        seen.add(nid)
        nodes.append({'id': nid, 'label': label[:120], 'file_type': ftype,
                      'source_file': src, 'source_location': None,
                      'source_url': None, 'captured_at': None,
                      'author': None, 'contributor': None})

    def add_edge(a, b, rel, src, conf='EXTRACTED', score=1.0):
        edges.append({'source': a, 'target': b, 'relation': rel,
                      'confidence': conf, 'confidence_score': score,
                      'source_file': src, 'source_location': None,
                      'weight': 1.0})

    for f in files:
        stem = stem_of(f)
        add_node(stem + '_file', os.path.basename(f), 'document', f)
        try:
            text = Path(f).read_text(encoding='utf-8', errors='replace')
        except OSError:
            continue
        for m in re.finditer(r'^(#{1,3})\s+(.+)$', text, re.M):
            ent = norm(m.group(2))[:60] or 'section'
            nid = stem + '_' + ent
            add_node(nid, m.group(2).strip()[:120], 'concept', f)
            add_edge(stem + '_file', nid, 'references', f)
        for m in re.finditer(r'\[(?:[^\]]*)\]\(([^)]+)\)', text):
            add_edge(stem + '_file', stem + '_link', 'references', f)
            break
        for m in re.finditer(r'`([^`\n]{3,80})`', text):
            cand = m.group(1).strip().replace('\\', '/')
            hit = next((c for c in corpus_paths
                        if c.lower().endswith(cand.lower())
                        or cand.lower() in c.lower()), None)
            if hit:
                add_edge(stem + '_file', stem_of(hit) + '_file',
                         'references', f)
                break
    return {'nodes': nodes, 'edges': edges, 'hyperedges': [],
            'input_tokens': 0, 'output_tokens': 0}


def extract_images(files):
    nodes = []
    for f in files:
        stem = stem_of(f)
        nodes.append({'id': stem + '_image',
                      'label': os.path.basename(f),
                      'file_type': 'image', 'source_file': f,
                      'source_location': None, 'source_url': None,
                      'captured_at': None, 'author': None,
                      'contributor': None})
    return {'nodes': nodes, 'edges': [], 'hyperedges': [],
            'input_tokens': 0, 'output_tokens': 0}


# ---------------------------------------------------------------- pasos
def step_detect(update):
    from graphify.detect import detect, detect_incremental
    if update:
        result = detect_incremental(ROOT)
        if result.get('new_total', 0) == 0 and \
                not result.get('deleted_files'):
            print('Sin cambios. Nada que actualizar.')
            return None
        save('.graphify_incremental.json', result)
        save('.graphify_detect.json', {
            'files': result.get('new_files', {}),
            'all_files': result.get('files', {}),
            'total_files': result.get('new_total', 0),
            'total_words': result.get('total_words', 0),
            'skipped_sensitive': result.get('skipped_sensitive', []),
            'needs_graph': True})
    else:
        result = detect(ROOT)
        save('.graphify_detect.json', result)
    n = (result.get('new_total', 0) if update
         else result.get('total_files', 0))
    print('detect: %d archivos' % n)
    return result


def step_ast():
    from graphify.extract import collect_files, extract
    detect = load('.graphify_detect.json')
    files = []
    for f in detect.get('files', {}).get('code', []):
        files.extend(collect_files(Path(f)) if Path(f).is_dir()
                     else [Path(f)])
    print('AST: %d ficheros' % len(files))
    if not files:
        res = {'nodes': [], 'edges': [], 'hyperedges': [],
               'input_tokens': 0, 'output_tokens': 0}
    else:
        res = extract(files, cache_root=ROOT)
    save('.graphify_ast.json', res)
    print('AST: %d nodos, %d aristas' % (len(res['nodes']),
                                         len(res['edges'])))


def step_semantic():
    detect = load('.graphify_detect.json')
    corpus = {f for fl in detect.get('all_files', detect.get('files', {}))
              .values() for f in fl}
    docs = [f for cat in ('document', 'paper')
            for f in detect.get('files', {}).get(cat, [])]
    imgs = list(detect.get('files', {}).get('image', []))
    print('docs: %d, imagenes: %d' % (len(docs), len(imgs)))
    d = extract_docs(docs, corpus)
    i = extract_images(imgs)
    merged = {'nodes': d['nodes'] + i['nodes'],
              'edges': d['edges'] + i['edges'], 'hyperedges': [],
              'input_tokens': 0, 'output_tokens': 0}
    save('.graphify_semantic.json', merged)
    print('semantico: %d nodos, %d aristas' % (len(merged['nodes']),
                                               len(merged['edges'])))


def step_merge(update):
    from graphify.build import build_from_json, build_merge
    ast = load('.graphify_ast.json')
    sem = load('.graphify_semantic.json')
    seen = {n['id'] for n in ast['nodes']}
    nodes = list(ast['nodes'])
    for n in sem['nodes']:
        if n['id'] not in seen:
            nodes.append(n)
            seen.add(n['id'])
    fresh = {'nodes': nodes, 'edges': ast['edges'] + sem['edges'],
             'hyperedges': sem.get('hyperedges', []),
             'input_tokens': 0, 'output_tokens': 0}
    if update:
        inc = load('.graphify_incremental.json')
        deleted = list(inc.get('deleted_files', []))
        G = build_merge([fresh], graph_path=str(OUT / 'graph.json'),
                        prune_sources=deleted or None, root=str(ROOT),
                        directed=False)
        merged = {
            'nodes': [{'id': n, **d} for n, d in G.nodes(data=True)],
            'edges': [{**{k: v for k, v in d.items() if k not in (
                '_src', '_tgt', 'source', 'target')},
                'source': d.get('_src', u), 'target': d.get('_tgt', v)}
                for u, v, d in G.edges(data=True)],
            'hyperedges': list(G.graph.get('hyperedges', [])),
            'input_tokens': 0, 'output_tokens': 0}
        print('merge: %d nodos (reemplazados los re-extraidos)' %
              len(merged['nodes']))
    else:
        merged = fresh
    save('.graphify_extract.json', merged)


def step_build():
    from graphify.build import build_from_json
    from graphify.cluster import cluster, score_all
    from graphify.analyze import (god_nodes, surprising_connections,
                                  suggest_questions)
    from graphify.report import generate
    from graphify.export import to_json
    ext = load('.graphify_extract.json')
    det = load('.graphify_detect.json')
    G = build_from_json(ext, root=str(ROOT), directed=False)
    if G.number_of_nodes() == 0:
        print('ERROR: grafo vacio')
        return None
    communities = cluster(G)
    cohesion = score_all(G, communities)
    # labels: heredar curadas por overlap, resto por directorio
    old_labels = {}
    if (OUT / '.graphify_labels.json').exists():
        old_labels = json.loads((OUT / '.graphify_labels.json')
                                .read_text(encoding='utf-8'))
    old_map = {}
    for backup in (OUT / '.graphify_old.json', OUT / 'graph.json'):
        if backup.exists():
            try:
                gd = json.loads(backup.read_text(encoding='utf-8'))
                nl = gd['nodes'] if isinstance(gd, dict) else gd
                for n in nl:
                    c = n.get('community')
                    if c is not None:
                        old_map[n.get('id')] = str(c)
                break
            except Exception:
                continue
    n2src = {n['id']: (n.get('source_file', '') or '')
             for n in ext['nodes']}
    labels, n_carry, n_dir = {}, 0, 0
    for cid, members in communities.items():
        votes = Counter(old_map[m] for m in members if m in old_map)
        done = False
        if votes:
            top, cnt = votes.most_common(1)[0]
            if top in old_labels and cnt >= max(2, len(members) // 2):
                labels[cid] = old_labels[top]
                n_carry += 1
                done = True
        if not done:
            dirs = Counter(
                (n2src.get(m, '').replace('\\', '/').rsplit('/', 1)[0]
                 if '/' in n2src.get(m, '') else '(root)') for m in members)
            top = dirs.most_common(1)[0][0]
            name = top.split('/')[-1].replace('_', ' ').title()
            labels[cid] = name or ('Community ' + str(cid))
            n_dir += 1
    print('labels: %d heredadas + %d por directorio' % (n_carry, n_dir))
    questions = suggest_questions(G, communities, labels)
    wrote = to_json(G, communities, str(OUT / 'graph.json'),
                    community_labels=labels)
    if not wrote:
        print('ERROR: shrink-guard (el grafo nuevo es menor)')
        return None
    report = generate(G, communities, cohesion, labels,
                      god_nodes(G), surprising_connections(G),
                      det, {'input': 0, 'output': 0}, str(ROOT),
                      suggested_questions=questions)
    (OUT / 'GRAPH_REPORT.md').write_text(report, encoding='utf-8')
    (OUT / '.graphify_labels.json').write_text(
        json.dumps({str(k): v for k, v in labels.items()},
                   ensure_ascii=False), encoding='utf-8')
    (OUT / '.graphify_analysis.json').write_text(json.dumps(
        {'communities': {str(k): v for k, v in communities.items()},
         'cohesion': {str(k): v for k, v in cohesion.items()}},
        ensure_ascii=False), encoding='utf-8')
    print('grafo: %d nodos, %d aristas, %d comunidades' % (
        G.number_of_nodes(), G.number_of_edges(), len(communities)))
    return True


def step_diag():
    from graphify.diagnostics import diagnose_extraction
    from graphify.diagnostics import format_diagnostic_report
    ext = load('.graphify_extract.json')
    s = diagnose_extraction(ext, directed=False, root=str(ROOT))
    print(format_diagnostic_report(s))
    bad = [k for k in ('dangling_endpoint_edges',
                       'missing_endpoint_edges', 'self_loop_edges') if s.get(k)]
    print('HEALTH: ' + ('; '.join('%s=%s' % (k, s[k]) for k in bad)
                        if bad else 'OK'))


def step_html():
    r = subprocess.call([sys.executable, '-m', 'graphify', 'export',
                         'html'], cwd=str(ROOT))
    print('html rc=%d' % r)
    return r == 0


def step_cost(update):
    from graphify.detect import save_manifest
    from graphify.cli import _stamped_manifest_files
    det = load('.graphify_detect.json')
    ext = load('.graphify_extract.json')
    if update:
        inc = load('.graphify_incremental.json')
        mf = _stamped_manifest_files(inc['files'], ext, ROOT)
        sem = ('document', 'paper', 'image')
        disp = {f for t, fl in inc.get('new_files', {}).items()
                if t in sem for f in fl}
        stamped = {f for fl in mf.values() for f in fl}
        cleared = disp - stamped
        scan = {f for fl in inc['files'].values() for f in fl}
        save_manifest(mf, root=str(ROOT), scan_corpus=scan,
                      clear_semantic=cleared or None)
    else:
        corpus = det.get('all_files') or det['files']
        mf = _stamped_manifest_files(corpus, ext, ROOT)
        scan = {f for fl in corpus.values() for f in fl}
        save_manifest(mf, root=str(ROOT), scan_corpus=scan)
    cp = OUT / 'cost.json'
    cost = json.loads(cp.read_text(encoding='utf-8')) if cp.exists() else {
        'runs': [], 'total_input_tokens': 0, 'total_output_tokens': 0}
    cost['runs'].append({'date': datetime.now(timezone.utc).isoformat(),
                         'input_tokens': 0, 'output_tokens': 0,
                         'files': det.get('total_files', 0)})
    cp.write_text(json.dumps(cost, indent=2, ensure_ascii=False),
                  encoding='utf-8')
    print('runs totales:', len(cost['runs']))


def cleanup():
    for p in ('.graphify_detect.json', '.graphify_ast.json',
              '.graphify_semantic.json', '.graphify_analysis.json',
              '.graphify_incremental.json', '.graphify_old.json'):
        try:
            (OUT / p).unlink()
        except OSError:
            pass
    for c in glob.glob(str(OUT / '.graphify_chunk_*.json')):
        try:
            os.remove(c)
        except OSError:
            pass
    print('limpieza ok')


def main(argv):
    update = '--full' not in argv
    no_html = '--no-html' in argv
    print('== graphify_index.py (%s) ==' % ('update' if update else 'full'))
    OUT.mkdir(exist_ok=True)
    if update and (OUT / 'graph.json').exists():
        import shutil
        shutil.copy(OUT / 'graph.json', OUT / '.graphify_old.json')
    det = step_detect(update)
    if det is None:
        return 0
    step_ast()
    step_semantic()
    step_merge(update)
    step_diag()
    if step_build() is None:
        return 1
    if not no_html:
        step_html()
    step_cost(update)
    cleanup()
    print('GRAPH OK en graphify-out/')
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
