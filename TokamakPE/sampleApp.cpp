#include "DXUT.h"
#include "sampleApp.h"

IDirect3DDevice9 *      g_pD3dDevice;

ID3DXEffect*            g_pEffect = NULL;       // D3DX effect interface
D3DXHANDLE              g_hMatW;                // Handles to transformation matrices in effect
D3DXHANDLE              g_hMatV;
D3DXHANDLE              g_hMatP;
D3DXHANDLE              g_hMatWV;
D3DXHANDLE              g_hMatWVP;
D3DXHANDLE              g_hDiffuse;             // Handles to material parameters in effect
D3DXHANDLE              g_hSpecular;
D3DXHANDLE              g_hTex;
D3DXHANDLE              g_hPower;
D3DXHANDLE              g_hShaderTech;          // Technique to use when using programmable rendering path
D3DXHANDLE				g_hAmbient;
D3DXHANDLE				g_hLightTokamak[NUM_LIGHT];
D3DXHANDLE				g_hLightColorTokamak[NUM_LIGHT];
D3DXHANDLE				g_hViewPos;

extern ID3DXFont*       g_pFont;         // Font for drawing text
extern ID3DXSprite*		g_pTextSprite;   // Sprite for batching draw text calls

HRESULT CALLBACK OnMyAppCreateDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    // Read the D3DX effect file
    WCHAR str[MAX_PATH];
    V_RETURN( DXUTFindDXSDKMediaFileCch( str, MAX_PATH, L"Main.fx" ) );

    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the 
    // shader debugger. Debugging vertex shaders requires either REF or software vertex 
    // processing, and debugging pixel shaders requires REF.  The 
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the 
    // shader debugger.  It enables source level debugging, prevents instruction 
    // reordering, prevents dead code elimination, and forces the compiler to compile 
    // against the next higher available software target, which ensures that the 
    // unoptimized shaders do not exceed the shader model limitations.  Setting these 
    // flags will cause slower rendering since the shaders will be unoptimized and 
    // forced into software.  See the DirectX documentation for more information about 
    // using the shader debugger.
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXFX_LARGEADDRESSAWARE;
    
#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
    dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
    dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif

    LPD3DXBUFFER compileErrors = NULL;

    // If this fails, there should be debug output as to 
    // they the .fx file failed to compile
    hr = ( D3DXCreateEffectFromFile( pd3dDevice, str, NULL, NULL, dwShaderFlags, 
        NULL, &g_pEffect, &compileErrors ) );

    if (compileErrors)
    {
        MessageBoxA(NULL, (LPCSTR) compileErrors->GetBufferPointer(), NULL, MB_OK);
    }

    // Obtain handles
    g_hMatW = g_pEffect->GetParameterByName( NULL, "g_mWorld" );
    g_hMatV = g_pEffect->GetParameterByName( NULL, "g_mView" );
    g_hMatP = g_pEffect->GetParameterByName( NULL, "g_mProj" );
    g_hMatWV = g_pEffect->GetParameterByName( NULL, "g_mWorldView" );
    g_hMatWVP = g_pEffect->GetParameterByName( NULL, "g_mWorldViewProj" );
    g_hDiffuse = g_pEffect->GetParameterByName( NULL, "g_matDiffuse" );
    g_hSpecular = g_pEffect->GetParameterByName( NULL, "g_matSpecular" );
    g_hTex = g_pEffect->GetParameterByName( NULL, "g_txScene" );
    g_hPower = g_pEffect->GetParameterByName( NULL, "g_matPower" );

    g_hLightTokamak[0] = g_pEffect->GetParameterByName(NULL, "g_LightTokamak0"); // light is object space
    g_hLightTokamak[1] = g_pEffect->GetParameterByName(NULL, "g_LightTokamak1"); // light is object space

    g_hLightColorTokamak[0] = g_pEffect->GetParameterByName(NULL, "g_LightColorTokamak0");
    g_hLightColorTokamak[1] = g_pEffect->GetParameterByName(NULL, "g_LightColorTokamak1");

    g_hAmbient = g_pEffect->GetParameterByName(NULL, "g_Ambient");
    g_hViewPos = g_pEffect->GetParameterByName(NULL, "g_ViewPos");

    g_hShaderTech = g_pEffect->GetTechniqueByName("RenderScenePS20");
    g_pEffect->SetTechnique( g_hShaderTech );
    g_pD3dDevice = pd3dDevice;
    g_hShaderTechTokamak = g_pEffect->GetTechniqueByName("RenderSceneTokamak");

    return D3D_OK;
}

void CALLBACK OnMyAppLostDevice( void* pUserContext )
{
    if( g_pEffect )
        g_pEffect->OnLostDevice();
}

HRESULT CALLBACK OnMyAppResetDevice( IDirect3DDevice9* pd3dDevice, const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
    HRESULT hr;

    if( g_pEffect )
        V_RETURN( g_pEffect->OnResetDevice() );

    return D3D_OK;
}

void MyRenderText(WCHAR ** StrList, int NumStr)
{
    CDXUTTextHelper txtHelper( g_pFont, g_pTextSprite, 15 );

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );

    for (int i = 0; i < NumStr; i++)
    {
        txtHelper.SetInsertionPos( 5, 5 + i * 15);
        txtHelper.DrawTextLine( *(StrList++) );
    }

    txtHelper.End();
}

CRenderPrimitive::CRenderPrimitive()
{
    mIsCylinder = 0;

    memset(&mMaterial, 0, sizeof(mMaterial));

    mMaterial.MatD3D.Diffuse.r = 0.7f;
    mMaterial.MatD3D.Diffuse.g = 0.7f;
    mMaterial.MatD3D.Diffuse.b = 0.7f;
    mMaterial.MatD3D.Diffuse.a = 1.0f;

    mMaterial.MatD3D.Ambient.r = 0.5f;
    mMaterial.MatD3D.Ambient.g = 0.5f;
    mMaterial.MatD3D.Ambient.b = 0.5f;
    mMaterial.MatD3D.Ambient.a = 1.0f;

    mMaterial.MatD3D.Specular.r = 1.0f;
    mMaterial.MatD3D.Specular.g = 1.0f;
    mMaterial.MatD3D.Specular.b = 1.0f;
    mMaterial.MatD3D.Specular.a = 1.0f;

    mMaterial.MatD3D.Power = 40;
}

void CRenderPrimitive::SetDiffuseColor(const D3DXCOLOR &c)
{
    mMaterial.MatD3D.Diffuse.r = c.r;
    mMaterial.MatD3D.Diffuse.g = c.g;
    mMaterial.MatD3D.Diffuse.b = c.b;
    mMaterial.MatD3D.Diffuse.a = c.a;
}

void CRenderPrimitive::SetGraphicBox(float Width, float Height, float Depth)
{
    //create the render mesh
    ID3DXMesh *pMesh;

    D3DXCreateBox( g_pD3dDevice,  
        Width, Height, Depth,
        &pMesh, NULL);

    mMesh.Create(g_pD3dDevice, pMesh, &mMaterial, 1);

    pMesh->Release();
}

void CRenderPrimitive::SetGraphicSphere(float radius)
{
    ID3DXMesh *pMesh;

    D3DXCreateSphere( g_pD3dDevice, radius,
        20, 20, &pMesh, NULL);

    mMesh.Create(g_pD3dDevice, pMesh, &mMaterial, 1);

    pMesh->Release();
}

void CRenderPrimitive::SetGraphicCylinder(float radius, float length)
{
    ID3DXMesh *pMesh;

    D3DXCreateCylinder( g_pD3dDevice, radius, radius, length,
        24, 3, &pMesh, NULL);

    mMesh.Create(g_pD3dDevice, pMesh, &mMaterial, 1);

    pMesh->Release();

    mIsCylinder = 1;
}

void CRenderPrimitive::Render(IDirect3DDevice9* pd3dDevice, neT3 * matrix)
{
    HRESULT hr;

    D3DXMATRIXA16 mView;
    D3DXMATRIXA16 mProj;
    D3DXMATRIXA16 mWorldView;
    D3DXMATRIXA16 mWorldViewProjection;

    D3DXMATRIXA16 mWorld;

    memcpy(&mWorld, matrix, sizeof(mWorld));

    mProj = *g_Camera.GetProjMatrix();
    mView = *g_Camera.GetViewMatrix();

    if (mIsCylinder)
    {
        D3DXMATRIXA16 rot;
        D3DXMatrixRotationX(&rot, NE_PI * 0.5f);
        mWorld = rot * mWorld; 
    }

    mWorldViewProjection = mWorld * mView * mProj;

    mWorldView = mWorld * mView;

    V( g_pEffect->SetMatrix( g_hMatWVP, &mWorldViewProjection ) );
    V( g_pEffect->SetMatrix( g_hMatWV, &mWorldView ) );
    V( g_pEffect->SetMatrix( g_hMatW, &mWorld ) );

    for (int i = 0; i < NUM_LIGHT; i++)
    {
        g_pEffect->SetVector(g_hLightTokamak[i], &vLightWorld[i]);
        g_pEffect->SetVector(g_hLightColorTokamak[i], &vLightColor[i]);
    }
    g_pEffect->SetVector(g_hViewPos, (D3DXVECTOR4*)g_Camera.GetEyePt());

    pd3dDevice->SetFVF( mMesh.m_pMesh->GetFVF() );

    g_pEffect->SetTechnique( g_hShaderTechTokamak );

    mMesh.RenderTokamak( g_pEffect, g_hDiffuse, (D3DXVECTOR4*)&mMaterial.MatD3D.Diffuse, 
        g_hSpecular, (D3DXVECTOR4*)&mMaterial.MatD3D.Specular, 
        g_hAmbient, (D3DXVECTOR4*)&mMaterial.MatD3D.Ambient,
        g_hPower, mMaterial.MatD3D.Power,
        true, false );
}

