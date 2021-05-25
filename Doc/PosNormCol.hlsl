float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

struct VS_INPUT{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};
struct VS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float4 color : COLOR;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};


VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = mul ( float4(input.pos,1.0f), gWorldViewProj );
	output.normal = normalize(mul(input.normal, (float3x3)gWorld));
	output.color = input.color;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float3 color_rgb= input.color.rgb;
	float color_a = input.color.a;
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	return float4( color_rgb , color_a );
}

technique11 Default
{
    pass P0
    {
	SetRasterizerState(NoCulling);
	SetDepthStencilState(EnableDepth, 0);
	SetVertexShader( CompileShader( vs_5_1, VS() ) );
	SetGeometryShader( NULL );
	SetPixelShader( CompileShader( ps_5_1, PS() ) );
    }
}
