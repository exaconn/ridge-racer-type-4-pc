#include "rr/glyph_list.h"
void func_80059700(s16 *stream,s32 color,s32 bank)
{
    s32 x, y, defaultPage, clut, page, offset, gap;
    u32 cachedGlyph;
    
    s32 basePage;
    s32 spriteCode;
    u8 *cursor;
    RrUiUvRect *uv;
    x = *stream++;
    y = *stream++;
    if (bank == 0) { clut = 0x7F78; defaultPage = 0x3E; } else { clut = 0x7E38; defaultPage = 0x25; }
    cachedGlyph = *(u16 *)stream;
    if (*stream >= 0) {
        basePage = defaultPage & 0xFFFF;
        do {
            s32 glyph;
            cursor = RR_GLYPH_SCRATCH->cursor;
            page = defaultPage;
            if (basePage == 0x3E) {
                if ((s16)cachedGlyph >= 0x400) page = 0x25;
            }
            cachedGlyph = *(u16 *)stream;
            glyph = *stream++;
            if (glyph >= 0x400) {
                offset = (s32)((cachedGlyph - 0x400U) << 16) >> 14;
                uv = (RrUiUvRect*)((u8 *)D_800A1158 + offset);
            } else {
                offset = glyph * 4;
                if (bank == 0) uv = (RrUiUvRect*)((u8 *)D_800A1090 + offset);
                else uv = (RrUiUvRect*)((u8 *)D_800A1158 + offset);
            }
            spriteCode = 0x66;
            ((RrSpritePacketSignedClut*)cursor)->len = 4;
            ((RrSpritePacketSignedClut*)cursor)->code = spriteCode;
            ((RrSpritePacketSignedClut*)cursor)->r = color; ((RrSpritePacketSignedClut*)cursor)->g = color; ((RrSpritePacketSignedClut*)cursor)->b = color;
            ((RrSpritePacketSignedClut*)cursor)->x = x;
            ((RrSpritePacketSignedClut*)cursor)->y = y;
            ((RrSpritePacketSignedClut*)cursor)->w = uv->w;
            ((RrSpritePacketSignedClut*)cursor)->h = uv->h;
            ((RrSpritePacketSignedClut*)cursor)->u = uv->u;
            ((RrSpritePacketSignedClut*)cursor)->v = uv->v;
            ((RrSpritePacketSignedClut*)cursor)->clut = clut;
            ((RrSpritePacketSignedClut*)cursor)->addr = (D_800ACC14->head70&0xFFFFFFU);
            D_800ACC14->head70 = (D_800ACC14->head70 & 0xFF000000) | ((u32)(unsigned long)cursor & 0xFFFFFF);
            cursor+=20;
            RR_GLYPH_SCRATCH->cursor=cursor;
            ((RrGlyphCommand8*)cursor)->len=1;
            ((RrGlyphCommand8*)cursor)->command=(page&0x9FF)|0xE1000200;
            ((RrGlyphCommand8*)cursor)->addr=D_800ACC14->head70;
            D_800ACC14->head70=(D_800ACC14->head70&0xFF000000U)|((u32)(unsigned long)cursor&0xFFFFFFU);
            RR_GLYPH_SCRATCH->cursor=cursor+8;
            gap = *stream++;
            x=(s32)((u32)x+(u32)(uv->w+gap));
            cachedGlyph = *(u16 *)stream;
        } while (*stream >= 0);
    }
}
