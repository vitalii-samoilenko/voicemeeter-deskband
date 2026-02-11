struct VSOutput {
	float4 position : SV_POSITION;
	float2 coord : TEXCOORD;
};

float4 g_coord : register(c0);

VSOutput Main(float4 position : POSITION, uint id : SV_VertexID) {
	VSOutput output;
	output.position = position;
	output.coord.x = id < 2
		? g_coord.x
		: g_coord.z;
	output.coord.y = id % 2
		? g_coord.w
		: g_coord.y;
	return output;
}
