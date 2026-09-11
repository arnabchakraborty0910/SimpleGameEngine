#pragma once

#include <glad/glad.h>

class Mesh
{
public:
	Mesh();
	void draw() const;

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int vertexCount;

};

