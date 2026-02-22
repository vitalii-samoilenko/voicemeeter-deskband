struct PSInput {
	float4 position : SV_POSITION;
	float2 coord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);
uint g_layers : register(c0);
float g_offset : register(c1);

float4 Main(PSInput input) : SV_TARGET {
	float4 rgba = g_texture.Sample(g_sampler, input.coord);
	for (uint layer = 1; layer < g_layers; ++layer) {
		input.coord.v += g_offset;
		float4 temp = g_texture.Sample(g_sampler, input.coord);
		rgba.rgb = temp.rgb * temp.a + rgba.rgb * (1 - temp.a);
	}
	return rgba;
}
