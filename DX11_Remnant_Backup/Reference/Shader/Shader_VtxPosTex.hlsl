#include "Shader_Defines.hlsli"

float4x4 g_matWorld, g_matView, g_matProj;

Texture2D g_Texture;
Texture2D g_DepthTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexCoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_matWorld);
    Out.vWorldPos = Out.vPosition;
    Out.vPosition = mul(Out.vPosition, g_matView);
    Out.vPosition = mul(Out.vPosition, g_matProj);

    Out.vTexCoord = In.vTexCoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
 
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexCoord);

    if (Out.vColor.a <= 0.2f)
        discard;
    
    return Out;
}

struct VS_OUT_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

VS_OUT_EFFECT VS_MAIN_EFFECT(VS_IN In)
{
    VS_OUT_EFFECT Out = (VS_OUT_EFFECT) 0;

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_matWorld);
    Out.vWorldPos = Out.vPosition;

    Out.vPosition = mul(Out.vPosition, g_matView);
    Out.vPosition = mul(Out.vPosition, g_matProj);
    
    Out.vTexCoord = In.vTexCoord;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct PS_IN_EFFECT
{
    float4 vPosition : SV_POSITION;
    float2 vTexCoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
    float4 vProjPos : TEXCOORD2;
};

PS_OUT PS_MAIN_EFFECT(PS_IN_EFFECT In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexCoord);

    float2 vTexCoord;

    vTexCoord.x = (In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f;
    vTexCoord.y = (In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f;

    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, vTexCoord);

    Out.vColor.a = Out.vColor.a * saturate(vDepthDesc.y - In.vProjPos.w);

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

    pass Effect
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        VertexShader = compile vs_5_0 VS_MAIN_EFFECT();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_EFFECT();
    }
}