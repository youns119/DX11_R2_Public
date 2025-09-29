#include "Shader_Defines.hlsli"

float4x4 g_matWorld, g_matView, g_matProj;

Texture2D g_DiffuseTexture[2];
//Texture2D g_NormalTexture;
Texture2D g_MaskTexture;
Texture2D g_BrushTexture;

vector g_vBrushPosition = vector(20.f, 0.f, 20.f, 1.f);
float g_fBrushRange = 10.f;

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    //float3 vTangent : TANGENT;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    //float4 vTangent : TANGENT;
    //float4 vBinormal : BINORMAL;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    matrix matWV, matWVP;

    matWV = mul(g_matWorld, g_matView);
    matWVP = mul(matWV, g_matProj);
    
    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vNormal = normalize(mul(float4(In.vNormal, 0.f), g_matWorld));

    Out.vTexCoord = In.vTexCoord;
    
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_matWorld);
    Out.vProjPos = Out.vPosition;
    //Out.vTangent = normalize(mul(float4(In.vTangent, 0.f), g_matWorld));
    //Out.vBinormal = vector(normalize(cross(Out.vNormal.xyz, Out.vTangent.xyz)), 0.f);

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
    //float4 vTangent : TANGENT;
    //float4 vBinormal : BINORMAL;
};

struct PS_OUT
{
    float4 vDiffuse : SV_TARGET0;
    float4 vNormal : SV_TARGET1;
    float4 vDepth : SV_TARGET2;
    float4 vPickDepth : SV_TARGET3;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vSrcDiffuse = g_DiffuseTexture[0].Sample(LinearSampler, In.vTexCoord * 30.f);
    vector vDstDiffuse = g_DiffuseTexture[1].Sample(LinearSampler, In.vTexCoord * 30.f);

    vector vMask = g_MaskTexture.Sample(LinearSampler, In.vTexCoord);
    vector vBrush = 0.f;
    
    if (g_vBrushPosition.x - g_fBrushRange <= In.vWorldPos.x && g_vBrushPosition.x + g_fBrushRange > In.vWorldPos.x &&
        g_vBrushPosition.z - g_fBrushRange <= In.vWorldPos.z && g_vBrushPosition.z + g_fBrushRange > In.vWorldPos.z)
    {
        float2 vTexCoord = 0.f;
        
        vTexCoord.x = (In.vWorldPos.x - (g_vBrushPosition.x - g_fBrushRange)) / (2.f * g_fBrushRange);
        vTexCoord.y = ((g_vBrushPosition.z + g_fBrushRange) - In.vWorldPos.z) / (2.f * g_fBrushRange);

        vBrush = g_BrushTexture.Sample(LinearSampler, vTexCoord);
    }

    vector vMtrlDiffuse = vDstDiffuse * vMask + vSrcDiffuse * (1.f - vMask) + vBrush;

    //if (vMtrlDiffuse.a < 0.1f)
    //    discard;

    //vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexCoord);
    //float3 vNormal = vNormalDesc.xyz * 2.f - 1.f;
    
    //float3x3 matWorld = float3x3(In.vTangent.xyz, In.vBinormal.xyz, In.vNormal.xyz);

    //vNormal = normalize(mul(vNormal, matWorld));
    
    Out.vDiffuse = vector(vMtrlDiffuse.rgb, 1.f);
    Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
    //Out.vNormal = vector(vNormal * 0.5f + 0.5f, 0.f);
    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 0.f, 0.f);
    Out.vPickDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w, 1.f, 0.f);
    
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}