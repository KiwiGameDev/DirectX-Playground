struct PS_INPUT
{
	float4 pos : SV_POSITION;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float m_time;
}

float4 main(PS_INPUT input) : SV_TARGET
{
    return float4(0.8f, 0.8f, 0.8f, 1.0f);
}