// R2: lector minimo de la seccion render de World.dat + feed al mesh pipeline.
// Formato espejado del writer (Tools/PreProcessor/Packer_PC/PCWorldPacker.cpp,
// SaveFile: version + 6 markers -> renderData = slot 5) y del reader D3D
// (render_a/src/sys/d3d/d3d_renderblock.cpp::Load). Solo extrae pos+uv0+color
// por vertice y nombre de textura por seccion; lightmaps/shaders/oclusores se
// saltan. Header-only: lo incluye host_sealhunter.cpp (sin cambios en CMake).
#pragma once
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>
#include <vector>

namespace Host {
// ---- matematicas MVP compartidas (columna-mayor, NDC Vulkan) ----
struct Vec3 {
    float x, y, z;
};
inline void matMul4(const float a[16], const float b[16], float o[16]) {
    for (int c = 0; c < 4; c++)
        for (int r = 0; r < 4; r++) {
            float s = 0;
            for (int k = 0; k < 4; k++) s += a[k * 4 + r] * b[c * 4 + k];
            o[c * 4 + r] = s;
        }
}
inline void buildView4(const Vec3& r, const Vec3& u, const Vec3& f,
                       const Vec3& c, float o[16]) {
    o[0] = r.x; o[1] = u.x; o[2] = f.x; o[3] = 0;
    o[4] = r.y; o[5] = u.y; o[6] = f.y; o[7] = 0;
    o[8] = r.z; o[9] = u.z; o[10] = f.z; o[11] = 0;
    o[12] = -(r.x * c.x + r.y * c.y + r.z * c.z);
    o[13] = -(u.x * c.x + u.y * c.y + u.z * c.z);
    o[14] = -(f.x * c.x + f.y * c.y + f.z * c.z);
    o[15] = 1;
}
inline void buildPersp4(float f, float aspect, float zn, float zf, float o[16]) {
    const float A = zf / (zf - zn), B = -zf * zn / (zf - zn);
    for (int i = 0; i < 16; i++) o[i] = 0;
    o[0] = f / aspect;
    o[5] = -f;
    o[10] = A;
    o[11] = 1.0f;
    o[14] = B;
}
// Camara ya orientada: construye VP desde base (r,u,f) + pos + fovX.
inline void vpFromBasis4(const float r[3], const float u[3], const float f[3],
                         const float c[3], float fovXdeg, float aspect,
                         float zn, float zf, float vp[16]) {
    Vec3 rr{r[0], r[1], r[2]}, uu{u[0], u[1], u[2]}, ff{f[0], f[1], f[2]},
        cc{c[0], c[1], c[2]};
    float V[16], P[16];
    buildView4(rr, uu, ff, cc, V);
    const float tx = tanf(fovXdeg * 0.5f * 3.14159265f / 180.0f);
    buildPersp4(1.0f / (tx / aspect), aspect, zn, zf, P);
    matMul4(P, V, vp);
}
// Camara look-at: f = norm(tgt-pos), r = norm(up x f), u = f x r.
inline void lookAt4(const Vec3& pos, const Vec3& tgt, float fovXdeg,
                    float aspect, float zn, float zf, float vp[16]) {
    Vec3 f{tgt.x - pos.x, tgt.y - pos.y, tgt.z - pos.z};
    const float fl = sqrtf(f.x * f.x + f.y * f.y + f.z * f.z);
    if (fl > 1e-6f) {
        f.x /= fl; f.y /= fl; f.z /= fl;
    } else {
        f.x = 0; f.y = 0; f.z = 1;
    }
    Vec3 r{f.z, 0, -f.x};
    float rl = sqrtf(r.x * r.x + r.z * r.z);
    if (rl < 1e-6f) {
        r.x = 1; r.y = 0; r.z = 0; rl = 1;
    }
    r.x /= rl; r.y = 0; r.z /= rl;
    Vec3 u{f.y * r.z - f.z * r.y, f.z * r.x - f.x * r.z,
           f.x * r.y - f.y * r.x};
    float V[16], P[16];
    buildView4(r, u, f, pos, V);
    const float tx = tanf(fovXdeg * 0.5f * 3.14159265f / 180.0f);
    buildPersp4(1.0f / (tx / aspect), aspect, zn, zf, P);
    matMul4(P, V, vp);
}

// ---- lector LE con cotas ----
struct DatReader {
    const uint8_t* d = nullptr;
    size_t n = 0, p = 0;
    bool ok = true;
    DatReader() {}
    DatReader(const uint8_t* dd, size_t nn) : d(dd), n(nn) {}
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
    float f32() {
        uint32_t v = u32();
        float f = 0;
        memcpy(&f, &v, 4);
        return f;
    }
    void skip(size_t k) { need(k) ? (void)(p += k) : (void)0; }
    std::string str() {
        // CGenLTStream::ReadString: u16 len + bytes (sin NUL).
        std::string s;
        const uint16_t len = u16();
        if (!ok) return s;
        if (!need(len)) return s;
        s.assign((const char*)(d + p), len);
        p += len;
        return s;
    }
    void vec3(float o[3]) {
        o[0] = f32(); o[1] = f32(); o[2] = f32();
    }
};

struct WorldVert {
    float x, y, z, u, v, r, g, b, a;
};
struct WorldSection {
    std::string tex0, tex1;
    uint8_t shader = 0;
    uint32_t triStart = 0, triCount = 0;
};
struct WorldBlock {
    float cx = 0, cy = 0, cz = 0, hx = 0, hy = 0, hz = 0;
    std::vector<WorldSection> sections;
    std::vector<WorldVert> verts;
    std::vector<uint32_t> idx; // 3 por tri
};
struct WorldData {
    std::vector<WorldBlock> blocks;
    uint32_t totalTris = 0;
};

inline bool skipGeomPoly(DatReader& r) {
    // operator>>(SRBGeometryPoly): u8 n + n*vec3 + normal + dist
    const uint8_t nv = r.u8();
    if (!r.ok || nv < 3) {
        r.ok = false;
        return false;
    }
    r.skip((size_t)nv * 12 + 12 + 4);
    return r.ok;
}
inline bool skipLightGroup(DatReader& r) {
    // operator>>(SRBLightGroup): u16 len + chars + color + blob + subLMs
    const uint16_t len = r.u16();
    if (!r.ok) return false;
    r.skip(len + 12);
    if (!r.ok) return false;
    const uint32_t dl = r.u32();
    if (!r.ok) return false;
    r.skip(dl);
    if (!r.ok) return false;
    const uint32_t nsec = r.u32();
    if (!r.ok || nsec > 100000) {
        r.ok = false;
        return false;
    }
    for (uint32_t i = 0; i < nsec; i++) {
        const uint32_t nsub = r.u32();
        if (!r.ok || nsub > 100000) {
            r.ok = false;
            return false;
        }
        for (uint32_t j = 0; j < nsub; j++) {
            r.skip(16); // left,top,w,h
            if (!r.ok) return false;
            const uint32_t sz = r.u32();
            if (!r.ok) return false;
            r.skip(sz);
            if (!r.ok) return false;
        }
    }
    return true;
}

inline bool parseRenderBlock(DatReader& r, WorldBlock& b) {
    float c[3], h[3];
    r.vec3(c); r.vec3(h);
    if (!r.ok) return false;
    b.cx = c[0]; b.cy = c[1]; b.cz = c[2];
    b.hx = h[0]; b.hy = h[1]; b.hz = h[2];
    const uint32_t nsec = r.u32();
    if (!r.ok || nsec > 100000) {
        r.ok = false;
        return false;
    }
    uint32_t triBase = 0;
    for (uint32_t i = 0; i < nsec; i++) {
        WorldSection s;
        s.tex0 = r.str();
        s.tex1 = r.str();
        s.shader = r.u8();
        s.triCount = r.u32();
        (void)r.str(); // texture effect
        if (!r.ok || s.triCount > 1000000) {
            r.ok = false;
            return false;
        }
        const uint32_t lmw = r.u32(), lmh = r.u32(), lmsz = r.u32();
        (void)lmw; (void)lmh;
        if (!r.ok) return false;
        r.skip(lmsz);
        if (!r.ok) return false;
        s.triStart = triBase;
        triBase += s.triCount;
        b.sections.push_back(s);
    }
    const uint32_t nverts = r.u32();
    if (!r.ok || nverts > 10000000) {
        r.ok = false;
        return false;
    }
    b.verts.reserve(nverts);
    for (uint32_t i = 0; i < nverts; i++) {
        // SRBVertex raw 68B: pos + uv0 + uv1(skip) + color + resto(skip)
        if (!r.need(68)) return false;
        WorldVert v;
        memcpy(&v.x, r.d + r.p, 12);
        memcpy(&v.u, r.d + r.p + 12, 8);
        uint32_t col = 0;
        memcpy(&col, r.d + r.p + 12 + 8 + 8, 4);
        r.p += 68;
        // m_nColor D3D 0xAARRGGBB -> float; alpha 0 (nunca set) = opaco.
        float a = ((col >> 24) & 255) / 255.0f;
        if (a <= 0) a = 1.0f;
        v.r = ((col >> 16) & 255) / 255.0f;
        v.g = ((col >> 8) & 255) / 255.0f;
        v.b = (col & 255) / 255.0f;
        v.a = a;
        b.verts.push_back(v);
    }
    const uint32_t ntris = r.u32();
    if (!r.ok || ntris > 10000000) {
        r.ok = false;
        return false;
    }
    b.idx.reserve((size_t)ntris * 3);
    for (uint32_t i = 0; i < ntris; i++) {
        const uint32_t i0 = r.u32(), i1 = r.u32(), i2 = r.u32();
        uint32_t poly = r.u32();
        (void)poly;
        if (!r.ok || i0 >= nverts || i1 >= nverts || i2 >= nverts) {
            r.ok = false;
            return false;
        }
        b.idx.push_back(i0);
        b.idx.push_back(i1);
        b.idx.push_back(i2);
    }
    uint32_t nsky = r.u32();
    if (!r.ok || nsky > 100000) {
        r.ok = false;
        return false;
    }
    for (uint32_t i = 0; i < nsky; i++)
        if (!skipGeomPoly(r)) return false;
    uint32_t nocc = r.u32();
    if (!r.ok || nocc > 100000) {
        r.ok = false;
        return false;
    }
    for (uint32_t i = 0; i < nocc; i++) {
        if (!skipGeomPoly(r)) return false;
        (void)r.u32(); // id
        if (!r.ok) return false;
    }
    uint32_t nlg = r.u32();
    if (!r.ok || nlg > 100000) {
        r.ok = false;
        return false;
    }
    for (uint32_t i = 0; i < nlg; i++)
        if (!skipLightGroup(r)) return false;
    (void)r.u8(); // childFlags
    r.skip(8);    // 2 x u32 child indices
    return r.ok;
}

inline bool parseRenderWorld(DatReader& r, WorldData& w, int depth) {
    if (depth > 8) {
        r.ok = false;
        return false;
    }
    const uint32_t nb = r.u32();
    if (!r.ok || nb > 100000) {
        r.ok = false;
        return false;
    }
    for (uint32_t i = 0; i < nb; i++) {
        WorldBlock b;
        if (!parseRenderBlock(r, b)) return false;
        w.totalTris += (uint32_t)(b.idx.size() / 3);
        w.blocks.push_back(b);
    }
    const uint32_t nwm = r.u32();
    if (!r.ok || nwm > 10000) {
        r.ok = false;
        return false;
    }
    for (uint32_t i = 0; i < nwm; i++) {
        (void)r.str(); // worldmodel name
        if (!r.ok) return false;
        if (!parseRenderWorld(r, w, depth + 1)) return false;
    }
    return true;
}

inline bool loadWorldDat(const std::string& path, WorldData& w) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false;
    fseek(f, 0, SEEK_END);
    const long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if (len <= 64) {
        fclose(f);
        return false;
    }
    std::vector<uint8_t> buf((size_t)len);
    const size_t got = fread(buf.data(), 1, (size_t)len, f);
    fclose(f);
    if (got != (size_t)len) return false;
    DatReader r(buf.data(), buf.size());
    (void)r.u32(); // version
    uint32_t slots[6] = {0, 0, 0, 0, 0, 0};
    for (int i = 0; i < 6; i++) slots[i] = r.u32();
    if (!r.ok || slots[5] == 0 || slots[5] >= buf.size()) return false;
    r.p = slots[5];
    return parseRenderWorld(r, w, 0) && r.ok;
}

// ---- texturas DTX ----
inline uint32_t dxtBlockSize(int bpp) { return bpp == 1 ? 8 : 16; }
inline void decodeDXT1Block(const uint8_t* b, uint8_t out[64], bool withAlpha) {
    uint16_t c0 = (uint16_t)(b[0] | (b[1] << 8));
    uint16_t c1 = (uint16_t)(b[2] | (b[3] << 8));
    uint8_t pal[4][4];
    for (int i = 0; i < 2; i++) {
        const uint16_t c = i ? c1 : c0;
        pal[i][0] = (uint8_t)(((c >> 11) & 31) * 255 / 31);
        pal[i][1] = (uint8_t)(((c >> 5) & 63) * 255 / 63);
        pal[i][2] = (uint8_t)((c & 31) * 255 / 31);
        pal[i][3] = 255;
    }
    if (c0 > c1 || !withAlpha) {
        pal[2][0] = (uint8_t)((2 * pal[0][0] + pal[1][0]) / 3);
        pal[2][1] = (uint8_t)((2 * pal[0][1] + pal[1][1]) / 3);
        pal[2][2] = (uint8_t)((2 * pal[0][2] + pal[1][2]) / 3);
        pal[2][3] = 255;
        pal[3][0] = (uint8_t)((pal[0][0] + 2 * pal[1][0]) / 3);
        pal[3][1] = (uint8_t)((pal[0][1] + 2 * pal[1][1]) / 3);
        pal[3][2] = (uint8_t)((pal[0][2] + 2 * pal[1][2]) / 3);
        pal[3][3] = 255;
    } else {
        pal[2][0] = (uint8_t)((pal[0][0] + pal[1][0]) / 2);
        pal[2][1] = (uint8_t)((pal[0][1] + pal[1][1]) / 2);
        pal[2][2] = (uint8_t)((pal[0][2] + pal[1][2]) / 2);
        pal[2][3] = 255;
        pal[3][0] = pal[3][1] = pal[3][2] = 0;
        pal[3][3] = 0;
    }
    const uint32_t bits = (uint32_t)b[4] | ((uint32_t)b[5] << 8) |
                          ((uint32_t)b[6] << 16) | ((uint32_t)b[7] << 24);
    for (int i = 0; i < 16; i++) {
        const uint8_t* c = pal[(bits >> (2 * i)) & 3];
        out[i * 4 + 0] = c[0];
        out[i * 4 + 1] = c[1];
        out[i * 4 + 2] = c[2];
        out[i * 4 + 3] = c[3];
    }
}
inline void decodeDXTBlock(const uint8_t* b, int kind, uint8_t out[64]) {
    // kind: 1=DXT1, 3=DXT3, 5=DXT5. out = 4x4 RGBA.
    const uint8_t* color = b;
    uint8_t alpha[16];
    for (int i = 0; i < 16; i++) alpha[i] = 255;
    if (kind == 3) {
        for (int i = 0; i < 16; i += 2) {
            alpha[i] = (uint8_t)((b[i] & 15) * 17);
            alpha[i + 1] = (uint8_t)((b[i] >> 4) * 17);
        }
        color = b + 8;
    } else if (kind == 5) {
        const uint8_t a0 = b[0], a1 = b[1];
        uint8_t apal[8];
        apal[0] = a0; apal[1] = a1;
        if (a0 > a1) {
            for (int i = 2; i < 8; i++)
                apal[i] = (uint8_t)(((8 - i) * a0 + (i - 1) * a1) / 7);
        } else {
            for (int i = 2; i < 6; i++)
                apal[i] = (uint8_t)(((6 - i) * a0 + (i - 1) * a1) / 5);
            apal[6] = 0; apal[7] = 255;
        }
        uint64_t ab = 0;
        for (int i = 0; i < 6; i++) ab |= (uint64_t)b[2 + i] << (8 * i);
        for (int i = 0; i < 16; i++) alpha[i] = apal[(ab >> (3 * i)) & 7];
        color = b + 8;
    }
    uint8_t rgb[64];
    decodeDXT1Block(color, rgb, kind != 1);
    for (int i = 0; i < 16; i++) {
        out[i * 4 + 0] = rgb[i * 4 + 0];
        out[i * 4 + 1] = rgb[i * 4 + 1];
        out[i * 4 + 2] = rgb[i * 4 + 2];
        out[i * 4 + 3] = alpha[i];
    }
}
// Decodifica el mip base de un .dtx a RGBA. bpp = BPPIdent del engine
// (3=BPP_32, 4/5/6=DXT1/3/5).
inline bool decodeDTX(const std::string& path, uint32_t& w, uint32_t& h,
                      std::vector<uint8_t>& rgba) {
    FILE* f = fopen(path.c_str(), "rb");
    if (!f) return false;
    uint8_t hdr[164];
    if (fread(hdr, 1, sizeof(hdr), f) != sizeof(hdr)) {
        fclose(f);
        return false;
    }
    const uint32_t bw = (uint32_t)(hdr[8] | (hdr[9] << 8));
    const uint32_t bh = (uint32_t)(hdr[10] | (hdr[11] << 8));
    const uint32_t nmips = (uint32_t)(hdr[12] | (hdr[13] << 8));
    const int bpp = (int)hdr[26]; // m_Extra[2] = BPPIdent
    if (!bw || !bh || bw > 2048 || bh > 2048) {
        fclose(f);
        return false;
    }
    w = bw; h = bh;
    rgba.resize((size_t)w * h * 4);
    bool ok = false;
    if (bpp == 3) {
        // BPP_32: pixeles directos (convencion D3D A8R8G8B8 -> bytes B,G,R,A).
        std::vector<uint8_t> px((size_t)w * h * 4);
        if (fread(px.data(), 1, px.size(), f) == px.size()) {
            for (size_t i = 0; i < (size_t)w * h; i++) {
                rgba[i * 4 + 0] = px[i * 4 + 2];
                rgba[i * 4 + 1] = px[i * 4 + 1];
                rgba[i * 4 + 2] = px[i * 4 + 0];
                rgba[i * 4 + 3] = px[i * 4 + 3];
            }
            ok = true;
        }
    } else if (bpp == 4 || bpp == 5 || bpp == 6) {
        const int dxt = (bpp == 4) ? 1 : (bpp == 5 ? 3 : 5);
        const uint32_t blk = (uint32_t)dxtBlockSize(dxt);
        const uint32_t nbw = (w + 3) / 4, nbh = (h + 3) / 4;
        std::vector<uint8_t> comp((size_t)nbw * nbh * blk);
        if (fread(comp.data(), 1, comp.size(), f) == comp.size()) {
            uint8_t cell[64];
            for (uint32_t by = 0; by < nbh; by++)
                for (uint32_t bx = 0; bx < nbw; bx++) {
                    decodeDXTBlock(&comp[((size_t)by * nbw + bx) * blk], dxt,
                                   cell);
                    for (int yy = 0; yy < 4; yy++)
                        for (int xx = 0; xx < 4; xx++) {
                            const uint32_t dx = bx * 4 + (uint32_t)xx;
                            const uint32_t dy = by * 4 + (uint32_t)yy;
                            if (dx >= w || dy >= h) continue;
                            uint8_t* p = &rgba[((size_t)dy * w + dx) * 4];
                            const uint8_t* s = &cell[(yy * 4 + xx) * 4];
                            p[0] = s[0]; p[1] = s[1]; p[2] = s[2]; p[3] = s[3];
                        }
                }
            ok = true;
        }
    }
    fclose(f);
    (void)nmips;
    return ok;
}
// Altura del terreno bajo (x,z): rayo vertical contra los tris del mundo
// (sin skybox). -1e30 si no hay impacto.
inline float groundHeight(const WorldData& w, float x, float z) {
    float best = -1e30f;
    for (size_t bi = 0; bi < w.blocks.size(); bi++) {
        const WorldBlock& b = w.blocks[bi];
        if (x < b.cx - b.hx || x > b.cx + b.hx || z < b.cz - b.hz ||
            z > b.cz + b.hz)
            continue;
        for (size_t si = 0; si < b.sections.size(); si++) {
            const WorldSection& s = b.sections[si];
            if (s.tex0.empty()) continue;
            // Skybox: se salta (sus verts llevan offset de camara).
            const std::string& t = s.tex0;
            if (t.size() >= 5 &&
                (t[0] == 'T' || t[0] == 't') &&
                (t[1] == 'E' || t[1] == 'e') &&
                (t[2] == 'X' || t[2] == 'x') && t[3] == 'F' &&
                t[4] == 'X')
                continue;
            for (uint32_t ti = 0; ti < s.triCount; ti++) {
                const WorldVert& a =
                    b.verts[b.idx[(size_t)(s.triStart + ti) * 3]];
                const WorldVert& c =
                    b.verts[b.idx[(size_t)(s.triStart + ti) * 3 + 1]];
                const WorldVert& e =
                    b.verts[b.idx[(size_t)(s.triStart + ti) * 3 + 2]];
                // Baricentricas 2D en XZ.
                const float d =
                    (c.z - e.z) * (a.x - e.x) + (e.x - c.x) * (a.z - e.z);
                if (d > -1e-6f && d < 1e-6f) continue;
                const float l0 =
                    ((c.z - e.z) * (x - e.x) + (e.x - c.x) * (z - e.z)) / d;
                const float l1 =
                    ((e.z - a.z) * (x - e.x) + (a.x - e.x) * (z - e.z)) / d;
                const float l2 = 1.0f - l0 - l1;
                if (l0 < 0 || l1 < 0 || l2 < 0) continue;
                const float y = l0 * a.y + l1 * c.y + l2 * e.y;
                if (y > best) best = y;
            }
        }
    }
    return best;
}

// Mundo compartido en vivo: drawWorldFrame lo carga una vez, el resto lo lee.
inline WorldData& worldStore() {
    static WorldData w;
    return w;
}
inline bool& worldReady() {
    static bool b = false;
    return b;
}
inline bool& worldTried() {
    static bool b = false;
    return b;
}
} // namespace Host
