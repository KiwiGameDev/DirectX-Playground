struct PS_INPUT
{
	float4 pos		: SV_POSITION;
    float3 color0	: COLOR0;
    float3 color1	: COLOR1;
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
    return float4(lerp(input.color0, input.color1, (sin(m_time) + 1.0f) / 2.0f).rgb, 1.0f);
}