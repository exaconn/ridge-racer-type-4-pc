#ifndef TEST_CAR_RENDER_WIRE_H
#define TEST_CAR_RENDER_WIRE_H

#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "rr/car_render_types.h"

/* Test-only PAL serialization of the actual native records. Pointer cells are
 * resolved by each harness; production structures never use encoded stand-ins. */
typedef void *(*CarRenderResolve)(u32);
typedef u32 (*CarRenderAddress)(const void *);
static inline u32 car_get32(const u8 **p) {u32 x;memcpy(&x,*p,4);*p+=4;return x;}
static inline void car_put32(u8 **p,u32 x){memcpy(*p,&x,4);*p+=4;}
static inline void car_trail_load(CarTrailBuffer *value,const u8 **p,CarRenderResolve resolve) {
    value->state=car_get32(p);value->vertices=resolve(car_get32(p));
}
static inline void car_trail_store(u8 **p,const CarTrailBuffer *value,CarRenderAddress address) {
    car_put32(p,value->state);car_put32(p,address(value->vertices));
}
static inline void car_slot_load(CarSlotBufferC8 *slot,const u8 *p,CarRenderResolve resolve) {
    const u8 *begin=p;unsigned i;
    slot->model=resolve(car_get32(&p));memcpy(&slot->reserved04,p,16);p+=16;
    for(i=0;i<4;i++)car_trail_load(&slot->main14[i],&p,resolve);
    memcpy(&slot->secondCount34,p,8);p+=8;
    for(i=0;i<4;i++)car_trail_load(&slot->second3C[i],&p,resolve);
    memcpy(&slot->phase5C,p,24);p+=24;memcpy(&slot->storedPosition74,p,16);p+=16;
    for(i=0;i<8;i++)car_trail_load(&slot->history84[i],&p,resolve);
    slot->sparksC4=resolve(car_get32(&p));assert(p-begin==200);
}
static inline void car_slot_store(u8 *p,const CarSlotBufferC8 *slot,CarRenderAddress address) {
    u8 *begin=p;unsigned i;
    car_put32(&p,address(slot->model));memcpy(p,&slot->reserved04,16);p+=16;
    for(i=0;i<4;i++)car_trail_store(&p,&slot->main14[i],address);
    memcpy(p,&slot->secondCount34,8);p+=8;
    for(i=0;i<4;i++)car_trail_store(&p,&slot->second3C[i],address);
    memcpy(p,&slot->phase5C,24);p+=24;memcpy(p,&slot->storedPosition74,16);p+=16;
    for(i=0;i<8;i++)car_trail_store(&p,&slot->history84[i],address);
    car_put32(&p,address(slot->sparksC4));assert(p-begin==200);
}

static inline void car_mesh_load(RroEffectMesh *mesh,const u8 **p,CarRenderResolve resolve) {
    mesh->vertices=resolve(car_get32(p));mesh->vertexCount=car_get32(p);mesh->commands=resolve(car_get32(p));
}
static inline void car_mesh_store(u8 **p,const RroEffectMesh *mesh,CarRenderAddress address) {
    car_put32(p,address(mesh->vertices));car_put32(p,mesh->vertexCount);car_put32(p,address(mesh->commands));
}
static inline void car_model_load(CarModelHeader *model,const u8 *p,CarRenderResolve resolve) {
    const u8 *begin=p;unsigned i;
    RroEffectMesh *meshes[]={&model->primary10,&model->secondary1C,&model->trailing28,&model->history34,&model->trailing40};
    memcpy(model->reserved00,p,12);p+=12;model->litQuadCommands0C=resolve(car_get32(&p));
    for(i=0;i<5;i++)car_mesh_load(meshes[i],&p,resolve);
    memcpy(model->reserved4C,p,12);p+=12;car_mesh_load(&model->shadow58,&p,resolve);
    for(i=0;i<4;i++)car_mesh_load(&model->body64[i],&p,resolve);
    assert(p-begin==0x94);
}
static inline void car_model_store(u8 *p,const CarModelHeader *model,CarRenderAddress address) {
    u8 *begin=p;unsigned i;
    const RroEffectMesh *meshes[]={&model->primary10,&model->secondary1C,&model->trailing28,&model->history34,&model->trailing40};
    memcpy(p,model->reserved00,12);p+=12;car_put32(&p,address(model->litQuadCommands0C));
    for(i=0;i<5;i++)car_mesh_store(&p,meshes[i],address);
    memcpy(p,model->reserved4C,12);p+=12;car_mesh_store(&p,&model->shadow58,address);
    for(i=0;i<4;i++)car_mesh_store(&p,&model->body64[i],address);
    assert(p-begin==0x94);
}

#endif
