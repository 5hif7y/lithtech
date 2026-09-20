// R3: lector minimo de .ltb (bind pose) + skinning CPU + feed al mesh pipeline.
// Espeja el writer (tools/Model_Packer/lta2ltb_d3d.cpp) y los readers
// (runtime/model/src/model_load.cpp, render_a/.../d3dmeshrendobj_{rigid,skel}.cpp).
// Solo LOD0, solo bind pose (sin anims): suficiente para focas, jugador,
// snowman y mazo. Header-only como host_world.h.
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace Host {
struct ModelVert {
    float x, y, z, u, v;
};
struct RawVert {
    float x, y, z, u, v;
    float b[3];
    uint8_t bi[4];
    bool hasIdx = false;
};
struct BoneSet {
    uint16_t first = 0, count = 0;
    uint8_t bones[4] = {0xFF, 0xFF, 0xFF, 0xFF};
};
struct ModelMesh {
    std::vector<ModelVert> verts;
    std::vector<uint32_t> idx;
    int texSlot = 0;
    std::string piece;
    // Datos crudos para skinning diferido (los nodos se parsean despues).
    std::vector<RawVert> raw;
    std::vector<BoneSet> sets;
    uint32_t effector = 0;
    bool isSkel = false, isMP = false, haveEffector = false;
};
struct ModelNode {
    std::string name;
    uint16_t idx = 0;
    float G[16]; // global bind (columna-mayor)
};
struct ModelFile {
    std::vector<ModelNode> nodes; // por idx de carga (root primero)
    std::vector<ModelMesh> meshes;
    std::vector<std::string> childFiles;
    std::vector<std::string> skins; // lo rellena el caller (slot -> .dtx)
    float visRadius = 0;
};

struct ModelReader {
    const uint8_t* d = nullptr;
    size_t n = 0, p = 0;
    bool ok = true;
    ModelReader() {}
    ModelReader(const uint8_t* dd, size_t nn) : d(dd), n(nn) {}
    bool need(size_t k) {
        if (!ok || p + k > n) {
            ok = false;
            return false;
        }
        return true;
    }
    uint8_t u8() {
        if (!need(1)) return 0;
        return d[p++];
    }
    uint16_t u16() {
        if (!need(2)) return 0;
        uint16_t v = (uint16_t)(d[p] | (d[p + 1] << 8));
        p += 2;
        return v;
    }
    uint32_t u32() {
        if (!need(4)) return 0;
        uint32_t v = (uint32_t)d[p] | ((uint32_t)d[p + 1] << 8) |
                     ((uint32_t)d[p + 2] << 16) | ((uint32_t)d[p + 3] << 24);
        p += 4;
        return v;
    }
    int32_t i32() { return (int32_t)u32(); }
    float f32() {
        uint32_t v = u32();
        float f = 0;
        memcpy(&f, &v, 4);
        return f;
    }
    void skip(size_t k) { need(k) ? (void)(p += k) : (void)0; }
    std::string str() {
        std::string s;
        const uint16_t len = u16();
        if (!ok) return s;
        if (!need(len)) return s;
        s.assign((const char*)(d + p), len);
        p += len;
        return s;
    }
};

// blendKind: 0=none, 1..3=nonindexed B1..B3, 4..6=indexed B1..B3.
inline bool modelVertLayout(int blendKind, uint32_t flags, int& nBlends,
                            bool& hasIdx, int& uvFloats, size_t& vsize) {
    nBlends = 0; hasIdx = false; uvFloats = 0;
    if (blendKind >= 1 && blendKind <= 3)
        nBlends = blendKind;
    else if (blendKind >= 4 && blendKind <= 6) {
        nBlends = blendKind - 3;
        hasIdx = true;
    } else if (blendKind != 0)
        return false;
    if (!(flags & 0x1)) return false; // POSITION requerido
    vsize = 12 + (size_t)nBlends * 4 + (hasIdx ? 4 : 0);
    if (flags & 0x2) vsize += 12;    // NORMAL
    if (flags & 0x4) vsize += 4;     // DIFFUSE
    if (flags & 0x8) vsize += 4;     // PSIZE
    if (flags & 0x10) uvFloats = 2;
    else if (flags & 0x20) uvFloats = 4;
    else if (flags & 0x40) uvFloats = 6;
    else if (flags & 0x80) uvFloats = 8;
    vsize += (size_t)uvFloats * 4;
    if (flags & 0x100) vsize += 36; // BASISVECTORS
    if (flags & ~0x1FF) return false;
    return true;
}

inline bool parseModelStream(ModelReader& r, int blendKind, uint32_t flags,
                             uint32_t nverts, std::vector<RawVert>& out) {
    int nBlends = 0, uvFloats = 0;
    bool hasIdx = false;
    size_t vsize = 0;
    if (!modelVertLayout(blendKind, flags, nBlends, hasIdx, uvFloats, vsize))
        return false;
    if (nverts > 1000000) {
        r.ok = false;
        return false;
    }
    out.reserve(out.size() + nverts);
    const bool hasNorm = (flags & 0x2) != 0;
    const bool hasDiff = (flags & 0x4) != 0;
    const bool hasPsize = (flags & 0x8) != 0;
    for (uint32_t i = 0; i < nverts; i++) {
        if (!r.need(vsize)) return false;
        RawVert v;
        v.x = r.f32(); v.y = r.f32(); v.z = r.f32();
        v.b[0] = v.b[1] = v.b[2] = 0;
        for (int k = 0; k < nBlends; k++) v.b[k] = r.f32();
        v.hasIdx = hasIdx;
        v.bi[0] = v.bi[1] = v.bi[2] = v.bi[3] = 0xFF;
        if (hasIdx) {
            v.bi[0] = r.u8(); v.bi[1] = r.u8();
            v.bi[2] = r.u8(); v.bi[3] = r.u8();
        }
        if (hasNorm) r.skip(12);
        if (hasDiff) r.skip(4);
        if (hasPsize) r.skip(4);
        v.u = 0; v.v = 0;
        if (uvFloats >= 2) {
            v.u = r.f32(); v.v = r.f32();
        }
        if (uvFloats > 2) r.skip((size_t)(uvFloats - 2) * 4);
        if (flags & 0x100) r.skip(36);
        if (!r.ok) return false;
        out.push_back(v);
    }
    return true;
}

inline void skinVert(const RawVert& sv, const BoneSet* set,
                     const uint8_t* idxBones, const ModelFile& m, float o[3]) {
    // Recolecta (nodo, peso); b4 implicito = 1 - suma.
    int nodes[4] = {-1, -1, -1, -1};
    float w[4] = {sv.b[0], sv.b[1], sv.b[2], 0};
    float sum = w[0] + w[1] + w[2];
    w[3] = 1.0f - sum;
    if (set) {
        for (int k = 0; k < 4; k++)
            nodes[k] = set->bones[k] == 0xFF ? -1 : (int)set->bones[k];
    } else if (idxBones) {
        for (int k = 0; k < 4; k++)
            nodes[k] = idxBones[k] == 0xFF ? -1 : (int)idxBones[k];
    } else if (sv.hasIdx) {
        for (int k = 0; k < 4; k++)
            nodes[k] = sv.bi[k] == 0xFF ? -1 : (int)sv.bi[k];
    }
    float wsum = 0;
    for (int k = 0; k < 4; k++)
        if (nodes[k] >= 0 && w[k] > 0) wsum += w[k];
    if (wsum <= 0 || (size_t)0 == m.nodes.size()) {
        o[0] = sv.x; o[1] = sv.y; o[2] = sv.z;
        return;
    }
    float x = 0, y = 0, z = 0;
    for (int k = 0; k < 4; k++) {
        if (nodes[k] < 0 || w[k] <= 0) continue;
        size_t ni = (size_t)nodes[k];
        if (ni >= m.nodes.size()) continue;
        const float* G = m.nodes[ni].G;
        const float ww = w[k] / wsum;
        // G columna-mayor: mundo = G * pos.
        x += ww * (G[0] * sv.x + G[4] * sv.y + G[8] * sv.z + G[12]);
        y += ww * (G[1] * sv.x + G[5] * sv.y + G[9] * sv.z + G[13]);
        z += ww * (G[2] * sv.x + G[6] * sv.y + G[10] * sv.z + G[14]);
    }
    o[0] = x; o[1] = y; o[2] = z;
}

inline bool parseModelNode(ModelReader& r, ModelFile& m, int depth) {
    if (depth > 64) {
        r.ok = false;
        return false;
    }
    ModelNode nd;
    nd.name = r.str();
    nd.idx = r.u16();
    (void)r.u8(); // flags
    if (!r.ok) return false;
    float mat[16];
    for (int i = 0; i < 16; i++) mat[i] = r.f32();
    if (!r.ok) return false;
    // LTMatrix fila-mayor m[r][c] -> columna-mayor G[c*4+r].
    for (int rr = 0; rr < 4; rr++)
        for (int cc = 0; cc < 4; cc++) nd.G[cc * 4 + rr] = mat[rr * 4 + cc];
    if (nd.idx >= 4096) {
        r.ok = false;
        return false;
    }
    if (nd.idx >= m.nodes.size()) m.nodes.resize((size_t)nd.idx + 1);
    m.nodes[(size_t)nd.idx] = nd;
    const uint32_t nc = r.u32();
    if (!r.ok || nc > 4096) {
        r.ok = false;
        return false;
    }
    for (uint32_t i = 0; i < nc; i++)
        if (!parseModelNode(r, m, depth + 1)) return false;
    return true;
}

inline bool parseModelMesh(ModelReader& r, int type, ModelFile& m,
                           ModelMesh& mesh) {
    // type: 4=rigid, 5=skel, 6=va (rechazado por ahora).
    // NOTA: el skinning se difiere a cookModelMesh (los nodos vienen despues).
    (void)m;
    const size_t blobStart = r.p;
    const uint32_t objSize = r.u32();
    if (!r.ok) return false;
    const size_t blobEnd = blobStart + 4 + objSize;
    bool ret = false;
    if (type == 4 || type == 5) {
        const uint32_t vc = r.u32(), pc = r.u32();
        const uint32_t mbt = r.u32(), mbv = r.u32();
        if (!r.ok || vc > 1000000 || pc > 1000000) {
            r.ok = false;
            return false;
        }
        int blendKind = 0;
        std::vector<BoneSet> sets;
        std::vector<uint8_t> reidx;
        uint32_t boneEffector = 0;
        bool haveEffector = false;
        if (type == 4) {
            if (mbt != 1 || mbv != 1) {
                r.ok = false;
                return false;
            }
            uint32_t sf[4];
            for (int i = 0; i < 4; i++) sf[i] = r.u32();
            boneEffector = r.u32();
            haveEffector = true;
            if (!r.ok) return false;
            std::vector<RawVert> raw;
            for (int s = 0; s < 4; s++) {
                if (!sf[s]) continue;
                if (s != 0) {
                    r.ok = false;
                    return false;
                } // multi-stream rigido: no soportado aun
                if (!parseModelStream(r, 0, sf[s], vc, raw)) return false;
            }
            if (raw.size() != vc) {
                r.ok = false;
                return false;
            }
            mesh.raw = raw;
            mesh.effector = boneEffector;
            mesh.haveEffector = haveEffector;
            mesh.isSkel = false;
            mesh.idx.reserve((size_t)pc * 3);
            for (uint32_t i = 0; i < pc * 3; i++) {
                const uint16_t ix = r.u16();
                if (!r.ok || ix >= vc) {
                    r.ok = false;
                    return false;
                }
                mesh.idx.push_back(ix);
            }
            ret = r.ok;
        } else {
            // skel: RD (directo, bonesets) o MP (paleta, indices por vert).
            const uint8_t reindexed = r.u8();
            uint32_t sf[4];
            for (int i = 0; i < 4; i++) sf[i] = r.u32();
            const uint8_t useMP = r.u8();
            if (!r.ok) return false;
            bool isMP = useMP != 0;
            if (isMP) {
                (void)r.u32(); // minBone
                (void)r.u32(); // maxBone
                if (!r.ok) return false;
                if (reindexed) {
                    const uint32_t nbc = r.u32();
                    if (!r.ok || nbc > 10000) {
                        r.ok = false;
                        return false;
                    }
                    r.skip((size_t)nbc * 4);
                    if (!r.ok) return false;
                }
                if (mbv < 2 || mbv > 4) {
                    r.ok = false;
                    return false;
                }
                blendKind = 3 + (int)mbv; // INDEXED_B(mbv-1)
            } else {
                if (mbt < 1 || mbt > 4) {
                    r.ok = false;
                    return false;
                }
                blendKind = mbt == 1 ? 0 : (int)mbt - 1; // NONINDEXED_B(mbt-1)
            }
            std::vector<RawVert> raw;
            for (int s = 0; s < 4; s++) {
                if (!sf[s]) continue;
                if (s != 0) {
                    r.ok = false;
                    return false;
                }
                if (!parseModelStream(r, blendKind, sf[s], vc, raw))
                    return false;
            }
            if (raw.size() != vc) {
                r.ok = false;
                return false;
            }
            mesh.idx.reserve((size_t)pc * 3);
            for (uint32_t i = 0; i < pc * 3; i++) {
                const uint16_t ix = r.u16();
                if (!r.ok || ix >= vc) {
                    r.ok = false;
                    return false;
                }
                mesh.idx.push_back(ix);
            }
            if (!isMP) {
                const uint32_t nbs = r.u32();
                if (!r.ok || nbs > 100000) {
                    r.ok = false;
                    return false;
                }
                sets.reserve(nbs);
                for (uint32_t i = 0; i < nbs; i++) {
                    BoneSet bs;
                    bs.first = r.u16();
                    bs.count = r.u16();
                    bs.bones[0] = r.u8(); bs.bones[1] = r.u8();
                    bs.bones[2] = r.u8(); bs.bones[3] = r.u8();
                    (void)r.u32(); // indexIntoBuff
                    if (!r.ok) return false;
                    sets.push_back(bs);
                }
            }
            if (!r.ok) return false;
            mesh.raw = raw;
            mesh.sets = sets;
            mesh.isSkel = true;
            mesh.isMP = isMP;
            ret = r.ok;
        }
    }
    // Siempre deja el cursor al final del blob (tolera extensiones).
    r.p = blobEnd;
    r.ok = r.ok && ret && r.p <= r.n;
    return r.ok;
}

inline bool cookModelMesh(const ModelFile& m, ModelMesh& mesh) {
    // Bind pose estatico = posiciones crudas tal cual las modelo el artista.
    // (El skinning solo aplica con animacion: current*invBind. Los nodos del
    // fichero son los globales de bind; con pose==bind la composicion es la
    // identidad, asi que el cocido es la copia directa.)
    (void)m;
    mesh.verts.clear();
    mesh.verts.reserve(mesh.raw.size());
    for (size_t i = 0; i < mesh.raw.size(); i++) {
        const RawVert& sv = mesh.raw[i];
        ModelVert mv;
        mv.x = sv.x; mv.y = sv.y; mv.z = sv.z;
        mv.u = sv.u; mv.v = sv.v;
        mesh.verts.push_back(mv);
    }
    return true;
}

inline bool loadLTB(const std::string& path, ModelFile& m) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false;
    fseek(f, 0, SEEK_END);
    const long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (len <= 64 || len > 200000000) {
        fclose(f);
        return false;
    }
    std::vector<uint8_t> buf((size_t)len);
    const size_t got = fread(buf.data(), 1, (size_t)len, f);
    fclose(f);
    if (got != (size_t)len) return false;
    ModelReader r(buf.data(), buf.size());
    if (r.u8() != 1) return false; // LTB_D3D_MODEL_FILE
    r.skip(1);
    if (r.u16() != 9) return false; // version
    r.skip(1 + 3 + 12);
    const uint32_t fileVersion = r.u32(); // 23+ soportado (MIN_MODEL_FILE_VERSION)
    if (!r.ok || fileVersion < 23) return false;
    uint32_t alloc[15];
    for (int i = 0; i < 15; i++) alloc[i] = r.u32();
    if (!r.ok) return false;
    (void)r.str(); // command string
    m.visRadius = r.f32();
    const uint32_t nobb = r.u32();
    if (!r.ok || nobb > 1024) return false;
    if (nobb) {
        // ModelOBB nuevo (fv>=24): 68B; deprecado: 64B.
        r.skip((size_t)nobb * (fileVersion >= 24 ? 68 : 64));
        if (!r.ok) return false;
    }
    const uint32_t npieces = r.u32();
    // NOTA: alloc[3] puede discrepar (playerbase declara 3 pero trae 0:
    // esqueleto/anims sin geometria propia). Solo se exige cordura.
    if (!r.ok || npieces > 64) return false;
    for (uint32_t pi = 0; pi < npieces; pi++) {
        const std::string pname = r.str();
        const uint32_t nlods = r.u32();
        if (!r.ok || nlods == 0 || nlods > 16) return false;
        r.skip((size_t)nlods * 4 + 8);
        if (!r.ok) return false;
        for (uint32_t li = 0; li < nlods; li++) {
            const uint32_t ntex = r.u32();
            int32_t texidx[4] = {0, 0, 0, 0};
            for (int k = 0; k < 4; k++) texidx[k] = r.i32();
            (void)r.i32(); // renderstyle
            (void)r.u8();  // priority
            const uint32_t rtype = r.u32();
            if (!r.ok) return false;
            if (li == 0) {
                ModelMesh mesh;
                mesh.piece = pname;
                mesh.texSlot = ntex ? texidx[0] : 0;
                if (!parseModelMesh(r, (int)rtype, m, mesh)) return false;
                m.meshes.push_back(mesh);
            } else {
                // Skips LODs extra via tamano del blob: re-lee el tipo?
                // parseModelMesh ya deja el cursor al final del blob solo si
                // conoce el tipo; para LODs extra exige tipos conocidos.
                ModelMesh dummy;
                if (!parseModelMesh(r, (int)rtype, m, dummy)) return false;
            }
            // usedNodeList: uno por LOD (ModelPiece::Load lo lee dentro del
            // loop de LODs, no una vez por piece).
            const uint8_t unl = r.u8();
            if (!r.ok) return false;
            r.skip(unl);
            if (!r.ok) return false;
        }
    }
    if (!parseModelNode(r, m, 0)) return false;
    const uint32_t nws = r.u32();
    if (!r.ok || nws > 1024) return false;
    for (uint32_t i = 0; i < nws; i++) {
        (void)r.str();
        const uint32_t nw = r.u32();
        if (!r.ok || nw > 100000) return false;
        r.skip((size_t)nw * 4);
        if (!r.ok) return false;
    }
    const uint32_t ncm = r.u32();
    if (!r.ok || ncm == 0 || ncm > 64) return false;
    for (uint32_t i = 1; i < ncm; i++) {
        m.childFiles.push_back(r.str());
        if (!r.ok) return false;
    }
    for (size_t i = 0; i < m.meshes.size(); i++)
        cookModelMesh(m, m.meshes[i]);
    return r.ok;
}
} // namespace Host
