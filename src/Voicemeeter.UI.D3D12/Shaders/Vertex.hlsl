struct VSOutput {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VSOutput Main(float4 position : POSITION) {
    VSOutput output;
    output.position = position;
    output.uv = position;
    return output;
}