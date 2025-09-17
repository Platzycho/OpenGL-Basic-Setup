#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>
#include "stb_image.h"
#include <string>
#include "glad/glad.h"

class Texture
{
public:
	Texture() : ID(0), type("texture"), path("")
	{}

	Texture(const char* imagePath, const std::string& typeName, bool alpha = false)
	{
		LoadTexture(imagePath, alpha);
	}

	void LoadTexture(const char* imagePath, bool alpha)
	{
		glGenTextures(1, &ID);
		glBindTexture(GL_TEXTURE_2D, ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		int width, height, nrChannels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			GenerateTexture(data, width, height, alpha);
		}
		else
		{
			std::cout << "Failed to load texture at path: " << imagePath << std::endl;
		}
		stbi_image_free(data);
	}
	void UseTexture(GLenum textureUnit = GL_TEXTURE0)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, ID);
	}


	unsigned int ID;
	std::string type;
	std::string path;

private:
	void GenerateTexture(unsigned char* data, int width, int height, bool alpha)
	{
		GLenum format = alpha ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

};
