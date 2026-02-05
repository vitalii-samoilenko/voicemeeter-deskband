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
        .r;
    // 0.04 = 0.6
    // 0.02 = 1.2
    // 0.01 = 2.
    // 0.009 = 2.6
    alpha = alpha < 0.5 ? 0. : 1.;//(clamp(alpha, 0.4955, 0.5045) - 0.4955) / 0.009;
    return float4(
        g_color.rgb,
        alpha * g_color.a);
}