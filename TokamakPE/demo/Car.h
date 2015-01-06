#ifndef CAR_H
#define CAR_H

#include "../include/tokamak.h"
#include "../sampleApp.h"

const f32 WHEEL_DIAMETER = 0.9f;
const f32 WHEEL_WIDTH = 0.3f;
const f32 MAX_SPEED = 8.0f;
const f32 MAX_REVERSE_SPEED = -3.0f;
const f32 MAX_STEER = 0.8f;
const f32 MAX_SLIDE = 0.7f;

#define N_CARS 10
const s32 N_BODY_BOXES = 2;
const s32 N_PARTS = 3;
const s32 N_RENDER_PRIMITIVES = (N_BODY_BOXES + N_PARTS);
const s32 MAX_OVERLAPPED_PAIR = 1000;

struct SensorData
{
    neV3 pos;
};

struct PartData
{
    neV3 boxSize;
    neV3 position;
    neV3 jointPos;
    neV3 jointRot;
    f32 lowerLimit;
    f32 upperLimit;
};

class CControllerCB: public neRigidBodyControllerCallback
{
public:
    void RigidBodyControllerCallback(neRigidBodyController * controller, float timeStep);
};

class CCar
{
public:
    CCar();

    void MakeCar(neSimulator * sim, neV3 & pos);
    void MakeParts(neSimulator * sim, neV3 & pos);
    void CarController(neRigidBodyController * controller);
    void Process(XINPUT_STATE &);

    CRenderPrimitive* AddRenderPrimitive();

public:
    neRigidBody * carRigidBody;
    neRigidBody * carParts[N_PARTS];
    CRenderPrimitive carRenderPrimitives[N_RENDER_PRIMITIVES];
    CRenderPrimitive wheelRenderPrimitive;
    neV3 displayLines[4][2];
    f32 suspensionLength[4];
    s32 id;
    f32 accel;
    f32 steer;
    f32 slide;
    neV3 steerDir;
    s32 nextRenderPrim;
};

#endif