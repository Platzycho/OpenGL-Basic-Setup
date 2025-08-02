
#include "Object.h"
#include "Shader.h"

//Try to make a class that could function as a prototype of a card template

Object::Object(Object_Type object_type,float scale, glm::vec3 color) : Position(0.0f), Scale(1.0f), RotationAngle(0.0f), RotationAxis(1.0f, 1.0f, 1.0f)
{
	if (object_type == CUBE)
	{
		CreateCube(1.0f*scale, 1.0f * scale, 1.0f * scale, color);
		SetupElementMesh();
	}
	RotationAngle = 0;
	RotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
	UpdateModelMatrix();
}

Object::~Object()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Object::Draw(Shader& shader)
{
	shader.use();
	shader.setMat4("model", ModelMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

//void Object::DrawLines(Shader& shader)
//{
//	shader.use();
//	shader.setMat4("model", ModelMatrix);
//	glBindVertexArray(VAO);
//	glDrawArrays(GL_LINEAR, 0, Vertices.size());
//	glBindVertexArray(0);
//}

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

void Object::changeColor(glm::vec3 color)
{
	CreateCube(1.f, 1.f, 1.f, color);
	UpdateModelMatrix();
}

void Object::rotate(glm::mat4 model, float time, glm::vec3 rotation)
{

}

void Object::SetupElementMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
	glEnableVertexAttribArray(1);

	// Barycentric coord
	/*glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexMex));
	glEnableVertexAttribArray(2);*/

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::UpdateModelMatrix()
{
	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, Position);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(RotationAngle), RotationAxis);
	ModelMatrix = glm::scale(ModelMatrix, Scale);
}
