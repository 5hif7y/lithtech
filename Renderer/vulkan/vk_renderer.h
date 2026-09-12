#pragma once
#include "Platform/platform.h"
#ifdef RENDERER_VULKAN
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>

// Triángulo 2D para el pipeline headless (pos en píxeles, color 0..1).
struct VkTriVert { float x, y, r, g, b, a; };

class VulkanRenderer {
public:
  VulkanRenderer();
  ~VulkanRenderer();

  HRESULT Init(SDL_Window* window);
  // Ventana nativa X11 (Linux): display/ventana ya creados por el caller.
  HRESULT InitNative(void* display, unsigned long window, uint32_t w, uint32_t h);
  // Swapchain presentable sin ventana (VK_EXT_headless_surface): ejercita
  // el mismo codigo acquire/record/submit/present que la ventana real.
  HRESULT InitHeadlessPresent(uint32_t w, uint32_t h);
  void Shutdown();
  HRESULT BeginScene();
  HRESULT EndScene();
  HRESULT Clear(uint32_t color);
  HRESULT Present();

  HRESULT DrawPrimitive(VkPrimitiveTopology topo, const void* verts, uint32_t vcount);

  // Frame con ventana: dibuja el batch acumulado (PushTri) sobre el
  // swapchain y presenta. Limpia el batch tras presentar.
  HRESULT RenderWindowFrame();

  // Headless offscreen: sin ventana/swapchain, con readback a PPM.
  bool InitHeadless(uint32_t w, uint32_t h);
  void PushTri(const VkTriVert v[3]);
  size_t PendingTris() const;
  bool SnapshotPPM(const char* path);

  VkDevice device() const { return m_device; }
  VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }

private:
  bool createInstance();
  bool createInstanceX11();
  bool pickPhysicalDevice();
  bool createLogicalDevice();
  bool createSwapchain(SDL_Window* window);
  bool createSwapchainWithExtent(uint32_t w, uint32_t h);
  bool createImageViews();
  bool createRenderPass();
  bool createFramebuffers();
  bool createCommandPool();
  bool createSyncObjects();
  bool createWindowPipeline();
  bool uploadBatch();

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

  // Estado headless offscreen.
  bool m_headless = false;
  VkExtent2D m_offExtent{};
  VkImage m_offImage = VK_NULL_HANDLE;
  VkDeviceMemory m_offMemory = VK_NULL_HANDLE;
  VkImageView m_offView = VK_NULL_HANDLE;
  VkFramebuffer m_offFb = VK_NULL_HANDLE;
  VkRenderPass m_offPass = VK_NULL_HANDLE;
  VkPipelineLayout m_pipeLayout = VK_NULL_HANDLE;
  VkPipeline m_pipeline = VK_NULL_HANDLE;
  VkBuffer m_vertBuf = VK_NULL_HANDLE;
  VkDeviceMemory m_vertMem = VK_NULL_HANDLE;
  VkDeviceSize m_vertCap = 0;
  VkBuffer m_stageBuf = VK_NULL_HANDLE;
  VkDeviceMemory m_stageMem = VK_NULL_HANDLE;
  std::vector<VkTriVert> m_batch;
};

#endif
