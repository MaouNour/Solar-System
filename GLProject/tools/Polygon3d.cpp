#pragma once
#include "Polygon.h"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum ColorMode
{
	COLOR,
	COLOR_VECTOR,
	TEXTURE,
	TEXTURE_VECTOR
};
class Polygon3d {
private:
	
protected:
	std::vector<glm::vec3> vertices;
	std::vector<std::shared_ptr<Polygon>> polygons;
	glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f);
	std::vector<glm::vec3> colorVector = {};
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 center = { 0, 0, 0 };
	ColorMode colorMode = COLOR;
	GLenum drawingMode = GL_TRIANGLE_FAN;
	std::vector<const char*> tex;
	std::vector<glm::vec2> TexCoords;
	Polygon3d() {};
	void init(std::vector<glm::vec3> vertices, std::vector<int> numberOfVerticesInFace, std::vector<int> faces) {
		init(vertices, numberOfVerticesInFace, faces, colorMode);
	}
	void init(std::vector<glm::vec3> vertices, std::vector<int> numberOfVerticesInFace, std::vector<int> faces , ColorMode colorMode) {
		this->colorMode = colorMode;
		int f = 0;
		for (int i = 0; i < numberOfVerticesInFace.size(); i++)
		{
			std::vector<glm::vec3> p = {};
			std::vector<glm::vec2> pTex = {};
			for (int j = 0; j < numberOfVerticesInFace.at(i); j++)
			{
				if(colorMode == TEXTURE_VECTOR)
				pTex.push_back(TexCoords.at(faces.at(f)));
				p.push_back(vertices.at(faces.at(f++)));
			}
			if(colorMode == TEXTURE)
			{
				//polygons.push_back(std::make_shared<PolygonWithTexture>(p, tex.at(0), TexCoords));
			}
			else if (colorMode == TEXTURE_VECTOR)
			{
				//polygons.push_back(std::make_shared<PolygonWithTexture>(p, tex.at(i), pTex));
			}
			else if (colorMode == COLOR_VECTOR)
			{
				polygons.push_back(std::make_shared<Polygon>(p, colorVector.at(i)));
			}
			else
				polygons.push_back(std::make_shared<Polygon>(p, color));
		}
		setCenter(computeCenter());
	}
public:
	Polygon3d(std::vector<glm::vec3> vertices , std::vector<int> numberOfVerticesInFace,std::vector<int> faces) {
		this->vertices = vertices;
		init(vertices, numberOfVerticesInFace, faces);
	}
	void draw(Shader& shader) {
		shader.use();
		shader.setMat4("model", model);
		if(colorMode == COLOR)
		shader.setVec3("objectColor", color);
		int k = 0;
		for (auto& p : polygons) {
			if(colorMode == COLOR_VECTOR)
				shader.setVec3("objectColor", colorVector.at(k++));
			p->setDrawMode(drawingMode);
			p->drawc(shader);
		}
	}
	void deleteBuffers() {
		for (int i = 0; i < polygons.size(); i++)
		{
			polygons.at(i)->deleteBuffers();
		}
	}
	void translate(const glm::vec3& t) {
		model = glm::translate(model, t);
	}
	void scale(float s) {
		model = glm::translate(model, center);
		model = glm::scale(model, glm::vec3(s));
		model = glm::translate(model, -center);
	}
	void scale(const glm::vec3& s) {
		model = glm::translate(model, center);
		model = glm::scale(model, s);
		model = glm::translate(model, -center);
	}
	void rotate(float angleDeg, const glm::vec3& axis) {
		float angleRad = glm::radians(angleDeg);
		model = glm::translate(model, center);
		model = glm::rotate(model, angleRad, axis);
		model = glm::translate(model, -center);
	}
	void rotateRad(float rad, const glm::vec3& axis) {
		model = glm::translate(model, center);
		model = glm::rotate(model, rad, axis);
		model = glm::translate(model, -center);
	}
	void resetTransformation() {
		model = glm::mat4(1.0f);
	}
	void setTexture(std::vector<glm::vec2> TexCoords, std::vector<const char*> tex) {
		colorMode = TEXTURE;
		this->TexCoords = TexCoords;
		this->tex = tex;
	}
	void setTexture(std::vector<glm::vec2> TexCoords, const char* tex) {
		colorMode = TEXTURE_VECTOR;
		this->TexCoords = TexCoords;
		this->tex = { tex };
	}
	void setColor(std::vector<glm::vec3> color) {
		colorMode = COLOR_VECTOR;
		colorVector = color;
	}
	void setColor(glm::vec3 color) {
		colorMode = COLOR;
		this->color = color;
	}
	void setCenter(const glm::vec3& c) {
		center = c;
	}
	glm::vec3 computeCenter() {
		glm::vec3 min(FLT_MAX);
		glm::vec3 max(-FLT_MAX);

		for (auto& poly : polygons) {
			for (auto& v : poly->getVertices()) {
				min = glm::min(min, v);
				max = glm::max(max, v);
			}
		}
		return (min + max) * 0.5f;
	}
	void setDrawMode(GLenum drawingMode) {
		this->drawingMode = drawingMode;
	}
	std::vector<std::shared_ptr<Polygon>> getPolygons() {
		return this->polygons;
	}
	glm::vec3 getCenter() { return center; }
};