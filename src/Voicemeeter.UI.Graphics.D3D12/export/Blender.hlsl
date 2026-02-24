struct PSInput {
	float4 position : SV_POSITION;
	float2 coord : TEXCOORD;
};
struct Layers {
	uint value;
};
struct Offset {
	float value;
};

ConstantBuffer<Layers> g_layers : register(b0, space0);
ConstantBuffer<Offset> g_offset : register(b1, space0);
SamplerState g_sampler : register(s0);
Texture2D g_texture : register(t0);

float4 Main(PSInput input) : SV_TARGET {
	float4 rgba = g_texture.Sample(g_sampler, input.coord);
	for (uint layer = 1; layer < g_layers.value; ++layer) {
		input.coord.y += g_offset.value;
		float4 temp = g_texture.Sample(g_sampler, input.coord);
		rgba = temp + rgba * (1 - temp.a);
	}
	return rgba;
}
