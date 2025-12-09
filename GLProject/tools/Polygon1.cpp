#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Polygon {
public:
	Polygon(std::vector<glm::vec3> v, glm::vec3 c) {
		vertices = v;
		head = v.at(0);
		color = c;
		model = glm::mat4(1.0f);

		//GLuint VBO, VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);

		// position attribute
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);
		/*this->VAO = VAO;
		this->VBO = VBO;*/
		glBindVertexArray(0);
	}
	void transformation(glm::mat4 t) {
		model = t;
	}
	void setDrawMode(GLenum drawingMode) {
		this->drawingMode = drawingMode;
	}
	virtual void draw(Shader &shader)
	{
		shader.use();
		shader.setVec3("objectColor", color);
		shader.setMat4("model", model);
		glBindVertexArray(this->VAO);
		
		glDrawArrays(drawingMode, 0, vertices.size());
	}
	virtual ~Polygon() = default;
	virtual void drawc(Shader& shader)
	{
		glBindVertexArray(this->VAO);
		glDrawArrays(drawingMode, 0, vertices.size());
	}
	void deleteBuffers() {
		glDeleteVertexArrays(1, &VAO);
		//glDeleteBuffers(1, &VBO);
	}
	std::vector<glm::vec3> getVertices() {
		return this->vertices;
	}
protected:
	glm::vec3 head;
	std::vector<glm::vec3> vertices;
	glm::vec3 color;
	glm::mat4 model;
	GLuint VAO;
	GLuint VBO;
	GLenum drawingMode = GL_TRIANGLE_FAN;
	Polygon() {};

private:
	
};

