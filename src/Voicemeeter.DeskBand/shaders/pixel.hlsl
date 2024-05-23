struct Input
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

float4 g_color : register(b0);

float4 main(Input input) : SV_TARGET
{
    float alpha = g_texture.Sample(g_sampler, input.uv);
    return g_color * alpha;
}