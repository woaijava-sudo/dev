#include "DXUT.h"
#include "../include/tokamak.h"
#include "../resource.h"
#include "../d3dapp.h"
#include "../sampleApp.h"
#include "Tank.h"
#include "Car.h"

D3DXHANDLE g_hShaderTechTokamak;

D3DXVECTOR4 vLightWorld[NUM_LIGHT] = {	D3DXVECTOR4(1.f,2.f,1.f,0.f),
    D3DXVECTOR4(-1.f,1.f,1.f,0.f)};

D3DXVECTOR4 vLightColor[NUM_LIGHT] = {	D3DXVECTOR4(0.7f,0.7f,0.7f,0.f),
    D3DXVECTOR4(0.5f,0.5f,0.5f,0.f)};

const D3DXVECTOR3 g_MinBound( -4.0f, -GROUND_Y, -6.0f );
const D3DXVECTOR3 g_MaxBound( 4.0f, GROUND_Y, 6.0f );

D3DXVECTOR3 g_Eye, g_At;
bool pause = false;

#define TIME_STEP (1.0f / 30.0f)

struct DemoData
{
    neV3 pos;
    neV3 boxSize;
    neV3 colour;
};

DemoData gFloor = {	{0.0f,-11.0f,0.0f}, {200.0f,2.0f,200.0f}, {0.3f,0.3f,0.6f}};
extern SensorData sensorData[4];

#define N_BARRIERS 4

DemoData gBarriers[N_BARRIERS] = {
    {{0.0f,-11.0f,-100.0f}, {200.0f,5.0f,2.0f}, {0.3f,0.3f,0.6f}},
    {{0.0f,-11.0f,100.0f}, {200.0f,5.0f,2.0f}, {0.3f,0.3f,0.6f}},
    {{-100.0f,-11.0f,0.0f}, {2.0f,5.0f,200.0f}, {0.3f,0.3f,0.6f}},
    {{100.0f,-11.0f,0.0f}, {2.0f,5.0f,200.0f}, {0.3f,0.3f,0.6f}}};
;;

#define N_RAMPS 4

struct RampData
{
    neV3 pos;
    neV3 boxSize;
    neV3 colour;
    neV3 angle;
};

RampData gRamps[N_RAMPS] = {
    {{-40.0f,-12.0f,-50.0f}, {30.0f,2.0f,30.0f}, {0.3f,0.3f,0.6f}, {0.0f,0.0f,NE_PI * 0.07f}},
    {{50.0f,-12.0f,30.0f}, {30.0f,2.0f,30.0f}, {0.3f,0.3f,0.6f}, {-NE_PI * 0.07f,0.0f,0.0f}},
    {{-30.0f,-12.0f,50.0f}, {30.0f,2.0f,30.0f}, {0.3f,0.3f,0.6f}, {0.0f,0.0f,-NE_PI * 0.07f}},
    {{50.0f,-12.0f,-30.0f}, {30.0f,2.0f,30.0f}, {0.3f,0.3f,0.6f}, {-NE_PI * 0.07f,0.0f,0.0f}}
};

class SimpleSim
{
public:
    SimpleSim(): sim(NULL){}

    void CreateGround()
    {
        ground = sim->CreateAnimatedBody();
        neGeometry * geom = ground->AddGeometry();	 
        geom->SetBoxSize(gFloor.boxSize);
        ground->UpdateBoundingInfo();
        ground->SetPos(gFloor.pos);
        groundRender.SetGraphicBox(gFloor.boxSize[0], gFloor.boxSize[1], gFloor.boxSize[2]);

        for (s32 i = 0; i < N_BARRIERS; i++)
        {
            barriers[i] = sim->CreateAnimatedBody();
            geom = barriers[i]->AddGeometry();
            geom->SetBoxSize(gBarriers[i].boxSize);
            barriers[i]->UpdateBoundingInfo();
            barriers[i]->SetPos(gBarriers[i].pos);
            barrierRenders[i].SetGraphicBox(gBarriers[i].boxSize[0], gBarriers[i].boxSize[1], gBarriers[i].boxSize[2]);
            barrierRenders[i].SetDiffuseColor(D3DXCOLOR(0.6f, 0.8f, 0.5f, 1));
        }

        for (s32 i = 0; i < N_RAMPS; i++)
        {
            ramps[i] = sim->CreateAnimatedBody();
            geom = ramps[i]->AddGeometry();
            geom->SetBoxSize(gRamps[i].boxSize);
            ramps[i]->UpdateBoundingInfo();
            ramps[i]->SetPos(gRamps[i].pos);
            neM3 rot; rot.RotateXYZ(gRamps[i].angle);
            ramps[i]->SetRotation(rot);
            rampRenders[i].SetGraphicBox(gRamps[i].boxSize[0], gRamps[i].boxSize[1], gRamps[i].boxSize[2]);
            rampRenders[i].SetDiffuseColor(D3DXCOLOR(0.6f, 0.2f, 0.8f, 1));
        }
    }

    void CreateSimulator()
    {
        if (sim)
        {
            neSimulator::DestroySimulator(sim);
            sim = NULL;
        }
        // creat the physics simulation

        neSimulatorSizeInfo sizeInfo;

        sizeInfo.rigidBodiesCount = (N_PARTS + 1) * N_CARS;
        sizeInfo.rigidParticleCount = N_PARTICLES;
        sizeInfo.animatedBodiesCount = N_RAMPS + N_BARRIERS + 1;
        sizeInfo.geometriesCount = (N_RENDER_PRIMITIVES + 1) * N_CARS + N_RAMPS + N_BARRIERS + N_PARTICLES + 1;
        sizeInfo.overlappedPairsCount = MAX_OVERLAPPED_PAIR;

        { //dont need any of these
            sizeInfo.terrainNodesStartCount = 200;
        }
        neV3 gravity; gravity.Set(0.0f, -7.0f, 0.0f);
        sim = neSimulator::CreateSimulator(sizeInfo, &allocator, &gravity);
    }

    void Reset()
    {
        Cleanup();

        perfReport.Reset();
        perfReport.SetReportType(nePerformanceReport::NE_PERF_RUNNING_AVERAGE);

        // reset the camera position
        D3DXVECTOR3 vecEye (30.0f, 12.0f, 40.0f);//(0.0f, -GROUND_Y + 1.7f, 0.0f);
        D3DXVECTOR3 vecAt (0.0f, 0.0f, 1.0f);//(0.0f, -GROUND_Y + 1.7f, 1.0f);
        g_Camera.SetViewParams( &vecEye, &vecAt );

        CreateSimulator();
        CreateGround();
        CreateParticle();

        neV3 carPos;
        carPos.Set(0.0f, 4.0f, 10.0f);
        for (int i = 0; i < N_CARS; i++)
        {
            cars[i].id = i;
            cars[i].MakeCar(sim, carPos);
            carPos[0] -= 7;
        }
    }

    void Cleanup()
    {
        groundRender.mMesh.Destroy();

        for (s32 i = 0; i < N_BARRIERS; i++)        barrierRenders[i].mMesh.Destroy();
        for (s32 i = 0; i < N_RAMPS; i++)           rampRenders[i].mMesh.Destroy();
        for (s32 i = 0; i < N_PARTICLES; i++)       particles.particleRenders[i].mMesh.Destroy();
        for (s32 i = 0; i < N_CARS; i++)
        {
            cars[i].wheelRenderPrimitive.mMesh.Destroy();
            for (s32 j = 0; j < N_RENDER_PRIMITIVES; j++)
            {
                cars[i].carRenderPrimitives[j].mMesh.Destroy();
            }
        }

        neSimulator::DestroySimulator(sim);
        sim = NULL;
    }

    void CreateParticle() 
    {
        neV3 position;
        position.SetZero();

        for (s32 i = 0; i < N_PARTICLES; i++)
        {
            position.Set(0.0f, 2.0f * i + 120.0f, 10.0f);
            particles.MakeParticle(sim, position, i);
        }
    }

    neSimulator * sim;
    neAllocatorDefault allocator;

    nePerformanceReport perfReport;

    CRenderPrimitive groundRender;
    neAnimatedBody * ground;

    CRenderPrimitive barrierRenders[N_BARRIERS];
    neAnimatedBody * barriers[N_BARRIERS];

    CRenderPrimitive rampRenders[N_RAMPS];
    neAnimatedBody * ramps[N_RAMPS];

    CParticle particles;

    CCar cars[N_CARS];
};

SimpleSim* g_sim = NULL;

void MyAppInit()
{
    // TODO: Perform any application-level initialization here
    // Setup the camera
    g_Camera.SetEnableYAxisMovement( true );
    g_Camera.SetRotateButtons( false, false, true );
    g_Camera.SetScalers( 0.01f, 50.0f );
    D3DXVECTOR3 vecEye (0.0f, 0.0f, -10.0f);//(0.0f, -GROUND_Y + 1.7f, 0.0f);
    D3DXVECTOR3 vecAt (0.0f, 0.0f, 1.0f);//(0.0f, -GROUND_Y + 1.7f, 1.0f);
    g_Camera.SetViewParams( &vecEye, &vecAt );
    DXUTGetD3D9Device()->SetTransform( D3DTS_PROJECTION, g_Camera.GetProjMatrix());

    for (s32 i = 0; i < NUM_LIGHT; i++)
    {
        D3DXVec4Normalize(&vLightWorld[i], &vLightWorld[i]);
    }

    g_sim = new SimpleSim;
    g_sim->Reset();
};

void CALLBACK OnMyAppFrameMove( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    g_Camera.FrameMove( fElapsedTime );

    if (!pause)
    {
        g_sim->sim->Advance(TIME_STEP, 1.0f / 30.0f, 1.0f/ 30.0f, &g_sim->perfReport);
    }

    D3DXVECTOR3 vecEye (30.0f, 12.0f, 40.0f);

    neV3 v = g_sim->cars[0].carRigidBody->GetPos();
    g_At[0] = v[0];
    g_At[1] = v[1];
    g_At[2] = v[2];
    D3DXVECTOR3 offset(0.0f, 0.0f, 80.0f);

    g_Eye = g_At + offset;
    g_Eye[1] = 40.0f;

    DWORD dwResult;
    XINPUT_STATE state;
    ZeroMemory( &state, sizeof(XINPUT_STATE) );

    // Simply get the state of the controller from XInput.
    dwResult = XInputGetState( 0, &state );

    //if( dwResult == ERROR_SUCCESS )
    { 
        // Controller is connected 
        g_sim->cars[0].Process(state);
    }
}

void CALLBACK OnMyAppFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
    pd3dDevice->SetTransform(D3DTS_VIEW, g_Camera.GetViewMatrix());

    neT3 t;
    t = g_sim->ground->GetTransform();
    t.MakeD3DCompatibleMatrix();
    g_sim->groundRender.Render(pd3dDevice, &t);

    for (s32 i = 0; i < N_BARRIERS; i++)
    {
        t = g_sim->barriers[i]->GetTransform();
        t.MakeD3DCompatibleMatrix();
        g_sim->barrierRenders[i].Render(pd3dDevice, &t);
    }

    for (s32 i = 0; i < N_RAMPS; i++)
    {
        t = g_sim->ramps[i]->GetTransform();
        t.MakeD3DCompatibleMatrix();
        g_sim->rampRenders[i].Render(pd3dDevice, &t);
    }

    for (s32 i = 0; i < N_PARTICLES; i++)
    {
        t = g_sim->particles.box[i]->GetTransform();
        t.MakeD3DCompatibleMatrix();
        g_sim->particles.particleRenders[i].Render(pd3dDevice, &t);
    }

    for (s32 i = 0; i < N_CARS; i++)
    {
        CCar * car = &g_sim->cars[i];
        t = car->carRigidBody->GetTransform();
        car->carRigidBody->BeginIterateGeometry();

        neGeometry * geom = car->carRigidBody->GetNextGeometry();
        while (geom)
        {
            neT3 t2 = geom->GetTransform();
            neT3 t3 = t * t2;
            t3.MakeD3DCompatibleMatrix();
            CRenderPrimitive * rp = (CRenderPrimitive *)geom->GetUserData();
            rp->Render(pd3dDevice, &t3);
            geom = car->carRigidBody->GetNextGeometry();
        }

        for (s32 j = 0; j < N_PARTS; j++)
        {
            t = car->carParts[j]->GetTransform();
            t.MakeD3DCompatibleMatrix();
            car->carParts[j]->BeginIterateGeometry();
            neGeometry * geom = car->carParts[j]->GetNextGeometry();
            CRenderPrimitive * rp = (CRenderPrimitive *)geom->GetUserData();
            rp->Render(pd3dDevice, &t);
        }

        for (s32 j = 0; j < 4; j++)
        {
            neT3 wheel2World, suspension2Body, tire2Suspension, cylinder2Tire;
            suspension2Body.SetIdentity();
            suspension2Body.pos.Set(0.0f, -car->suspensionLength[j] + WHEEL_DIAMETER / 2.0f, 0.0f);
            suspension2Body.pos += sensorData[j].pos;
            cylinder2Tire.SetIdentity();
            neV3 rot; rot.Set(NE_PI * 0.5f, 0.0f, 0.0f);
            cylinder2Tire.rot.RotateXYZ(rot);
            tire2Suspension.SetIdentity();

            if (j == 0 || j == 1)
            {
                rot.Set(0.0f, car->steer, 0.0f);
                tire2Suspension.rot.RotateXYZ(rot);
            }

            wheel2World = car->carRigidBody->GetTransform() * suspension2Body * tire2Suspension * cylinder2Tire;
            
            if (j == 0 || j == 1)
            {
                car->steerDir = wheel2World.rot[0];
            }
            wheel2World.MakeD3DCompatibleMatrix();
            car->wheelRenderPrimitive.Render(pd3dDevice, &wheel2World);
        }
    }

    f32 processtime = g_sim->perfReport.time[nePerformanceReport::NE_PERF_TOTAL_TIME];
    s32 mem;
    g_sim->sim->GetMemoryAllocated(mem);
    WCHAR ss[256];
    swprintf_s(ss, 255, L"Time span: %.3lf ms  Memory: %d bytes", 1000.0f * processtime, mem);
    PWCHAR str[] = {ss, L"", L"С����ͻͻͻ~", L"", L"ͻͻͻͻ~~~"};
    MyRenderText(str, 5);
}

LRESULT CALLBACK MyAppMsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, 
    bool* pbNoFurtherProcessing, void* pUserContext )
{
    return 0;
}

void CALLBACK OnMyAppDestroyDevice( void* pUserContext )
{
    SAFE_RELEASE( g_pEffect );
    if (g_sim)
    {
        g_sim->Cleanup();
        delete g_sim;
        g_sim = NULL;
    }
}

void CALLBACK MyAppKeyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
    if( bKeyDown )
    {
        switch( nChar )
        {
        case 'R':
            {
                neV3 zeroPos = {0.0f, -1.0f, 10.0f};
                g_sim->cars[0].carRigidBody->SetPos(zeroPos);
                for (s32 i = 0; i < N_PARTS; i++)
                {
                    g_sim->cars[0].carParts[i]->SetPos(zeroPos);
                }
            }
            break;
        case 'P':
            pause = !pause;
            break;
        }
    }
}
