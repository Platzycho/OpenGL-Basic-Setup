#pragma once
#include <string>
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

enum Card_Tribe
{
	BEAST,
	ELF,
	THERIAN,
	DRAGON,
	MAGIC
};

enum Card_Type
{
	FIRE,
	EARTH,
	AIR,
	WATER,
	LIGHT,
	DARK
};


class Object
{
public:
	Object(Object_Type object_type = CUBE, float scale = 1.f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	~Object();

	void Draw(Shader& shader);
	void DrawLines(Shader& shader);
	void CreateCube(float width = 1.0f, float height = 1.0f, float depth = 1.0f, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	void CreateCubeOutlines(float width = 1.0f, float height = 1.0f, float depth = 1.0f, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f));

	void CreateSphere(float radius = 1.0f, unsigned int sectorCount = 10, unsigned int stackCount = 10, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));
	void CreateSphereOutlines(float radius = 1.0f, unsigned int sectorCount = 10, unsigned int stackCount = 10, glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f));

	void CreateCard(Card_Tribe tribe, Card_Type type, int cost, int attack, int health, std::string name, std::string description);

	void ChangeColor(glm::vec3 color);
	void SetRotation(float rotAngle, glm::vec3 rotAxis = glm::vec3(1.0f, 1.0f, 1.0f));
	void UpdatePosition(glm::vec3 position);

private:
	unsigned int VAO, VBO, EBO;
	unsigned int VAOut, VBOut, EBOut;
	std::vector<Vertex> Vertices;
	std::vector<Vertex> OutlineVertices;
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::mat4 ModelMatrix;
	glm::vec3 RotationAxis;
	std::vector<unsigned int> Indices;
	std::vector<unsigned int> OutlineIndices;

	float RotationAngle;

	void SetupElementMesh();
	void SetupOutlineElementMesh();
	void UpdateModelMatrix();
};

