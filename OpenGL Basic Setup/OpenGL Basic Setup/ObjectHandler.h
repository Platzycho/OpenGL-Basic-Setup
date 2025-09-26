#pragma once
#include "Shader.h"
#include "Object.h"
#include "ObjectStorage.h"
#include "Spline_Comp1.h"
#include <algorithm>

class ObjectHandler
{
public:
	ObjectHandler()
	{

		for (int i = 0; i < surfaceTileRow; i++)
		{
			for (int j = 0; j < surfaceTileStack; j++)
			{
				auto object = new Object(PLANE, 1.f, glm::vec3(0.5f, 0.5f, 0.0f));
				object->SetPosition(glm::vec3((float)i - (float)surfaceTileRow * 0.5f + 0.5f, 0.0f, (float)j - (float)surfaceTileStack * 0.5f + 0.5f));
			}
		}

		testSphere.SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));
	}

	~ObjectHandler()
	{
		for (const auto& obj : ObjectStorage::getInstance().GetPlaneObjects())
		{
			obj->~Object();
		}

		axisLines.~Object();
	}


	void drawObjects(Shader& myShader)
	{
		for (const auto& obj : ObjectStorage::getInstance().GetPlaneObjects())
		{
			obj->Draw(myShader);
			obj->DrawLines(myShader);
		}

		testSphere.Draw(myShader);
		testSphere.DrawLines(myShader);

		axisLines.DrawLines(myShader);

		splineCurve.DrawLines(myShader);


		//rotationAngle += 0.5f;
		//defaultCube.SetRotation(rotationAngle, glm::vec3(0.5f, 0.0f, 0.5f));
	}


private:
	int samplePoints = 100;
	std::vector<glm::vec3> controlPoints = {
	{1.0f, 0.0f, 0.0f},
	{1.0f, 1.0f, 2.0f},
	{1.0f, 2.0f, 0.0f}
	};

	std::vector<glm::vec3> controlPoints4 = {
	{0.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 2.0f, 0.0f}
	};


	std::vector<float> knots = { 0, 0, 0, 1, 1, 1 };
	int degree = 2;

	Object axisLines = Object(AXIS);
	Object testSphere = Object(SPHERE, 0.5, glm::vec3(0.8, 0.0, 0.0));
	Spline_Comp1 splineCurve = Spline_Comp1(samplePoints, controlPoints4, knots, degree);
	

	int surfaceTileRow = 6;
	int surfaceTileStack = 5;
	float rotationAngle = 0.0f;
};