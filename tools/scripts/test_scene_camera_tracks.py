"""Real scene camera consumers with complete native tracks and MIPS traces.

Matrix/renderer services publish deterministic outputs and may mutate their
callers' shared state. This checks ownership, arithmetic and call ordering,
not the trigonometric or GPU implementation. A relative endpoint's unused
fourth VECTOR word is undefined in the original; only that word is masked
when the endpoint is copied whole. All three coordinates remain checked.
"""
import ctypes as C
import os
from pathlib import Path
import struct
import subprocess
from tempfile import TemporaryDirectory
import unittest

from elftools.elf.elffile import ELFFile
from unicorn import Uc, UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN, UC_MODE_MIPS32
from unicorn.mips_const import UC_MIPS_REG_A0, UC_MIPS_REG_A1, UC_MIPS_REG_V0, UC_MIPS_REG_V1

from c_functions import address_of, definition_source, source_owners
from test_vehicle_initializer import Cpu
from mips_preview_engine import signed

ROOT = Path(__file__).resolve().parents[2]
NAMES = ['ApplyCameraState', 'StepCarShowcasePhysics', 'StepCarShowcaseScene']
SERVICES = ['RotMatrixZYX', 'RotMatrix', 'ApplyMatrix', 'SetGeomScreen',
            'BuildCameraMatrices', 'UpdateCarShowcaseCamera', 'ApplyCameraState',
            'StepCarShowcasePhysics', 'UpdateSparkEffects', 'ScaleMatrixRow1',
            'DrawGarageFloorReflection']
STACK, STOP = 0x803F0000, 0x803FF000


class CameraCpu(Cpu):
    def step(self):
        instruction = self.load(self.pc, 4)
        if instruction >> 26 or instruction & 63 != 26:
            return super().step()
        dividend = signed(self.r[(instruction >> 21) & 31])
        divisor = signed(self.r[(instruction >> 16) & 31])
        assert divisor and not (dividend == -2147483648 and divisor == -1)
        quotient = abs(dividend)//abs(divisor)
        if (dividend < 0) != (divisor < 0):quotient = -quotient
        self.lo = quotient & 0xFFFFFFFF;self.hi = (dividend-quotient*divisor) & 0xFFFFFFFF
        destination = self.delay;self.delay = None
        self.pc = (self.pc+4 if destination is None else destination) & 0xFFFFFFFF


PREFIX = r'''
#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "vehicle_wire.h"
#include "rr/scene_camera_state.h"
#include "rr/projection_state.h"
#include "game/car/bounds.h"
#include "game/effect/carfx.h"
#include "game/gfx/gfxotag.h"
#include "game/gfx/meshquad.h"
typedef struct {u32 before[4];SceneRaceContext value;u32 after[4];} ContextOwner;
typedef struct {u32 before[4];SceneCameraState160 value;u32 after[4];} CameraOwner;
typedef struct {u32 before[4];SceneCameraTrack24 rows[3];u32 after[4];} TrackOwner;
typedef struct {u32 before[4];RaceVehicle values[2];u32 after[4];} CarOwner;
typedef struct {u32 before[4];CarBounds value;u32 after[4];} BoundsOwner;
typedef struct {u32 before[4];CarBounds *values[3];u32 after[4];} BoundsPointers;
typedef struct {u32 before[4];ViewMatrixScratch48 value;u32 after[4];} ScratchOwner;
typedef struct {u32 before[4];u32 words[32];u32 after[4];} ArenaOwner;
typedef struct {u32 before[4];s32 value;u32 after[4];} SegmentOwner;
typedef struct {
 ContextOwner context;CameraOwner cameras[2];TrackOwner banks[3];CarOwner cars;
 BoundsOwner bounds[3];BoundsPointers table;ScratchOwner scratch;ArenaOwner arena;
 VectorWords16 fallback;Vec3s8 angles;s32 projection;u16 buttons;SegmentOwner segment;
} World;
enum {WIRE_SIZE=0x564+32+2*384+3*(3*1056+32)+1632+3*44+44+104+160+28+2+36};
typedef struct {World entry,after;u32 operation,args[4],sizes[2],resultSize,outputSlot,result;
 u8 inputs[2][32],output[32];unsigned undefinedPositionWord;} Event;
typedef struct {World final;unsigned count;Event events[12];u32 result;unsigned undefinedPositionWord;} Capture;
static World live,initial;static Capture actual,expected,*capture;
static unsigned scenario,kind,serial,carIndex,trackIndex,viewIndex,segmentIndex;
static unsigned undefinedPositionWord,keySeen[24],durationSeen[3],endSeen[2],absoluteSeen[4],
 rootSeen[3],bankSeen[3],rowSeen[3],aliasSeen[4],serviceSeen[12],wrapSeen,mutationSeen[2];
static u8 firstWire[WIRE_SIZE],lastWire[WIRE_SIZE],eventWire[WIRE_SIZE];
#define RaceContext live.context.value
#define PlayerCars live.cars.values
#define CarBoundsTable live.table.values
#define D_800A0FC0 live.fallback
#define D_800A0FD0 live.angles
#define D_800A0FD8 live.projection
#define Pad0Buttons live.buttons
#define SCRATCHPAD (&live.scratch.value.projection)
#define SCRATCH_ROT_MATRIX (&live.scratch.value.matrix28)
static u32 next(u32 *seed){u32 n=*seed;n^=n<<13;n^=n>>17;n^=n<<5;return *seed=n;}
static s32 word(u32 n){s32 r;memcpy(&r,&n,4);return r;}
static s32 add(s32 a,u32 b){int64_t n=(int64_t)a+(int64_t)(s32)b;wrapSeen+=n<INT32_MIN||n>INT32_MAX;return word((u32)a+b);}
static SceneCameraState160 *camera(unsigned n){assert(n<3);return n?&live.cameras[n-1].value:&RaceContext.cameraEC;}
static SceneCameraView24 *view(void){return viewIndex?&live.banks[(viewIndex-1)/3].rows[(viewIndex-1)%3][7].view00:&RaceContext.view60;}
static s32 *segment(void){
 SceneCameraState160 *c=camera((scenario/6)%3);
 return segmentIndex==0?&live.segment.value:segmentIndex==1?&c->segment10C:segmentIndex==2?&c->phase108:&live.banks[(scenario/18)%3].rows[trackIndex][1].time24;
}
static u32 pointer(const void *p){
 if(!p)return 0;
 if(p==&RaceContext)return @RACE@;
 if(p==&RaceContext.cameraEC)return @RACE@+0xEC;
 if(p==&RaceContext.view60)return @RACE@+0x60;
 for(unsigned i=0;i<2;i++)if(p==&live.cameras[i].value)return 0x80180010U+i*4096U;
 for(unsigned b=0;b<3;b++)for(unsigned row=0;row<3;row++){
  const SceneCameraKeyframe2C *keys=live.banks[b].rows[row];u32 address=0x801B0010U+b*65536U+row*1056U;
  if(p==&live.banks[b].rows[row])return address;
  for(unsigned k=0;k<24;k++){
   if(p==&keys[k].view00)return address+k*44U;
   if(p==&keys[k].time24)return address+k*44U+0x24;
  }
 }
 for(unsigned c=0;c<3;c++){
  SceneCameraState160 *cam=camera(c);u32 address=c?0x80180010U+(c-1)*4096U:@RACE@+0xEC;
  if(p==&cam->segment10C)return address+0x10C;if(p==&cam->phase108)return address+0x108;
 }
 for(unsigned i=0;i<2;i++)if(p==&PlayerCars[i])return @CARS@+i*800U;
 for(unsigned i=0;i<3;i++)if(p==&live.bounds[i].value)return 0x801A0010U+i*4096U;
 if(p==&live.segment.value)return 0x801E0010U;
 if(p==SCRATCHPAD)return 0x1F800000U;if(p==SCRATCH_ROT_MATRIX)return 0x1F800028U;
 for(unsigned i=0;i<128;i++)if(p==(u8*)live.arena.words+i)return 0x801F0010U+i;
 /* Service locals must be real full-width native stack objects. */
 assert((uintptr_t)p>UINT32_MAX);return 0;
}
static void put32(u8 *p,u32 n){memcpy(p,&n,4);}
static void cameraWire(u8 *p,const SceneCameraState160 *c){
 memcpy(p,c,0x138);put32(p+0x138,pointer(c->tracks138));memcpy(p+0x13C,&c->mode13C,0x24);
}
static void wire(const World *w,u8 *out){
 u8 *p=out;memcpy(p,w->context.before,16);p+=16;memcpy(p,&w->context.value,0xEC);p+=0xEC;
 cameraWire(p,&w->context.value.cameraEC);p+=0x160;put32(p,pointer(w->context.value.camera24C));p+=4;
 memcpy(p,w->context.value.reserved250,0x314);p+=0x314;memcpy(p,w->context.after,16);p+=16;
 for(unsigned i=0;i<2;i++){memcpy(p,w->cameras[i].before,16);p+=16;cameraWire(p,&w->cameras[i].value);p+=0x160;memcpy(p,w->cameras[i].after,16);p+=16;}
 memcpy(p,w->banks,sizeof(w->banks));p+=sizeof(w->banks);memcpy(p,w->cars.before,16);p+=16;
 for(unsigned i=0;i<2;i++){rr_vehicle_wire_store_fields(p,&w->cars.values[i]);p+=800;}
 memcpy(p,w->cars.after,16);p+=16;memcpy(p,w->bounds,sizeof(w->bounds));p+=sizeof(w->bounds);
 memcpy(p,w->table.before,16);p+=16;for(unsigned i=0;i<3;i++){put32(p,pointer(w->table.values[i]));p+=4;}memcpy(p,w->table.after,16);p+=16;
 memcpy(p,w->scratch.before,16);p+=16;const ProjectionScratchPrefix *s=&w->scratch.value.projection;
 put32(p,pointer(s->pointers.cursor));put32(p+4,pointer(s->pointers.order));memcpy(p+8,&s->pose08.position00,32);p+=40;
 memcpy(p,&w->scratch.value.matrix28,32);p+=32;memcpy(p,w->scratch.after,16);p+=16;
 memcpy(p,&w->arena,sizeof(w->arena));p+=sizeof(w->arena);memcpy(p,&w->fallback,28);p+=28;
 memcpy(p,&w->buttons,2);p+=2;memcpy(p,&w->segment,36);p+=36;assert(p-out==WIRE_SIZE);
}
static void mutate(unsigned operation){
 unsigned n=serial++;if(!(scenario&1))return;
 unsigned root=(scenario+n)%3;RaceContext.camera24C=camera(root);rootSeen[root]++;
 RaceContext.camera24C->tracks138=live.banks[(scenario+n)%3].rows;
 RaceContext.camera24C->frame104=word(0x7FFFFFD0U+scenario+n);
 RaceContext.camera24C->phase108=word(0x7FFFFFE0U+scenario*3U+n);
 RaceContext.camera24C->extentA0=word(scenario*0x17171717U+n);
 live.table.values[0]=&live.bounds[(scenario+n)%3].value;
 PlayerCars[0].position20.words[n%3]^=0xA55AA55AU;
 for(unsigned i=0;i<4;i++)live.fallback.words[i]^=scenario*0x1234567U+n+i;
 live.angles.fields.x=(s16)(scenario+n);live.angles.fields.y=(s16)~(scenario+n);live.projection=word(scenario*0x9E3779B9U+n);
 SceneCameraView24 *v=view();v->pose00.position00.words[n%3]^=0x87654321U+n;v->pose00.rotation10.y=(s16)(scenario*37U+n);
 live.buttons^=0x800U;
 if(operation==6){
  SceneCameraKeyframe2C *keys=live.banks[(scenario/18)%3].rows[trackIndex];
  if((scenario/9)%4==0)for(unsigned k=0;k<24;k++)keys[k].time24=40;
  for(unsigned k=0;k<24;k++){keys[k].view00.pose00.position00.words[n%3]^=0x99887766U;keys[k].view00.distance1C^=0x76543210;}
 }
}
static Event *begin(unsigned operation){
 unsigned index=capture->count++;assert(index<12);Event *e=&capture->events[index];memset(e,0,sizeof(*e));
 memcpy(&e->entry,&live,sizeof(live));e->operation=operation;e->outputSlot=4;e->undefinedPositionWord=undefinedPositionWord;serviceSeen[operation]++;return e;
}
static void finish(Event *e){mutate(e->operation);memcpy(&e->after,&live,sizeof(live));}
static void rotation(unsigned operation,const void *angles,MatrixWords32 *out){
 Event *e=begin(operation);e->args[0]=pointer(angles);e->args[1]=pointer(out);e->sizes[0]=6;memcpy(e->inputs[0],angles,6);
 u32 seed=operation*0x17171717U;for(unsigned i=0;i<6;i++)seed=seed*33U+e->inputs[0][i];
 for(unsigned i=0;i<8;i++)out->w[i]=word(next(&seed));e->resultSize=32;e->outputSlot=1;memcpy(e->output,out,32);finish(e);
}
static void traceRotZYX(s16 *angles,s32 *matrix){rotation(1,angles,(MatrixWords32*)matrix);}
static void traceRot(const Vec3sFields8 *angles,MatrixWords32 *matrix){rotation(2,angles,matrix);}
Vec3i *ApplyMatrix(const MatrixWords32 *matrix,const Vec3s8 *input,Vec3i *output){
 Event *e=begin(3);e->args[0]=pointer(matrix);e->args[1]=pointer(input);e->args[2]=pointer(output);e->sizes[0]=32;e->sizes[1]=6;
 memcpy(e->inputs[0],matrix,32);memcpy(e->inputs[1],input,6);u32 seed=0x571ACEDU;
 for(unsigned i=0;i<32;i++)seed=seed*33U+e->inputs[0][i];for(unsigned i=0;i<6;i++)seed=seed*37U+e->inputs[1][i];
 s32 values[3];for(unsigned i=0;i<3;i++)values[i]=word(next(&seed));memcpy(output,values,12);
 e->resultSize=12;e->outputSlot=2;memcpy(e->output,output,12);finish(e);return output;
}
void SetGeomScreen(s32 projection){Event *e=begin(4);e->args[0]=(u32)projection;finish(e);}
void BuildCameraMatrices(void){Event *e=begin(5);finish(e);}
static void traceUpdate(s32 phase){Event *e=begin(6);e->args[0]=(u32)phase;finish(e);}
static void traceApply(const SceneCameraView24 *state){
 Event *e=begin(7);e->args[0]=pointer(state);e->sizes[0]=36;
 /* The event input buffer stores the consumed pose and two scalar fields. */
 memcpy(e->inputs[0],state,24);memcpy(e->inputs[0]+24,&state->distance1C,8);
 for(unsigned i=0;i<3;i++){
  live.scratch.value.projection.pose08.position00.words[i]=word((u32)state->pose00.position00.words[i]+scenario*0x17171717U+i);
 }
 live.scratch.value.projection.pose08.angles10.xyz.x=-state->pose00.rotation10.x;
 live.scratch.value.projection.pose08.angles10.xyz.y=state->pose00.rotation10.y;
 live.scratch.value.projection.pose08.angles10.xyz.z=state->pose00.rotation10.z;finish(e);
}
static int traceStep(RaceVehicle *car,s32 time,s32 track,s32 *selected){
 Event *e=begin(8);e->args[0]=pointer(car);e->args[1]=(u32)time;e->args[2]=(u32)track;e->args[3]=pointer(selected);
 *selected=word(scenario*0x17171717U);e->result=word(scenario*0x9E3779B9U);finish(e);return word(e->result);
}
void UpdateSparkEffects(void){Event *e=begin(9);finish(e);}
void ScaleMatrixRow1(s32 scale,MatrixWords32 *matrix){
 Event *e=begin(10);e->args[0]=(u32)scale;e->args[1]=pointer(matrix);e->sizes[0]=32;memcpy(e->inputs[0],matrix,32);
 for(unsigned i=0;i<3;i++)matrix->fields.rotation00[0][i]=(s16)((u32)(s32)matrix->fields.rotation00[0][i]*(u32)scale);finish(e);
}
void DrawGarageFloorReflection(void){Event *e=begin(11);finish(e);}
'''
REFERENCE = r'''
static u32 interpolate(s32 end,s32 start,s32 elapsed,s32 duration){
 int64_t delta=(int64_t)end-start;u64 product=(u64)(u32)(uint64_t)delta*(u32)elapsed;
 wrapSeen+=delta<INT32_MIN||delta>INT32_MAX||product>INT32_MAX;
 s32 numerator=word((u32)product);assert(duration>0);return (u32)((int64_t)numerator/duration);
}
static void referenceApply(const SceneCameraView24 *state){
 ProjectionScratchPrefix *p=SCRATCHPAD;MatrixWords32 matrix;Vec3s8 angles,offset;Vec3i result;
 p->pose08.angles10.xyz.x=-(s32)state->pose00.rotation10.x;p->pose08.angles10.xyz.y=state->pose00.rotation10.y;p->pose08.angles10.xyz.z=state->pose00.rotation10.z;
 angles.fields.x=state->pose00.rotation10.x;angles.fields.y=state->pose00.rotation10.y;angles.fields.z=0;
 offset.fields.x=offset.fields.y=0;offset.fields.z=(s16)(u16)(0U-(u16)state->distance1C);
 traceRotZYX(angles.halves,matrix.w);ApplyMatrix(&matrix,&offset,&result);
 p->pose08.position00.xyz.x=add(state->pose00.position00.xyz.x,(u32)result.x);
 p->pose08.position00.xyz.y=add(state->pose00.position00.xyz.y,(u32)result.y);
 p->pose08.position00.xyz.z=add(state->pose00.position00.xyz.z,(u32)result.z);
 SetGeomScreen(state->projection20);BuildCameraMatrices();
}
static void orbit(VectorWords16 *position){
 Vec3s8 angles,offset;MatrixWords32 matrix;angles.fields.x=0;angles.fields.y=-2560;angles.fields.z=0;
 offset.fields.x=(s16)live.fallback.xyz.x;offset.fields.y=(s16)live.fallback.xyz.y;offset.fields.z=(s16)live.fallback.xyz.z;
 traceRot(&angles.fields,&matrix);ApplyMatrix(&matrix,&offset,&position->xyz);
 for(unsigned i=0;i<3;i++)position->words[i]=add(position->words[i],(u32)((uint64_t)(u32)PlayerCars[0].position20.words[i]*4U));
}
static void publishOrbit(const VectorWords16 *position){
 memcpy(&live.scratch.value.projection.pose08.position00.xyz,&position->xyz,12);
 live.scratch.value.projection.pose08.angles10.xyz.x=live.angles.fields.x;
 live.scratch.value.projection.pose08.angles10.xyz.y=live.angles.fields.y;
 live.scratch.value.projection.pose08.angles10.xyz.z=live.angles.fields.z;SetGeomScreen(live.projection);
}
static int referencePhysics(RaceVehicle *car,s32 time,s32 track,s32 *selected){
 SceneCameraKeyframe2C *keys=RaceContext.camera24C->tracks138[track];unsigned end=0;
 for(unsigned i=1;i<24;i++)if(word((u32)((uint64_t)(u32)keys[i].time24*10U))>=time){end=i;*selected=(s32)i-1;break;}
 keySeen[end]++;
 if(!end){VectorWords16 position;orbit(&position);publishOrbit(&position);BuildCameraMatrices();return 1;}
 unsigned start=end-1;
 if((s32)start!=RaceContext.camera24C->segment10C){
  RaceContext.camera24C->segment10C=(s32)start;
  if(keys[start].restart28)RaceContext.camera24C->phase108=word((u32)((uint64_t)(u32)keys[start].restart28*10U));
 }
 traceUpdate(RaceContext.camera24C->phase108);
 s32 duration=word((u32)((uint64_t)((u32)keys[end].time24-(u32)keys[start].time24)*10U));
 s32 elapsed=word((u32)time-(u32)((uint64_t)(u32)keys[start].time24*10U));durationSeen[duration>0?2:duration<0?0:1]++;
 VectorWords16 positions[2];unsigned indices[2]={start,end};
 for(unsigned n=0;n<2;n++){
  SceneCameraView24 *v=&keys[indices[n]].view00;
  if(v->absolute18)positions[n]=v->pose00.position00;
  else for(unsigned i=0;i<3;i++)positions[n].words[i]=word((u32)((uint64_t)(u32)car->position20.words[i]*4U));
 }
 absoluteSeen[!!keys[start].view00.absolute18+2U*!!keys[end].view00.absolute18]++;
 SceneCameraView24 *v=&RaceContext.view60;unsigned high=keys[end].time24>=80;endSeen[high]++;
 if(!high){
  if(duration>0){
   for(unsigned i=0;i<3;i++)v->pose00.position00.words[i]=add(positions[0].words[i],interpolate(positions[1].words[i],positions[0].words[i],elapsed,duration));
   Vec3sFields8 a=keys[start].view00.pose00.rotation10,b=keys[end].view00.pose00.rotation10;
   v->pose00.rotation10.x=(s16)(u16)((u32)(s32)a.x+interpolate(b.x,a.x,elapsed,duration));
   v->pose00.rotation10.y=(s16)(u16)((u32)(s32)a.y+interpolate(b.y,a.y,elapsed,duration));
   v->pose00.rotation10.z=(s16)(u16)((u32)(s32)a.z+interpolate(b.z,a.z,elapsed,duration));
   v->distance1C=add(keys[start].view00.distance1C,interpolate(keys[end].view00.distance1C,keys[start].view00.distance1C,elapsed,duration));
   v->projection20=add(keys[start].view00.projection20,interpolate(keys[end].view00.projection20,keys[start].view00.projection20,elapsed,duration));
  }else{
   if(keys[end].view00.absolute18)v->pose00.position00=positions[1];
   else {memcpy(&v->pose00.position00.xyz,&positions[1].xyz,12);undefinedPositionWord=1;}
   v->pose00.rotation10=keys[end].view00.pose00.rotation10;v->distance1C=keys[end].view00.distance1C;v->projection20=keys[end].view00.projection20;
  }
  traceApply(v);return 0;
 }
 VectorWords16 position;orbit(&position);
 if(duration>0){
  memcpy(&v->pose00.position00.xyz,&positions[0].xyz,12);
  v->pose00.rotation10.x=keys[start].view00.pose00.rotation10.x;v->pose00.rotation10.y=keys[start].view00.pose00.rotation10.y;v->pose00.rotation10.z=keys[start].view00.pose00.rotation10.z;
  v->distance1C=keys[start].view00.distance1C;v->projection20=keys[start].view00.projection20;traceApply(v);
  v->pose00.position00=live.scratch.value.projection.pose08.position00;
  v->pose00.rotation10.x=(s16)live.scratch.value.projection.pose08.angles10.xyz.x;v->pose00.rotation10.y=(s16)live.scratch.value.projection.pose08.angles10.xyz.y;v->pose00.rotation10.z=(s16)live.scratch.value.projection.pose08.angles10.xyz.z;
  for(unsigned i=0;i<3;i++)live.scratch.value.projection.pose08.position00.words[i]=add(v->pose00.position00.words[i],interpolate(position.words[i],v->pose00.position00.words[i],elapsed,duration));
  s16 angles[3]={v->pose00.rotation10.x,v->pose00.rotation10.y,v->pose00.rotation10.z};
  for(unsigned i=0;i<3;i++)live.scratch.value.projection.pose08.angles10.words[i]=add(angles[i],interpolate(live.angles.halves[i],angles[i],elapsed,duration));
  v->projection20=add(keys[start].view00.projection20,interpolate(live.projection,keys[start].view00.projection20,elapsed,duration));SetGeomScreen(v->projection20);
 }else publishOrbit(&position);
 BuildCameraMatrices();return 0;
}
static int referenceScene(s32 track){
 SceneCameraState160 *c=RaceContext.camera24C;
 c->distance100=word((u32)c->extentA0-4U-(u32)((s32)CarBoundsTable[0]->unk08+PlayerCars[0].halfSpan10E/3));
 s32 selected[2];int result=traceStep(&PlayerCars[0],c->frame104,track,selected);
 UpdateSparkEffects();ScaleMatrixRow1(2,SCRATCH_ROT_MATRIX);DrawGarageFloorReflection();
 c=RaceContext.camera24C;c->frame104=add(c->frame104,1);c->phase108=add(c->phase108,1);
 if(c->frame104>=802)c->frame104=801;if(c->phase108>=1302)c->phase108=1301;
 if((live.buttons&0x800)&&c->frame104>=51)result=1;return result;
}
static void initialize(unsigned number){
 scenario=number;kind=number%3;serial=0;undefinedPositionWord=0;carIndex=(number/3)%2;trackIndex=(number/54)%3;viewIndex=(number/162)%10;segmentIndex=(number/486)%4;
 u32 seed=number*0x9E3779B9U+0x5642CU;u8 *p=(u8*)&live;for(unsigned i=0;i<sizeof(live);i++)p[i]=(u8)next(&seed);
 rr_vehicle_wire_reset();for(unsigned i=0;i<2;i++){CarRecord *r=rr_vehicle_record_resolve(0x80200000U+i*4096U);PlayerCars[i].control.record18=r;PlayerCars[i].control.handling1C=&r->handling1C8;}
 for(unsigned i=0;i<3;i++){camera(i)->tracks138=live.banks[(number/18+i)%3].rows;live.table.values[i]=&live.bounds[i].value;}
 unsigned root=(number/6)%3,bank=(number/18)%3;RaceContext.camera24C=camera(root);RaceContext.camera24C->tracks138=live.banks[bank].rows;
 live.scratch.value.projection.pointers.cursor=(u8*)live.arena.words+5;live.scratch.value.projection.pointers.order=&live.arena.words[16];
 unsigned end=(number/1944)%24,profile=(number/243)%4;SceneCameraKeyframe2C *keys=live.banks[bank].rows[trackIndex];
 s32 before=profile==2?0x40000000:profile==3?INT32_MIN:-1000;
 for(unsigned i=0;i<24;i++)keys[i].time24=before;
 s32 endTime=profile==2?0x4000000A:profile==3?INT32_MIN+40:(number&2)?100:40;
 if(end){keys[end].time24=endTime;keys[end-1].time24=profile==1?0x40000000:profile==2?0x40000000:profile==3?INT32_MIN+30:endTime-10;}
 for(unsigned i=0;i<24;i++){keys[i].view00.absolute18=(s32)((number/(i%2?27:9))&1);keys[i].restart28=number&8?word(number*0x17171717U):0;}
 rootSeen[root]++;bankSeen[bank]++;rowSeen[trackIndex]++;aliasSeen[segmentIndex]++;mutationSeen[number&1]++;
 static const s32 counters[]={INT32_MIN,-1,0,50,51,800,801,802,1300,1301,1302,INT32_MAX};
 for(unsigned i=0;i<3;i++){camera(i)->frame104=counters[(number/3+i)%12];camera(i)->phase108=counters[(number/36+i)%12];}
 memcpy(&initial,&live,sizeof(live));
}
static s32 timeArgument(void){unsigned profile=(scenario/243)%4;return profile==2?INT32_MIN+50:profile==3?350:(scenario&2)?950:350;}
static u32 run(unsigned reference){
 if(kind==0){if(reference)referenceApply(view());else ApplyCameraState(view());return 0;}
 if(kind==1)return (u32)(reference?referencePhysics(&PlayerCars[carIndex],timeArgument(),trackIndex,segment()):StepCarShowcasePhysics(&PlayerCars[carIndex],timeArgument(),trackIndex,segment()));
 return (u32)(reference?referenceScene(trackIndex):StepCarShowcaseScene(trackIndex));
}
static void normalize(World *w){if(undefinedPositionWord)w->context.value.view60.pose00.position00.words[3]=0;}
unsigned exercise_native(void){
 _Static_assert(sizeof(SceneCameraTrack24)==1056&&sizeof(TrackOwner)==3200,"complete three-row track owner");
 _Static_assert(sizeof(((SceneCameraState160*)0)->tracks138)==sizeof(void*),"full native track pointer");
 for(unsigned n=0;n<49152;n++){
  initialize(n);assert((uintptr_t)RaceContext.camera24C>UINT32_MAX&&(uintptr_t)RaceContext.camera24C->tracks138>UINT32_MAX);
  capture=&expected;capture->count=0;capture->result=run(1);capture->undefinedPositionWord=undefinedPositionWord;memcpy(&capture->final,&live,sizeof(live));
  memcpy(&live,&initial,sizeof(live));serial=0;undefinedPositionWord=0;capture=&actual;capture->count=0;capture->result=run(0);
  assert(actual.count==expected.count&&actual.result==expected.result);undefinedPositionWord=expected.undefinedPositionWord;
  for(unsigned i=0;i<actual.count;i++){
   if(expected.events[i].undefinedPositionWord){normalize(&actual.events[i].entry);normalize(&actual.events[i].after);normalize(&expected.events[i].entry);normalize(&expected.events[i].after);
    if(actual.events[i].operation==7){memset(actual.events[i].inputs[0]+12,0,4);memset(expected.events[i].inputs[0]+12,0,4);}}
   actual.events[i].undefinedPositionWord=expected.events[i].undefinedPositionWord;
   assert(!memcmp(&actual.events[i],&expected.events[i],sizeof(Event)));
  }
  normalize(&live);normalize(&expected.final);assert(!memcmp(&live,&expected.final,sizeof(live)));
 }
 for(unsigned i=0;i<24;i++)assert(keySeen[i]);for(unsigned i=0;i<3;i++)assert(rootSeen[i]&&bankSeen[i]&&rowSeen[i]&&durationSeen[i]);
 for(unsigned i=0;i<4;i++)assert(absoluteSeen[i]&&aliasSeen[i]);for(unsigned i=1;i<12;i++)assert(serviceSeen[i]);
 assert(endSeen[0]&&endSeen[1]&&wrapSeen&&mutationSeen[0]&&mutationSeen[1]);return 49152;
}
unsigned prepare_trace(unsigned n){initialize(n);wire(&live,firstWire);capture=&expected;capture->count=0;capture->result=run(1);capture->undefinedPositionWord=undefinedPositionWord;wire(&live,lastWire);return capture->count;}
const void *initial_wire(void){return firstWire;}const void *final_wire(void){return lastWire;}
const void *event_wire(unsigned i,unsigned after){assert(i<capture->count);wire(after?&capture->events[i].after:&capture->events[i].entry,eventWire);return eventWire;}
const void *event_input(unsigned i,unsigned slot){return capture->events[i].inputs[slot];}const void *event_output(unsigned i){return capture->events[i].output;}
u32 event_value(unsigned i,unsigned field){Event *e=&capture->events[i];return field==0?e->operation:field<5?e->args[field-1]:field<7?e->sizes[field-5]:field==7?e->resultSize:field==8?e->outputSlot:field==9?e->result:e->undefinedPositionWord;}
u32 argument(unsigned slot){return kind==0?pointer(view()):kind==1?(slot==0?pointer(&PlayerCars[carIndex]):slot==1?(u32)timeArgument():slot==2?trackIndex:pointer(segment())):trackIndex;}
unsigned wire_size(void){return WIRE_SIZE;}u32 result(void){return capture->result;}unsigned undefined_word(void){return capture->undefinedPositionWord;}
'''


class SceneCameraTracksTest(unittest.TestCase):
    def test_complete_native_owners_and_original_mips(self):
        owners = source_owners(ROOT/'src/main/PAL')
        bodies = [definition_source(owners[n].read_text(), n) for n in NAMES]
        helper = definition_source(owners[NAMES[1]].read_text(), 'InterpolateCameraDelta')
        bodies[0] = bodies[0].replace('RotMatrixZYX(', 'traceRotZYX(')
        bodies[1] = bodies[1].replace('RotMatrix(', 'traceRot(').replace('UpdateCarShowcaseCamera(', 'traceUpdate(').replace('ApplyCameraState(', 'traceApply(')
        bodies[2] = bodies[2].replace('StepCarShowcasePhysics(', 'traceStep(')
        prefix = PREFIX.replace('@RACE@', hex(address_of('RaceContext'))+'U').replace('@CARS@', hex(address_of('PlayerCars'))+'U')
        with TemporaryDirectory() as temporary:
            source, library = Path(temporary)/'native.c', Path(temporary)/'native.dylib'
            source.write_text(prefix+helper+'\n'.join(bodies)+REFERENCE)
            subprocess.run(['cc', '-std=c11', '-O2', '-shared', '-fPIC', '-Wall', '-Wextra', '-Werror',
                            '-Wno-unused-variable', '-fsanitize=undefined,bounds', '-fno-sanitize-recover=all',
                            '-I', str(ROOT/'include'), '-I', str(ROOT/'tools/scripts'), str(source), '-o', str(library)], check=True)
            native = C.CDLL(str(library));native.exercise_native.restype = C.c_uint
            self.assertEqual(native.exercise_native(), 49152)
            for name in ['initial_wire', 'final_wire', 'event_wire', 'event_input', 'event_output']:
                getattr(native, name).restype = C.c_void_p
            for name, arguments in [('prepare_trace', 1), ('event_wire', 2), ('event_input', 2), ('event_output', 1), ('event_value', 2), ('argument', 1)]:
                getattr(native, name).argtypes = [C.c_uint]*arguments
            for name in ['prepare_trace', 'wire_size', 'event_value', 'argument', 'result', 'undefined_word']:
                getattr(native, name).restype = C.c_uint32
            regions = [(address_of('RaceContext')-16, 0x564+32), (0x80180000, 384), (0x80181000, 384),
                       *((0x801B0000+b*65536, 3200) for b in range(3)), (address_of('PlayerCars')-16, 1632),
                       *((0x801A0000+b*4096, 44) for b in range(3)), (address_of('CarBoundsTable')-16, 44),
                       (0x1F7FFFF0, 104), (0x801F0000, 160), (address_of('D_800A0FC0'), 28),
                       (address_of('Pad0Buttons'), 2), (0x801E0000, 36)]
            size = native.wire_size();self.assertEqual(size, sum(n for _, n in regions))
            intervals = sorted((a, a+n) for a, n in regions)
            self.assertTrue(all(a[1] <= b[0] for a, b in zip(intervals, intervals[1:])))
            memory = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32)
            memory.mem_map(0, 0x400000);memory.mem_map(0x1F7FF000, 0x2000)
            write = lambda a, b: memory.mem_write(a & 0x1FFFFFFF, bytes(b))
            read = lambda a, n: bytes(memory.mem_read(a & 0x1FFFFFFF, n))
            original = (ROOT/'assets/PAL/main.exe').read_bytes();base = struct.unpack_from('<I', original, 24)[0];write(base, original[0x800:])
            if os.getenv('RR_COMPILED_ELF'):
                with open(os.environ['RR_COMPILED_ELF'], 'rb') as stream:
                    elf = ELFFile(stream)
                    for name in NAMES:
                        symbol = elf.get_section_by_name('.symtab').get_symbol_by_name(name)[0]
                        section = elf.get_section(symbol['st_shndx']);offset = symbol['st_value']-section['sh_addr']
                        self.assertEqual(symbol['st_value'], address_of(name));write(symbol['st_value'], section.data()[offset:offset+symbol['st_size']])

            def store(cpu, access, address, length, value, user):
                address &= 0x1FFFFFFF
                self.assertTrue(any((a & 0x1FFFFFFF) <= address and address+length <= (a & 0x1FFFFFFF)+n
                                    for a, n in regions+[(STACK-168, 168)]), (hex(address), length))

            cpu = CameraCpu(memory, 0, 100, 101, 102, store)
            # Cross-check the signed DIV extension against independent Unicorn
            # execution of a straight block, including negative remainders.
            block = struct.pack('<III', (4 << 21) | (5 << 16) | 26, (2 << 11) | 18, (3 << 11) | 16)
            divider = Uc(UC_ARCH_MIPS, UC_MODE_LITTLE_ENDIAN | UC_MODE_MIPS32);divider.mem_map(0, 4096)
            divider.mem_write(0, block);write(0x80000800, block)
            for number in range(1024):
                a = (number*0x9E3779B9) & 0xFFFFFFFF;b = ((number*0x17171717) & 0xFFFFFFFF) or 1
                divider.reg_write(UC_MIPS_REG_A0, a);divider.reg_write(UC_MIPS_REG_A1, b);divider.emu_start(0, 12, count=3)
                cpu.reset();cpu.r[4], cpu.r[5], cpu.pc = a, b, 0x80000800
                for _ in range(3):cpu.step()
                self.assertEqual((cpu.r[2], cpu.r[3]), (divider.reg_read(UC_MIPS_REG_V0), divider.reg_read(UC_MIPS_REG_V1)))

            def write_state(data):
                offset = 0
                for address, length in regions:
                    write(address, data[offset:offset+length]);offset += length

            def state():
                return b''.join(read(a, n) for a, n in regions)

            def normalized(data, undefined):
                if not undefined:return data
                data = bytearray(data);data[16+0x60+12:16+0x60+16] = bytes(4);return bytes(data)

            services = {address_of(n): i+1 for i, n in enumerate(SERVICES)}
            for case in range(1536):
                number = (case*7919) % 49152;kind = number % 3;events = native.prepare_trace(number)
                arguments = [native.argument(i) for i in range([1, 4, 1][kind])]
                write_state(C.string_at(native.initial_wire(), size));write(STACK-256, b'\xA5'*512)
                cpu.reset();cpu.r = [0]+[0xBEEF0000+r for r in range(1, 32)];cpu.r[4:4+len(arguments)] = arguments
                cpu.r[29], cpu.r[31], cpu.pc = STACK, STOP, address_of(NAMES[kind]);event = 0
                for instruction in range(8000):
                    if cpu.pc == STOP:break
                    if cpu.pc not in services or cpu.pc == address_of(NAMES[kind]):cpu.step();continue
                    operation = services[cpu.pc];self.assertLess(event, events, (number, operation))
                    self.assertEqual(operation, native.event_value(event, 0), (number, event))
                    undefined = native.event_value(event, 10)
                    self.assertEqual(normalized(state(), undefined), normalized(C.string_at(native.event_wire(event, 0), size), undefined), (number, event, 'entry'))
                    count = [2, 2, 3, 1, 0, 1, 1, 4, 0, 2, 0][operation-1]
                    pointers = {1: (0, 1), 2: (0, 1), 3: (0, 1, 2), 7: (0,), 8: (0, 3), 10: (1,)}.get(operation, ())
                    for slot in range(count):
                        expected = native.event_value(event, slot+1)
                        if slot in pointers and not expected:
                            self.assertTrue(STACK-168 <= cpu.r[4+slot] < STACK, (number, operation, slot, hex(cpu.r[4+slot])))
                        else:self.assertEqual(cpu.r[4+slot], expected, (number, event, slot))
                    for slot in range(2):
                        length = native.event_value(event, slot+5)
                        if not length:continue
                        address = cpu.r[4+(1 if operation == 10 else slot)]
                        if operation == 7:
                            data = read(address, 24)+read(address+28, 8);expected = C.string_at(native.event_input(event, slot), 32)
                            if undefined:data=data[:12]+bytes(4)+data[16:];expected=expected[:12]+bytes(4)+expected[16:]
                        else:data=read(address, length);expected=C.string_at(native.event_input(event, slot), length)
                        self.assertEqual(data, expected, (number, event, 'input', slot))
                    length = native.event_value(event, 7);output_slot = native.event_value(event, 8)
                    if length:write(cpu.r[4+output_slot], C.string_at(native.event_output(event), length))
                    if operation == 8:write(cpu.r[7], struct.pack('<I', (number*0x17171717) & 0xFFFFFFFF))
                    write_state(C.string_at(native.event_wire(event, 1), size));result = native.event_value(event, 9)
                    resume = cpu.r[31]
                    for register in (1, 2, 3, *range(4, 16), 24, 25):cpu.r[register] = (0xA1230000+event*257+register) & 0xFFFFFFFF
                    cpu.r[2] = result;cpu.pc = resume;cpu.hi = 0x12345678;cpu.lo = 0x87654321;event += 1
                self.assertEqual(cpu.pc, STOP, number);self.assertEqual(event, events)
                undefined = native.undefined_word()
                self.assertEqual(normalized(state(), undefined), normalized(C.string_at(native.final_wire(), size), undefined), (number, 'final'))
                if kind:self.assertEqual(cpu.r[2], native.result(), (number, 'return'))
                self.assertEqual(cpu.r[29], STACK);self.assertEqual(cpu.r[31], STOP)
                for register in (*range(16, 24), 28, 30):self.assertEqual(cpu.r[register], 0xBEEF0000+register)
                self.assertEqual(read(STACK-256, 88), b'\xA5'*88);self.assertEqual(read(STACK, 256), b'\xA5'*256)
            print('PASS 49152 native and 1536', 'compiled' if os.getenv('RR_COMPILED_ELF') else 'original',
                  'MIPS camera-track cases; complete rows, live roots, pointer widths, wrapped interpolation, guards and UBSan')


if __name__ == '__main__':
    unittest.main()
