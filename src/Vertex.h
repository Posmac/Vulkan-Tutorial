#include "vulkan/vulkan.h"
#include "glm/glm.hpp"

#include <array>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;

	static VkVertexInputBindingDescription getBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attrDescr{};
		attrDescr[0].binding = 0;
		attrDescr[0].location = 0;
		attrDescr[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attrDescr[0].offset = offsetof(Vertex, position);

		attrDescr[1].binding = 0;
		attrDescr[1].location = 1;
		attrDescr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attrDescr[1].offset = offsetof(Vertex, color);

		return attrDescr;
	}
};