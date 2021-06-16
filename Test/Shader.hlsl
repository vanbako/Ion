/*
Cel Shader
2DAE02 Koen Van Bastelaere

- Diffuse (Texture & Color)
	- Regular Diffuse
- Ambient (Color)
- Normal (Texture)
- Shadow

*/

// Global Matrices

float4x4 gMatrixWVP : WORLDVIEWPROJECTION;
float4x4 gMatrixViewInverse : VIEWINVERSE;
float4x4 gMatrixWorld : WORLD;
float4x4 gWorldViewProj_Light;

// Shadow

float gShadowMapBias = 0.002f;
Texture2D gShadowMap;

// States

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState gRS_FrontCulling
{ 
	CullMode = NONE; 
};

// Sampler States

SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
 	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};

SamplerComparisonState cmpSampler
{
	// sampler state
	Filter = COMPARISON_MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;

	// sampler comparison state
	ComparisonFunc = LESS_EQUAL;
};

// Light

float3 gLightDirection :DIRECTION
<
	string UIName = "Light Direction";
	string Object = "TargetLight";
> = float3(-0.577f, -0.577f, 0.577f);

// Diffuse

bool gUseTextureDiffuse
<
	string UIName = "Use Diffuse Texture?";
	string UIWidget = "Bool";
> = false;

float4 gColorDiffuse
<
	string UIName = "Diffuse Color";
	string UIWidget = "Color";
> = float4(1,1,1,1);

Texture2D gTextureDiffuse
<
	string UIName = "Diffuse Texture";
	string UIWidget = "Texture";
	string ResourceName = "CobbleStone_DiffuseMap.dds";
>;

// Ambient

float4 gColorAmbient
<
	string UIName = "Ambient Color";
	string UIWidget = "Color";
> = float4(0,0,0,1);

float gAmbientIntensity
<
	string UIName = "Ambient Intensity";
	string UIWidget = "slider";
	float UIMin = 0;
	float UIMax = 1;
	float UIStep = 0.01;
>  = 0.0f;

// Normal Mapping

bool gFlipGreenChannel
<
	string UIName = "Flip Green Channel";
	string UIWidget = "Bool";
> = false;

bool gUseTextureNormal
<
	string UIName = "Use Normal Texture?";
	string UIWidget = "Bool";
> = false;

Texture2D gTextureNormal
<
	string UIName = "Normal Texture";
	string UIWidget = "Texture";
	string ResourceName = "CobbleStone_NormalMap.dds";
>;

// Shadow

bool gUseShadow
<
	string UIName = "Use Shadow?";
	string UIWidget = "Bool";
> = false;

// VS In & Out

struct VS_Input
{
	float3 Position: POSITION;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
};

struct VS_Output
{
	float4 Position: SV_POSITION;
	float4 WorldPosition: COLOR0;
	float3 Normal: NORMAL;
	float3 Tangent: TANGENT;
	float2 TexCoord: TEXCOORD0;
	float4 lPos : TEXCOORD1;
};

struct PS_OUTPUT
{
	float4 Color: SV_Target0;
	float4 Normal: SV_Target1;
};

// Diffuse Function

float3 CalculateDiffuse(float3 normal, float2 texCoord)
{
	float3 diffuseColor;
	if (gUseTextureDiffuse)
		diffuseColor = (gTextureDiffuse.Sample( gTextureSampler, texCoord ) * gColorDiffuse).xyz;
	else
		diffuseColor = gColorDiffuse.xyz;

	float diffuseStrength = saturate(dot(normal, -gLightDirection));
   	if (diffuseStrength > 0.95)
       	diffuseColor = float3(1.0, 1.0, 1.0) * diffuseColor;
   	else if (diffuseStrength > 0.5)
       	diffuseColor = float3(0.7, 0.7, 0.7) * diffuseColor;
   	else if (diffuseStrength > 0.05)
       	diffuseColor = float3(0.35, 0.35, 0.35) * diffuseColor;
   	else
       	diffuseColor = float3(0.1, 0.1, 0.1) * diffuseColor;
	return diffuseColor;
}

// Normal Mapping Function

float3 CalculateNormal(float3 tangent, float3 normal, float2 texCoord)
{
	float3 newNormal = normal;
	if (gUseTextureNormal)
	{
		float3 binormal = normalize(cross(tangent, normal));
		if (gFlipGreenChannel)
			binormal = -binormal;
		float3x3 localAxis = transpose( float3x3( tangent, binormal, normal )) ;
		float3 sampledNormal = gTextureNormal.Sample(gTextureSampler, texCoord).xyz;
		sampledNormal = (sampledNormal * 2.0f) - 1.0f;
		newNormal = normalize( mul(localAxis, sampledNormal) );
	}

	return newNormal;
}

// Main Vertex Shader

VS_Output MainVS(VS_Input input) {
	
	VS_Output output = (VS_Output)0;
	
	output.Position = mul(float4(input.Position, 1.0f), gMatrixWVP);
	output.WorldPosition = mul(float4(input.Position, 1.0f), gMatrixWorld);
	output.Normal = mul(input.Normal, (float3x3)gMatrixWorld);
	output.Tangent = mul(input.Tangent, (float3x3)gMatrixWorld);
	output.TexCoord = input.TexCoord;
	output.lPos = mul(float4(input.Position, 1.0f), gWorldViewProj_Light);
	
	return output;
}

float2 texOffset(int u, int v)
{
	return float2(u / 1280.0f, v / 720.0f);
}

float EvaluateShadowMap(float4 lpos)
{
	lpos.xyz /= lpos.w;
	if (lpos.x < -1.0f || lpos.x > 1.0f ||
		lpos.y < -1.0f || lpos.y > 1.0f ||
		lpos.z < 0.0f || lpos.z > 1.0f)
		return 1.0f;
	lpos.x = lpos.x / 2 + 0.5f;
	lpos.y = lpos.y / -2 + 0.5f;
	lpos.z -= gShadowMapBias;
	float sum = 0;
	float x, y;
	for (y = -1.5f; y <= 1.5f; y += 1.0f)
	{
		for (x = -1.5f; x <= 1.5f; x += 1.0f)
		{
			sum += gShadowMap.SampleCmpLevelZero(cmpSampler, lpos.xy + texOffset(x, y), lpos.z);
		}
	}
	return sum / 16.0f;
}

// Main Pixel Shader

PS_OUTPUT MainPS(VS_Output input) : SV_TARGET
{
	PS_OUTPUT output;
	// Normalize
	input.Normal = normalize(input.Normal);
	input.Tangent = normalize(input.Tangent);
	
	//float3 viewDirection = normalize(input.WorldPosition.xyz - gMatrixViewInverse[3].xyz);
		
	// Normal
	float3 newNormal = CalculateNormal(input.Tangent, input.Normal, input.TexCoord);
		
	// Diffuse
	float3 diffColor = CalculateDiffuse(newNormal, input.TexCoord);
		
	// Ambient
	float3 ambientColor = gColorAmbient.xyz * gAmbientIntensity;
		
	// Final Color Calculation
	float3 finalColor = diffColor + ambientColor;
	if (gUseShadow)
		finalColor *= EvaluateShadowMap(input.lPos) / 2.0f + 0.5f;
	
	output.Color = float4(finalColor, 1.0f);
	output.Normal = float4(newNormal, 1.0f);
	return output;
}

technique11 Default {
	pass p0 {
		SetRasterizerState(gRS_FrontCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetHullShader( NULL );
		SetDomainShader( NULL );
		SetGeometryShader( NULL );
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
