struct PS_INPUT
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

float4 psmain(PS_INPUT input) : SV_TARGET
{
	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}