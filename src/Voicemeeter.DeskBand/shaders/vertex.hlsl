struct Output
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD;
};

Output main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
    Output result;
    result.pos = pos;
    result.uv = uv;
    return result;
}