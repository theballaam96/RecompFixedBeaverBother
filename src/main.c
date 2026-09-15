#include "modding.h"
#include "ultra64.h"
#include "enums.h"
#include "common_structs.h"

typedef struct Struct807FDC90 Struct807FDC90;

// Use this for D_global_asm_807FDC90
struct Struct807FDC90 {
    Struct807FDC90 *unk0; // TODO: This type may not be correct
    Actor *unk4;
    s16 unk8;
    s16 unkA; // Used: X Position
    s16 unkC; // Used
    s16 unkE; // Used: Z Position
    s16 unk10; // Used
    s16 unk12;
    s16 unk14; // Used
    s16 unk16;
    u16 unk18;
    u16 unk1A; // Used
    union {
        struct {
            u16 unk1C; // Used
            u8 unk1E;
            u8 unk1F;
        };
        s32 unk1C_s32;
    };
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    u8 unk24;
    u8 unk25;
    s16 unk26; // Used
    s32 unk28;
    s16 unk2C;
    s16 unk2E; // TODO: This might not be correct
    f32 unk30; // Used
    u8 unk34;
    u8 unk35; // Used
    u8 unk36;
    u8 unk37;
    u8 unk38;
    u8 unk39; // Don't think this is used?
    u8 unk3A; // Also don't think this is used?
};

void initializeCharacterSpawnerActor(void);
void func_global_asm_80724CA4(s16 arg0, s16 arg1);
void func_global_asm_8072B79C(s16 arg0, s16 arg1, s16 arg2);
Actor *func_global_asm_8067ADB4(u16);
void func_global_asm_806AD260(s16 arg0, u8 arg1, s32 arg2);
void func_global_asm_80724E48(u8 arg0);
void renderActor(Actor *arg0, u8 arg1);
void func_global_asm_80613194(Actor *actor, s16 arg1);
extern Maps current_map;
extern Actor *gCurrentActorPointer;
extern CharacterSpawner *D_global_asm_807FDC9C;
extern Struct807FDC90 *D_global_asm_807FDC90;
extern u16 enemies_killed;

#define SCARE_DURATION 20

RECOMP_PATCH void func_global_asm_806AD54C(void) {
    f32 dx, dz;
    Actor *klaptrap;
    s32 angleSign;

    initializeCharacterSpawnerActor();
    if (ACTOR_UNINITIALIZED(gCurrentActorPointer)) {
        func_global_asm_80724CA4(2, 1);
        func_global_asm_8072B79C(0x1F8, 0x1F8, 0x1F9);
        D_global_asm_807FDC90->unk3A = FALSE;
        if (current_map == MAP_BEAVER_BOTHER_EASY || current_map == MAP_BEAVER_BOTHER_NORMAL || current_map == MAP_BEAVER_BOTHER_HARD) {
            gCurrentActorPointer->unk132 = 2;
            if (RandChance(0.2)) {
                D_global_asm_807FDC90->unk3A = TRUE;
                func_global_asm_80613194(gCurrentActorPointer, 0x1B); // Gold Beaver
            }
        }
    }
    if ((gCurrentActorPointer->unk132 == 2) && (gCurrentActorPointer->control_state != 0x36)) {
        klaptrap = func_global_asm_8067ADB4(0x11A);
        if (klaptrap) {
            if (klaptrap->control_state != 0) {
                // Barking
                if (!(D_global_asm_807FDC90->unk1A & 0x8000)) {
                    dx = klaptrap->x_position - gCurrentActorPointer->x_position;
                    dz = klaptrap->z_position - gCurrentActorPointer->z_position;
                    if ((SQ(dx) + SQ(dz)) < SQ(40.0f)) {
                        angleSign = ((klaptrap->y_rotation - gCurrentActorPointer->y_rotation) & 0xFFF) >= 0x801 ? 1 : -1;
                        gCurrentActorPointer->unkB8 = D_global_asm_807FDC9C->unkD * 1.8;
                        gCurrentActorPointer->y_rotation += angleSign * 400;
                        gCurrentActorPointer->unkEE = gCurrentActorPointer->y_rotation;
                        D_global_asm_807FDC90->unk1A |= 0x8000;
                        gCurrentActorPointer->control_state = 0x10;
                        gCurrentActorPointer->control_state_progress = 0;
                        gCurrentActorPointer->noclip_byte = 0x23; // Give the beavers noclip. This should make it easier for them to go through "the wall"
                        D_global_asm_807FDC90->unk39 = SCARE_DURATION;
                    }
                }
            } else if (D_global_asm_807FDC90->unk39 == 0) {
                // Not Snapping
                D_global_asm_807FDC90->unk1A &= ~0x8000;
                gCurrentActorPointer->noclip_byte = 0x24; // Remove noclip from the beavers
            }
            if (D_global_asm_807FDC90->unk39 > 0) {
                D_global_asm_807FDC90->unk39--;
            }
        }
    }
    if (D_global_asm_807FDC90->unk3A) {
        // Is Gold Beaver
        if ((gCurrentActorPointer->unk132 == 2) && (gCurrentActorPointer->control_state != 0x36)) {
            if (gCurrentActorPointer->control_state == 0x40) { // Has been killed
                enemies_killed++;
            }
        }
    }
    func_global_asm_806AD260(0x1FA, 0, 0);
    func_global_asm_80724E48(gCurrentActorPointer->control_state == 0x23 ? 0 : 2);
    renderActor(gCurrentActorPointer, 0);
}