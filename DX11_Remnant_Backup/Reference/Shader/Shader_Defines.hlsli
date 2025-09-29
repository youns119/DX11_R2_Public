sampler LinearSampler = sampler_state
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = wrap;
	AddressV = wrap;
};

sampler PointSampler = sampler_state
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = wrap;
    AddressV = wrap;
};

RasterizerState RS_Default
{
	CullMode = BACK;
	
};
RasterizerState RS_Wireframe
{
	FillMode = WIREFRAME;
};

RasterizerState RS_Cull_CW
{
	FillMode = SOLID;
	CullMode = FRONT;
};

DepthStencilState DSS_Default
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};

DepthStencilState DSS_Skip_Z
{
	DepthEnable = FALSE;
	DepthWriteMask = ZERO;
};

BlendState BS_Default
{
	BlendEnable[0] = FALSE;
};

BlendState BS_AlphaBlend
{
	BlendEnable[0] = TRUE;	

	BlendOp = Add;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};

BlendState BS_Blend
{
	BlendEnable[0] = TRUE;
	BlendEnable[1] = TRUE;

	BlendOp = Add;
	SrcBlend = ONE;
	DestBlend = ONE;
};
