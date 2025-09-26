#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <cassert>
#include <algorithm>
#include "Shader.h"
#include "Structs.h"

class Spline_Comp1
{
public:
	Spline_Comp1(int samplePoints, std::vector<glm::vec3> controlPoints, std::vector<float> knots, int degree);
	~Spline_Comp1();

	float N(int i, int k, float t, const std::vector<float>& knots);

	glm::vec3 DeBoor(float t, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots);

	std::vector<glm::vec3> SampleBSpline(int samples, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots);

	std::vector<VertexColor> SampleBSplineVertices(
		int samples,
		int degree,
		const std::vector<glm::vec3>& controlPoints,
		const std::vector<float>& knots,
		const glm::vec3& colorStart,
		const glm::vec3& colorEnd
	);

	std::vector<glm::vec3> EvaluateSplineAt(const std::vector<float>& tValues, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots);
	void QuadraticBasisDerivatives(float u, float& dN0, float& dN1, float& dN2);
	glm::vec3 SplineDerivative(float u, const std::vector<glm::vec3>& controlPoints);
	glm::vec3 SplineDerivative(float u, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots);

	static std::vector<std::vector<float>> BuildBasisTable(float u, int degree, const std::vector<float>& knots, int nControl);

	static std::vector<float> BasisDerivatives(float u, int degree, const std::vector<float>& knots, int nControl);


	std::vector<glm::vec3> splineToDraw;

	void DrawLines(Shader& shader);

	std::vector<float> tValues = { 0.0f, 0.5f, 1.0f };

private:
	unsigned int VAO, VBO, EBO;
	unsigned int VAOut, VBOut, EBOut;

	std::vector<VertexColor> Vertices;
	std::vector<VertexTexture> TexVertices;
	std::vector<VertexColor> OutlineVertices;
	glm::vec3 Position;
	glm::vec3 Scale;
	glm::mat4 ModelMatrix;
	glm::vec3 RotationAxis;
	std::vector<unsigned int> Indices;
	std::vector<unsigned int> OutlineIndices;

	float RotationAngle;
	

	void SetupElementMesh();
	void UpdateModelMatrix();

};

