#pragma once

#include <glad/glad.h>

enum class Primitive { Cube, Plane };

class Mesh
{
public:
	Mesh(Primitive type = Primitive::Cube);
	void draw() const;

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int vertexCount;

};

