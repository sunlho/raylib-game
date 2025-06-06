#ifndef TILEMAP_UTILS_H
#define TILEMAP_UTILS_H

#define TILEMAP_LAYER_OFFSET 10000

static inline float GetLayerSortY(float layer_index, float sort_y) {
    return layer_index * TILEMAP_LAYER_OFFSET + sort_y;
}

static inline int CeilDivSafe(int a, int b) {
    return b == 0 ? 0 : (a + b - 1) / b;
}

static inline uint64_t MortonEncode(int32_t x, int32_t y) {
    uint64_t answer = 0;
    for (int i = 0; i < 32; ++i) {
        answer |= ((x >> i) & 1ull) << (2 * i);
        answer |= ((y >> i) & 1ull) << (2 * i + 1);
    }
    return answer;
}

#endif // TILEMAP_UTILS_H
