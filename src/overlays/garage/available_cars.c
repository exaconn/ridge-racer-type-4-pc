#include "rr/overlay662_available_cars.h"
#include "rr/load_cursor.h"
#include "rr/overlay662_car_bars.h"
#include "rr/frame_storage_types.h"
#include "game/system/pad.h"
#include "game/car/carwheel.h"
#include "rr/player_cars.h"
#include "rr/preview_rgb.h"
#include "rr/ui.h"

extern u8 *ImageRows;
extern s16 FadeCountdown;
extern s16 ovl662_saved_car_choice_idx;
void ovl662_init_team_car_layout(void);
void SetupMenuCarCamera(s32);
void SetupViewport(s32);
s32 LoadCarRangeStep(s32, s32);
void SetCarBounds(RaceVehicle *);
void SetMenuFrameOptions(s32, s32, s32);
void StepMenuValue(s32, s32, char *, s32, s32);
void PlaySfx(s32);
void SetModelTexturePage(s32);
void AddScreenFadeTiles(u32 *, s32, u32);
void DrawUiRow(s32);
void DrawSelectionIconGrid(s32);
void UpdateCarWorldTransform(RaceVehicle *);
void update_and_draw_car(RaceVehicle *, s32, s32, s32);
void UpdateBadgeRowAnim(s32);
s32 UpdateImageRow(s32);
void DrawMenuStepArrows(s32, s32, s32, s32, s32, s32, s32);
void UpdateScreenFade(s32);

void ovl662_init_available_cars(void){
 u8 parts[3];
 s32 decoded;
 s32 arenaOffset;
 s32 i,count;s32 j;
 s16 *status;GarageAvailableCar *entry;
 u8 *choice;
 GarageAvailableCar *captured;
 ModeFrame=0;
 ScreenFrame=0;
 ovl662_menu_substate=0;
 ovl662_selected_car_slot=0;
 MenuCursor=0;
 MenuItemCount=2;
 ResetScreenCounters();
 arenaOffset=0x2D000;i=0;count=i;
 MenuArenas.second=CdDirectoryBuffer+arenaOffset;
 InitMenuArenas();LoadCursor=MenuArenas.second;
 InitUiRow(0,24,88,68,32,0,0,1,0,83);
 InitUiRow(1,24,128,68,32,0,0,1,0,85);
 InitPreviewImageRows();InitBadgeRow(1,24,192,84,34,0,0x201);SetupRenderer(5);
 status=ovl662_car_slot_statuses;entry=ovl662_available_car_slots;
 do{
  SelectionRows.choice.bytes[2]=i;
  *status=(SaveProgress.prefix.progress.progress10[SelectionRows.choice.bytes[0]][SelectionRows.choice.bytes[1]]>>i)&1;
  if(*status!=0){
   entry->value=PackChoiceGlyph(&SelectionRows.choice);
   j=0;
   captured=entry;
   choice=(u8*)&CourseChoiceTable;
   do{
    UnpackChoiceGlyph((s8*)parts,((CourseChoice4*)(choice+36))->glyph);
    if(parts[2]<39)parts[2]%=20;
    decoded=PackChoiceGlyph(parts);
    if(captured->value==(s16)decoded)*status=2;
    j++;choice+=4;
   }while(j<8);
   entry++;count++;
  }
  i++;status++;
 }while(i<20);
 ovl662_available_car_count=count;
 UnpackChoiceGlyph((s8*)&SelectionRows.choice,ovl662_available_car_slots[0].value);
 PlayerCourseChoices[0].first=0;PlayerCourseChoices[0].second=0;
 ModeGroup=1;ModeIndex=67;SubmodeState=0;
 LoadCursor=MenuArenaBase;MenuArenas.first=MenuArenaBase;
}

void ovl662_draw_car_slot_bars(s32 selected)
{
#ifndef RR_GPU_SCRATCH
#define RR_GPU_SCRATCH ((GpuScratchPointers *)0x1F800000U)
#endif
    UiUvRect strip;
    void *result;
    u32 *head;
    void *initial;
    s32 x = 40, index, r, g, b;
    s32 y = 288;
    s16 *status;
    void *cursor;

    strip.v = 64;
    strip.w = 240;
    strip.h = 24;
    strip.u = 0;
    head = &CurrentDrawBuf->buffer.ordering70.words[0];
    initial = RR_GPU_SCRATCH->cursor;
    cursor = AddSpritePrim(head, initial, 40, y, &strip, 0x7FA4, 1);

    index = 0;
    status = ovl662_car_slot_statuses;
    do {
        if (*status != 0) {
            r = 255;
            if (index == selected) {
                r = 240;
                g = 122;
                b = 20;
            } else {
                g = 189;
                b = 0;
            }
            cursor = AddTilePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor,
                                  x + 1, y, 10, 22, r, g, b);
        }
        x += 12;
        index++;
        status++;
    } while (index < 20);

    result = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], cursor, 0x39);
    RR_GPU_SCRATCH->cursor = result;
    if (ovl662_car_slot_statuses[(SelectionRows.choice.bytes[2] % 20) & 255] == 2) {
        result = AddSpritePrim(&CurrentDrawBuf->buffer.ordering70.words[0], result, 142, 256,
                              &ovl662_car_slot_bar_uv, 0x7CBB, 0);
        RR_GPU_SCRATCH->cursor = AddDrawModePrim(&CurrentDrawBuf->buffer.ordering70.words[0], result, 0x38);
    }
}

void ovl662_update_available_cars(void)
{
    PreviewColor color;
    s8 shown;
    s32 ready;
    s32 before;
    s32 status;
    s32 state;
    s32 cell;
    s32 i;
    s32 fadeMode;
    s16 steps;
    s32 fadeLevel;
    s8 shade;
    RaceVehicle *vehicle;

    shown = 0;
    ModeFrame++;
    vehicle = PlayerCars;
    ScreenFrame++;
    vehicle->rot54 = (vehicle->rot54 + 8) & 0xFFF;
    SetupMenuCarCamera(1);
    SetupViewport(6);
    ready = LoadCarRangeStep(0, 1);
    if (ready != 1) {
        if ((s8)ImageRows[1] == 1) {
            ImageRows[1] = 0;
        }
    } else {
        SetCarBounds(vehicle);
        if ((s8)ImageRows[1] == 7) {
            ImageRows[1] = ready;
        }
        shown = 1;
        switch (SubmodeState) {
        case 0:
            SetMenuFrameOptions(1, 1, 1);
            before = PackChoiceGlyph((s8 *)&SelectionRows.choice);
            StepMenuValue(3, 2, (char *)&ovl662_selected_car_slot, ovl662_available_car_count, 0);
            UnpackChoiceGlyph((s8 *)&SelectionRows.choice, ovl662_available_car_slots[ovl662_selected_car_slot].value);
            status = ovl662_car_slot_statuses[(SelectionRows.choice.bytes[2] % 20) & 0xFF];
            if ((s16)before != (s16)PackChoiceGlyph((s8 *)&SelectionRows.choice)) {
                ResetScreenCounters();
                LoadCursor = MenuArenas.first;
                UnpackChoiceGlyph((s8 *)&SelectionRows.choice, ovl662_available_car_slots[ovl662_selected_car_slot].value);
                ImageRows[1] = 6;
                PlayerCourseChoices[0].first = 0;
                PlayerCourseChoices[0].second = 0;
            }
            UiRows[MenuCursor].cells[0] = 0;
            StepMenuValue(0, 2, (char *)&MenuCursor, 2, 0);
            cell = 2;
            if (status != 1) {
                UiRows[0].half20 = 1;
                if (MenuCursor == 0) {
                    cell = 4;
                }
            } else {
                UiRows[0].half20 = 0;
            }
            UiRows[MenuCursor].cells[0] = cell;
            if (UiRows[0].byte14 != 2) {
                break;
            }
            state = BadgeRows[0].state10;
            if (state != 3) {
                break;
            }
            if (PlayerControllers[0].action28) {
                SetMenuFrameOptions(0, 1, 1);
                UiRows[MenuCursor].cells[0] = 1;
                if (MenuCursor == 0) {
                    if (status == 1) {
                        PlaySfx(1);
                        SetModelTexturePage(-1);
                        SubmodeState = 3;
                        BadgeRows[0].state10 = 4;
                        BadgeRows[1].state10 = 4;
                        ImageRows[1] = 3;
                        for (i = 0; i < 4; i++) {
                            if (UiRows[i].byte14 == 2) {
                                UiRows[i].byte14 = 3;
                            }
                        }
                        FadeCountdown = 64;
                    } else {
                        PlaySfx(4);
                    }
                } else if (MenuCursor == 1) {
                    SetModelTexturePage(-1);
                    PlaySfx(1);
                    BadgeRows[0].state10 = 4;
                    BadgeRows[1].state10 = 4;
                    ImageRows[1] = state;
                    for (i = 0; i < UiRowCount; i++) {
                        if (UiRows[i].byte14 == 2) {
                            UiRows[i].byte14 = 3;
                        }
                    }
                    FadeCountdown = 64;
                    SubmodeState = 8;
                }
            } else if (PlayerControllers[0].action2C) {
                PlaySfx(2);
                SetModelTexturePage(-1);
                SetMenuFrameOptions(0, 1, 1);
                BadgeRows[0].state10 = 4;
                BadgeRows[1].state10 = 4;
                ImageRows[1] = state;
                for (i = 0; i < UiRowCount; i++) {
                    if (UiRows[i].byte14 == 2) {
                        UiRows[i].byte14 = 3;
                    }
                }
                FadeCountdown = 64;
                SubmodeState = 8;
            }
            break;
        case 3:
            FadeCountdown--;
            steps = 64;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (steps - FadeCountdown) * 4, 69);
            if (FadeCountdown <= 0) {
                CourseChoiceTable[1][ovl662_saved_car_choice_idx].glyph = ovl662_available_car_slots[ovl662_selected_car_slot].value;
                CourseChoiceTable[1][ovl662_saved_car_choice_idx].first = 0;
                CourseChoiceTable[1][ovl662_saved_car_choice_idx].second = 0;
                ovl662_selected_car_slot = ovl662_saved_car_choice_idx;
                ovl662_init_team_car_layout();
                ModeGroup = ready;
                ModeIndex = steps;
                SubmodeState = 0;
                return;
            }
            break;
        case 8:
            FadeCountdown--;
            steps = 64;
            AddScreenFadeTiles(CurrentDrawBuf->ordering.ordering70.words, (steps - FadeCountdown) * 4, 69);
            if (FadeCountdown <= 0) {
                ovl662_selected_car_slot = ovl662_saved_car_choice_idx;
                ovl662_init_team_car_layout();
                ModeGroup = ready;
                ModeIndex = steps;
                SubmodeState = 0;
                return;
            }
            break;
        }
    }
    if (ovl662_menu_substate == 0 && (s8)ImageRows[1] == 2) {
        ovl662_menu_substate = 1;
        BadgeRows[0].half02 = 1;
        BadgeRows[1].half02 = 1;
    }
    for (i = 0; i < 2; i++) {
        DrawUiRow(i);
    }
    if (BadgeRows[1].state10 == 3) {
        ovl662_draw_car_slot_bars(SelectionRows.choice.bytes[2]);
    }
    i = shown;
    if ((u8)(ImageRows[1] - 1) < 2 && i == 1) {
        DrawSelectionIconGrid(0);
        UpdateCarWorldTransform(PlayerCars);
        fadeLevel = 0x100;
        if ((s8)ImageRows[1] == i) {
            shade = ~((ImageRows[2] * 255) / 30);
            color.channels.r = shade;
            color.channels.g = shade;
            color.channels.b = shade;
            fadeMode = 0x102;
            fadeLevel = (ImageRows[2] << 8) / 30;
        } else {
            fadeMode = 1;
            color.channels.r = 0x80;
            color.channels.g = 0x80;
            color.channels.b = 0x80;
        }
        DrawPreviewCar(PlayerCars, color.argument, fadeLevel, fadeMode);
    }
    UpdateBadgeRowAnim(0);
    UpdateBadgeRowAnim(1);
    if (UpdateImageRow(0) > 0) {
        DrawMenuStepArrows(160, 144, 8, 1, ovl662_selected_car_slot, -1, ovl662_available_car_count + 1);
    }
    UpdateScreenFade(0x1008E);
}
