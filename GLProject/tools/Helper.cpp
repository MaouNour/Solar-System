#pragma once
#include "Polygon3d.cpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Helper  {
public:
	/*
	*
	* iam trying to make something like bridgeEdgeLoop Function in blender 
	* 
	*/
	static std::vector<std::shared_ptr<Polygon>> bridgeEdgeLoop(std::vector<glm::vec3> edge1 , std::vector<glm::vec3> edge2) {
		std::vector<std::shared_ptr<Polygon>> polygons = {};
		std::vector<glm::vec3> vertices = { edge1};
		int currsize = vertices.size();
		for (int i = 0; i <currsize; i++)
		{
			vertices.push_back(edge2.at(i));
		}
		for (int i = 0; i < currsize; i++)
		{
			std::vector<glm::vec3> face = {};
			int next = (i + 1) % currsize; // wraps around for the last vertex
			face.push_back(vertices.at(i));           // front i
			face.push_back(vertices.at(next));        // front next
			face.push_back(vertices.at(next + currsize));    // back next
			face.push_back(vertices.at(i + currsize));       // back i
			Polygon p(face,glm::vec3(0.7f,0.7f,0.7f));
			polygons.push_back(std::make_shared<Polygon>(p));
		}
		return polygons;
	}

};