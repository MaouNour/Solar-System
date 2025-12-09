#pragma once
#include "Polygon3d.cpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class MyPoly :public Polygon3d {
public:
	MyPoly(std::vector<glm::vec3> v, float depth, ColorMode colorMode = COLOR,std::vector<glm::vec2> TexCoords = {}, std::vector<const char*> texVector = {},std::vector<glm::vec3> colorVector = {}) {
		this->vertices = v;
		this->tex = texVector;
		this->colorVector = colorVector;
		int currsize = vertices.size();
		std::vector<int> faces = {};
		std::vector<int> numberOfVerticesInFace = {};
		for (int i = 0; i < currsize; i++)
		{
			vertices.push_back(glm::vec3(vertices.at(i).x, vertices.at(i).y,depth));
		}
		//make front and back faces
		for (int i = 0; i < vertices.size(); i++)
		{
			faces.push_back(i);
		}
		numberOfVerticesInFace.push_back(currsize);
		numberOfVerticesInFace.push_back(currsize);
		//make all through faces
		int size = vertices.size();
		for (int i = 0; i < currsize; i++)
		{
			int next = (i + 1) % currsize; // wraps around for the last vertex
			faces.push_back(i);           // front i
			faces.push_back(next);        // front next
			faces.push_back(next + currsize);    // back next
			faces.push_back(i + currsize);       // back i
			numberOfVerticesInFace.push_back(4);
		}
		if (!tex.empty() && !TexCoords.empty())
		{
			if(tex.size()>1)
			Polygon3d::setTexture(TexCoords, tex);
			else
			Polygon3d::setTexture(TexCoords, tex.at(0));
		}
		Polygon3d::init(vertices,numberOfVerticesInFace,faces,colorMode);
	}
	void draw(Shader& shader) {
		Polygon3d::draw(shader);
	}
	void deleteBuffers() {
		Polygon3d::deleteBuffers();
	}
	MyPoly(){}
};