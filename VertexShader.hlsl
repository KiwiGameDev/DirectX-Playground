struct VS_INPUT
{
	float4 pos : POSITION0;
	float3 color1 : COLOR0;
	float3 color2 : COLOR1;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 color1 : COLOR0;
	float3 color2 : COLOR1;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
    float m_time;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;
	
    output.pos = mul(input.pos, m_world);
    output.pos = mul(output.pos, m_view);
    output.pos = mul(output.pos, m_proj);
	output.color1 = input.color1;
	output.color2 = input.color2;
	return output;
}