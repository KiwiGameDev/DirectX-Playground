struct PS_INPUT
{
	float4 pos : SV_POSITION;
    float3 color : COLOR0;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	float m_time;
}

float4 psmain(PS_INPUT input) : SV_TARGET
{
    return float4(input.color.rgb, 1.0f);
}