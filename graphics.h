#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

typedef struct
{
    uint32_t w0,
             w1;
} GCmd;

typedef struct
{
    int16_t x, y, z,                // position in modelspace
            w;
    int16_t s, t;                   // texture coordinates
    union
    {
        struct
        {
            int8_t nx, ny, nz,      // normal direction
                   nw;
        };
        struct
        {
            uint8_t r, g, b,        // color
                    a;              // alpha (opacity)
        };
    };
} Vert;

enum
{
    G_NOOP      = 0x00,
    G_LOADVERTS = 0x01,
    G_DRAWTRIS  = 0x02,
    G_END       = 0xFF,
};

#define gNoop() ((GCmd){ G_NOOP << 24, 0 })

#define gLoadVerts(vCacheIndex, numVerts, vramAddr) \
    ((GCmd){ \
        G_NOOP << 24 | ((vCacheIndex) & 0xFFF) << 12 \
                     | (((numVerts) - 1) & 0xFFF), \
        (vramAddr) \
    })

#define gDrawTris(indSize, numTris, vramAddr) \
    ((GCmd){ \
        G_DRAWTRIS << 24 | (((indSize) - 1) & 0xFF) << 16 \
                         | (((numTris) - 1) & 0xFFFF), \
        (vramAddr) \
    })

#define gEnd() ((GCmd){ G_END << 24, 0 })

#endif
