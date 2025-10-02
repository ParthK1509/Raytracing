#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		//takes already clamped color between 0 and 1
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		return (uint32_t(a) << 24) | (uint32_t(r) << 16) | (uint32_t(g) << 8) | (uint32_t(b) << 0);
		// aRGB
	}
}

void Renderer::OnResize(uint32_t width, uint32_t height) 
{
	if (m_FinalImage)
	{
		if(m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return; // No need to resize if dimensions are the same

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData; // free old buffer
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render() 
{	
	for(uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for(uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };
			coord = coord * 2.0f - 1.0f; // remap to -1 to 1

			glm::vec4 color = PerPixel(coord);
			color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color); 
		}
	}

	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
	glm::vec3 rayOrigin(0.0f, 0.0f, 3.0f);
	glm::vec3 rayDirection(coord.x, coord.y, -1.0f);
	float radius = 1.0f;	

	//rayDirection = glm::normalize(rayDirection);
	/*
	* (bx^2 + by^2) + (2(axbx + ayby)) + (ax^2 + ay^2 - r^2) = 0
	* 
	* a = ray origin
	* b = ray direction
	* r = sphere radius
	* t = intersection distance
	*/
	float a = glm::dot(rayDirection, rayDirection);
	float b = 2.0f * glm::dot(rayOrigin, rayDirection);
	float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;

	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0.0f)
	{
		return glm::vec4(0, 0, 0, 1);
	}

	float t0 = (-b - glm::sqrt(discriminant)) / (2.0f * a);
	float closestT = (-b + glm::sqrt(discriminant)) / (2.0f * a);

	//glm::vec3 h0 = rayOrigin + rayDirection * t0;
	glm::vec3 h1 = rayOrigin + rayDirection * closestT;
	glm::vec3 normal = glm::normalize(h1 - glm::vec3(0, 0, 0));

	glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));

	float d = glm::max(glm::dot(normal, -lightDir),0.0f);

	glm::vec3 sphereColor(1,0,1); // 0xff000000
	sphereColor *= d;
	return glm::vec4(sphereColor,1.0f);

}