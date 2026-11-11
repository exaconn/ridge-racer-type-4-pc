#ifndef RR_TEST_MENU_GLYPH_WIRE_H
#define RR_TEST_MENU_GLYPH_WIRE_H
/* Transport for historical PAL-word fixtures. The frozen record and Worlds
 * remain 28-byte wire data. Actual production bodies use the canonical native
 * record, with complete pointers and a 32-byte stride on LP64.
 *
 * Legacy controller cases treat image words as opaque tokens (including zero)
 * and never decode their payload. Bind those tokens to real allocated image
 * storage. Logical PAL wrap occurs only when serializing a token, never while
 * reconstructing or accessing a host pointer. New image-pipeline tests use
 * actual image buffers and cover the payload contract separately. */
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "rr/menu_glyph_types.h"

typedef MenuGlyphRow1C TestNativeMenuGlyphRow;
typedef struct {
    u8 flags00, pad01;
    s8 control02;
    u8 pad03[4], glyph07;
    s16 x08, y0A, w0C, h0E;
    u8 pad10[8];
    s32 phase18;
} FrozenMenuGlyphRow1C;
_Static_assert(sizeof(FrozenMenuGlyphRow1C)==28,"unchanged frozen glyph row");
_Static_assert(offsetof(FrozenMenuGlyphRow1C,phase18)==24,"frozen image word");
_Static_assert(sizeof(TestNativeMenuGlyphRow)==24+sizeof(void *),"actual native stride");
_Static_assert(offsetof(TestNativeMenuGlyphRow,imageBase18)==24,"actual image pointer");
_Static_assert(_Generic(((TestNativeMenuGlyphRow*)0)->imageBase18,u8*:1,default:0),"actual byte pointer type");

enum {
    RR_GLYPH_IMAGE_MOVE=4*0x30000,
    RR_GLYPH_IMAGE_HALF=2*RR_GLYPH_IMAGE_MOVE,
    RR_GLYPH_IMAGE_SIZE=2*RR_GLYPH_IMAGE_HALF+1,
    RR_GLYPH_IMAGE_BINDINGS=256,
    RR_GLYPH_ROW_BINDINGS=8,
    RR_GLYPH_TOKEN_BUCKETS=4096
};
typedef struct {u8 *storage;u32 token;int next;} TestGlyphImageBinding;
typedef struct {
    FrozenMenuGlyphRow1C *wire;
    u8 *storage;
    TestNativeMenuGlyphRow *rows;
    u8 **lastPointer;
    u32 *lastToken;
    size_t count,capacity;
} TestGlyphRowsBinding;
static TestGlyphImageBinding rr_glyph_images[RR_GLYPH_IMAGE_BINDINGS];
static TestGlyphRowsBinding rr_glyph_rows[RR_GLYPH_ROW_BINDINGS];
static unsigned rr_glyph_image_count,rr_glyph_row_count,rr_glyph_epoch;
static unsigned rr_glyph_bucket_epoch[RR_GLYPH_TOKEN_BUCKETS];
static int rr_glyph_bucket_head[RR_GLYPH_TOKEN_BUCKETS];

static inline void rr_glyph_check_guard(const u8 *guard){
    for(unsigned i=0;i<16;i++)assert(guard[i]==0xA5);
}
static inline void rr_test_glyph_reset(void){
    rr_glyph_image_count=0;rr_glyph_row_count=0;
    rr_glyph_epoch++;assert(rr_glyph_epoch);
}
static inline u8 *rr_glyph_bind_image(u32 token){
    unsigned bucket=token>>20;
    for(int delta=-1;delta<=1;delta++){
        unsigned b=(bucket+(unsigned)delta)&(RR_GLYPH_TOKEN_BUCKETS-1);
        if(rr_glyph_bucket_epoch[b]!=rr_glyph_epoch)continue;
        for(int n=rr_glyph_bucket_head[b];n>=0;n=rr_glyph_images[n].next){
            TestGlyphImageBinding *image=&rr_glyph_images[n];
            u32 bits=token-image->token;s32 offset;memcpy(&offset,&bits,4);
            if(offset>=-RR_GLYPH_IMAGE_MOVE&&offset<=RR_GLYPH_IMAGE_MOVE)
                return image->storage+16+RR_GLYPH_IMAGE_HALF+offset;
        }
    }
    assert(rr_glyph_image_count<RR_GLYPH_IMAGE_BINDINGS);
    unsigned n=rr_glyph_image_count++;TestGlyphImageBinding *image=&rr_glyph_images[n];
    if(!image->storage){
        image->storage=malloc((size_t)RR_GLYPH_IMAGE_SIZE+32);assert(image->storage);
        memset(image->storage,0xA5,16);memset(image->storage+16+RR_GLYPH_IMAGE_SIZE,0xA5,16);
        assert((uintptr_t)image->storage>UINT32_MAX);
    }
    rr_glyph_check_guard(image->storage);rr_glyph_check_guard(image->storage+16+RR_GLYPH_IMAGE_SIZE);
    image->token=token;
    image->next=rr_glyph_bucket_epoch[bucket]==rr_glyph_epoch?rr_glyph_bucket_head[bucket]:-1;
    rr_glyph_bucket_epoch[bucket]=rr_glyph_epoch;rr_glyph_bucket_head[bucket]=(int)n;
    return image->storage+16+RR_GLYPH_IMAGE_HALF;
}
static inline u32 rr_glyph_image_token(const u8 *pointer){
    if(!pointer)return 0;
    uintptr_t p=(uintptr_t)pointer;
    for(unsigned n=0;n<rr_glyph_image_count;n++){
        TestGlyphImageBinding *image=&rr_glyph_images[n];uintptr_t base=(uintptr_t)image->storage+16;
        if(p>=base&&p-base<=RR_GLYPH_IMAGE_SIZE){
            intptr_t offset=(intptr_t)(p-base)-RR_GLYPH_IMAGE_HALF;
            return image->token+(u32)offset;
        }
    }
    assert(!"image pointer outside complete native bindings");return 0;
}
static inline void rr_test_glyph_register(FrozenMenuGlyphRow1C *wire,size_t count){
    assert(count&&rr_glyph_row_count<RR_GLYPH_ROW_BINDINGS);
    TestGlyphRowsBinding *binding=&rr_glyph_rows[rr_glyph_row_count++];
    if(binding->capacity<count){
        free(binding->storage);free(binding->lastPointer);free(binding->lastToken);
        binding->storage=malloc(32+count*sizeof(*binding->rows));
        binding->lastPointer=malloc(count*sizeof(*binding->lastPointer));
        binding->lastToken=malloc(count*sizeof(*binding->lastToken));
        assert(binding->storage&&binding->lastPointer&&binding->lastToken);binding->capacity=count;
    }
    binding->wire=wire;binding->count=count;binding->rows=(TestNativeMenuGlyphRow *)(binding->storage+16);
    memset(binding->storage,0xA5,16);memset((u8*)(binding->rows+count),0xA5,16);
    assert((uintptr_t)binding->rows>UINT32_MAX);
    for(size_t i=0;i<count;i++){
        u32 token;memcpy(&token,&wire[i].phase18,4);memcpy(&binding->rows[i],&wire[i],24);
        binding->rows[i].imageBase18=rr_glyph_bind_image(token);
        binding->lastPointer[i]=binding->rows[i].imageBase18;binding->lastToken[i]=token;
    }
}
static inline TestNativeMenuGlyphRow *rr_test_glyph_root(const FrozenMenuGlyphRow1C *wire){
    uintptr_t p=(uintptr_t)wire;
    for(unsigned n=0;n<rr_glyph_row_count;n++){
        TestGlyphRowsBinding *binding=&rr_glyph_rows[n];uintptr_t base=(uintptr_t)binding->wire;
        if(p>=base&&p-base<binding->count*sizeof(*wire)){
            size_t offset=p-base;assert(offset%sizeof(*wire)==0);
            return binding->rows+offset/sizeof(*wire);
        }
    }
    assert(!"wire glyph root outside registered row owners");return NULL;
}
static inline void rr_test_glyph_store(void){
    for(unsigned n=0;n<rr_glyph_row_count;n++){
        TestGlyphRowsBinding *binding=&rr_glyph_rows[n];
        rr_glyph_check_guard(binding->storage);rr_glyph_check_guard((u8*)(binding->rows+binding->count));
        for(size_t i=0;i<binding->count;i++){
            TestNativeMenuGlyphRow *row=&binding->rows[i];u32 token=binding->lastToken[i];
            if(row->imageBase18!=binding->lastPointer[i])token=rr_glyph_image_token(row->imageBase18);
            memcpy(&binding->wire[i],row,24);memcpy(&binding->wire[i].phase18,&token,4);
            binding->lastToken[i]=token;binding->lastPointer[i]=row->imageBase18;
        }
    }
}
static inline void rr_test_glyph_load(void){
    for(unsigned n=0;n<rr_glyph_row_count;n++){
        TestGlyphRowsBinding *binding=&rr_glyph_rows[n];
        for(size_t i=0;i<binding->count;i++){
            u32 token;memcpy(&token,&binding->wire[i].phase18,4);memcpy(&binding->rows[i],&binding->wire[i],24);
            if(token!=binding->lastToken[i])binding->rows[i].imageBase18=rr_glyph_bind_image(token);
            binding->lastToken[i]=token;binding->lastPointer[i]=binding->rows[i].imageBase18;
        }
    }
}
static inline void rr_test_glyph_check(void){
    for(unsigned n=0;n<rr_glyph_row_count;n++){
        TestGlyphRowsBinding *b=&rr_glyph_rows[n];rr_glyph_check_guard(b->storage);rr_glyph_check_guard((u8*)(b->rows+b->count));
    }
    for(unsigned n=0;n<rr_glyph_image_count;n++){
        u8 *p=rr_glyph_images[n].storage;rr_glyph_check_guard(p);rr_glyph_check_guard(p+16+RR_GLYPH_IMAGE_SIZE);
    }
}
#endif
