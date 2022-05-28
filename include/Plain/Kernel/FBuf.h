#ifndef PLAIN_KERNEL_FBUF_
#define PLAIN_KERNEL_FBUF_
#include <Plain/Common.h>

/*- PSF Font Header -*/
struct PSF_Font
{
    uint32_t magic;    /* magic bytes to identify PSF */
    uint32_t version;  /* zero */
    uint32_t headersz; /* offset of bitmaps in file, 32 */
    uint32_t flags;    /* 0 if there's no unicode table */
    uint32_t numglyph; /* number of glyphs */
    uint32_t glyphsz;  /* size of each glyph */
    uint32_t height;   /* height in pixels */
    uint32_t width;    /* width in pixels */
};

/*- Font Base -*/
struct Font
{
    const char *name;      /* font name */
    uint8_t width, height; /* dimensions of each symbol */
};


/*- Linear Framebuffer -*/
struct Framebuffer
{
    uint32_t width, height; /* size of buffer in pixels */
    uint32_t *buffer;       /* pointer to first pixel */
    struct Font *font;      /* font to use when rendering chars */
};

/*- Bitmap Font -*/
struct BM_Font
{
    struct Font super;          /* font data */
    const char (*data)[128][8]; /* bitmap data */
};

void BM_Font_RenderSymbol(struct BM_Font *this,
                          struct Framebuffer *fb,
                          uint32_t x, uint32_t y,
                          unsigned char c,
                          uint32_t col);


void Framebuffer_Intiailize(struct Framebuffer *this, /* buffer to initialize */
                            uint32_t *buf,            /* buffer */
                            uint32_t w, uint32_t h,   /* dimensions in pixels */
                            struct Font *f            /* font to use */);
                            
void Framebuffer_Pixel(struct Framebuffer *this,
                       uint32_t x, uint32_t y,
                       uint32_t col);

void Framebuffer_Rect(struct Framebuffer *this,
                      uint32_t x, uint32_t y,
                      uint32_t w, uint32_t h,
                      uint32_t col);
                                    
void Framebuffer_Char(struct Framebuffer *this,
                      uint32_t x, uint32_t y,
                      char c,
                      uint32_t col);

void Framebuffer_Text(struct Framebuffer *this,
                      uint32_t x, uint32_t y,
                      const char *string,
                      uint32_t color);
#endif
