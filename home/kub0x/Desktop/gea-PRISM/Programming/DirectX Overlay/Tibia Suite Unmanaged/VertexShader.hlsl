cbuffer ShaderConstants : register(b0) {
	float4 g_translationAndExtent : packoffset(c0);
	float4 g_diffuseColor : packoffset(c1);
};

struct VertexShaderInput {
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
};

struct VertexShaderOutput {
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
	float4 color : COLOR0;
};

// shader entry point
void VertexMain(in VertexShaderInput input, out VertexShaderOutput output) {
	output.position.xy = input.position.xy + g_translationAndExtent.xy;
	output.position.x = (input.position.x * 2.0f) / g_translationAndExtent.z - 1.0f;
	output.position.y = 1.0f - (input.position.y * 2.0f) / g_translationAndExtent.w;
	output.position.zw = float2(0.0f, 1.0f);
	output.texCoord = input.texCoord + (1.0f / (g_translationAndExtent.zw * 2.0f));
	output.color.r = input.color.r * g_diffuseColor.r;
	output.color.g = input.color.g * g_diffuseColor.g;
	output.color.b = input.color.b * g_diffuseColor.b;
	output.color.a = input.color.a * g_diffuseColor.a;
}