#include "Shader.h"
#include "GLFW/glfw3.h"
#include "Camera.h"
#include <algorithm>

struct CallbackData {
    Shader* myShader;
    Camera* myCamera;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to init GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	Shader myShader("shader.vs", "shader.fs");

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	CallbackData callbackData;
	callbackData.myShader = &myShader;
	callbackData.myCamera = &camera;

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetWindowUserPointer(window, &callbackData);

	glfwSetCursorPosCallback(window, mouse_callback);

	float vertices[] = {
		// Back face		// Color
		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // Top-right
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-left

		// Front face		// Color
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // Bottom-left
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Bottom-right
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-right
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // Top-left
	};

	int indices[] = 
	{
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

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Normal
	/*glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myShader.use();

		// Pass projection matrix to shader (note: in this case it's fine to do it once per frame)
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f); 
		
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, -1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);

	CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));

	if (data != nullptr && data->myShader != nullptr) {
		glm::mat4 projection = glm::perspective(glm::radians(data->myCamera->Zoom), (float)width / (float)height, 0.1f, 100.0f);
		data->myShader->use();
		glUniformMatrix4fv(glGetUniformLocation(data->myShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static float lastX = 800.0f / 2.0;
	static float lastY = 600.0f / 2.0;
	static bool firstMouse = true;

	CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	if (data && data->myCamera) {
		data->myCamera->ProcessMouseMovement(xoffset, yoffset);
	}


}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	CallbackData* data = static_cast<CallbackData*>(glfwGetWindowUserPointer(window));
	if (data && data->myCamera) {
		data->myCamera->ProcessMouseScroll(yoffset);
	}
}