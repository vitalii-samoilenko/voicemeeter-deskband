struct VSOutput {
	float4 position : SV_POSITION;
	float2 coord : TEXCOORD;
};
struct Coord {
	float4 value;
};

ConstantBuffer<Coord> g_coord : register(b0, space0);

VSOutput Main(float4 position : POSITION, uint id : SV_VertexID) {
	VSOutput output;
	output.position = position;
	output.coord.x = id < 2
		? g_coord.value.x
		: g_coord.value.z;
	output.coord.y = id % 2
		? g_coord.value.w
		: g_coord.value.y;
	return output;
}
