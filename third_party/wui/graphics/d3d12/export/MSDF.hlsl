struct PSInput {
	float4 position : SV_POSITION;
	float2 coord : TEXCOORD;
};
struct ScreenPxRange {
	float value;
};
struct Color {
	float4 value;
};

ConstantBuffer<ScreenPxRange> g_screenPxRange : register(b0, space0);
ConstantBuffer<Color> g_color : register(b1, space0);
SamplerState g_sampler : register(s0);
Texture2D g_texture : register(t0);

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

float4 Main(PSInput input) : SV_TARGET {
	float3 msd = g_texture.Sample(g_sampler, input.coord);
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = g_screenPxRange.value * (sd - 0.5);
	float a = clamp(screenPxDistance + 0.5, 0.0, 1.0);
	return g_color.value * a;
}
