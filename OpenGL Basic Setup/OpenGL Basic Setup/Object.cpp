
#include "Object.h"
#include "Shader.h"

//Try to make a class that could function as a prototype of a card template

Object::Object(Object_Type object_type)
{
	if (object_type == CUBE)
	{
		CreateCube();
		SetupElementMesh();
	}
}

Object::~Object()
{
}

void Object::Draw(Shader& shader)
{
	shader.use();
	shader.setMat4("model", ModelMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Object::CreateCube(float width, float height, float depth, glm::vec3 color)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	float halfDepth = depth * 0.5f;

	Vertices= {
		{{-halfWidth, -halfHeight, -halfDepth}, {color}},
		{{halfWidth, -halfHeight, -halfDepth}, {color}},
		{{halfWidth, halfHeight, -halfDepth}, {color}},
		{{-halfWidth, halfHeight, -halfDepth}, {color}},

		{{-halfWidth, -halfHeight, halfDepth}, {color}},
		{{halfWidth, -halfHeight, halfDepth}, {color}},
		{{halfWidth, halfHeight, halfDepth}, {color}},
		{{-halfWidth, halfHeight, halfDepth}, {color}}
	};

	Indices = {
		// Back face
		0, 1, 2, 2, 3, 0,
		// Front face
		4, 5, 6, 6, 7, 4,
		// Left face
		0, 3, 7, 7, 4, 0,
		// Right face
		1, 5, 6, 6, 2, 1,
		// Bottom face
		0, 1, 5, 5, 4, 0,
		// Top face
		3, 2, 6, 6, 7, 3
	};
}

void Object::SetupElementMesh()
{
}

void Object::UpdateModelMatrix()
{
}
