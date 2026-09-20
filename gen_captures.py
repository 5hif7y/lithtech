#!/usr/bin/env python3
"""Genera las capturas de verificacion en ./temp-captures/.

Uso: python gen_captures.py [--exe RUTA]
Corre los probes + el juego y convierte PPM->PNG en temp-captures/.
"""
import os
import subprocess
import sys

ROOT = os.path.dirname(os.path.abspath(__file__))
EXE = (sys.argv[sys.argv.index('--exe') + 1]
       if '--exe' in sys.argv else os.path.join(
           ROOT, 'build-msvc', 'Samples', 'Samples', 'networking',
           'sealhunter', 'RelWithDebInfo', 'networking_sealhunter.exe'))
OUT = os.path.join(ROOT, 'temp-captures')
os.makedirs(OUT, exist_ok=True)

try:
    from PIL import Image
except ImportError:
    print('falta Pillow: pip install pillow')
    sys.exit(2)

JOBS = [
    (['--mesh-test', '--ppm=%s' % os.path.join(OUT, 'r1-mesh.ppm')],
     'r1-mesh'),
    (['--world-test', '--ppm=%s' % os.path.join(OUT, 'r2-world.ppm')],
     'r2-world'),
    (['--model-test', '--ppm=%s' % os.path.join(OUT, 'r3-models.ppm')],
     'r3-models'),
    (['--vulkan', '--frames=120',
       '--ppm=%s' % os.path.join(OUT, 'r4-gameplay.ppm')],
     'r4-gameplay'),
]

ok = True
for args, name in JOBS:
    ppm = os.path.join(OUT, name + '.ppm')
    png = os.path.join(OUT, name + '.png')
    log = os.path.join(OUT, name + '.log')
    print('== %s ==' % name)
    with open(log, 'w') as lf:
        rc = subprocess.call([EXE] + args, stdout=lf, stderr=subprocess.STDOUT,
                             cwd=ROOT)
    tail = open(log).read().strip().splitlines()[-3:]
    for line in tail:
        print('   ', line.strip()[:110])
    if os.path.exists(ppm):
        Image.open(ppm).save(png)
        print('    -> %s (rc=%d)' % (png, rc))
    else:
        print('    !! sin ppm (rc=%d), ver %s' % (rc, log))
        ok = False
print('OK' if ok else 'FALLO parcial (ver logs en temp-captures/)')
sys.exit(0 if ok else 1)
