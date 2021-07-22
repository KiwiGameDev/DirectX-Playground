struct VS_INPUT
{
	float4 pos_start	: POSITION0;
	float4 pos_end		: POSITION1;
	float3 color_start	: COLOR0;
    float3 color_end	: COLOR1;
};

struct VS_OUTPUT
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

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;
	
    output.pos = lerp(input.pos_start, input.pos_end, (sin(m_time) + 1.0f) / 2.0f); //-1..1  ->  0..1
    output.color0 = input.color_start;
    output.color1 = input.color_end;
	return output;
}