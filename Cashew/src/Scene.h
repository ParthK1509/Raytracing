#pragma once

#include <glm/glm.hpp>	
#include <vector>

struct Sphere
{
	glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
	float Radius = 1.0f;

	glm::vec3 Albedo{ 1.0f, 1.0f, 1.0f };
};

struct Scene
{
	std::vector<Sphere> Spheres;
	//glm::vec3 LightDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f));
};