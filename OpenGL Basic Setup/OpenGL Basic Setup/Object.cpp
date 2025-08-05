
#include "Object.h"
#include "Shader.h"
#include "ObjectStorage.h"

const double M_PI = 3.141592653589793;


//Try to make a class that could function as a prototype of a card template

Object::Object(Object_Type object_type,float scale, glm::vec3 color) : Position(0.0f), Scale(1.0f), RotationAngle(0.0f), RotationAxis(1.0f, 1.0f, 1.0f)
{
	switch(object_type)
	{
	case CUBE:
		CreateCube(1.0f*scale, 1.0f * scale, 1.0f * scale, color);
		SetupElementMesh();
		CreateCubeOutlines(1.01f * scale, 1.01f * scale, 1.01f * scale);
		SetupOutlineElementMesh();
		ObjectStorage::getInstance().AddCubeObject(this);
		break;

	case SPHERE:
		CreateSphere(1.0f * scale, 10, 10, color);
		SetupElementMesh();
		CreateSphereOutlines(1.01f * scale, 10, 10);
		SetupOutlineElementMesh();
		ObjectStorage::getInstance().AddSphereObject(this);
		break;

	case PYRAMID:

		break;

	case PLANE:
		CreatePlane();
		SetupElementMesh();
		CreatePlaneOutlines();
		SetupOutlineElementMesh();
		ObjectStorage::getInstance().AddPlaneObject(this);

		break;

	case AXIS:
		CreateAxisLines();
		SetupOutlineElementMesh();
		break;

	default:
		break;
	}
	RotationAngle = 0;
	RotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
	UpdateModelMatrix();
	ObjectStorage::getInstance().AddObject(this);
}

Object::~Object()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glDeleteVertexArrays(1, &VAOut);
	glDeleteBuffers(1, &VBOut);
	glDeleteBuffers(1, &EBOut);
}

void Object::Draw(Shader& shader)
{
	shader.use();
	shader.setMat4("model", ModelMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Object::DrawLines(Shader& shader)
{
	shader.use();
	shader.setMat4("model", ModelMatrix);
	glBindVertexArray(VAOut);
	glLineWidth(4.0f);
	glDrawElements(GL_LINES, OutlineIndices.size(), GL_UNSIGNED_INT, nullptr);
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

void Object::CreateCubeOutlines(float width, float height, float depth, glm::vec3 color)
{
	float halfWidth = width * 0.5f;
	float halfHeight = height * 0.5f;
	float halfDepth = depth * 0.5f;

	OutlineVertices = {
		{{-halfWidth, -halfHeight, -halfDepth}, {color}},
		{{halfWidth, -halfHeight, -halfDepth}, {color}},
		{{halfWidth, halfHeight, -halfDepth}, {color}},
		{{-halfWidth, halfHeight, -halfDepth}, {color}},

		{{-halfWidth, -halfHeight, halfDepth}, {color}},
		{{halfWidth, -halfHeight, halfDepth}, {color}},
		{{halfWidth, halfHeight, halfDepth}, {color}},
		{{-halfWidth, halfHeight, halfDepth}, {color}}
	};

	OutlineIndices = 
	{
		// Back face (4 edges)
	0, 1, 1, 2, 2, 3, 3, 0,
	// Front face (4 edges)
	4, 5, 5, 6, 6, 7, 7, 4,
	// Connecting edges (4 edges)
	0, 4, 1, 5, 2, 6, 3, 7
	};
}

void Object::CreateSphere(float radius, unsigned int sectorCount, unsigned int stackCount, glm::vec3 color)
{
	float x, y, z, xy;
	float sectorStep = 2 * M_PI / sectorCount;
	float stackStep = M_PI / stackCount;
	float sectorAngle, stackAngle;

	for (unsigned int i = 0; i <= stackCount; ++i) {
		stackAngle = M_PI / 2 - i * stackStep; 
		xy = radius * cosf(stackAngle); 
		z = radius * sinf(stackAngle);

		for (unsigned int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep; 

			
			x = xy * cosf(sectorAngle); 
			y = xy * sinf(sectorAngle); 
			Vertex vertex;
			vertex.Position = glm::vec3(x, y, z);
			float u = (float)j / sectorCount;
			float v = (float)i / stackCount;
			//vertex.TexMex = glm::vec2(u, v);
			vertex.Color = color;

			//vertex.Barycentric = (i + j) % 2 == 0 ? glm::vec3(1.0, 0.0, 0.0) : glm::vec3(0.0, 1.0, 0.0);

			Vertices.push_back(vertex);
		}
	}

	for (unsigned int i = 0; i < stackCount; ++i) {
		for (unsigned int j = 0; j < sectorCount; ++j) {
			unsigned int first = i * (sectorCount + 1) + j;
			unsigned int second = first + sectorCount + 1;

			Indices.push_back(first);
			Indices.push_back(second);
			Indices.push_back(first + 1);

			Indices.push_back(second);
			Indices.push_back(second + 1);
			Indices.push_back(first + 1);
		}
	}
}

void Object::CreateSphereOutlines(float radius, unsigned int sectorCount, unsigned int stackCount, glm::vec3 color)
{
	float x, y, z, xy; 
	float sectorStep = 2 * M_PI / sectorCount;
	float stackStep = M_PI / stackCount;
	float sectorAngle, stackAngle;

	for (unsigned int i = 0; i <= stackCount; ++i) {
		stackAngle = M_PI / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (unsigned int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep;


			x = xy * cosf(sectorAngle);
			y = xy * sinf(sectorAngle);
			Vertex vertex;
			vertex.Position = glm::vec3(x, y, z);
			float u = (float)j / sectorCount;
			float v = (float)i / stackCount;
			//vertex.TexMex = glm::vec2(u, v);
			vertex.Color = color;

			//vertex.Barycentric = (i + j) % 2 == 0 ? glm::vec3(1.0, 0.0, 0.0) : glm::vec3(0.0, 1.0, 0.0);

			OutlineVertices.push_back(vertex);
		}
	}

	for (unsigned int i = 0; i < stackCount; ++i) {
		for (unsigned int j = 0; j < sectorCount; ++j) {
			unsigned int first = i * (sectorCount + 1) + j;
			unsigned int second = first + sectorCount + 1;

			OutlineIndices.push_back(first);
			OutlineIndices.push_back(second);
			OutlineIndices.push_back(first + 1);

			OutlineIndices.push_back(second);
			OutlineIndices.push_back(second + 1);
			OutlineIndices.push_back(first + 1);
		}
	}
}

void Object::CreatePlane(float scale, Plane_Up_Axis axis, glm::vec3 color)
{
	float trueScale = scale * 0.5f;

	switch (axis)
	{
	case XAXIS:

		Vertices = {
		{{-trueScale, -trueScale, -trueScale}, {color}},
		{{-trueScale, trueScale, -trueScale}, {color}},
		{{-trueScale, trueScale, trueScale}, {color}},
		{{-trueScale, -trueScale, trueScale}, {color}}
		};
		break;

	case YAXIS:
		Vertices = {
		{{-trueScale, -trueScale, -trueScale}, {color}},
		{{trueScale, -trueScale, -trueScale}, {color}},
		{{trueScale, -trueScale, trueScale}, {color}},
		{{-trueScale, -trueScale, trueScale}, {color}}
	};
		break; 

	case ZAXIS:
		Vertices = {
		{{-trueScale, -trueScale, -trueScale}, {color}},
		{{trueScale, -trueScale, -trueScale}, {color}},
		{{trueScale, trueScale, -trueScale}, {color}},
		{{-trueScale, trueScale, -trueScale}, {color}}
	};
		break;

	default:
		break;
	}

	Indices = {
		0, 1, 2, 2, 3, 0
	};
}

void Object::CreatePlaneOutlines(float scale, Plane_Up_Axis axis, glm::vec3 color)
{
	float trueScale = scale * 0.5f;
	float outlineDiff = 0.001f;

	switch (axis)
	{
	case XAXIS:

		OutlineVertices = {
		{{-trueScale + outlineDiff, -trueScale, -trueScale}, {color}},
		{{-trueScale + outlineDiff, trueScale, -trueScale}, {color}},
		{{-trueScale + outlineDiff, trueScale, trueScale}, {color}},
		{{-trueScale + outlineDiff, -trueScale, trueScale}, {color}}
		};
		break;

	case YAXIS:
		OutlineVertices = {
		{{-trueScale, -trueScale + outlineDiff, -trueScale}, {color}},
		{{trueScale, -trueScale + outlineDiff, -trueScale}, {color}},
		{{trueScale, -trueScale + outlineDiff, trueScale}, {color}},
		{{-trueScale, -trueScale + outlineDiff, trueScale}, {color}}
		};
		break;

	case ZAXIS:
		OutlineVertices = {
		{{-trueScale, -trueScale, -trueScale + outlineDiff}, {color}},
		{{trueScale, -trueScale, -trueScale + outlineDiff}, {color}},
		{{trueScale, trueScale, -trueScale + outlineDiff}, {color}},
		{{-trueScale, trueScale, -trueScale + outlineDiff}, {color}}
		};
		break;

	default:
		break;
	}

	OutlineIndices = {
		0, 1, 1, 2,
		2, 3, 3, 0
	};
}

void Object::CreateAxisLines()
{
	OutlineVertices = {
		{{-20.f, 0.f, 0.f},{1.0f, 0.0f, 0.0f}},
		{{20.f, 0.f, 0.f},{1.0f, 0.0f, 0.0f}},
		{{0.f, -20.f, 0.f},{0.0f, 1.0f, 0.0f}},
		{{0.f, 20.f, 0.f},{0.0f, 1.0f, 0.0f}},
		{{0.f, 0.f, -20.f},{0.0f, 0.0f, 1.0f}},
		{{0.f, 0.f, 20.f},{0.0f, 0.0f, 1.0f}}
	};

	OutlineIndices = {
		0, 1,
		2, 3,
		4, 5
	};
}

void Object::SetRotation(float rotAngle, glm::vec3 rotAxis)
{
	RotationAngle = rotAngle;
	RotationAxis = rotAxis;
	UpdateModelMatrix();
}

void Object::UpdatePosition(glm::vec3 position)
{
	Position = position;
	UpdateModelMatrix();
}

void Object::ChangeColor(glm::vec3 color)
{
	CreateCube(1.f, 1.f, 1.f, color);
	UpdateModelMatrix();
}

void Object::SetPosition(glm::vec3 position)
{
	Position = position;
	UpdateModelMatrix();
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

void Object::SetupOutlineElementMesh()
{
	glGenVertexArrays(1, &VAOut);
	glGenBuffers(1, &VBOut);
	glGenBuffers(1, &EBOut);

	glBindVertexArray(VAOut);

	glBindBuffer(GL_ARRAY_BUFFER, VBOut);
	glBufferData(GL_ARRAY_BUFFER, OutlineVertices.size() * sizeof(Vertex), OutlineVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOut);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, OutlineIndices.size() * sizeof(unsigned int), OutlineIndices.data(), GL_STATIC_DRAW);

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
