#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include "Structs.h"

class Shader;

enum Object_Type
{
	CUBE,
	SPHERE,
	PYRAMID,
	PLANE
};


class Object
{
	Object(Object_Type object_type = CUBE);
	~Object();

	void Draw(Shader& shader);
	void CreateCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

private:
	unsigned int VAO, VBO, EBO;
	std::vector<Vertex> Vertices;
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::mat4 ModelMatrix;
	glm::vec3 RotationAxis;
	std::vector<unsigned int> Indices;

	float RotationAngle;

	void SetupElementMesh();
	void UpdateModelMatrix();
};

