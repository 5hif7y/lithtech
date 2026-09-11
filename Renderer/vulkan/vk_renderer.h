#pragma once
#include "Platform/platform.h"
#ifdef RENDERER_VULKAN
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>

class VulkanRenderer {
public:
  VulkanRenderer();
  ~VulkanRenderer();

  HRESULT Init(SDL_Window* window);
  void Shutdown();
  HRESULT BeginScene();
  HRESULT EndScene();
  HRESULT Clear(uint32_t color);
  HRESULT Present();

  HRESULT DrawPrimitive(VkPrimitiveTopology topo, const void* verts, uint32_t vcount);

  VkDevice device() const { return m_device; }
  VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }

private:
  bool createInstance();
  bool pickPhysicalDevice();
  bool createLogicalDevice();
  bool createSwapchain(SDL_Window* window);
  bool createImageViews();
  bool createRenderPass();
  bool createFramebuffers();
  bool createCommandPool();
  bool createSyncObjects();

  VkInstance m_instance = VK_NULL_HANDLE;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkDevice m_device = VK_NULL_HANDLE;
  VkQueue m_graphicsQueue = VK_NULL_HANDLE;
  VkSurfaceKHR m_surface = VK_NULL_HANDLE;
  VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
  std::vector<VkImage> m_swapImages;
  std::vector<VkImageView> m_swapViews;
  std::vector<VkFramebuffer> m_framebuffers;
  VkRenderPass m_renderPass = VK_NULL_HANDLE;
  VkCommandPool m_cmdPool = VK_NULL_HANDLE;
  VkCommandBuffer m_cmdBuffer = VK_NULL_HANDLE;
  VkSemaphore m_imageAvailable = VK_NULL_HANDLE;
  VkSemaphore m_renderFinished = VK_NULL_HANDLE;
  VkFence m_inFlight = VK_NULL_HANDLE;
  VkFormat m_swapFormat = VK_FORMAT_B8G8R8A8_SRGB;
  VkExtent2D m_swapExtent{};
  VkClearValue m_clearValue{};
  uint32_t m_graphicsQueueFamily = 0;
  bool m_hasSwapchain = false;
};

#endif
