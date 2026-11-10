#ifndef RR_TEST_SCENE_CAMERA_WIRE_H
#define RR_TEST_SCENE_CAMERA_WIRE_H

/* Test-only PAL serialization. Native pointer fields always refer to complete
 * owned track rows; arbitrary pre-existing wire tokens are kept in bindings,
 * never reconstructed from a host pointer's high bits. */
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "rr/scene_camera_types.h"

enum {RR_SCENE_CAMERA_WIRE_SIZE=0x160,RR_SCENE_TRACK_BINDINGS=3,RR_SCENE_TRACK_ROWS=3};
typedef struct {
    u32 address;
    u32 before[4];
    SceneCameraTrack24 rows[RR_SCENE_TRACK_ROWS];
    u32 after[4];
} TestSceneTrackBinding;
static TestSceneTrackBinding rr_scene_tracks[RR_SCENE_TRACK_BINDINGS];
static unsigned rr_scene_track_count;

static inline void rr_scene_track_reset(void) {
    rr_scene_track_count=0;
    memset(rr_scene_tracks,0xA5,sizeof(rr_scene_tracks));
}

static inline SceneCameraTrack24 *rr_scene_track_resolve(u32 address) {
    if(!address)return NULL;
    for(unsigned i=0;i<rr_scene_track_count;i++)
        if(rr_scene_tracks[i].address==address)return rr_scene_tracks[i].rows;
    assert(rr_scene_track_count<RR_SCENE_TRACK_BINDINGS);
    TestSceneTrackBinding *binding=&rr_scene_tracks[rr_scene_track_count++];
    binding->address=address;
    assert((uintptr_t)binding->rows>UINT32_MAX);
    return binding->rows;
}

static inline u32 rr_scene_track_address(const SceneCameraTrack24 *rows) {
    if(!rows)return 0;
    for(unsigned i=0;i<rr_scene_track_count;i++)
        for(unsigned j=0;j<RR_SCENE_TRACK_ROWS;j++)
            if(rows==&rr_scene_tracks[i].rows[j])
                return rr_scene_tracks[i].address+j*sizeof(SceneCameraTrack24);
    assert(!"track pointer outside complete fixture owners");return 0;
}

static inline void rr_scene_tracks_unchanged(void) {
    for(unsigned i=0;i<rr_scene_track_count;i++){
        const u8 *p=(const u8*)rr_scene_tracks[i].before;
        size_t bytes=sizeof(rr_scene_tracks[i])-offsetof(TestSceneTrackBinding,before);
        for(size_t j=0;j<bytes;j++)assert(p[j]==0xA5);
    }
}

static inline void rr_scene_camera_load(SceneCameraState160 *camera,const u8 wire[0x160]) {
    u32 address;
    memset(camera,0xA5,sizeof(*camera));
    memcpy(camera,wire,0x138);
    memcpy(&address,wire+0x138,4);
    camera->tracks138=rr_scene_track_resolve(address);
    memcpy(&camera->mode13C,wire+0x13C,0x160-0x13C);
}

static inline void rr_scene_camera_store(u8 wire[0x160],const SceneCameraState160 *camera) {
    u32 address=rr_scene_track_address(camera->tracks138);
    memcpy(wire,camera,0x138);
    memcpy(wire+0x138,&address,4);
    memcpy(wire+0x13C,&camera->mode13C,0x160-0x13C);
}

_Static_assert(sizeof(SceneCameraKeyframe2C)==44,"fixed track keyframe");
_Static_assert(sizeof(SceneCameraTrack24)==1056,"complete track row");
_Static_assert(sizeof(((SceneCameraState160*)0)->tracks138)==sizeof(void*),"full native track pointer");
_Static_assert(offsetof(SceneCameraState160,mode13C)==offsetof(SceneCameraState160,tracks138)+sizeof(void*),"native suffix follows pointer");

#endif
