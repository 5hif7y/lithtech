#!/usr/bin/env python3
"""Embed tri.vert.spv + tri.frag.spv as uint32 arrays in tri_spv.h."""
import sys


def emit(path, name):
    with open(path, 'rb') as f:
        data = f.read()
    assert len(data) % 4 == 0, path
    words = [str(int.from_bytes(data[i:i + 4], 'little'))
             for i in range(0, len(data), 4)]
    return 'static const uint32_t %s[]={%s};\n' % (name, ','.join(words))


name1 = sys.argv[4] if len(sys.argv) > 4 else 'kTriVertSpv'
name2 = sys.argv[5] if len(sys.argv) > 5 else 'kTriFragSpv'
with open(sys.argv[3], 'w') as h:
    h.write('#pragma once\n#include <stdint.h>\n')
    h.write(emit(sys.argv[1], name1))
    h.write(emit(sys.argv[2], name2))
