#include "DXUT.h"
#include "Tank.h"

void CParticle::MakeParticle( neSimulator* sim, neV3 position, s32 index )
{
    s32 cur;
    cur = index;
    box[cur] = sim->CreateRigidParticle();
    neGeometry * geom = box[cur]->AddGeometry();
    const neV3 boxSize1 = {1.f, 1.f, 1.f};
//     geom->SetBoxSize(boxSize1[0],boxSize1[1],boxSize1[2]);
    geom->SetSphereDiameter(1.f);
    box[cur]->UpdateBoundingInfo();
    f32 mass = 0.1f;
    box[cur]->SetInertiaTensor(neBoxInertiaTensor(boxSize1[0], boxSize1[1], boxSize1[2], mass));
    box[cur]->SetMass(mass);
    box[cur]->SetPos(position);
//     particleRenders[cur].SetGraphicBox(boxSize1[0], boxSize1[1], boxSize1[2]);
    particleRenders[cur].SetGraphicSphere(0.5f);
    particleRenders[cur].SetDiffuseColor(D3DXCOLOR(0.6f, 0.7f, 0.7f, 1));
}
