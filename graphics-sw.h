static void
load_vertices()
{
}

static void
do_it()
{
    // clip triangles
    // do perspective division
    // rasterize triangles
}

// The format of the coordinates is u12.4 fixed point, which allows for up to
// 4096 pixels with 4 subpixels per pixel.
static void
fill_triangle(unsigned short x1, unsigned short y1, unsigned short z1,
              unsigned short x2, unsigned short y2, unsigned short z2,
              unsigned short x3, unsigned short y3, unsigned short z3)
{
    // double Bresenham line drawing algorithm with linear interpolation
}