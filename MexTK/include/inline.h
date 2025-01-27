#ifndef MEX_H_INLINE
#define MEX_H_INLINE

#include "structs.h"
#include "fighter.h"
#include "obj.h"
#include "mex.h"
#include "datatypes.h"
#include "gx.h"
#include "hsd.h"
#include "math.h"
#include "useful.h"

/*** Functions ***/

float fabs(float x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

int abs(int x)
{
    if (x < 0)
        return -x;
    else
        return x;
}

void enterKnockback(GOBJ *fighter, int angle, float mag)
{
    FighterData *fighter_data = ((FighterData *)fighter->userdata);

    // store damage variables
    fighter_data->dmg.hit_log.force_applied = mag;
    fighter_data->dmg.hit_log.kb_angle = angle;
    fighter_data->dmg.hit_log.direction = fighter_data->facing_direction;
    fighter_data->dmg.hit_log.damaged_hurtbox = 0;
    fighter_data->dmg.applied = 0;
    fighter_data->dmg.hit_log.collpos.X = fighter_data->phys.pos.X;
    fighter_data->dmg.hit_log.collpos.Y = fighter_data->phys.pos.Y;
    fighter_data->dmg.hit_log.collpos.Z = fighter_data->phys.pos.Z;

    Fighter_EnterDamageState(fighter, -1, 0);

    return;
}

void null()
{
    return;
}

void C_QUATMtx(Vec4 *r, Mtx m)
{
    /*---------------------------------------------------------------------------*
   Name:         QUATMtx

   Description:  Converts a matrix to a unit quaternion.

   Arguments:    r   - result quaternion
                m   - the matrix

   Returns:      NONE
    *---------------------------------------------------------------------------*/

    float tr, s;
    int i, j, k;
    int nxt[3] = {1, 2, 0};
    float q[3];

    tr = m[0][0] + m[1][1] + m[2][2];
    if (tr > 0.0f)
    {
        s = (float)sqrtf(tr + 1.0f);
        r->W = s * 0.5f;
        s = 0.5f / s;
        r->X = (m[1][2] - m[2][1]) * s;
        r->Y = (m[2][0] - m[0][2]) * s;
        r->Z = (m[0][1] - m[1][0]) * s;
    }
    else
    {
        i = 0;
        if (m[1][1] > m[0][0])
            i = 1;
        if (m[2][2] > m[i][i])
            i = 2;
        j = nxt[i];
        k = nxt[j];
        s = (float)sqrtf((m[i][i] - (m[j][j] + m[k][k])) + 1.0f);
        q[i] = s * 0.5f;

        if (s != 0.0f)
            s = 0.5f / s;

        r->W = (m[j][k] - m[k][j]) * s;
        q[j] = (m[i][j] + m[j][i]) * s;
        q[k] = (m[i][k] + m[k][i]) * s;

        r->X = q[0];
        r->Y = q[1];
        r->Z = q[2];
    }
}

HSD_Pad *PadGet(int playerIndex, int padType)
{
    HSD_Pads *pads;

    // get the correct pad
    if (padType == PADGET_MASTER)
        pads = 0x804c1fac;
    else if (padType == PADGET_ENGINE)
        pads = 0x804c21cc;

    return (&pads->pad[playerIndex]);
}

float JOBJ_GetAnimFrame(JOBJ *joint)
{
    // check for AOBJ in jobj
    JOBJ *jobj;
    jobj = joint;
    while (jobj != 0)
    {
        if (jobj->aobj != 0)
        {
            return jobj->aobj->curr_frame;
        }

        // check for AOBJ in dobj
        DOBJ *dobj;
        dobj = jobj->dobj;
        while (dobj != 0)
        {
            if (dobj->aobj != 0)
            {
                return dobj->aobj->curr_frame;
            }

            // check for AOBJ in mobj
            MOBJ *mobj;
            mobj = dobj->mobj;
            if (mobj->aobj != 0)
            {
                return mobj->aobj->curr_frame;
            }

            // check for AOBJ in tobj
            TOBJ *tobj;
            tobj = mobj->tobj;
            while (tobj != 0)
            {
                if (tobj->aobj != 0)
                {
                    return tobj->aobj->curr_frame;
                }
                tobj = tobj->next;
            }

            dobj = dobj->next;
        }

        jobj = jobj->child;
    }

    // no aobj found, return -1
    return -1;
}

AOBJ *JOBJ_GetFirstAOBJ(JOBJ *jobj)
{
    // check for AOBJ in this jobj

    if (jobj->aobj != 0)
    {
        return jobj->aobj;
    }
    // check for AOBJ in dobj
    DOBJ *dobj;
    dobj = jobj->dobj;
    while (dobj != 0)
    {
        if (dobj->aobj != 0)
        {
            return dobj->aobj;
        }

        // check for AOBJ in mobj
        MOBJ *mobj;
        mobj = dobj->mobj;
        if (mobj->aobj != 0)
        {
            return mobj->aobj;
        }

        // check for AOBJ in tobj
        TOBJ *tobj;
        tobj = mobj->tobj;
        while (tobj != 0)
        {
            if (tobj->aobj != 0)
            {
                return tobj->aobj;
            }
            tobj = tobj->next;
        }

        dobj = dobj->next;
    }

    // this jobj doesnt have an aobj, check the child
    if (jobj->child != 0)
    {
        AOBJ *aobj = JOBJ_GetFirstAOBJ(jobj->child);
        if (aobj != 0)
            return aobj;
    }

    // child did not have an aobj, check the sibling
    if (jobj->sibling != 0)
    {
        AOBJ *aobj = JOBJ_GetFirstAOBJ(jobj->sibling);
        if (aobj != 0)
            return aobj;
    }

    // no aobj found, return 0
    return 0;
}

AOBJ *JOBJ_GetJointAOBJ(JOBJ *jobj)
{
    // check for AOBJ in this jobj

    if (jobj->aobj != 0)
    {
        return jobj->aobj;
    }

    // this jobj doesnt have an aobj, check the child
    if (jobj->child != 0)
    {
        AOBJ *aobj = JOBJ_GetJointAOBJ(jobj->child);
        if (aobj != 0)
            return aobj;
    }

    // child did not have an aobj, check the sibling
    if (jobj->sibling != 0)
    {
        AOBJ *aobj = JOBJ_GetJointAOBJ(jobj->sibling);
        if (aobj != 0)
            return aobj;
    }

    // no aobj found, return 0
    return 0;
}

DOBJ *JOBJ_GetDObjChild(JOBJ *joint, int dobj_index)
{

    int count = 0;
    DOBJ *dobj = joint->dobj;

    while (count < dobj_index)
    {
        if (dobj->next == 0)
            assert("dobj not found!");

        else
            dobj = dobj->next;

        count++;
    }

    return dobj;
}

float Math_Vec2Angle(Vec2 *a, Vec2 *b)
{
    // get angle
    // float angle = atan((b->Y - a->Y) / (b->X - a->X));
    float angle = atan2((b->Y - a->Y), (b->X - a->X));

    /*
    //Ensure above 0 and below 6.28319
    while (angle < 0)
    {
        angle += M_PI;
    }
    while (angle > M_PI * 2)
    {
        angle -= M_PI;
    }
    */

    return angle;
}

float pow (float, float);

float Math_Vec2Distance(Vec2 *a, Vec2 *b)
{
    return sqrtf(pow((a->X - b->X), 2) + pow((a->Y - b->Y), 2));
}

float Math_Vec3Distance(Vec3 *a, Vec3 *b)
{
    return sqrtf(pow((a->X - b->X), 2) + pow((a->Y - b->Y), 2) + pow((a->Z - b->Z), 2));
}

float fmin(float a, float b)
{
    return (((a) < (b)) ? (a) : (b));
}

float fmax(float a, float b)
{
    return (((a) > (b)) ? (a) : (b));
}

/*---------------------------------------------*
 * GXVertex functions                          *
 *---------------------------------------------*/
void GXPosition3f32(f32 x, f32 y, f32 z)
{
    gx_pipe->d.F32 = x;
    gx_pipe->d.F32 = y;
    gx_pipe->d.F32 = z;
}
void GXPosition3u16(u16 x, u16 y, u16 z)
{
    gx_pipe->d.U16 = x;
    gx_pipe->d.U16 = y;
    gx_pipe->d.U16 = z;
}
void GXPosition3s16(s16 x, s16 y, s16 z)
{
    gx_pipe->d.S16 = x;
    gx_pipe->d.S16 = y;
    gx_pipe->d.S16 = z;
}
void GXPosition3u8(u8 x, u8 y, u8 z)
{
    gx_pipe->d.U8 = x;
    gx_pipe->d.U8 = y;
    gx_pipe->d.U8 = z;
}
void GXPosition3s8(s8 x, s8 y, s8 z)
{
    gx_pipe->d.S8 = x;
    gx_pipe->d.S8 = y;
    gx_pipe->d.S8 = z;
}
void GXPosition2f32(f32 x, f32 y)
{
    gx_pipe->d.F32 = x;
    gx_pipe->d.F32 = y;
}
void GXPosition2u16(u16 x, u16 y)
{
    gx_pipe->d.U16 = x;
    gx_pipe->d.U16 = y;
}
void GXPosition2s16(s16 x, s16 y)
{

    gx_pipe->d.S16 = x;
    gx_pipe->d.S16 = y;
}
void GXPosition2u8(u8 x, u8 y)
{
    gx_pipe->d.U8 = x;
    gx_pipe->d.U8 = y;
}
void GXPosition2s8(s8 x, s8 y)
{
    gx_pipe->d.S8 = x;
    gx_pipe->d.S8 = y;
}
void GXPosition1x16(u16 index)
{
    gx_pipe->d.U16 = index;
}
void GXPosition1x8(u8 index)
{
    gx_pipe->d.U8 = index;
}

void GXNormal3f32(f32 x, f32 y, f32 z)
{
    gx_pipe->d.F32 = x;
    gx_pipe->d.F32 = y;
    gx_pipe->d.F32 = z;
}
void GXNormal3s16(s16 x, s16 y, s16 z)
{
    gx_pipe->d.S16 = x;
    gx_pipe->d.S16 = y;
    gx_pipe->d.S16 = z;
}
void GXNormal3s8(s8 x, s8 y, s8 z)
{
    gx_pipe->d.S8 = x;
    gx_pipe->d.S8 = y;
    gx_pipe->d.S8 = z;
}
void GXNormal1x16(u16 index)
{
    gx_pipe->d.U32 = index;
}
void GXNormal1x8(u8 index)
{
    gx_pipe->d.U8 = index;
}

void GXColor4u8(u8 r, u8 g, u8 b, u8 a)
{
    gx_pipe->d.U8 = r;
    gx_pipe->d.U8 = g;
    gx_pipe->d.U8 = b;
    gx_pipe->d.U8 = a;
}
void GXColor3u8(u8 r, u8 g, u8 b)
{
    gx_pipe->d.U8 = r;
    gx_pipe->d.U8 = g;
    gx_pipe->d.U8 = b;
}
void GXColor1u32(u32 clr)
{
    gx_pipe->d.U32 = clr;
}
void GXColor1u16(u16 clr)
{
    gx_pipe->d.U16 = clr;
}
void GXColor1x16(u16 index)
{
    gx_pipe->d.U16 = index;
}
void GXColor1x8(u8 index)
{
    gx_pipe->d.U8 = index;
}

void GXTexCoord2f32(f32 s, f32 t)
{
    gx_pipe->d.F32 = s;
    gx_pipe->d.F32 = t;
}
void GXTexCoord2u16(u16 s, u16 t)
{
    gx_pipe->d.U16 = s;
    gx_pipe->d.U16 = t;
}
void GXTexCoord2s16(s16 s, s16 t)
{
    gx_pipe->d.S16 = s;
    gx_pipe->d.S16 = t;
}
void GXTexCoord2u8(u8 s, u8 t)
{
    gx_pipe->d.U8 = s;
    gx_pipe->d.U8 = t;
}
void GXTexCoord2s8(s8 s, s8 t)
{
    gx_pipe->d.S8 = s;
    gx_pipe->d.S8 = t;
}
void GXTexCoord1f32(f32 s, f32 t)
{
    gx_pipe->d.F32 = s;
    gx_pipe->d.F32 = t;
}
void GXTexCoord1u16(u16 s, u16 t)
{
    gx_pipe->d.U16 = s;
    gx_pipe->d.U16 = t;
}
void GXTexCoord1s16(s16 s, s16 t)
{
    gx_pipe->d.S16 = s;
    gx_pipe->d.S16 = t;
}
void GXTexCoord1u8(u8 s, u8 t)
{
    gx_pipe->d.U8 = s;
    gx_pipe->d.U8 = t;
}
void GXTexCoord1s8(s8 s, s8 t)
{
    gx_pipe->d.S8 = s;
    gx_pipe->d.S8 = t;
}
void GXTexCoord1x16(u16 index)
{
    gx_pipe->d.U16 = index;
}
void GXTexCoord1x8(u8 index)
{
    gx_pipe->d.U8 = index;
}

void GX_Draw(float x, float y, float z, GXColor *color)
{
    gx_pipe->d.F32 = x;
    gx_pipe->d.F32 = y;
    gx_pipe->d.F32 = z;
    gx_pipe->d.U8 = color->r;
    gx_pipe->d.U8 = color->g;
    gx_pipe->d.U8 = color->b;
    gx_pipe->d.U8 = color->a;
    return;
}

#endif