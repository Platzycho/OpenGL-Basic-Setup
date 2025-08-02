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

	void CreateCard(Card_Tribe tribe, Card_Type type, int cost, int attack, int health, std::string name, std::string description);

	void changeColor(glm::vec3 color);
	void rotate(glm::mat4 model, float time, glm::vec3 rotation);

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

