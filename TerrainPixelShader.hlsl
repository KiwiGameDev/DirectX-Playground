struct PS_INPUT
{
	float4 pos: SV_POSITION;
	float4 pos_world: POSITION;
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
    float y_norm = input.pos_world.y / 8.0f;
	
    if (y_norm <= 0.34f) // Water
        return float4(0.21f, 0.62f, 0.79f, 1.0f); 
    if (y_norm <= 0.45f) // Sand
        return float4(0.85f, 0.90f, 0.63f, 1.0f); 
	if (y_norm <= 0.75f) // Sand -> Grass
        return lerp(float4(0.85f, 0.90f, 0.63f, 1.0f), float4(0.19f, 0.89f, 0.25f, 1.0f), (y_norm - 0.45f) / 0.3f);
	if (y_norm <= 0.85f) // Grass
        return float4(0.19f, 0.89f, 0.25f, 1.0f);
    return float4(0.95f, 0.95f, 0.95f, 1.0f);
}
