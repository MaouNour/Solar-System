#pragma once
#include "Polygon3d.cpp"
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/*
*This is a new class to simulate blender methods 
* i will probably add the extrude here insted of it's hardcoded in Mypoly to extend along z axis 
* and i will also make it so that i can extend along glm::vec3 like in blender 
* also i will calculate the normals probably so it will have the same size in all of the object
*/
class Helper  {
public:
	/*
	*iam trying to make something like bridgeEdgeLoop Function in blender 
	*if you are reading here and wondring why iam trying to simulate the blender way
	*because two things :
	*1 - i don't like making something that work for a specific thing only i like making a general thing and using it every where
	*2 - i have been using blender for about 8 years so it's the most 3d program iam comfortable with 
	*with my regards Nour (i will move this to the readme.md later on )
	*/
	static std::vector<std::shared_ptr<Polygon>> bridgeEdgeLoop(std::vector<glm::vec3> edge1 , std::vector<glm::vec3> edge2) {
		std::vector<std::shared_ptr<Polygon>> polygons = {};
		std::vector<glm::vec3> vertices = { edge1};
		int currsize = vertices.size();
		for (int i = 0; i <currsize; i++)
		{
			vertices.push_back(edge2.at(i));
		}
		/*
		*The same code in Mypoly for extend an edge into a 2d Object 
		*will be replced later with extrudeEdge Method like in blender
		*/
		for (int i = 0; i < currsize; i++)
		{
			std::vector<glm::vec3> face = {};
			int next = (i + 1) % currsize; // wraps around for the last vertex
			face.push_back(vertices.at(i));           
			face.push_back(vertices.at(next));        
			face.push_back(vertices.at(next + currsize));   
			face.push_back(vertices.at(i + currsize));     
			Polygon p(face,glm::vec3(0.7f,0.7f,0.7f));
			polygons.push_back(std::make_shared<Polygon>(p));
		}
		return polygons;
	}

};