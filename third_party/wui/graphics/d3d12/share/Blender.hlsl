struct PSInput {
	float4 position : SV_POSITION;
	float2 coord : TEXCOORD;
};

SamplerState g_sampler : register(s0);
Texture2D g_texture : register(t0);

float4 Main(PSInput input) : SV_TARGET {
	float4 rgba = g_texture.Sample(g_sampler, input.coord);
	return rgba;
}
