struct PSInput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);
float4 g_color : register(c0);
float4 g_transform : register(c1);

float4 Main(PSInput input) : SV_TARGET {
    return float4(
        g_color.rgb,
        g_texture.Sample(
                g_sampler,
                g_transform.xy + g_transform.zw * input.uv)
            .a * g_color.a);
}