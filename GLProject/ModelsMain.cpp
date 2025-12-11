#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "tools/Polygon.h"
#include "tools/Cube.h"
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
#include "stb_image.h"
#include "tools/Orbit.cpp"
#include "tools/Shapes.cpp"

using namespace std;
using namespace glm;

int SCR_WIDTH = 1280;
int SCR_HEIGHT = 720;

Camera camera(vec3(0.0f, 0.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

vec3 firstLight(0.0f, 0.0f, 0.0f);
vec3 lightPos(0.0f, 0.0f, -2.5f);

bool blinn = true;
bool timeGoing = true;
float lastTime = 0.0f;
float simulationTime = 0.0f;

enum SpecialActions {
	SOLAR_ECLIPSE,LUNAR_ECLIPSE,DEFAULT_MOVEMENT,START_MOVEMENT
};
SpecialActions action = DEFAULT_MOVEMENT;
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

vector<GLuint> loadTextures(vector<string> paths, GLuint wrapOption = GL_REPEAT, GLuint filterOption = GL_LINEAR) {
	vector<GLuint> textures = {};

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterOption);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterOption);

	for (string path : paths)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);


		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			std::cout << "Failed to load texture" << std::endl;
		stbi_image_free(data);

		textures.push_back(texture);
	}

	return textures;
}

int main()
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Solar System", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//glfwSwapInterval(1);


	Shader allShader("./shaders/vs/L5.vs", "./shaders/fs/L5-Multi.fs");
	Shader lightSourceShader("./shaders/vs/LightSource.vs", "./shaders/fs/LightSource-Texture.fs");
	//Shader lightSourceShader("./shaders/vs/L5.vs", "./shaders/fs/L5-Multi.fs");


	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);
#pragma region Lightings
	vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f)
	};
	glm::vec3 pointLightColors[] = {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(0.2f, 0.2f, 0.2f)
	};
#pragma endregion
	vector<string> texturePaths = {};
	texturePaths.push_back("./textures/earth.jpeg");
	texturePaths.push_back("./textures/mars.jpeg");
	texturePaths.push_back("./textures/sun.jpeg");
	texturePaths.push_back("./textures/moon.jpeg");

	vector<GLuint> textures = loadTextures(texturePaths);
	Model earthPlanet("./models/Sphere.glb");
	Model marsPlanet("./models/Sphere.glb");
	Model sun("./models/Sphere.glb");
	Model moon("./models/Sphere.glb");
	float r = 4.0f;
	float translateZ = 1.0f;
	Orbit earthOrbit(Shapes::makeEllipse(r, 2 * r, 32, glm::vec3(0.0, 0.0, translateZ)), 0.01, 0.01);
	earthOrbit.setColor(glm::vec3(0.0,0.0,0.3));
	float DME = 0.4f;
	Orbit moonOrbit(Shapes::makeCircle(32,DME,vec3(0.0,0.0,1.0)),0.01,0.01);
	moonOrbit.setColor(vec3(0.9f, 0.9f, 0.9f));

	//Cube LightSource(vec3(0.0,0.0,0.0), 0.2f, vec3(1.0f, 1.0f, 1.0f));

	bool BPressed = false;

	while (!glfwWindowShouldClose(window))
	{
		allShader.use();
		allShader.setBool("blinn", true);
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float time = (float)glfwGetTime();
		float timeSpeed = 1.0;
		float speed = 364.25 * 24 * 30; // 2 minutes
		//float speed = 364.25 * 24 * 60  ;
		// speed up from 1 year to 1 minute
		// 364.25 -> 1 minute -> 
		// moon: 30 days -> 0.082 minute -> 4.94 seconds
		processInput(window);
		switch (action)
		{
		case SOLAR_ECLIPSE:
			timeSpeed = 10 * timeSpeed;
			break;
		case LUNAR_ECLIPSE:
			timeSpeed = 10 * timeSpeed;
			break;
		case START_MOVEMENT:
			timeGoing = true;
			action = DEFAULT_MOVEMENT;
			break;
		case DEFAULT_MOVEMENT:
			break;
		default:
			break;
		}
		
		


		glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mat4 projection = mat4(1.0f);
		projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		allShader.setMat4("projection", projection);
		

		mat4 view = mat4(1.0f);
		view = view = camera.GetViewMatrix();
		allShader.setMat4("view", view);

		
		mat4 trans = mat4(1.0f);
		trans = rotate(trans, time, vec3(1.0f, 0.0f, 1.0f));

		/*vec4 rotatedVectorHomogeneous = trans * vec4(firstLight, 1.0f);
		lightPos = vec3(rotatedVectorHomogeneous);*/

		//allShader.setVec3("lightPos", lightPos);
		allShader.setVec3("viewPos", camera.Position);
		allShader.setVec3("objectColor", vec3(1.0f, 1.0f, 1.0f));
		allShader.setMat4("model", mat4(1.0f));

		/*glBindTexture(GL_TEXTURE_2D, textures[0]);
		plainBall.Draw(allShader);*/
		

		if (timeGoing)
			simulationTime += deltaTime * timeSpeed;
		
		mat4 earthTrans = mat4(1.0f);
		float earthRotationSpeed = 1.0f/(364.25*24*3600); // in 1 year
		float theta = glm::radians(simulationTime) *360.0* earthRotationSpeed *speed;
		float r = 2.0f;//least distance between earth and sun
		float translateZ = r/2;
		float x = r*cos(theta);
		float z = 2 * r*sin(theta) + translateZ;
		earthTrans = glm::translate(earthTrans, vec3(x, 0.0f, z));
		earthTrans = glm::scale(earthTrans, vec3(0.2f, 0.2f, 0.2f));
		/*
		it takes for Earth to complete one full rotation relative to distant fixed stars is 23 hours
		, 56 minutes, and 4 seconds, a period known as a sidereal day
		Source WikiPedia : https://en.wikipedia.org/wiki/Earth's_rotation
		*/
		float earthSelfRotationSpeed = 1.0f / ((23 * 3600) + 56 * 60 + 4);
		//float earthSelfRotationSpeed = 1.0f / ((1000 * 3600) + 56 * 60 + 4 ); // imaginary to be able to track it
		float spinSpeed = glm::radians(simulationTime) * 360.0f * earthSelfRotationSpeed * speed;
		/*
		Earth's orbital plane is known ... on the celestial sphere. It is denoted by the Greek letter Epsilon.
		Earth currently has an axial tilt of about 23.44°
		Source WikiPedia : https://en.wikipedia.org/wiki/Axial_tilt
		*/
		mat4 earthFinalRotation = glm::rotate(earthTrans, glm::radians(23.44f), vec3(0.0f, 0.0f, 1.0f));
		earthFinalRotation = glm::rotate(earthFinalRotation, spinSpeed, vec3(0.0f, 1.0f, 0.0f));
		float orbitInclination = glm::radians(0.0f); 
		//orbitInclination = glm::radians(5.0f);
		// moon orbit is tilted by 5 degree which 
		//makes solar and lunar blab(i don't know what the word is) impossible to predict
		//the word is eclipse i searched
		// i won't put the 5 degree here because it will make the simulation almost impossible to happen 
		// it will still happen though once a year or so.
		moonOrbit.resetTransformation();
		moonOrbit.translate(vec3(x,0.0f,z));
		
		mat4 moonTrans = glm::rotate(earthTrans, orbitInclination, vec3(1.0, 0.0, 0.0));
		float moonRotationSpeed = 1.0f/(30*24*3600);
		float t = glm::radians(simulationTime) * 360.0* moonRotationSpeed *speed;
		moonTrans = glm::rotate(moonTrans, t, vec3(0.0, 1.0, 0.0));
		float DME = 2.0f;//distance between moon and earth
		moonTrans = glm::translate(moonTrans, vec3(DME, 0.0, 0.0));
		moonTrans = glm::scale(moonTrans, vec3(0.6f, 0.6f, 0.6f));
		

		//sun moon earth pos 
		glm::vec3 sunPos = glm::vec3(0, 0, 0);
		glm::vec3 earthPos = glm::vec3(x, 0, z);
		glm::vec3 moonPos = glm::vec3(moonTrans[3]);
		glm::vec3 SM = glm::normalize(moonPos - sunPos);
		glm::vec3 SE = glm::normalize(earthPos - sunPos);
		//it's drewed in the image in the project why it's like this
		float dotValue = glm::dot(SE, SM);
		bool aligned = dotValue > 0.999999f;
		bool moonBetween = glm::length(moonPos - sunPos) < glm::length(earthPos - sunPos);
		bool solar = aligned && moonBetween;
		bool lunar = aligned && !moonBetween;
		bool move = false;
		if (solar && action == SOLAR_ECLIPSE)
		{
			if(timeGoing)
				std::cout << "SOLAR_ECLIPSE";
			timeGoing = false;
		}
		if (lunar && action == LUNAR_ECLIPSE) {
			if (timeGoing)
				std::cout << "LUNAR_ECLIPSE";
			timeGoing = false;
		}
		pointLightPositions[1] = moonPos;
#pragma region Light Settings
				// Point light 1
				allShader.setVec3("pointLights[0].position", pointLightPositions[0]);
				allShader.setVec3("pointLights[0].ambient", pointLightColors[0].x * 0.1f, pointLightColors[0].y * 0.1f, pointLightColors[0].z * 0.1f);
				allShader.setVec3("pointLights[0].diffuse", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
				allShader.setVec3("pointLights[0].specular", pointLightColors[0].x, pointLightColors[0].y, pointLightColors[0].z);
				allShader.setFloat("pointLights[0].constant", 1.0f);
				allShader.setFloat("pointLights[0].linear", 0.14f);
				allShader.setFloat("pointLights[0].quadratic", 0.07f);
				// Point light 2
				allShader.setVec3("pointLights[1].position", pointLightPositions[1]);
				allShader.setVec3("pointLights[1].ambient", pointLightColors[1].x * 0.0f, pointLightColors[1].y * 0.0f, pointLightColors[1].z * 0.0f);
				allShader.setVec3("pointLights[1].diffuse", pointLightColors[1].x, pointLightColors[1].y, pointLightColors[1].z);
				allShader.setVec3("pointLights[1].specular", pointLightColors[1].x *1.5 , pointLightColors[1].y *1.5, pointLightColors[1].z *1.5);
				allShader.setFloat("pointLights[1].constant", 1.0f);
				allShader.setFloat("pointLights[1].linear", 0.14f);
				allShader.setFloat("pointLights[1].quadratic", 0.07f);
		#pragma endregion

		allShader.setMat4("model", earthFinalRotation);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		if (solar) 
		{
			allShader.setBool("shadowNumber", 1);
		}
		else
			allShader.setBool("shadowNumber", 0);
		earthPlanet.Draw(allShader);
		allShader.setMat4("model", moonTrans);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		if (lunar) 
		{
			allShader.setBool("shadowNumber", 1);
		}
		else
			allShader.setBool("shadowNumber", 0);
		moon.Draw(allShader);


		//LightSource Shader
		lightSourceShader.use();
		lightSourceShader.setMat4("projection", projection);
		lightSourceShader.setMat4("view", view);
		lightSourceShader.setVec3("objectColor", vec3(1.0f,1.0f,1.0f));
		mat4 sunTransform = mat4(1.0f);
		sunTransform = glm::scale(sunTransform, vec3(0.5f, 0.5f, 0.5f));
		lightSourceShader.setMat4("model", sunTransform);
		glBindTexture(GL_TEXTURE_2D, textures[2]);
		sun.Draw(lightSourceShader);
		
		earthOrbit.draw(lightSourceShader);
		moonOrbit.draw(lightSourceShader);
		//LightSource.draw(lightSourceShader);
		/*lightSourceShader.setMat4("model", earthFinalRotation);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		earthPlanet.Draw(lightSourceShader);
		lightSourceShader.setMat4("model", moonTrans);
		glBindTexture(GL_TEXTURE_2D, textures[3]);
		earthPlanet.Draw(lightSourceShader);*/


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	//solar
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		action = SOLAR_ECLIPSE;
	//lunar
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		action = LUNAR_ECLIPSE;
	//move time
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		action = START_MOVEMENT;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
