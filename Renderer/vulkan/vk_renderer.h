#pragma once
#include "Platform/platform.h"
#ifdef RENDERER_VULKAN
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>

// Abstraccion minima para porte D3D -> Vulkan
// Jupiter usa CRenderer / CD3DDevice - aqui mapeamos a VkDevice

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

  // Wraps D3D DrawPrimitive
  HRESULT DrawPrimitive(VkPrimitiveTopology topo, const void* verts, uint32_t vcount);

  VkDevice device() const { return m_device; }
  VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }

private:
  bool createInstance();
  bool pickPhysicalDevice();
  bool createLogicalDevice();
  bool createSwapchain(SDL_Window* window);

  VkInstance m_instance = VK_NULL_HANDLE;
  VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
  VkDevice m_device = VK_NULL_HANDLE;
  VkQueue m_graphicsQueue = VK_NULL_HANDLE;
  VkSurfaceKHR m_surface = VK_NULL_HANDLE;
  VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
  std::vector<VkImage> m_swapImages;
  uint32_t m_graphicsQueueFamily = 0;
};

#endif
