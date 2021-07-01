struct VS_INPUT
{
	float4 pos: POSITION;
};

struct VS_OUTPUT
{
	float4 pos: SV_POSITION;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;
	// output.pos = lerp(input.pos, input.pos1, (sin(m_time / 1000.0f) + 1.0f) / 2.0f);
	output.pos = mul(input.pos, m_world);
	output.pos = mul(output.pos, m_view);
	output.pos = mul(output.pos, m_proj);
	return output;
}