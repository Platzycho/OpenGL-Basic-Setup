#include "Spline_Comp1.h"

Spline_Comp1::Spline_Comp1(int samplePoints, std::vector<glm::vec3> controlPoints, std::vector<float> knots, int degree) : Position(0.0f), Scale(1.0f), RotationAngle(0.0f), RotationAxis(1.0f, 1.0f, 1.0f)
{
	Vertices = SampleBSplineVertices(samplePoints, degree, controlPoints, knots, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	EvaluateSplineAt(tValues, degree, controlPoints, knots);

	SplineDerivative(tValues[0],controlPoints);
	SplineDerivative(tValues[1], controlPoints);
	SplineDerivative(tValues[2], controlPoints);

	SplineDerivative(tValues[0], degree, controlPoints, knots);
	SplineDerivative(tValues[1], degree, controlPoints, knots);
	SplineDerivative(tValues[2], degree, controlPoints, knots);


	SetupElementMesh();
	RotationAngle = 0;
	RotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
	UpdateModelMatrix();
}

Spline_Comp1::~Spline_Comp1()
{
}

float Spline_Comp1::N(int i, int k, float t, const std::vector<float>& knots)
{
	if (k == 0)
	{
		if (knots[i] <= t && t < knots[i + 1])
			return 1.0f;
		if (t == knots.back() && t == knots[i + 1])
			return 1.0f;

		return 0.0f;
	}
	else
	{
		float denom1 = knots[i + k] - knots[i];
		float denom2 = knots[i + k + 1] - knots[i + 1];

		float term1 = (denom1 != 0) ? ((t - knots[i]) / denom1) * N(i, k - 1, t, knots) : 0.0f;
		float term2 = (denom2 != 0) ? ((knots[i + k + 1] - t) / denom2) * N(i + 1, k - 1, t, knots) : 0.0f;

		return term1 + term2;
	}
}

glm::vec3 Spline_Comp1::DeBoor(float t, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots)
{
	glm::vec3 point(0.0f);
	int n = controlPoints.size();

	for (int i = 0; i < n; i++)
	{
		float b = N(i, degree, t, knots);
		point += b * controlPoints[i];
	}
	return point;
}

std::vector<glm::vec3> Spline_Comp1::SampleBSpline(int samples, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots)
{
	std::vector<glm::vec3> curve;
	float tMin = knots[degree];
	float tMax = knots[knots.size() - degree - 1];

	for (int j = 0; j <= samples; j++)
	{
		float t = tMin + (tMax - tMin) * (float(j) / samples);
		curve.push_back(DeBoor(t, degree, controlPoints, knots));
		
	}
	return curve;
}

std::vector<VertexColor> Spline_Comp1::SampleBSplineVertices(int samples, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots, const glm::vec3& colorStart, const glm::vec3& colorEnd)
{
	std::vector<VertexColor> vertices;
	float tMin = knots[degree];
	float tMax = knots[knots.size() - degree - 1];

	for (int j = 0; j <= samples; j++) {
		float u = float(j) / samples;              // normalized [0,1]
		float t = tMin + (tMax - tMin) * u;        // actual spline parameter

		glm::vec3 pos = DeBoor(t, degree, controlPoints, knots);

		// Color interpolation (gradient)
		glm::vec3 col = (1.0f - u) * colorStart + u * colorEnd;

		VertexColor v;
		v.Position = pos;
		v.Color = col;
		//v.TexCoords = glm::vec2(0.0f); // not used
		vertices.push_back(v);
	}
	return vertices;
}

std::vector<glm::vec3> Spline_Comp1::EvaluateSplineAt(const std::vector<float>& tValues, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots)
{
	std::vector<glm::vec3> results;
	results.reserve(tValues.size());

	for (float t: tValues)
	{
		results.push_back(DeBoor(t, degree, controlPoints, knots));
	}


	for (int i = 0; i < results.size(); i++)
	{
		std::cout << "f(" << tValues[i] << ") = ("
			<< results[i].x << ", "
			<< results[i].y << ", "
			<< results[i].z << ")\n\n";
	}


	return results;


}

void Spline_Comp1::QuadraticBasisDerivatives(float u, float& dN0, float& dN1, float& dN2)
{
	dN0 = -2.0 * (1.0f - u);
	dN1 = 2.0f - 4.0f * u;
	dN2 = 2.0f * u;
}

glm::vec3 Spline_Comp1::SplineDerivative(float u, const std::vector<glm::vec3>& controlPoints)
{
	float dN0, dN1, dN2;
	QuadraticBasisDerivatives(u, dN0, dN1, dN2);
	glm::vec3 result = dN0 * controlPoints[0] + dN1 * controlPoints[1] + dN2 * controlPoints[2];
	std::cout << "At t=" << u << ":\n";
	std::cout << "B'_{0,2}(t) = " << dN0 << "\n";
	std::cout << "B'_{1,2}(t) = " << dN1 << "\n";
	std::cout << "B'_{2,2}(t) = " << dN2 << "\n\n";
	return dN0 * controlPoints[0] + dN1 * controlPoints[1] + dN2 * controlPoints[2];
}

glm::vec3 Spline_Comp1::SplineDerivative(float u, int degree, const std::vector<glm::vec3>& controlPoints, const std::vector<float>& knots)
{
	int n = (int)controlPoints.size();
	assert((int)knots.size() == n + degree + 1);

	if (n == 0) return glm::vec3(0.0f);

	std::vector<float> dN = BasisDerivatives(u, degree, knots, n);

	glm::vec3 result(0.0f);
	for (int i = 0; i < n; ++i) {
		result += dN[i] * controlPoints[i];
	}
	std::cout << "Spline Derivative for t value: " << u << " = (" << result.x << ", " << result.y << ", " << result.z << ")\n";

	return result;
}
std::vector<std::vector<float>> Spline_Comp1::BuildBasisTable(float u, int degree, const std::vector<float>& knots, int nControl)
{
	assert((int)knots.size() == nControl + degree + 1);

	std::vector<std::vector<float>> N_table(degree + 1, std::vector<float>(nControl, 0.0f));

	for (int i = 0; i < nControl; ++i) {
		if ((knots[i] <= u && u < knots[i + 1]) || (u == knots.back() && knots[i + 1] == knots.back()))
			N_table[0][i] = 1.0f;
		else
			N_table[0][i] = 0.0f;
	}

	for (int k = 1; k <= degree; ++k) {
		for (int i = 0; i < nControl; ++i) {
			float left = 0.0f, right = 0.0f;

			float denom1 = knots[i + k] - knots[i];
			if (denom1 != 0.0f)
				left = ((u - knots[i]) / denom1) * N_table[k - 1][i];

			if (i + 1 < nControl) {
				float denom2 = knots[i + k + 1] - knots[i + 1];
				if (denom2 != 0.0f)
					right = ((knots[i + k + 1] - u) / denom2) * N_table[k - 1][i + 1];
			}

			N_table[k][i] = left + right;
		}
	}

	return N_table;
}

std::vector<float> Spline_Comp1::BasisDerivatives(float u, int degree, const std::vector<float>& knots, int nControl)
{
	assert(degree >= 1); 
	assert((int)knots.size() == nControl + degree + 1);

	auto N_table = BuildBasisTable(u, degree, knots, nControl);

	std::vector<float> dN(nControl, 0.0f);
	for (int i = 0; i < nControl; ++i) {
		float term1 = 0.0f, term2 = 0.0f;

		float denom1 = knots[i + degree] - knots[i];
		if (denom1 != 0.0f) {
			term1 = (float)degree / denom1 * N_table[degree - 1][i];
		}

		if (i + 1 < nControl) {
			float denom2 = knots[i + degree + 1] - knots[i + 1];
			if (denom2 != 0.0f) {
				term2 = (float)degree / denom2 * N_table[degree - 1][i + 1];
			}
		}

		dN[i] = term1 - term2;
	}

	return dN;
}



void Spline_Comp1::DrawLines(Shader& shader)
{
	shader.use();
	shader.setMat4("model", ModelMatrix);
	glBindVertexArray(VAO);
	glLineWidth(4.0f);
	glDrawArrays(GL_LINE_STRIP, 0, Vertices.size());

	glBindVertexArray(0);
}

void Spline_Comp1::SetupElementMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(VertexColor), Vertices.data(), GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)offsetof(VertexColor, Position));
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexColor), (void*)offsetof(VertexColor, Color));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}



void Spline_Comp1::UpdateModelMatrix()
{
	ModelMatrix = glm::mat4(1.0f);
	ModelMatrix = glm::translate(ModelMatrix, Position);
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(RotationAngle), RotationAxis);
	ModelMatrix = glm::scale(ModelMatrix, Scale);
}
