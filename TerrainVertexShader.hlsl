Texture2D<float> Texture : register(t0);
SamplerState TextureSampler : register(s0);

struct VS_INPUT
{
	float4 pos: POSITION;
};

struct VS_OUTPUT
{
	float4 pos: SV_POSITION;
    float4 pos_world: POSITION;
};

cbuffer constant : register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
	unsigned int m_time;
};

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    float4 position = input.pos;
    position.y += Texture.SampleLevel(TextureSampler, input.pos.xz, 0) * 8.0f;
	if (position.y <= 2.66f)
        position.y = 2.66f;
    output.pos_world = mul(position, m_world);
    output.pos = mul(output.pos_world, m_view);
	output.pos = mul(output.pos, m_proj);
	return output;
}
