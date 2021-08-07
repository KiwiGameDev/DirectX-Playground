struct VS_INPUT
{
	float4 pos : POSITION0;
	float3 color : COLOR0;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

cbuffer constant: register(b0)
{
	row_major float4x4 m_world;
    row_major float4x4 m_game_cam;
	row_major float4x4 m_editor_view;
	row_major float4x4 m_editor_proj;
    int m_is_project;
    float m_time;
}

VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output;

    float4 pos = input.pos;
    pos.w = 1.0f;
    pos = mul(pos, m_world);

	// Projection to other camera's NDC space
    if (m_is_project)
    {
		float4 ndc = mul(pos, m_game_cam);
        output.pos = lerp(pos, ndc, (sin(m_time) + 1.0f) / 2.0f);
    }
	else
	{
        output.pos = pos;
    }

	// Projection to main camera
    output.pos = mul(output.pos, m_editor_view);
    output.pos = mul(output.pos, m_editor_proj);
	output.color = input.color;
	return output;
}