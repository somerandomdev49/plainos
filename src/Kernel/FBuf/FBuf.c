#include <Plain/Kernel/FBuf.h>
/*#include <Plain/Kernel/Font/PSF.h>
#include <Plain/Kernel/Font/BM.h>*/

void BM_Font_RenderSymbol(struct BM_Font *this,
                          struct Framebuffer *fb,
                          uint32_t x, uint32_t y,
                          unsigned char c,
                          uint32_t col)
{
    if(c >= 128) return; /* don't render unknown characters */
    if(x >= fb->width - 8 || y >= fb->width - 8) return;
    const char *sym = this->data[0][c];
    const uint32_t add = fb->width - 8;
    uint32_t *b = &fb->buffer[x + y * fb->width];
    for(uint32_t i = 0; i < 8; ++i)
    {
        for(uint8_t j = 0; j < 8; ++j)
        {
            *b = (sym[i] & (1 << j)) ? col : *b;
            b += 1; /* next pixel */
        }
        b += add; /* next line */
    }
}

void Framebuffer_Intiailize(struct Framebuffer *this, /* buffer to initialize */
                            uint32_t *buf,            /* buffer */
                            uint32_t w, uint32_t h,   /* dimensions in pixels */
                            struct Font *f            /* font to use */)
{
    this->width = w;
    this->height = h;
    this->buffer = buf;
    this->font = f;
}

void Framebuffer_Pixel(struct Framebuffer *this,
                       uint32_t x, uint32_t y,
                       uint32_t col)
{
    this->buffer[x + y * this->width] = col;
}

void Framebuffer_Rect(struct Framebuffer *this,
                      uint32_t x, uint32_t y,
                      uint32_t w, uint32_t h,
                      uint32_t col)
{
    const uint32_t fw = this->width;
    const uint32_t idx_end = (y + h) * fw + x;
    for(uint32_t idx = y * fw + x; idx < idx_end; idx += fw)
    for(uint32_t i = 0; i != w; ++i)
        this->buffer[idx + i] = col;
}

void Framebuffer_Char(struct Framebuffer *this,
                      uint32_t x, uint32_t y,
                      char c,
                      uint32_t col)
{
    BM_Font_RenderSymbol((struct BM_Font*)this->font, this, x, y, c, col);
}

void Framebuffer_Text(struct Framebuffer *this,
                      uint32_t x, uint32_t y,
                      const char *string,
                      uint32_t color)
{
    uint32_t px = 0, py = 0;
    while(*string)
    {
        Framebuffer_Char(this,
                         x + px * (this->font->width + 1),
                         y + py * (this->font->height + 1),
                         *string,
                         color);
            
        switch(*string++)
        {
            case '\n': px = 0; ++y; break;
            case '\t': px = (px + 8 - 1) & -8; break;
            case '\v': ++py;
            default: ++px;
        }
    }
}
