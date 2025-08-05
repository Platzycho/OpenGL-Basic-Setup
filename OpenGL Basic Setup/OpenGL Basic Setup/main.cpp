#include "ObjectHandler.h"
#include "GLFW/glfw3.h"
#include "Camera.h"

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
	ObjectHandler world;

	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	CallbackData callbackData;
	callbackData.myShader = &myShader;
	callbackData.myCamera = &camera;

	glfwSetScrollCallback(window, scroll_callback);

	glfwSetWindowUserPointer(window, &callbackData);

	glfwSetCursorPosCallback(window, mouse_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	std::cout << ObjectStorage::getInstance().GetPlaneObjects().size() << std::endl;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// Pass projection matrix to shader (note: in this case it's fine to do it once per frame)
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 model = glm::mat4(1.0f); 

		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, -1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		world.drawObjects(myShader);

		//testSurface.Draw
		//defaultCube.changeColor(glm::vec3);
		//defaultCube.DrawLines(myShader);
		//defaultCube.Draw(myShader);

		//defaultCubeOutline.DrawLines(myShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	
	world.~ObjectHandler();
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