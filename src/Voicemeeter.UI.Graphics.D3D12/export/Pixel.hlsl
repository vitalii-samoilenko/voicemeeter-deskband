struct PSInput {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);
float4 g_color : register(c0);
float4 g_transform : register(c1);

float4 Main(PSInput input) : SV_TARGET {
	float alpha = g_texture.Sample(
			g_sampler,
			g_transform.xy + g_transform.zw * input.uv)
		.r - 0.5;
	alpha = clamp(0.5 + alpha / fwidth(alpha), 0.0, 1.0);
	return float4(
		g_color.rgb,
		alpha * g_color.a);
}
