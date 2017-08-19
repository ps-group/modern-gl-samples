#pragma once
#include "Mesh.h"
#include "libmath/Transform2D.h"
#include "libplatform/OpenGL.h"

struct TexturedShape
{
	math::Transform2D transform;
	ps::TextureObject texture;
	MeshP2T2 mesh;
};
