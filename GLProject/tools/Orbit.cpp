#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <learnopengl/shader_m.h>
#include <glm/gtc/matrix_transform.hpp>
#include "MyPoly.cpp"

using namespace std;
using namespace glm;

class Orbit{
	
public:
	Orbit(std::vector<glm::vec3> v, float depth , float yDepth){
		this->vertices = v;
		int currsize = v.size();
		for (int i = 0; i < currsize; i++)
		{
			vertices.push_back(glm::vec3(vertices.at(i).x, vertices.at(i).y + yDepth, depth));
		}
		_3dPoly = MyPoly(vertices, depth);
	}
	void draw(Shader& shader) {
		_3dPoly.draw(shader);
	}
private:
	vector<glm::vec3> vertices;
	MyPoly _3dPoly;
};