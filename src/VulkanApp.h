#pragma once

#include "vulkan/vulkan.h"
#include "Vertex.h"
#include "Structs.h"

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <optional>
#include <set>
#include <limits>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <unordered_map>

const int MAX_FRAMES_IN_FLIGHT = 2;

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilitiesKhr;
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanApp {
public:
	VulkanApp(int width, int height,
		 const std::string& modelPath, const std::string& texturePath);
	void run();
private:
	//window initialization
	void initWindow();

	//vulkan init
	void initVulkan();

	//main functions
	void mainLoop();
	void cleanup();

	//create instance, check for available extensions, validation layers and
	// setup debug messenger for layers
	void createInstance();
	bool checkValidationLayersSupport();
	void setupDebugMessenger();

	//pick physical device, check queue families
	void pickPhysicalDevice();
	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	//create logical device
	void createLogicalDevice();
	void createSurface();
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);

	//create swapchain
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps);
	void createSwapChain();
	void recreateSwapChain();
	void cleanupSwapChain();

	//create image views
	void createImageViews();

	//create pipeline 
	void createGraphicsPipeline();
	std::vector<char> readShaderSPIRV(const std::string& path);
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void createRenderPass();
	void createFramebuffers();

	//command pools
	void createCommandPool();
	void createCommandBuffers();
	void recordCommandBuffer(VkCommandBuffer buffer, uint32_t imageIndex);

	//command buffers
	void createVertexBuffer();
	void createIndexBuffer();
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags props);
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
		VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

	//draw
	void drawFrame();

	//sync
	void createSyncObjects();

	//uniforms
	void createDescriptorSetLayout();
	void createUniformBuffers();
	void updateUniformBuffer(uint32_t currentImage);
	void createDescriptorPool();
	void createDescriptorSets();

	//textures
	void createTextureImage();
	void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, 
		VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties,
		VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    void createTextureImageView();
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    void createTextureSampler();

	//depth test
	void createDepthResources();
	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates,
		VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat findDepthFormat();
	bool hasStencilComponent(VkFormat format);

	//loading models
	void loadModel();

	//mipmaps
	void generateMipMaps(VkImage image, VkFormat format, int32_t width, int32_t height, uint32_t mipLevels);

	//multisampling
	VkSampleCountFlagBits getMaxUsableSampleCount();
	void createColorResources();

	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	VkResult createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
		const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	std::vector<const char*> getRequiredExtensions();

private:
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}

private:
	int width;
	int height;
	bool enableValidationLayers = true;
	bool framebufferResized = false;

	std::string modelPath;
	std::string texturePath;

	std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	GLFWwindow* window;

	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;

	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;

	VkSwapchainKHR swapChain = VK_NULL_HANDLE;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkDescriptorSetLayout descriptorSetLayout;
	VkPipeline graphicsPipeline;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;

	VkCommandPool commandPool;

	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

	uint32_t currentFrame = 0;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	uint32_t mipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	VkDebugUtilsMessengerEXT debugUtilsMessengerExt = VK_NULL_HANDLE;
};