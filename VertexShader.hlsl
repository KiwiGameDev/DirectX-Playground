struct VS_INPUT
{
	float4 pos: POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};

struct VS_OUTPUT
{
	float4 pos: SV_POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};

cbuffer constant: register(b0)
{
	unsigned int m_time;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;
	output.pos = input.pos;
	output.color = input.color;
	output.color1 = input.color1;
	return output;
}