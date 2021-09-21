#pragma once
#include "Prerequisites.h"

class MeshFactory
{
public:
	static void initializeAllMeshes();
	
	static MeshPtr createSphereMesh();
	static MeshPtr createCubeMesh();
	static MeshPtr createTexturedCubeMesh();
	static MeshPtr createCapsuleMesh(float height = 0.5f, float radius = 1.0f);
	static MeshPtr createPlaneMesh();
};

