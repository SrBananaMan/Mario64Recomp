#include "common.h"

/* rodata */
extern char D_801103D0[];

extern f64 D_801104F8;
extern Collider D_80236980[128];
extern s32 D_8020D8F4;
extern f64 D_801106A0;
extern f64 D_801106A8;

extern Vec3f D_80108F9C;
extern Vec3f D_80108FA8;
extern Vec3f D_80108FB4;
extern Vec3f D_80108FC0;
extern f32 D_80108FCC;
extern s32 D_80108FD0;
extern s32 D_80108FD4;
extern f32 D_80108FD8;
extern f32 D_80108FDC;
extern s32 D_80108FE0;
extern f32 D_80108FE4;
extern s32 D_80108FE8;
extern s32 D_80108FEC;
extern Vec3f D_802489C8[8];

/* Migrated BSS */
//TODO: type this data correctly
Shadow gShadows[64];
s32 gShadowCount;
Vec3f D_80248518;

typedef struct UnkPolyStruct {
    void* unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
    f32 unk18;
} UnkPolyStruct;

UnkPolyStruct D_80248528[32];

typedef struct UnkMinDistance {
    f32 temp; //unused?
    f32* unk_04;
    s32* unk_08;
    Vec3f* vec;
} UnkMinDistance;

typedef struct UnkArg4 {
    Poly* unk0;
    s32 unk4;
    char unk8[8];
    Vec3f unk10;
} UnkArg4;

s32 D_802488A8;
char D_802488B0[0x10];
char gHasShadow[0x100];
char D_802489C0[0x08];
Vec3f D_802489C8[8];
char D_80248A28[0x08];

void func_800D5394(PlayerActor*, Tongue*, Camera*, Vec3f*, Vec3f*, s32);
void func_800D6864(PlayerActor*, Tongue*, Camera*, Vec3f*, Vec3f*);
Collider* func_800CAF88(Vec3f, f32, f32);
Collider* SearchPolygonBetween(Vec3f, Vec3f, s32, s32, s32);
void OrderRectBounds(Rect3D*);
void func_800C9748(Rect3D*, s32, s32);
void func_800CA734(Vec3f*, Vec3f, f32, s32);
void func_800CBC08(Actor*);
void func_800CC814(Actor*, Vec3f, s32);
Vec3f* WorldToLocal(Vec3f* outVec, Vec3f vec, Poly* poly);
Vec3f* LocalToWorld(Vec3f* outVec, Vec3f vec, Poly* poly);

void ClearPolygon(void) {
    D_80236968 = 0;
}

const char D_80110180[] = "\n";

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800C8F0C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800C9504.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800C9600.s")

void func_800C9704(Collider* arg0) {
    D_8023696C = 0;
    func_800C9504(arg0);
}

void func_800C9728(Collider* arg0) {
    func_800C9504(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800C9748.s")

s32 func_800C982C(Rect3D* arg0, Collider* arg1) {
    s32 var_s3;
    Collider* temp_s0;
    Collider** var_s2;
    s32 i;

    var_s3 = 0;
    D_8023696C = 0;
    
    for (i = 0, var_s2 = &D_80240898; i < gFieldCount; i++, var_s2++){
        temp_s0 = *var_s2;
        if (((temp_s0->unk_10 != 0x21) || (arg1->unk_6C == 0)) && (temp_s0->unk_114 & 2) && (temp_s0->unk_0C & 0x77)) {
            if (IfRectsIntersect(arg0, &temp_s0->unk_CC) == 0) {
                //yes...this is required
                continue;
            } else {
                func_800C9504(temp_s0);
                var_s3++;
            }
        }        
    }
    return var_s3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800C9928.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800C9A24.s")

// Checks if Poly's bounding box intersects with the given rectangle
s32 IfPolyBoundIntersectsRect(Poly* poly, Rect3D* rect) {
    if (poly->unk_00 < 0) {
        return 0;
    }

    func_800D79E4(poly, 1);

    if (IfRectsIntersect(rect, &poly->boundBox) == 0) {
        return 0;
    }
    return 1;
}

f32 Distance3DTo2DProjectedXY(Vec3f vecA, Vec2f vecB) {
    f32 deltaX;
    f32 deltaY;

    deltaX = vecA.x - vecB.x;
    deltaY = vecA.y - vecB.y;
    return NORM_3(deltaX, deltaY, vecA.z);
}

void DistanceWithLine(Vec3f point, Vec2f lineStart, Vec2f lineEnd, f32* distance, Vec3f* closestPoint3D, Poly* polygon) {
    f32 deltaYLine;
    f32 deltaXLine;
    f32 lineSegmentLengthSquared;
    f32 sp5C;  // Unused
    f32 deltaYPoint;
    f32 deltaXPoint;
    f32 sp58;  // Unused
    f32 sp54;  // Unused
    Vec2f closestPoint2D;
    Vec3f closestPoint3DLocal;
    f32 projectionFactor;

    deltaXPoint = point.x - lineStart.x;
    deltaYPoint = point.y - lineStart.y;
    deltaXLine = lineEnd.x - lineStart.x;
    deltaYLine = lineEnd.y - lineStart.y;

    lineSegmentLengthSquared = (SQ(deltaXLine)) + (SQ(deltaYLine));
    
    if (lineSegmentLengthSquared == 0.0f) {
        DummiedPrintf3("DistanceWithLine(): determinant is 0\n");
    }
    projectionFactor = ((deltaXLine * deltaXPoint) + (deltaYLine * deltaYPoint)) / lineSegmentLengthSquared;
    
    if (projectionFactor <= 0.0f) {
        closestPoint2D = lineStart;
    } else if (projectionFactor >= 1.0f) {
        closestPoint2D = lineEnd;
    } else {
        closestPoint2D.x = ((1.0f - projectionFactor) * lineStart.x) + (projectionFactor * lineEnd.x);
        closestPoint2D.y = ((1.0f - projectionFactor) * lineStart.y) + (projectionFactor * lineEnd.y);
    }
    
    *distance = Distance3DTo2DProjectedXY(point, closestPoint2D);

    closestPoint3DLocal.x = closestPoint2D.x;
    closestPoint3DLocal.y = closestPoint2D.y;
    closestPoint3DLocal.z = 0.0f;
    LocalToWorld(closestPoint3D, closestPoint3DLocal, polygon);
}

void MinimunDistance(Vec3f arg0, Poly* polygon, UnkMinDistance arg5) {
    f32 temp_f0;
    f32 temp_f2;
    u32 var_v0;

    WorldToLocal(&arg0, arg0, polygon);
    var_v0 = 0;
    temp_f0 = (polygon->unk_74 * arg0.y) + (polygon->unk_6C * arg0.x);
    temp_f2 = (polygon->unk_78 * arg0.y) + (polygon->unk_70 * arg0.x);

    if (temp_f0 < 0.0f) {
        var_v0 = 1;
    }
    if (temp_f2 < 0.0f) {
        var_v0 += 2;
    }
    if ((temp_f0 + temp_f2) > 1.0f) {
        var_v0 += 4;
    }

    switch (var_v0) {
    case 0:
        *arg5.vec = arg0;
        arg5.vec->z = 0.0f;
        LocalToWorld(arg5.vec, *arg5.vec, polygon);
        *arg5.unk_04 = arg0.z;
        *arg5.unk_08 = 0;
        return;
    case 3:
        *arg5.vec = polygon->offset;
        *arg5.unk_04 = Distance3DTo2DProjectedXY(arg0, polygon->unk_7C);
        *arg5.unk_08 = 1;
        return;
    case 6:
        *arg5.vec = polygon->unkVec;
        *arg5.unk_04 = Distance3DTo2DProjectedXY(arg0, polygon->unk_84);
        *arg5.unk_08 = 1;
        return;
    case 5:
        *arg5.vec = polygon->unkVec2;
        *arg5.unk_04 = Distance3DTo2DProjectedXY(arg0, polygon->unk_8C);
        *arg5.unk_08 = 1;
        return;
    case 2:
        DistanceWithLine(arg0, polygon->unk_7C, polygon->unk_84, arg5.unk_04, arg5.vec, polygon);
        *arg5.unk_08 = 1;
        return;
    case 4:
        DistanceWithLine(arg0, polygon->unk_84, polygon->unk_8C, arg5.unk_04, arg5.vec, polygon);
        *arg5.unk_08 = 1;
        return;
    case 1:
        DistanceWithLine(arg0, polygon->unk_8C, polygon->unk_7C, arg5.unk_04, arg5.vec, polygon);
        *arg5.unk_08 = 1;
        return;
    default:
        DummiedPrintf3("MinimunDistance(): invalid case\n");
        return;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/ListUpTouchedPolygon.s")

UnkPolyStruct* func_800CA3FC(void) {
    UnkPolyStruct* curUnkPoly;
    UnkPolyStruct* nextUnkPoly;
    f32 temp_f0;
    f32 temp_f2;
    s32 i;

    if (D_802488A8 == 0) {
        return NULL;
    }
    
    if (D_802488A8 == 1) {
        return D_80248528;
    }
    
    nextUnkPoly = &D_80248528[1];
    curUnkPoly = D_80248528;
    
    for (i = 1; i < D_802488A8; i++, nextUnkPoly++) {
        temp_f0 = curUnkPoly->unk8;
        temp_f2 = nextUnkPoly->unk8;
        if (!(temp_f0 < temp_f2) && ((temp_f0 != temp_f2) || (nextUnkPoly->unk4 == 0) || (curUnkPoly->unk4 == 0) || !(nextUnkPoly->unkC < curUnkPoly->unkC))) {
            curUnkPoly = nextUnkPoly;
        }        
    }
    return curUnkPoly;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CA4BC.s")

Vec3f* func_800CA5B4(Vec3f* arg0, Vec3f arg1, UnkArg4* arg4, f32 arg5) {
    Vec3f sp54;
    f32 temp_f14;
    f32 temp_f18;
    f32 temp_f0;
    f32 pad;
    f32 temp_f2;
    Vec3f sp34;
    Poly* polygon;

    if (arg4->unk4) {
        sp54 = arg4->unk10;
        temp_f0 = arg1.x - sp54.x;
        temp_f18 = arg1.y - sp54.y;
        temp_f14 = arg1.z - sp54.z;
        temp_f2 = arg5 / NORM_3(temp_f0, temp_f18, temp_f14);
        
        temp_f0 *= temp_f2;
        arg1.x = sp54.x + (temp_f0);
        
        temp_f18 *= temp_f2;
        arg1.y = sp54.y + (temp_f18);
        
        temp_f14 *= temp_f2;
        arg1.z = sp54.z + (temp_f14);
    } else {
        polygon = arg4->unk0;
        WorldToLocal(&sp34, arg1, polygon);
        sp34.z = arg5;
        LocalToWorld(&arg1, sp34, polygon);
    }
    *arg0 = arg1;
    return arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CA734.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/SearchPolygonBetween.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CAE08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CAF88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CB294.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Reset.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Set.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CB99C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBB2C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBB98.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBC08.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBD24.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBE74.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CBF54.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Draw_AntQueen.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/Shadows_Draw.s")

/**
 * @brief Calculates the angle of the (x,z) vector from a 3dim vector, with respect to the positive z-axis.
 * 
 * @param vec: The 3dim vector to calculate the angle of. 
 */
void func_800CC7E0(Vec3f vec) {
    // In this instance the z component is flipped.
    CalculateAngleOfVector(vec.x, -vec.z);
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CC814.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcThrownEnemyNext.s")

void func_800CCDCC(Actor* arg0) {
    Vec3f sp24;

    sp24.x = arg0->unknownPositionThings[0].unk_00 + arg0->pos.x;
    sp24.y = arg0->pos.y;
    sp24.z = arg0->unknownPositionThings[0].unk_08 + arg0->pos.z;
    arg0->unk_98 = 0;
    arg0->unk_9C = 0;
    func_800CC814(arg0, sp24, 1);
    func_800CBC08(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CCE4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcWalkingEnemyNext.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcJumpingEnemyNext.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CEB10.s")

void CalcEnemyNextPosition(Actor* arg0) {
   switch (arg0->unk_A0.unk_04) {
   case 0:
       func_800CCDCC(arg0);
       break;
   case 1:
       func_800CCE4C(arg0);
       break;
   case 2:
       CalcWalkingEnemyNext(arg0);
       break;
   case 3:
       CalcJumpingEnemyNext(arg0);
       break;
   case 4:
       func_800CEB10(arg0);
       break;
   default:
       DummiedPrintf3("CalcEnemyNextPosition(): Unknown ATR_IDOU_XXXX\n");
       // double 810000
       break;
   }
   if (arg0->tongueCollision >= 2) {
       func_800CBD24(arg0);
   }
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF080.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF3B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF654.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CF808.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CFB6C.s")

void func_800CFDB8(PlayerActor* arg0) {
    arg0->vaulting = 0;
    arg0->surface = -1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CFDC8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800CFE14.s")

void func_800CFF64(PlayerActor* player) {
    player->surfaceSlide = 0;
    player->move.z = 0.0f;
    player->move.y = 0.0f;
    player->move.x = 0.0f;
}

void func_800CFF7C(Vec3f* arg0) {
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f14;
    Rect3D* temp_v0;

    if (isInOverworld != TRUE) {
        if ((gCurrentStage == STAGE_ANT) && ((gCurrentZone == ZONE_SPIN_ROOM_1) || (gCurrentZone == STAGE_BOSSRUSH)) && (StageFlags[SF_SPIN_ROOM_1_COMPLETED] != 0)) {
            temp_f14 = SUM_OF_SQUARES(arg0->x, arg0->z);
            if (810000.0 < temp_f14) {
                temp_f0_2 = sqrtf((f32) (810000.0 / temp_f14));
                arg0->x = arg0->x * temp_f0_2;
                arg0->z = arg0->z * temp_f0_2;
            }
        } else {
            temp_v0 = &gZoneFields[gCurrentZone].roomBounds;
            
            if (temp_v0->min.x > arg0->x ) {
                arg0->x = temp_v0->min.x;
            }
            
            if (temp_v0->max.x < arg0->x) {
                arg0->x = temp_v0->max.x;
            }

            if (temp_v0->min.z > arg0->z) {
                arg0->z = temp_v0->min.z;
            }
  
            if (temp_v0->max.z < arg0->z) {
                arg0->z = temp_v0->max.z;
            }
        }
    }
}


Vec3f* func_800D00DC(Vec3f* arg0, Collider* arg1) {
    Vec3f sp24;
    Collider* temp_v1;

    if (arg1->unk80 < 0) {
        Vec3f_Zero(&sp24);
    } else {
        temp_v1 = &D_80236980[arg1->unk80];
        if (((temp_v1->unk_10 == 0x14) || (temp_v1->unk_10 == 0x15) || (temp_v1->unk_10 == 0x16)) && (arg1->unk_11C != 4)) {
            sp24.x = temp_v1->unk_8C;
            sp24.y = temp_v1->unk_90;
            sp24.z = temp_v1->unk_94;
        } else {
            Vec3f_Zero(&sp24);
        }
    }
    *arg0 = sp24;
    return arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D01A8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D0448.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D04B0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D0694.s")
//referred to in US1.0 as "Poly.c CalcNextPosition"
#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/CalcNextPosition.s")

void func_800D34CC(void) {
    s32 i;

    Vec3f_Zero(&D_80108F9C);
    Vec3f_Zero(&D_80108FA8);
    Vec3f_Zero(&D_80108FB4);
    Vec3f_Zero(&D_80108FC0);
    D_80108FCC = 0.0f;
    D_80108FD0 = 0;
    D_80108FD4 = 0;
    D_80108FD8 = 0.0f;
    D_80108FDC = 0.0f;
    D_80108FE0 = 0;
    D_80108FE4 = 0.0f;
    D_80108FE8 = 0;
    D_80108FEC = 0;
    for (i = 0; i < ARRAY_COUNT(D_802489C8); i++) {
        Vec3f_Zero(&D_802489C8[i]);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3590.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D363C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3854.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3D80.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D3FC0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4200.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D44C8.s")

void func_800D4550(s32 arg0, s32 arg1, Poly* arg2, Vec3f* arg3, Vec3f* arg4) {
    Field* temp_v0 = &gZoneFields[gCurrentZone];

    arg3->x = temp_v0->unkA4;
    arg3->y = temp_v0->unkA8 + (temp_v0->unkD0 * arg2->unkVectorStruct.vec1.x);
    arg3->z = temp_v0->unkAC;
    arg4->x = temp_v0->cameraAnchor.x;
    arg4->y = temp_v0->cameraAnchor.y + (temp_v0->unkD0 * arg2->unkVectorStruct.vec1.x);
    arg4->z = temp_v0->cameraAnchor.z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D45D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4720.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4D90.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D4F50.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D52E8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D5394.s")

void func_800D6864(PlayerActor* arg0, Tongue* arg1, Camera* arg2, Vec3f* arg3, Vec3f* arg4) {
    Field* collider;

    collider = &gZoneFields[gCurrentZone];
    arg3->x = arg2->f1.z;
    arg3->y = arg2->f2.x + (collider->unkD0 * arg2->size1);
    arg3->z = arg2->f2.y;
    arg4->x = arg2->f3.x;
    arg4->y = arg2->f3.y + (collider->unkD0 * arg2->size1);
    arg4->z = arg2->f3.z;
}

void ApplyRotationToVector(Vec3f* vecA, Vec3f* vecB, f32 degreesAngle) {
    Vec3f differenceVector;

    // Calculate the difference vector between the two vectors
    differenceVector.x = vecA->x - vecB->x;
    differenceVector.y = vecA->y - vecB->y;
    differenceVector.z = vecA->z - vecB->z;
    
    // Rotate the difference vector by the given angle around the y-axis
    RotateVector3D(&differenceVector, differenceVector, DEGREES_TO_RADIANS_PI(degreesAngle), 2);
    
    // Add the rotated difference vector to the second vector to get the first vector
    vecA->x = vecB->x + differenceVector.x;
    vecA->y = vecB->y + differenceVector.y;
    vecA->z = vecB->z + differenceVector.z;
}

#pragma GLOBAL_ASM("asm/nonmatchings/code/poly/func_800D69D0.s")

void SetCameraParameters(void) {
    Field* temp = &gZoneFields[gCurrentZone];
    Camera* cam;
    s32 pad;
    s32 i;
    Vec3f sp3C;
    Vec3f sp30;

    if ((gCurrentStage == STAGE_GHOST) && (gCurrentZone == ZONE_BILLIARDS)) {
        func_800D6864(gPlayerActors, gTongues, gCamera, &sp3C, &sp30);
    } else if ((isInOverworld == TRUE) && (D_8020D8F4 == 0)) {
        func_800D3854(gPlayerActors, gTongues, gCamera, &sp3C, &sp30, 0);
    } else if (gCamera[0].unk0 == 1) {
        func_800D69D0(temp->cameraMode, gPlayerActors, gTongues, gCamera, &sp3C, &sp30, 0);
    } else {
        func_800D5394(gPlayerActors, gTongues, gCamera, &sp3C, &sp30, 0);
    }
    
    cam = &gCamera[0];
    
    for (i = 0; i < ARRAY_COUNT(gCamera); i++, cam++) {
        cam->f5.x = sp3C.x;
        cam->f5.y = sp3C.y;
        cam->f5.z = sp3C.z;

        cam->f4.x = sp30.x;
        cam->f4.y = sp30.y;
        cam->f4.z = sp30.z;
        if (gCurrentStage != STAGE_VS) {
            break;
        }
    }
}

/* Create a rectangle using two vectors then perform an unknown operation */
void func_800D71E8(f32 x1, f32 x2, f32 y1, f32 y2, f32 z1, f32 z2) {
    Rect3D r;

    // define a rectangle with the given bounds
    r.min.x = x1;
    r.max.x = x2;
    r.min.y = y1;
    r.min.z = z1;
    r.max.y = y2;
    r.max.z = z2;
    
    // ensure max > min
    OrderRectBounds(&r);
    func_800C9748(&r, 0x77, 2); //unknown
}

s32 func_800D7248(f32 x, f32 y, f32 z, f32 arg3, f32 arg4, f32* outX, f32* arg6, f32* arg7) {
    Vec3f vec;
    s32 var_v1;
    Collider* collider;

    vec.x = x;
    vec.y = y;
    vec.z = z;
    
    collider = func_800CAF88(vec, arg3, arg4);
    
    // if a collider was found assign its position to the output variables then return 1 for success
    return (collider != NULL) ?
        *outX = collider->unk_94,
        *arg6 = collider->unk_98,
        *arg7 = collider->unk_9C,
        1 :
        0;
}

s32 func_800D72DC(f32 x1, f32 y1, f32 z1, f32 x2, f32 y2, f32 z2, f32* outX, f32* outY, f32* outZ) {
    Vec3f vecOne;
    Vec3f vecTwo;
    Collider* collider;

    vecOne.x = x1;
    vecOne.y = y1;
    vecOne.z = z1;

    vecTwo.x = x2;
    vecTwo.y = y2;
    vecTwo.z = z2;
    
    collider = SearchPolygonBetween(vecOne, vecTwo, 0x77, 1, 1);
    return (collider != NULL) ?
        *outX = collider->unk_94,
        *outY = collider->unk_98,
        *outZ = collider->unk_9C,
        1 :
        0;
}

void func_800D73BC(f32* x, f32* y, f32* z, f32 arg3) {
    Vec3f destVec;
    Vec3f srcVec;

    srcVec.x = *x;
    srcVec.y = *y;
    srcVec.z = *z;
    
    //wrong number of args(?)
    func_800CA734(&destVec, srcVec, arg3, 0x77);
    
    *x = destVec.x;
    *y = destVec.y;
    *z = destVec.z;
}
