#pragma once
#include "Platform/platform.h"
#ifdef RENDERER_VULKAN
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <vector>

// Triángulo 2D para el pipeline headless (pos en píxeles, color 0..1).
struct VkTriVert { float x, y, r, g, b, a; };
// Quad texturado 2D (pos NDC, uv 0..1, color 0..1 para modular).
struct VkTexVert { float x, y, u, v, r, g, b, a; };
// Triangulo 3D texturado (pos en espacio mundo, MVP via SetViewProj).
struct VkMeshVert { float x, y, z, u, v, r, g, b, a; };

class VulkanRenderer {
public:
  VulkanRenderer();
  ~VulkanRenderer();

  HRESULT Init(SDL_Window* window);
  // Ventana nativa X11 (Linux): display/ventana ya creados por el caller.
  HRESULT InitNative(void* display, unsigned long window, uint32_t w, uint32_t h);
#ifdef _WIN32
  // Ventana nativa Win32 (Windows): HWND ya creado por el caller (ventana
  // SDL plain). No usa SDL_Vulkan_* (el SDL2 de vcpkg no trae backend Vulkan).
  HRESULT InitNativeWin32(void* hwnd, uint32_t w, uint32_t h);
#endif
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

  // Texturas para UI/splash: registra RGBA8, devuelve id (>0) o 0 si falla.
  uint32_t RegisterTexture(uint32_t w, uint32_t h, const uint8_t* rgba);
  // Quad texturado (4 verts, orden 0..3) con el id de RegisterTexture.
  void PushTexQuad(uint32_t tex, const VkTexVert v[4]);
  size_t PendingTexQuads() const;

  // Triangulos 3D (R1): espacio mundo, MVP combinado via SetViewProj
  // (columna-mayor, 16 floats), con test de profundidad. Requieren tex>0;
  // para color plano usar una textura blanca 1x1.
  void PushTri3D(uint32_t tex, const VkMeshVert v[3]);
  // Skybox (fondo): batch propio; drawOrdered lo dibuja primero SIN
  // escribir profundidad, para que el mundo lo tape donde corresponda.
  void PushTri3DSky(uint32_t tex, const VkMeshVert v[3]);
  void SetViewProj(const float m[16]);
  size_t PendingMeshTris() const;
  size_t PendingSkyTris() const;

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
#ifdef _WIN32
  bool createInstanceWin32();
#endif
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
  bool ensureTexObjects();
  bool ensureUploadPool();
  bool createTexPipeline(VkRenderPass pass, uint32_t w, uint32_t h,
                         VkPipeline& out);
  bool uploadTexBatch();
  bool drawOrdered(VkCommandBuffer cmd, VkRenderPass pass);
  // Pipeline 3D (R1): crea el pipeline mesh para el pass dado, con depth
  // solo si el pass tiene attachment de profundidad (m_depthFormat valido).
  // depthWrite=false para el pipeline del skybox (fondo sin ocluir).
  bool createMeshPipeline(VkRenderPass pass, uint32_t w, uint32_t h,
                          VkPipeline& out, bool depthWrite = true);
  bool uploadMeshBatch();
  // Formato de profundidad soportado por el dispositivo fisico, o
  // VK_FORMAT_UNDEFINED si no hay (el 3D dibuja sin depth en ese caso).
  VkFormat findDepthFormat();
  // Crea img/mem/view de profundidad (uso interno ventana y headless).
  static bool createDepthImage(VkPhysicalDevice phys, VkDevice dev,
                               uint32_t w, uint32_t h, VkFormat fmt,
                               VkImage& img, VkDeviceMemory& mem,
                               VkImageView& view);
  // Crea m_depthImg/Mem/View (ventana) de wxh; destruye previos.
  bool createDepthResources(uint32_t w, uint32_t h);
  void destroyDepthResources();

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

  // Estado de texturas/UI.
  struct WinTex {
    VkImage image = VK_NULL_HANDLE;
    VkDeviceMemory mem = VK_NULL_HANDLE;
    VkImageView view = VK_NULL_HANDLE;
    VkDescriptorSet set = VK_NULL_HANDLE;
  };
  struct TexQuad { uint32_t tex = 0; VkTexVert v[6]; };
  std::vector<WinTex> m_textures;
  std::vector<TexQuad> m_texBatch;
  // Submission order across flat/textured/items 3D (Vulkan has no implicit
  // order between pipelines: replay submission order explicitly).
  struct DrawItem { bool tex = false; bool mesh = false; uint32_t idx = 0; };
  std::vector<DrawItem> m_order;
  VkDescriptorSetLayout m_texLayout = VK_NULL_HANDLE;
  VkDescriptorPool m_texPool = VK_NULL_HANDLE;
  VkSampler m_texSampler = VK_NULL_HANDLE;
  VkPipelineLayout m_texPipeLayout = VK_NULL_HANDLE;
  VkPipeline m_texPipeWin = VK_NULL_HANDLE;
  VkPipeline m_texPipeOff = VK_NULL_HANDLE;
  VkBuffer m_texBuf = VK_NULL_HANDLE;
  VkDeviceMemory m_texMem = VK_NULL_HANDLE;
  VkDeviceSize m_texCap = 0;
  // Estado del pipeline 3D (R1): batch de tris mundo + MVP + depth.
  struct MeshTri { uint32_t tex = 0; VkMeshVert v[3]; };
  std::vector<MeshTri> m_meshBatch;
  // Batch del skybox: mismos verts, pipeline sin depthWrite, va primero.
  std::vector<MeshTri> m_skyBatch;
  float m_viewProj[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
  VkBuffer m_meshBuf = VK_NULL_HANDLE;
  VkDeviceMemory m_meshMem = VK_NULL_HANDLE;
  VkDeviceSize m_meshCap = 0;
  VkPipelineLayout m_meshPipeLayout = VK_NULL_HANDLE;
  VkPipeline m_meshPipeWin = VK_NULL_HANDLE;
  VkPipeline m_meshPipeOff = VK_NULL_HANDLE;
  VkPipeline m_skyPipeWin = VK_NULL_HANDLE;
  VkPipeline m_skyPipeOff = VK_NULL_HANDLE;
  VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;
  VkImage m_depthImg = VK_NULL_HANDLE;
  VkDeviceMemory m_depthMem = VK_NULL_HANDLE;
  VkImageView m_depthView = VK_NULL_HANDLE;
  VkImage m_offDepthImg = VK_NULL_HANDLE;
  VkDeviceMemory m_offDepthMem = VK_NULL_HANDLE;
  VkImageView m_offDepthView = VK_NULL_HANDLE;
  VkClearValue m_depthClear{};
  VkCommandPool m_upPool = VK_NULL_HANDLE;
  VkCommandBuffer m_upBuf = VK_NULL_HANDLE;
  VkFence m_upFence = VK_NULL_HANDLE;
};

#endif
