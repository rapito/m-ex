#ifndef SLIPPI_COMMON_H
#define SLIPPI_COMMON_H
#include <stdbool.h>
#include "../MexTK/mex.h";

#define NULL ((void *)0)
#define spawn(X) calloc(sizeof(X)) 

/** Functions **/
void* (*bp)(void) = (void *)0x8021b2d8;

// TODO: find a better way to do this, I'm not entirely sure this is fine
bool GOBJ_IsAlive(GOBJ* gobj)
{

    
    return gobj != NULL;
}

#endif SLIPPI_COMMON_H
