struct PSInput {
	float4 position : SV_POSITION;
	float2 coord : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);
float4 g_color : register(c0);
float g_screenPxRange : register(c1);

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

float4 Main(PSInput input) : SV_TARGET {
	float3 msd = g_texture.Sample(
		g_sampler, input.coord);
	float sd = median(msd.r, msd.g, msd.b);
	float screenPxDistance = g_screenPxRange * (sd - 0.5);
	float a = clamp(0.5 + screenPxDistance, 0.0, 1.0);
	return float4(
		g_color.rgb,
		a * g_color.a);
}
