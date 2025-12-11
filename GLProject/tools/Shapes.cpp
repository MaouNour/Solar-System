#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>
/*
*the same class as the previos homework i only add 
* ellipse which is the shape of the orbit of earth 
* (looks like an egg)
*/
class Shapes {
public:
	static std::vector<glm::vec2> squareTexture()
	{ 
		return {  
				glm::vec2(1.0f, 0.0f),
				glm::vec2(0.0f, 0.0f),
				glm::vec2(0.0f, 1.0f),
				glm::vec2(1.0f, 1.0f),
		};
	}
	static std::vector<glm::vec2> invSquareTexture()
	{
		return {
				glm::vec2(0.0f, 1.0f),
				glm::vec2(0.0f, 0.0f),
				glm::vec2(-1.0f, 0.0f),
				glm::vec2(-1.0f, 1.0f)
		};
	}
	static std::vector<glm::vec2> makeCircleTextureUV(int numberOfPoints)
	{
		std::vector<glm::vec2> circleUV = {};
		int step = 360 / numberOfPoints;
		float pi = glm::pi<float>();
		for (int i = 0; i < 360; i += step) {
			float rad = i * pi / 180.0;
			circleUV.push_back(glm::vec2((cos(rad) +1.0f)/2.0f, (sin(rad)+1.0)/2.0f));
		}
		return circleUV;
	}
	static std::vector<glm::vec3> makeCircle(int numberOfPoints,float r , glm::vec3 axis = glm::vec3(0.0f,1.0f,0.0f)) {
		std::vector<glm::vec3> circle = {};
		int step = 360 / numberOfPoints;
		float pi = glm::pi<float>();
		for (int i = 0; i < 360; i+=step) {
			float rad = i * pi/ 180.0;
			circle.push_back(glm::vec3(r*cos(rad),r*sin(rad) * axis.y,r*sin(rad)*axis.z));
		}
		return circle;
	}
	static std::vector<glm::vec3> makeRect(float width, float height) {
		std::vector<glm::vec3> rect = {};
		rect.push_back(glm::vec3(width / 2.0, height / 2.0, 0.0));
		rect.push_back(glm::vec3(-width / 2.0, height / 2.0, 0.0));
		rect.push_back(glm::vec3(-width / 2.0, -height / 2.0, 0.0));
		rect.push_back(glm::vec3(width/2.0,-height/2.0,0.0));
		return rect;
	}
	static std::vector<glm::vec3> makeTriangle(float edge,float height) {
		std::vector<glm::vec3> triangle = {};
		triangle.push_back(glm::vec3(0.0f, 2.0*height/3.0f, 0.0f));
		triangle.push_back(glm::vec3(-edge / 2.0f, -height/3.0f, 0.0f));
		triangle.push_back(glm::vec3(edge/2.0f, -height/3.0f,0.0f));
		return triangle;
	}
	static std::vector<glm::vec3> makeTriangle(float edge) {
		float h = edge * sqrt(3.0) / 2.0;
		return makeTriangle(edge,h);
	}
	static std::vector<glm::vec3> makeArrow(float widthRect, float heightRect,float edge, float height=0) {
		std::vector<glm::vec3> arrow = {};
		std::vector<glm::vec3> rect = makeRect(widthRect,heightRect); // uright uleft lleft lright
		std::vector<glm::vec3> triangle = (height == 0 ? makeTriangle(edge) : makeTriangle(edge, height)); //top left right
		height = height == 0 ? edge * sqrt(3.0) / 2.0 : height;
		//triangle = move(triangle,glm::vec3(0.0f,heightRect/2.0+height/3.0,0.0f));
		rect = move(rect, glm::vec3(0.0f, -heightRect / 2.0 -height/3.0, 0.0f));

		std::vector<glm::vec3> result = { rect.at(3),rect.at(0),triangle.at(2),triangle.at(0),triangle.at(1),rect.at(1),rect.at(2) };
		return move(result, glm::vec3(0.0f,heightRect/2.0,0.0f));
	}
	static std::vector<glm::vec3> makeEllipse(float width, float height , int points , glm::vec3 center=glm::vec3(0.0,0.0,0.0)) {
		float a = width / 2;
		float b = height / 2;
		int step = 360 / points;
		std::vector<glm::vec3> ellipse = {};
		for (float i = 0; i < 360; i+=step)
		{
			float x = a * cos(glm::radians(i)) + center.x;
			float z = b * sin(glm::radians(i)) + center.z;
			ellipse.push_back(glm::vec3(x, center.y, z));
			//ellipse.push_back(glm::vec3(a * cos(glm::radians(i)), b * sin(glm::radians(i)), 0.0));
		}
		return ellipse;
	}
	static std::vector<glm::vec3> move(std::vector<glm::vec3> shape, glm::vec3 transition) {
		std::vector<glm::vec3> result = {};
		for (auto& vertex : shape)
		{
			result.push_back(glm::operator+(vertex, transition));
		}
		return result;
	}


};