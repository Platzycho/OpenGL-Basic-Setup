#pragma once
#include <vector>

class Shader;

//struct Cube
//{
//	float vertices[] = {
//		// Back face		// Color
//		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
//		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
//		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Top-right
//		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Top-right (again, for the second triangle)
//		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-left
//		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-left (again, to close the face)
//
//		// Front face		// Color
//		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
//		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
//		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-right
//		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-right (again)
//		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-left
//		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Bottom-left (again)
//
//		// Left face		// Color
//		-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-right (front)
//		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-left (back)
//		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-left (back)
//		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left (back, again)
//		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right (front)
//		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-right (front, again)
//
//		// Right face		// Color
//		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Top-left (front)
//		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-right (back)
//		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right (back)
//		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-right (back, again)
//		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom-left (front)
//		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-left (front, again)
//
//		 // Bottom face		// Color
//		 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Top-right (back)
//		  0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-left (back)
//		  0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Bottom-left (front)
//		  0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left (front, again)
//		 -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right (front)
//		 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Top-right (back, again)
//
//		 // Top face		// Color
//		 -0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Top-left (back)
//		  0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-right (back)
//		  0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Bottom-right (front)
//		  0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Bottom-right (front, again)
//		 -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom-left (front)
//		 -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Top-left (back, again)
//	};
//};


class Object
{

};

