#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <learnopengl/shader_m.h>
#include <glm/gtc/matrix_transform.hpp>
#include "MyPoly.cpp"
#include "Helper.cpp"


using namespace std;
using namespace glm;

class Orbit{
	
public:
	Orbit(std::vector<glm::vec3> v, float depth , float yDepth){
		this->vertices = v;
		vector<vec3> edge2 = {};
		int currsize = v.size();
	for (int i = 0; i < currsize; i++)
		{
			edge2.push_back(glm::vec3(vertices.at(i).x,  vertices.at(i).y, vertices.at(i).z + depth));
		}
		//_3dPoly = MyPoly(vertices, depth);
		this->polygons = Helper::bridgeEdgeLoop(vertices,edge2);
		this->polygon = Polygon3d(polygons);
	}
	void draw(Shader& shader) {
		/*for(auto var : polygons)
		{
			var.setColor(color);
			var.draw(shader);
		}*/
		polygon.draw(shader);
	}
	void setColor(vec3 color) {
		this->polygon.setColor(color);
	}
	void setCenter(const glm::vec3& c) {
		polygon.setCenter(c);
	}
	void translate(const glm::vec3& t){
		this->polygon.translate(t);
	}
	void resetTransformation() {
		this->polygon.resetTransformation();
	}
private:
	vector<glm::vec3> vertices;
	std::vector<std::shared_ptr<Polygon>> polygons;
	Polygon3d polygon;
	glm::vec3 color;
};