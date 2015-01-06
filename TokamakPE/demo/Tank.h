#ifndef TANK_H
#define TANK_H

#include "../include/tokamak.h"
#include "../sampleApp.h"

#define N_PARTICLES 40

class CParticle
{
public:
    void MakeParticle(neSimulator * sim, neV3 position, s32 index);

    CRenderPrimitive particleRenders[N_PARTICLES];
    neRigidBody * box[N_PARTICLES];
};



#endif