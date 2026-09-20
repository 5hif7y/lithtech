#include "vk_renderer.h"
#ifdef RENDERER_VULKAN
#include "Platform/security.h"
#include "tri_spv.h"
#include "tri_tex_spv.h"
#include "mesh3d_spv.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <limits>
#include <stdexcept>

namespace {
uint32_t findMemType(const VkPhysicalDeviceMemoryProperties& mp,
                     uint32_t bits, VkMemoryPropertyFlags want) {
    for (uint32_t i = 0; i < mp.memoryTypeCount; i++)
        if ((bits & (1u << i)) &&
            (mp.memoryTypes[i].propertyFlags & want) == want)
            return i;
    return UINT32_MAX;
}
void logDevice(VkPhysicalDevice dev) {
    VkPhysicalDeviceProperties pp{};
    vkGetPhysicalDeviceProperties(dev, &pp);
    printf("HOST: vulkan device=%s vendor=0x%04x device=0x%04x\n",
           pp.deviceName, pp.vendorID, pp.deviceID);
}
} // namespace

VulkanRenderer::VulkanRenderer() {
  m_clearValue.color.float32[0] = 0.1f;
  m_clearValue.color.float32[1] = 0.1f;
  m_clearValue.color.float32[2] = 0.2f;
  m_clearValue.color.float32[3] = 1.0f;
  m_depthClear.depthStencil.depth = 1.0f;
  m_depthClear.depthStencil.stencil = 0;
}
VulkanRenderer::~VulkanRenderer() { Shutdown(); }

HRESULT VulkanRenderer::Init(SDL_Window* window) {
  if (!lith_validate_ptr(window)) return E_INVALIDARG;
  if (!createInstance()) return E_FAIL;
  if (!SDL_Vulkan_CreateSurface(window, m_instance, &m_surface)) return E_FAIL;
  if (!lith_validate_ptr(m_instance) || !lith_validate_ptr(m_surface)) return E_FAIL;
  if (!pickPhysicalDevice()) return E_FAIL;
  if (!createLogicalDevice()) return E_FAIL;
  if (!createSwapchain(window)) return E_FAIL;
  return S_OK;
}

void VulkanRenderer::Shutdown() {
  if (m_device) vkDeviceWaitIdle(m_device);
  if (m_vertBuf) { vkDestroyBuffer(m_device, m_vertBuf, nullptr); m_vertBuf = VK_NULL_HANDLE; }
  if (m_vertMem) { vkFreeMemory(m_device, m_vertMem, nullptr); m_vertMem = VK_NULL_HANDLE; }
  m_vertCap = 0;
  if (m_stageBuf) { vkDestroyBuffer(m_device, m_stageBuf, nullptr); m_stageBuf = VK_NULL_HANDLE; }
  if (m_stageMem) { vkFreeMemory(m_device, m_stageMem, nullptr); m_stageMem = VK_NULL_HANDLE; }
  if (m_pipeline) { vkDestroyPipeline(m_device, m_pipeline, nullptr); m_pipeline = VK_NULL_HANDLE; }
  if (m_pipeLayout) { vkDestroyPipelineLayout(m_device, m_pipeLayout, nullptr); m_pipeLayout = VK_NULL_HANDLE; }
  if (m_texPipeWin) { vkDestroyPipeline(m_device, m_texPipeWin, nullptr); m_texPipeWin = VK_NULL_HANDLE; }
  if (m_texPipeOff) { vkDestroyPipeline(m_device, m_texPipeOff, nullptr); m_texPipeOff = VK_NULL_HANDLE; }
  if (m_meshPipeWin) { vkDestroyPipeline(m_device, m_meshPipeWin, nullptr); m_meshPipeWin = VK_NULL_HANDLE; }
  if (m_meshPipeOff) { vkDestroyPipeline(m_device, m_meshPipeOff, nullptr); m_meshPipeOff = VK_NULL_HANDLE; }
  if (m_meshPipeLayout) { vkDestroyPipelineLayout(m_device, m_meshPipeLayout, nullptr); m_meshPipeLayout = VK_NULL_HANDLE; }
  if (m_meshBuf) { vkDestroyBuffer(m_device, m_meshBuf, nullptr); m_meshBuf = VK_NULL_HANDLE; }
  if (m_meshMem) { vkFreeMemory(m_device, m_meshMem, nullptr); m_meshMem = VK_NULL_HANDLE; }
  m_meshCap = 0;
  m_meshBatch.clear();
  if (m_texPipeLayout) { vkDestroyPipelineLayout(m_device, m_texPipeLayout, nullptr); m_texPipeLayout = VK_NULL_HANDLE; }
  if (m_texSampler) { vkDestroySampler(m_device, m_texSampler, nullptr); m_texSampler = VK_NULL_HANDLE; }
  for (auto& t : m_textures) {
    if (t.view) vkDestroyImageView(m_device, t.view, nullptr);
    if (t.image) vkDestroyImage(m_device, t.image, nullptr);
    if (t.mem) vkFreeMemory(m_device, t.mem, nullptr);
  }
  m_textures.clear();
  if (m_texPool) { vkDestroyDescriptorPool(m_device, m_texPool, nullptr); m_texPool = VK_NULL_HANDLE; }
  m_texLayout = VK_NULL_HANDLE;
  m_texBatch.clear();
  m_order.clear();
  if (m_texBuf) { vkDestroyBuffer(m_device, m_texBuf, nullptr); m_texBuf = VK_NULL_HANDLE; }
  if (m_texMem) { vkFreeMemory(m_device, m_texMem, nullptr); m_texMem = VK_NULL_HANDLE; }
  m_texCap = 0;
  if (m_upFence) { vkDestroyFence(m_device, m_upFence, nullptr); m_upFence = VK_NULL_HANDLE; }
  if (m_upPool) { vkDestroyCommandPool(m_device, m_upPool, nullptr); m_upPool = VK_NULL_HANDLE; m_upBuf = VK_NULL_HANDLE; }
  if (m_offFb) { vkDestroyFramebuffer(m_device, m_offFb, nullptr); m_offFb = VK_NULL_HANDLE; }
  if (m_offDepthView) { vkDestroyImageView(m_device, m_offDepthView, nullptr); m_offDepthView = VK_NULL_HANDLE; }
  if (m_offDepthImg) { vkDestroyImage(m_device, m_offDepthImg, nullptr); m_offDepthImg = VK_NULL_HANDLE; }
  if (m_offDepthMem) { vkFreeMemory(m_device, m_offDepthMem, nullptr); m_offDepthMem = VK_NULL_HANDLE; }
  if (m_offPass) { vkDestroyRenderPass(m_device, m_offPass, nullptr); m_offPass = VK_NULL_HANDLE; }
  if (m_offView) { vkDestroyImageView(m_device, m_offView, nullptr); m_offView = VK_NULL_HANDLE; }
  if (m_offImage) { vkDestroyImage(m_device, m_offImage, nullptr); m_offImage = VK_NULL_HANDLE; }
  if (m_offMemory) { vkFreeMemory(m_device, m_offMemory, nullptr); m_offMemory = VK_NULL_HANDLE; }
  m_headless = false;
  m_batch.clear();
  if (m_inFlight) { vkDestroyFence(m_device, m_inFlight, nullptr); m_inFlight = VK_NULL_HANDLE; }
  if (m_imageAvailable) { vkDestroySemaphore(m_device, m_imageAvailable, nullptr); m_imageAvailable = VK_NULL_HANDLE; }
  if (m_renderFinished) { vkDestroySemaphore(m_device, m_renderFinished, nullptr); m_renderFinished = VK_NULL_HANDLE; }
  if (m_cmdPool) { vkDestroyCommandPool(m_device, m_cmdPool, nullptr); m_cmdPool = VK_NULL_HANDLE; m_cmdBuffer = VK_NULL_HANDLE; }
  for (auto fb : m_framebuffers) if (fb) vkDestroyFramebuffer(m_device, fb, nullptr);
  m_framebuffers.clear();
  destroyDepthResources();
  if (m_renderPass) { vkDestroyRenderPass(m_device, m_renderPass, nullptr); m_renderPass = VK_NULL_HANDLE; }
  for (auto v : m_swapViews) if (v) vkDestroyImageView(m_device, v, nullptr);
  m_swapViews.clear();
  m_swapImages.clear();
  if (m_swapchain) { vkDestroySwapchainKHR(m_device, m_swapchain, nullptr); m_swapchain = VK_NULL_HANDLE; }
  m_hasSwapchain = false;
  if (m_surface) { vkDestroySurfaceKHR(m_instance, m_surface, nullptr); m_surface = VK_NULL_HANDLE; }
  if (m_device) { vkDestroyDevice(m_device, nullptr); m_device = VK_NULL_HANDLE; m_graphicsQueue = VK_NULL_HANDLE; }
  if (m_instance) { vkDestroyInstance(m_instance, nullptr); m_instance = VK_NULL_HANDLE; }
}

HRESULT VulkanRenderer::BeginScene() { return S_OK; }
HRESULT VulkanRenderer::EndScene() { return S_OK; }

HRESULT VulkanRenderer::Clear(uint32_t color) {
  // color es 0xAARRGGBB (D3D compatible) o 0x00RRGGBB; alpha por defecto 255 si 0
  float a = ((color >> 24) & 0xFF) / 255.0f;
  float r = ((color >> 16) & 0xFF) / 255.0f;
  float g = ((color >>  8) & 0xFF) / 255.0f;
  float b = ((color      ) & 0xFF) / 255.0f;
  // si alpha 0 y color !=0, asumir opaco (comportamiento D3D Clear legacy)
  if (a == 0.0f && color != 0) a = 1.0f;
  m_clearValue.color.float32[0] = r;
  m_clearValue.color.float32[1] = g;
  m_clearValue.color.float32[2] = b;
  m_clearValue.color.float32[3] = a;
  return S_OK;
}

HRESULT VulkanRenderer::Present() {
  if (!m_hasSwapchain || !m_swapchain || !m_device || !m_graphicsQueue) return S_OK;
  if (!m_renderPass || m_framebuffers.empty() || !m_cmdBuffer) return S_OK;

  uint32_t idx = 0;
  VkResult acq = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, m_imageAvailable, VK_NULL_HANDLE, &idx);
  if (acq == VK_ERROR_OUT_OF_DATE_KHR || acq == VK_SUBOPTIMAL_KHR) return S_OK;
  if (acq != VK_SUCCESS) return S_OK;
  if (idx >= m_framebuffers.size()) return E_FAIL;

  vkWaitForFences(m_device, 1, &m_inFlight, VK_TRUE, UINT64_MAX);
  vkResetFences(m_device, 1, &m_inFlight);

  // Record command buffer: renderpass con clear
  VkCommandBufferBeginInfo bi{};
  bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkResetCommandBuffer(m_cmdBuffer, 0);
  if (vkBeginCommandBuffer(m_cmdBuffer, &bi) != VK_SUCCESS) return E_FAIL;

  VkRenderPassBeginInfo rp{};
  rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rp.renderPass = m_renderPass;
  rp.framebuffer = m_framebuffers[idx];
  rp.renderArea.offset = {0,0};
  rp.renderArea.extent = m_swapExtent;
  VkClearValue clears[2] = {m_clearValue, m_depthClear};
  rp.clearValueCount = (m_depthFormat != VK_FORMAT_UNDEFINED) ? 2u : 1u;
  rp.pClearValues = clears;
  vkCmdBeginRenderPass(m_cmdBuffer, &rp, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdEndRenderPass(m_cmdBuffer);
  vkEndCommandBuffer(m_cmdBuffer);

  VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  VkSubmitInfo si{};
  si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  si.waitSemaphoreCount = 1;
  si.pWaitSemaphores = &m_imageAvailable;
  si.pWaitDstStageMask = &waitStage;
  si.commandBufferCount = 1;
  si.pCommandBuffers = &m_cmdBuffer;
  si.signalSemaphoreCount = 1;
  si.pSignalSemaphores = &m_renderFinished;
  if (vkQueueSubmit(m_graphicsQueue, 1, &si, m_inFlight) != VK_SUCCESS) return E_FAIL;

  VkPresentInfoKHR pi{};
  pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pi.waitSemaphoreCount = 1;
  pi.pWaitSemaphores = &m_renderFinished;
  pi.swapchainCount = 1;
  pi.pSwapchains = &m_swapchain;
  pi.pImageIndices = &idx;
  VkResult pr = vkQueuePresentKHR(m_graphicsQueue, &pi);
  if (pr == VK_ERROR_OUT_OF_DATE_KHR || pr == VK_SUBOPTIMAL_KHR) return S_OK;
  // wait for frame to keep simple demo sin múltiples frames en vuelo
  vkWaitForFences(m_device, 1, &m_inFlight, VK_TRUE, UINT64_MAX);
  return S_OK;
}

HRESULT VulkanRenderer::DrawPrimitive(VkPrimitiveTopology topo, const void* verts, uint32_t vcount) {
  if (!lith_validate_ptr(verts) || !lith_validate_size(vcount, 1<<20)) return E_INVALIDARG;
  if (!lith_validate_ptr(m_device)) return E_FAIL;
  (void)topo;
  return S_OK;
}

bool VulkanRenderer::createInstance() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "NOLF2 LithTech Jupiter";
  appInfo.apiVersion = VK_API_VERSION_1_2;

  unsigned extCount = 0;
  if (!SDL_Vulkan_GetInstanceExtensions(nullptr, &extCount, nullptr)) extCount = 0;
  std::vector<const char*> exts(extCount);
  if (extCount) SDL_Vulkan_GetInstanceExtensions(nullptr, &extCount, exts.data());

  VkInstanceCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  ci.pApplicationInfo = &appInfo;
  ci.enabledExtensionCount = (uint32_t)exts.size();
  ci.ppEnabledExtensionNames = exts.data();
  return vkCreateInstance(&ci, nullptr, &m_instance) == VK_SUCCESS;
}

#ifdef _LINUX
#include <X11/Xlib.h>
#include <vulkan/vulkan_xlib.h>

bool VulkanRenderer::createInstanceX11() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "NOLF2 LithTech Jupiter";
  appInfo.apiVersion = VK_API_VERSION_1_2;
  const char* exts[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_XLIB_SURFACE_EXTENSION_NAME
  };
  VkInstanceCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  ci.pApplicationInfo = &appInfo;
  ci.enabledExtensionCount = 2;
  ci.ppEnabledExtensionNames = exts;
  return vkCreateInstance(&ci, nullptr, &m_instance) == VK_SUCCESS;
}

HRESULT VulkanRenderer::InitNative(void* display, unsigned long window,
                                   uint32_t w, uint32_t h) {
  if (!display || !window) return E_INVALIDARG;
  if (!createInstanceX11()) return E_FAIL;
  VkXlibSurfaceCreateInfoKHR sci{};
  sci.sType = VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR;
  sci.dpy = (Display*)display;
  sci.window = (Window)window;
  if (vkCreateXlibSurfaceKHR(m_instance, &sci, nullptr, &m_surface) != VK_SUCCESS)
    return E_FAIL;
  if (!lith_validate_ptr(m_instance) || !lith_validate_ptr(m_surface)) return E_FAIL;
  if (!pickPhysicalDevice()) return E_FAIL;
  if (!createLogicalDevice()) return E_FAIL;
  if (!createSwapchainWithExtent(w ? w : 800, h ? h : 600)) return E_FAIL;
  return S_OK;
}
#endif

#ifdef _WIN32
#include <vulkan/vulkan_win32.h>

bool VulkanRenderer::createInstanceWin32() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "NOLF2 LithTech Jupiter";
  appInfo.apiVersion = VK_API_VERSION_1_2;
  const char* exts[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_KHR_WIN32_SURFACE_EXTENSION_NAME
  };
  VkInstanceCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  ci.pApplicationInfo = &appInfo;
  ci.enabledExtensionCount = 2;
  ci.ppEnabledExtensionNames = exts;
  return vkCreateInstance(&ci, nullptr, &m_instance) == VK_SUCCESS;
}

HRESULT VulkanRenderer::InitNativeWin32(void* hwnd, uint32_t w, uint32_t h) {
  if (!hwnd) return E_INVALIDARG;
  if (!createInstanceWin32()) return E_FAIL;
  VkWin32SurfaceCreateInfoKHR sci{};
  sci.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
  sci.hinstance = GetModuleHandle(nullptr);
  sci.hwnd = (HWND)hwnd;
  if (vkCreateWin32SurfaceKHR(m_instance, &sci, nullptr, &m_surface) != VK_SUCCESS)
    return E_FAIL;
  if (!lith_validate_ptr(m_instance) || !lith_validate_ptr(m_surface)) return E_FAIL;
  if (!pickPhysicalDevice()) return E_FAIL;
  if (!createLogicalDevice()) return E_FAIL;
  if (!createSwapchainWithExtent(w ? w : 800, h ? h : 600)) return E_FAIL;
  return S_OK;
}
#endif

bool VulkanRenderer::pickPhysicalDevice() {
  if (!lith_validate_ptr(m_instance) || !lith_validate_ptr(m_surface)) return false;
  uint32_t count = 0;
  if (vkEnumeratePhysicalDevices(m_instance, &count, nullptr) != VK_SUCCESS) return false;
  if (count == 0 || count > 16) return false;
  std::vector<VkPhysicalDevice> devs(count);
  if (vkEnumeratePhysicalDevices(m_instance, &count, devs.data()) != VK_SUCCESS) return false;
  for (auto dev : devs) {
    if (!lith_validate_ptr(dev)) continue;
    uint32_t qCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &qCount, nullptr);
    if (qCount == 0 || qCount > 64) continue;
    std::vector<VkQueueFamilyProperties> qProps(qCount);
    vkGetPhysicalDeviceQueueFamilyProperties(dev, &qCount, qProps.data());
    for (uint32_t i = 0; i < qCount; ++i) {
      bool graphics = (qProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0;
      VkBool32 present = VK_FALSE;
      vkGetPhysicalDeviceSurfaceSupportKHR(dev, i, m_surface, &present);
      if (graphics && present) {
        m_physicalDevice = dev;
        m_graphicsQueueFamily = i;
        logDevice(dev);
        return true;
      }
    }
  }
  // fallback: primer device si ninguno anuncia present (p.e. headless)
  if (!lith_validate_ptr(devs[0])) return false;
  m_physicalDevice = devs[0];
  m_graphicsQueueFamily = 0;
  logDevice(devs[0]);
  return true;
}

bool VulkanRenderer::createLogicalDevice() {
  float prio = 1.0f;
  VkDeviceQueueCreateInfo qci{};
  qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  qci.queueFamilyIndex = m_graphicsQueueFamily;
  qci.queueCount = 1;
  qci.pQueuePriorities = &prio;

  const char* devExts[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
  VkDeviceCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  ci.queueCreateInfoCount = 1;
  ci.pQueueCreateInfos = &qci;
  ci.enabledExtensionCount = 1;
  ci.ppEnabledExtensionNames = devExts;

  if (vkCreateDevice(m_physicalDevice, &ci, nullptr, &m_device) != VK_SUCCESS) return false;
  vkGetDeviceQueue(m_device, m_graphicsQueueFamily, 0, &m_graphicsQueue);
  return true;
}

bool VulkanRenderer::createSwapchain(SDL_Window* window) {
  if (!lith_validate_ptr(window) || !lith_validate_ptr(m_physicalDevice) || !lith_validate_ptr(m_device) || !lith_validate_ptr(m_surface))
    return false;
  int w = 800, h = 600;
  SDL_Vulkan_GetDrawableSize(window, &w, &h);
  if (w == 0 || h == 0) SDL_GetWindowSize(window, &w, &h);
  if (w == 0) w = 800;
  if (h == 0) h = 600;
  return createSwapchainWithExtent((uint32_t)w, (uint32_t)h);
}

bool VulkanRenderer::createSwapchainWithExtent(uint32_t ew, uint32_t eh) {
  if (!lith_validate_ptr(m_physicalDevice) || !lith_validate_ptr(m_device) || !lith_validate_ptr(m_surface))
    return false;

  // --- surface format ---
  uint32_t fmtCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &fmtCount, nullptr);
  if (fmtCount == 0) return false;
  std::vector<VkSurfaceFormatKHR> formats(fmtCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &fmtCount, formats.data());
  VkSurfaceFormatKHR chosen = formats[0];
  for (auto &f : formats) {
    if (f.format == VK_FORMAT_B8G8R8A8_SRGB && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) { chosen = f; break; }
    if (f.format == VK_FORMAT_B8G8R8A8_UNORM && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) { chosen = f; }
  }
  m_swapFormat = chosen.format;

  // --- present mode ---
  uint32_t pmCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &pmCount, nullptr);
  std::vector<VkPresentModeKHR> modes;
  VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
  if (pmCount) {
    modes.resize(pmCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &pmCount, modes.data());
    // prefer MAILBOX if available, else FIFO (always available)
    for (auto m : modes) if (m == VK_PRESENT_MODE_MAILBOX_KHR) { presentMode = m; break; }
  }

  // --- capabilities & extent ---
  VkSurfaceCapabilitiesKHR caps{};
  if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &caps) != VK_SUCCESS) return false;

  VkExtent2D extent = caps.currentExtent;
  if (extent.width == std::numeric_limits<uint32_t>::max()) {
    extent.width  = std::clamp<uint32_t>(ew, caps.minImageExtent.width, caps.maxImageExtent.width);
    extent.height = std::clamp<uint32_t>(eh, caps.minImageExtent.height, caps.maxImageExtent.height);
  }
  m_swapExtent = extent;

  uint32_t imageCount = caps.minImageCount + 1;
  if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount) imageCount = caps.maxImageCount;

  VkSwapchainCreateInfoKHR sci{};
  sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  sci.surface = m_surface;
  sci.minImageCount = imageCount;
  sci.imageFormat = m_swapFormat;
  sci.imageColorSpace = chosen.colorSpace;
  sci.imageExtent = extent;
  sci.imageArrayLayers = 1;
  sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  sci.preTransform = caps.currentTransform;
  sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  sci.presentMode = presentMode;
  sci.clipped = VK_TRUE;
  sci.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(m_device, &sci, nullptr, &m_swapchain) != VK_SUCCESS) return false;

  uint32_t imgCount = 0;
  vkGetSwapchainImagesKHR(m_device, m_swapchain, &imgCount, nullptr);
  if (imgCount==0) return false;
  m_swapImages.resize(imgCount);
  vkGetSwapchainImagesKHR(m_device, m_swapchain, &imgCount, m_swapImages.data());

  if (!createImageViews()) return false;
  if (!createRenderPass()) return false;
  if (!createFramebuffers()) return false;
  if (!createCommandPool()) return false;
  if (!createSyncObjects()) return false;
  m_hasSwapchain = true;
  return true;
}

bool VulkanRenderer::createImageViews() {
  m_swapViews.resize(m_swapImages.size());
  for (size_t i=0;i<m_swapImages.size();++i) {
    VkImageViewCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    ci.image = m_swapImages[i];
    ci.viewType = VK_IMAGE_VIEW_TYPE_2D;
    ci.format = m_swapFormat;
    ci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    ci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    ci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    ci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    ci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    ci.subresourceRange.baseMipLevel = 0;
    ci.subresourceRange.levelCount = 1;
    ci.subresourceRange.baseArrayLayer = 0;
    ci.subresourceRange.layerCount = 1;
    if (vkCreateImageView(m_device, &ci, nullptr, &m_swapViews[i]) != VK_SUCCESS) return false;
  }
  return true;
}

bool VulkanRenderer::createRenderPass() {
  if (m_depthFormat == VK_FORMAT_UNDEFINED) findDepthFormat();
  const bool hasDepth = (m_depthFormat != VK_FORMAT_UNDEFINED);
  VkAttachmentDescription atts[2]{};
  atts[0].format = m_swapFormat;
  atts[0].samples = VK_SAMPLE_COUNT_1_BIT;
  atts[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  atts[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  atts[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  atts[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  atts[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  atts[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference ref{};
  ref.attachment = 0;
  ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthRef{};
  if (hasDepth) {
    atts[1].format = m_depthFormat;
    atts[1].samples = VK_SAMPLE_COUNT_1_BIT;
    atts[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    atts[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    atts[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    atts[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    atts[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    atts[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depthRef.attachment = 1;
    depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  }

  VkSubpassDescription sub{};
  sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  sub.colorAttachmentCount = 1;
  sub.pColorAttachments = &ref;
  if (hasDepth) sub.pDepthStencilAttachment = &depthRef;

  VkSubpassDependency dep{};
  dep.srcSubpass = VK_SUBPASS_EXTERNAL;
  dep.dstSubpass = 0;
  dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                     VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dep.srcAccessMask = 0;
  dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                      VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  ci.attachmentCount = hasDepth ? 2u : 1u;
  ci.pAttachments = atts;
  ci.subpassCount = 1;
  ci.pSubpasses = &sub;
  ci.dependencyCount = 1;
  ci.pDependencies = &dep;

  return vkCreateRenderPass(m_device, &ci, nullptr, &m_renderPass) == VK_SUCCESS;
}

VkFormat VulkanRenderer::findDepthFormat() {
  m_depthFormat = VK_FORMAT_UNDEFINED;
  if (!m_physicalDevice) return m_depthFormat;
  const VkFormat cands[] = {VK_FORMAT_D32_SFLOAT,
                            VK_FORMAT_D24_UNORM_S8_UINT,
                            VK_FORMAT_D16_UNORM};
  for (size_t i = 0; i < sizeof(cands) / sizeof(cands[0]); i++) {
    VkFormatProperties fp{};
    vkGetPhysicalDeviceFormatProperties(m_physicalDevice, cands[i], &fp);
    if (fp.optimalTilingFeatures &
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
      m_depthFormat = cands[i];
      break;
    }
  }
  return m_depthFormat;
}

bool VulkanRenderer::createDepthImage(VkPhysicalDevice phys, VkDevice dev, uint32_t w,
                             uint32_t h, VkFormat fmt, VkImage& img,
                             VkDeviceMemory& mem, VkImageView& view) {
  img = VK_NULL_HANDLE;
  mem = VK_NULL_HANDLE;
  view = VK_NULL_HANDLE;
  if (!phys || !dev || !w || !h || fmt == VK_FORMAT_UNDEFINED) return false;
  VkImageCreateInfo ii{};
  ii.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ii.imageType = VK_IMAGE_TYPE_2D;
  ii.format = fmt;
  ii.extent = {w, h, 1};
  ii.mipLevels = 1;
  ii.arrayLayers = 1;
  ii.samples = VK_SAMPLE_COUNT_1_BIT;
  ii.tiling = VK_IMAGE_TILING_OPTIMAL;
  ii.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
  ii.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (vkCreateImage(dev, &ii, nullptr, &img) != VK_SUCCESS) return false;
  VkMemoryRequirements mr{};
  vkGetImageMemoryRequirements(dev, img, &mr);
  VkPhysicalDeviceMemoryProperties mp{};
  vkGetPhysicalDeviceMemoryProperties(phys, &mp);
  uint32_t mi = findMemType(mp, mr.memoryTypeBits,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (mi == UINT32_MAX) {
    vkDestroyImage(dev, img, nullptr);
    img = VK_NULL_HANDLE;
    return false;
  }
  VkMemoryAllocateInfo mai{};
  mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  mai.allocationSize = mr.size;
  mai.memoryTypeIndex = mi;
  if (vkAllocateMemory(dev, &mai, nullptr, &mem) != VK_SUCCESS) {
    vkDestroyImage(dev, img, nullptr);
    img = VK_NULL_HANDLE;
    return false;
  }
  vkBindImageMemory(dev, img, mem, 0);
  VkImageViewCreateInfo vi{};
  vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  vi.image = img;
  vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
  vi.format = fmt;
  vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
  if (fmt == VK_FORMAT_D24_UNORM_S8_UINT)
    vi.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
  vi.subresourceRange.levelCount = 1;
  vi.subresourceRange.layerCount = 1;
  if (vkCreateImageView(dev, &vi, nullptr, &view) != VK_SUCCESS) {
    vkFreeMemory(dev, mem, nullptr);
    vkDestroyImage(dev, img, nullptr);
    img = VK_NULL_HANDLE;
    mem = VK_NULL_HANDLE;
    return false;
  }
  return true;
}

bool VulkanRenderer::createDepthResources(uint32_t w, uint32_t h) {
  destroyDepthResources();
  if (m_depthFormat == VK_FORMAT_UNDEFINED) return false;
  return createDepthImage(m_physicalDevice, m_device, w, h, m_depthFormat,
                         m_depthImg, m_depthMem, m_depthView);
}

void VulkanRenderer::destroyDepthResources() {
  if (!m_device) {
    m_depthImg = VK_NULL_HANDLE;
    m_depthMem = VK_NULL_HANDLE;
    m_depthView = VK_NULL_HANDLE;
    return;
  }
  if (m_depthView) {
    vkDestroyImageView(m_device, m_depthView, nullptr);
    m_depthView = VK_NULL_HANDLE;
  }
  if (m_depthImg) {
    vkDestroyImage(m_device, m_depthImg, nullptr);
    m_depthImg = VK_NULL_HANDLE;
  }
  if (m_depthMem) {
    vkFreeMemory(m_device, m_depthMem, nullptr);
    m_depthMem = VK_NULL_HANDLE;
  }
}

bool VulkanRenderer::createFramebuffers() {
  const bool hasDepth = (m_depthFormat != VK_FORMAT_UNDEFINED);
  if (hasDepth &&
      !createDepthResources(m_swapExtent.width, m_swapExtent.height))
    return false;
  m_framebuffers.resize(m_swapViews.size());
  for (size_t i=0;i<m_swapViews.size();++i) {
    VkImageView atts[2] = {m_swapViews[i], m_depthView};
    VkFramebufferCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    ci.renderPass = m_renderPass;
    ci.attachmentCount = hasDepth ? 2u : 1u;
    ci.pAttachments = atts;
    ci.width = m_swapExtent.width;
    ci.height = m_swapExtent.height;
    ci.layers = 1;
    if (vkCreateFramebuffer(m_device, &ci, nullptr, &m_framebuffers[i]) != VK_SUCCESS) return false;
  }
  return true;
}

bool VulkanRenderer::createCommandPool() {
  VkCommandPoolCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  ci.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  ci.queueFamilyIndex = m_graphicsQueueFamily;
  if (vkCreateCommandPool(m_device, &ci, nullptr, &m_cmdPool) != VK_SUCCESS) return false;
  VkCommandBufferAllocateInfo ai{};
  ai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  ai.commandPool = m_cmdPool;
  ai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  ai.commandBufferCount = 1;
  if (vkAllocateCommandBuffers(m_device, &ai, &m_cmdBuffer) != VK_SUCCESS) return false;
  return true;
}

bool VulkanRenderer::createSyncObjects() {
  VkSemaphoreCreateInfo si{};
  si.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
  VkFenceCreateInfo fi{};
  fi.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fi.flags = VK_FENCE_CREATE_SIGNALED_BIT;
  if (vkCreateSemaphore(m_device, &si, nullptr, &m_imageAvailable)!=VK_SUCCESS) return false;
  if (vkCreateSemaphore(m_device, &si, nullptr, &m_renderFinished)!=VK_SUCCESS) return false;
  if (vkCreateFence(m_device, &fi, nullptr, &m_inFlight)!=VK_SUCCESS) return false;
  return true;
}

bool VulkanRenderer::createWindowPipeline() {
  if (!m_device || !m_renderPass) return false;
  uint32_t w = m_swapExtent.width, h = m_swapExtent.height;
  if (!w || !h) return false;
  VkShaderModule vsm = VK_NULL_HANDLE, fsm = VK_NULL_HANDLE;
  VkShaderModuleCreateInfo sci{};
  sci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  sci.codeSize = sizeof(kTriVertSpv);
  sci.pCode = kTriVertSpv;
  if (vkCreateShaderModule(m_device, &sci, nullptr, &vsm) != VK_SUCCESS)
    return false;
  sci.codeSize = sizeof(kTriFragSpv);
  sci.pCode = kTriFragSpv;
  if (vkCreateShaderModule(m_device, &sci, nullptr, &fsm) != VK_SUCCESS) {
    vkDestroyShaderModule(m_device, vsm, nullptr);
    return false;
  }
  VkPipelineShaderStageCreateInfo stages[2]{};
  stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = vsm;
  stages[0].pName = "main";
  stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = fsm;
  stages[1].pName = "main";
  VkVertexInputBindingDescription bind{};
  bind.stride = sizeof(VkTriVert);
  bind.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  VkVertexInputAttributeDescription attrs[2]{};
  attrs[0].location = 0;
  attrs[0].format = VK_FORMAT_R32G32_SFLOAT;
  attrs[0].offset = 0;
  attrs[1].location = 1;
  attrs[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attrs[1].offset = 8;
  VkPipelineVertexInputStateCreateInfo vii{};
  vii.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vii.vertexBindingDescriptionCount = 1;
  vii.pVertexBindingDescriptions = &bind;
  vii.vertexAttributeDescriptionCount = 2;
  vii.pVertexAttributeDescriptions = attrs;
  VkPipelineInputAssemblyStateCreateInfo iai{};
  iai.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  iai.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkViewport vp{0, 0, (float)w, (float)h, 0.0f, 1.0f};
  VkRect2D sc{{0, 0}, {w, h}};
  VkPipelineViewportStateCreateInfo vpi{};
  vpi.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vpi.viewportCount = 1;
  vpi.pViewports = &vp;
  vpi.scissorCount = 1;
  vpi.pScissors = &sc;
  VkPipelineRasterizationStateCreateInfo rsi{};
  rsi.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rsi.polygonMode = VK_POLYGON_MODE_FILL;
  rsi.cullMode = VK_CULL_MODE_NONE;
  rsi.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rsi.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo msi{};
  msi.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  msi.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineColorBlendAttachmentState ba{};
  ba.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  VkPipelineColorBlendStateCreateInfo bci{};
  bci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  bci.attachmentCount = 1;
  bci.pAttachments = &ba;
  VkPipelineLayoutCreateInfo pli{};
  pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  if (vkCreatePipelineLayout(m_device, &pli, nullptr, &m_pipeLayout) != VK_SUCCESS) {
    vkDestroyShaderModule(m_device, vsm, nullptr);
    vkDestroyShaderModule(m_device, fsm, nullptr);
    return false;
  }
  VkGraphicsPipelineCreateInfo gpi{};
  gpi.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  gpi.stageCount = 2;
  gpi.pStages = stages;
  gpi.pVertexInputState = &vii;
  gpi.pInputAssemblyState = &iai;
  gpi.pViewportState = &vpi;
  gpi.pRasterizationState = &rsi;
  gpi.pMultisampleState = &msi;
  gpi.pColorBlendState = &bci;
  gpi.layout = m_pipeLayout;
  gpi.renderPass = m_renderPass;
  bool ok = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &gpi,
                                      nullptr, &m_pipeline) == VK_SUCCESS;
  vkDestroyShaderModule(m_device, vsm, nullptr);
  vkDestroyShaderModule(m_device, fsm, nullptr);
  return ok;
}

bool VulkanRenderer::uploadBatch() {
  VkDeviceSize need = (VkDeviceSize)m_batch.size() * sizeof(VkTriVert);
  if (need > m_vertCap) {
    if (m_vertBuf) vkDestroyBuffer(m_device, m_vertBuf, nullptr);
    if (m_vertMem) vkFreeMemory(m_device, m_vertMem, nullptr);
    m_vertBuf = VK_NULL_HANDLE;
    m_vertMem = VK_NULL_HANDLE;
    m_vertCap = 0;
    if (need) {
      VkPhysicalDeviceMemoryProperties mp{};
      vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &mp);
      VkBufferCreateInfo bci{};
      bci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bci.size = need;
      bci.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
      if (vkCreateBuffer(m_device, &bci, nullptr, &m_vertBuf) != VK_SUCCESS)
        return false;
      VkMemoryRequirements mr{};
      vkGetBufferMemoryRequirements(m_device, m_vertBuf, &mr);
      uint32_t mi = findMemType(mp, mr.memoryTypeBits,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      if (mi == UINT32_MAX) return false;
      VkMemoryAllocateInfo mai{};
      mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      mai.allocationSize = mr.size;
      mai.memoryTypeIndex = mi;
      if (vkAllocateMemory(m_device, &mai, nullptr, &m_vertMem) != VK_SUCCESS)
        return false;
      vkBindBufferMemory(m_device, m_vertBuf, m_vertMem, 0);
      m_vertCap = need;
    }
  }
  if (need) {
    void* dst = nullptr;
    if (vkMapMemory(m_device, m_vertMem, 0, need, 0, &dst) != VK_SUCCESS)
      return false;
    memcpy(dst, m_batch.data(), (size_t)need);
    vkUnmapMemory(m_device, m_vertMem);
  }
  return true;
}

HRESULT VulkanRenderer::InitHeadlessPresent(uint32_t w, uint32_t h) {
  Shutdown();
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "NOLF2 LithTech Jupiter";
  appInfo.apiVersion = VK_API_VERSION_1_2;
  const char* exts[] = {
    VK_KHR_SURFACE_EXTENSION_NAME,
    VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME
  };
  VkInstanceCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  ci.pApplicationInfo = &appInfo;
  ci.enabledExtensionCount = 2;
  ci.ppEnabledExtensionNames = exts;
  if (vkCreateInstance(&ci, nullptr, &m_instance) != VK_SUCCESS) return E_FAIL;
  PFN_vkCreateHeadlessSurfaceEXT fp =
      (PFN_vkCreateHeadlessSurfaceEXT)vkGetInstanceProcAddr(
          m_instance, "vkCreateHeadlessSurfaceEXT");
  if (!fp) return E_FAIL;
  VkHeadlessSurfaceCreateInfoEXT sci{};
  sci.sType = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT;
  if (fp(m_instance, &sci, nullptr, &m_surface) != VK_SUCCESS) return E_FAIL;
  if (!pickPhysicalDevice()) return E_FAIL;
  if (!createLogicalDevice()) return E_FAIL;
  if (!createSwapchainWithExtent(w ? w : 800, h ? h : 600)) return E_FAIL;
  return S_OK;
}

HRESULT VulkanRenderer::RenderWindowFrame() {
  if (!m_hasSwapchain || !m_swapchain || !m_device || !m_graphicsQueue)
    return E_FAIL;
  if (!m_renderPass || m_framebuffers.empty() || !m_cmdBuffer) return E_FAIL;
  if (!m_pipeline && !createWindowPipeline()) return E_FAIL;
  if (!uploadBatch()) return E_FAIL;

  uint32_t idx = 0;
  VkResult acq = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX,
                                       m_imageAvailable, VK_NULL_HANDLE, &idx);
  if (acq == VK_ERROR_OUT_OF_DATE_KHR || acq == VK_SUBOPTIMAL_KHR) return S_OK;
  if (acq != VK_SUCCESS) return E_FAIL;
  if (idx >= m_framebuffers.size()) return E_FAIL;

  vkWaitForFences(m_device, 1, &m_inFlight, VK_TRUE, UINT64_MAX);
  vkResetFences(m_device, 1, &m_inFlight);

  VkCommandBufferBeginInfo bi{};
  bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkResetCommandBuffer(m_cmdBuffer, 0);
  if (vkBeginCommandBuffer(m_cmdBuffer, &bi) != VK_SUCCESS) return E_FAIL;
  VkRenderPassBeginInfo rp{};
  rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rp.renderPass = m_renderPass;
  rp.framebuffer = m_framebuffers[idx];
  rp.renderArea.offset = {0, 0};
  rp.renderArea.extent = m_swapExtent;
  VkClearValue clears[2] = {m_clearValue, m_depthClear};
  rp.clearValueCount = (m_depthFormat != VK_FORMAT_UNDEFINED) ? 2u : 1u;
  rp.pClearValues = clears;
  vkCmdBeginRenderPass(m_cmdBuffer, &rp, VK_SUBPASS_CONTENTS_INLINE);
  if (!drawOrdered(m_cmdBuffer, m_renderPass)) return E_FAIL;
  vkCmdEndRenderPass(m_cmdBuffer);
  if (vkEndCommandBuffer(m_cmdBuffer) != VK_SUCCESS) return E_FAIL;

  VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  VkSubmitInfo si{};
  si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  si.waitSemaphoreCount = 1;
  si.pWaitSemaphores = &m_imageAvailable;
  si.pWaitDstStageMask = &waitStage;
  si.commandBufferCount = 1;
  si.pCommandBuffers = &m_cmdBuffer;
  si.signalSemaphoreCount = 1;
  si.pSignalSemaphores = &m_renderFinished;
  if (vkQueueSubmit(m_graphicsQueue, 1, &si, m_inFlight) != VK_SUCCESS)
    return E_FAIL;

  VkPresentInfoKHR pi{};
  pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pi.waitSemaphoreCount = 1;
  pi.pWaitSemaphores = &m_renderFinished;
  pi.swapchainCount = 1;
  pi.pSwapchains = &m_swapchain;
  pi.pImageIndices = &idx;
  VkResult pr = vkQueuePresentKHR(m_graphicsQueue, &pi);
  if (pr == VK_ERROR_OUT_OF_DATE_KHR || pr == VK_SUBOPTIMAL_KHR) return S_OK;
  vkWaitForFences(m_device, 1, &m_inFlight, VK_TRUE, UINT64_MAX);
  m_batch.clear();
  m_texBatch.clear();
  m_meshBatch.clear();
  m_order.clear();
  return S_OK;
}

void VulkanRenderer::PushTexQuad(uint32_t tex, const VkTexVert v[4]) {
  if (!tex || !v) return;
  TexQuad q;
  q.tex = tex;
  q.v[0] = v[0]; q.v[1] = v[1]; q.v[2] = v[2];
  q.v[3] = v[0]; q.v[4] = v[2]; q.v[5] = v[3];
  m_texBatch.push_back(q);
  DrawItem it;
  it.tex = true;
  it.idx = (uint32_t)(m_texBatch.size() - 1);
  m_order.push_back(it);
}

size_t VulkanRenderer::PendingTexQuads() const { return m_texBatch.size(); }

bool VulkanRenderer::ensureTexObjects() {
  if (!m_device) return false;
  if (m_texLayout && m_texPool && m_texSampler) return true;
  VkDescriptorSetLayoutBinding b{};
  b.binding = 0;
  b.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  b.descriptorCount = 1;
  b.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
  VkDescriptorSetLayoutCreateInfo li{};
  li.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  li.bindingCount = 1;
  li.pBindings = &b;
  if (vkCreateDescriptorSetLayout(m_device, &li, nullptr, &m_texLayout) != VK_SUCCESS)
    return false;
  VkDescriptorPoolSize ps{};
  ps.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  ps.descriptorCount = 128;
  VkDescriptorPoolCreateInfo pi{};
  pi.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  pi.maxSets = 128;
  pi.poolSizeCount = 1;
  pi.pPoolSizes = &ps;
  if (vkCreateDescriptorPool(m_device, &pi, nullptr, &m_texPool) != VK_SUCCESS)
    return false;
  VkSamplerCreateInfo sci{};
  sci.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  sci.magFilter = VK_FILTER_LINEAR;
  sci.minFilter = VK_FILTER_LINEAR;
  sci.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sci.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  sci.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
  if (vkCreateSampler(m_device, &sci, nullptr, &m_texSampler) != VK_SUCCESS)
    return false;
  return true;
}

bool VulkanRenderer::ensureUploadPool() {
  if (m_upPool && m_upBuf && m_upFence) return true;
  VkCommandPoolCreateInfo cpi{};
  cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  cpi.queueFamilyIndex = m_graphicsQueueFamily;
  if (vkCreateCommandPool(m_device, &cpi, nullptr, &m_upPool) != VK_SUCCESS)
    return false;
  VkCommandBufferAllocateInfo cai{};
  cai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  cai.commandPool = m_upPool;
  cai.commandBufferCount = 1;
  if (vkAllocateCommandBuffers(m_device, &cai, &m_upBuf) != VK_SUCCESS)
    return false;
  VkFenceCreateInfo fi{};
  fi.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  if (vkCreateFence(m_device, &fi, nullptr, &m_upFence) != VK_SUCCESS)
    return false;
  return true;
}

uint32_t VulkanRenderer::RegisterTexture(uint32_t w, uint32_t h,
                                         const uint8_t* rgba) {
  if (!m_device || !m_graphicsQueue || !w || !h || !rgba) return 0;
  if (!ensureTexObjects() || !ensureUploadPool()) return 0;
  VkPhysicalDeviceMemoryProperties mp{};
  vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &mp);
  VkDeviceSize bytes = (VkDeviceSize)w * h * 4;

  // Staging buffer with the pixel data.
  VkBuffer stg = VK_NULL_HANDLE;
  VkDeviceMemory stgMem = VK_NULL_HANDLE;
  VkBufferCreateInfo bci{};
  bci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bci.size = bytes;
  bci.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
  if (vkCreateBuffer(m_device, &bci, nullptr, &stg) != VK_SUCCESS) return 0;
  VkMemoryRequirements mr{};
  vkGetBufferMemoryRequirements(m_device, stg, &mr);
  uint32_t mi = findMemType(mp, mr.memoryTypeBits,
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
  if (mi == UINT32_MAX) { vkDestroyBuffer(m_device, stg, nullptr); return 0; }
  VkMemoryAllocateInfo mai{};
  mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  mai.allocationSize = mr.size;
  mai.memoryTypeIndex = mi;
  if (vkAllocateMemory(m_device, &mai, nullptr, &stgMem) != VK_SUCCESS) {
    vkDestroyBuffer(m_device, stg, nullptr);
    return 0;
  }
  vkBindBufferMemory(m_device, stg, stgMem, 0);
  void* dst = nullptr;
  if (vkMapMemory(m_device, stgMem, 0, bytes, 0, &dst) != VK_SUCCESS) {
    vkDestroyBuffer(m_device, stg, nullptr);
    vkFreeMemory(m_device, stgMem, nullptr);
    return 0;
  }
  memcpy(dst, rgba, (size_t)bytes);
  vkUnmapMemory(m_device, stgMem);

  // Device-local sampled image.
  VkImage img = VK_NULL_HANDLE;
  VkDeviceMemory imgMem = VK_NULL_HANDLE;
  VkImageCreateInfo ii{};
  ii.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
  ii.imageType = VK_IMAGE_TYPE_2D;
  ii.format = VK_FORMAT_R8G8B8A8_UNORM;
  ii.extent = {w, h, 1};
  ii.mipLevels = 1;
  ii.arrayLayers = 1;
  ii.samples = VK_SAMPLE_COUNT_1_BIT;
  ii.tiling = VK_IMAGE_TILING_OPTIMAL;
  ii.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
  ii.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  if (vkCreateImage(m_device, &ii, nullptr, &img) != VK_SUCCESS) {
    vkDestroyBuffer(m_device, stg, nullptr);
    vkFreeMemory(m_device, stgMem, nullptr);
    return 0;
  }
  vkGetImageMemoryRequirements(m_device, img, &mr);
  mi = findMemType(mp, mr.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  if (mi == UINT32_MAX) {
    vkDestroyImage(m_device, img, nullptr);
    vkDestroyBuffer(m_device, stg, nullptr);
    vkFreeMemory(m_device, stgMem, nullptr);
    return 0;
  }
  mai.allocationSize = mr.size;
  mai.memoryTypeIndex = mi;
  if (vkAllocateMemory(m_device, &mai, nullptr, &imgMem) != VK_SUCCESS) {
    vkDestroyImage(m_device, img, nullptr);
    vkDestroyBuffer(m_device, stg, nullptr);
    vkFreeMemory(m_device, stgMem, nullptr);
    return 0;
  }
  vkBindImageMemory(m_device, img, imgMem, 0);

  // Upload: UNDEFINED -> DST -> copy -> SHADER_READ.
  VkCommandBufferBeginInfo bi{};
  bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkResetCommandBuffer(m_upBuf, 0);
  if (vkBeginCommandBuffer(m_upBuf, &bi) != VK_SUCCESS) {
    vkDestroyImage(m_device, img, nullptr);
    vkFreeMemory(m_device, imgMem, nullptr);
    vkDestroyBuffer(m_device, stg, nullptr);
    vkFreeMemory(m_device, stgMem, nullptr);
    return 0;
  }
  VkImageMemoryBarrier bar{};
  bar.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  bar.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  bar.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  bar.image = img;
  bar.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  bar.subresourceRange.levelCount = 1;
  bar.subresourceRange.layerCount = 1;
  bar.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  bar.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  bar.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  vkCmdPipelineBarrier(m_upBuf, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
                       VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0,
                       nullptr, 1, &bar);
  VkBufferImageCopy cp{};
  cp.imageExtent = {w, h, 1};
  cp.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  cp.imageSubresource.layerCount = 1;
  vkCmdCopyBufferToImage(m_upBuf, stg, img,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &cp);
  bar.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  bar.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  bar.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  bar.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
  vkCmdPipelineBarrier(m_upBuf, VK_PIPELINE_STAGE_TRANSFER_BIT,
                       VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr,
                       0, nullptr, 1, &bar);
  if (vkEndCommandBuffer(m_upBuf) != VK_SUCCESS) {
    vkDestroyImage(m_device, img, nullptr);
    vkFreeMemory(m_device, imgMem, nullptr);
    vkDestroyBuffer(m_device, stg, nullptr);
    vkFreeMemory(m_device, stgMem, nullptr);
    return 0;
  }
  VkSubmitInfo si{};
  si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  si.commandBufferCount = 1;
  si.pCommandBuffers = &m_upBuf;
  vkResetFences(m_device, 1, &m_upFence);
  if (vkQueueSubmit(m_graphicsQueue, 1, &si, m_upFence) != VK_SUCCESS) {
    vkDestroyImage(m_device, img, nullptr);
    vkFreeMemory(m_device, imgMem, nullptr);
    vkDestroyBuffer(m_device, stg, nullptr);
    vkFreeMemory(m_device, stgMem, nullptr);
    return 0;
  }
  vkWaitForFences(m_device, 1, &m_upFence, VK_TRUE, UINT64_MAX);
  vkDestroyBuffer(m_device, stg, nullptr);
  vkFreeMemory(m_device, stgMem, nullptr);

  VkImageViewCreateInfo vi{};
  vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
  vi.image = img;
  vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
  vi.format = VK_FORMAT_R8G8B8A8_UNORM;
  vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  vi.subresourceRange.levelCount = 1;
  vi.subresourceRange.layerCount = 1;
  VkImageView view = VK_NULL_HANDLE;
  if (vkCreateImageView(m_device, &vi, nullptr, &view) != VK_SUCCESS) {
    vkDestroyImage(m_device, img, nullptr);
    vkFreeMemory(m_device, imgMem, nullptr);
    return 0;
  }
  VkDescriptorSetAllocateInfo ai{};
  ai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  ai.descriptorPool = m_texPool;
  ai.descriptorSetCount = 1;
  ai.pSetLayouts = &m_texLayout;
  VkDescriptorSet set = VK_NULL_HANDLE;
  if (vkAllocateDescriptorSets(m_device, &ai, &set) != VK_SUCCESS) {
    vkDestroyImageView(m_device, view, nullptr);
    vkDestroyImage(m_device, img, nullptr);
    vkFreeMemory(m_device, imgMem, nullptr);
    return 0;
  }
  VkDescriptorImageInfo ii2{};
  ii2.sampler = m_texSampler;
  ii2.imageView = view;
  ii2.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  VkWriteDescriptorSet wr{};
  wr.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
  wr.dstSet = set;
  wr.descriptorCount = 1;
  wr.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  wr.pImageInfo = &ii2;
  vkUpdateDescriptorSets(m_device, 1, &wr, 0, nullptr);

  WinTex t;
  t.image = img;
  t.mem = imgMem;
  t.view = view;
  t.set = set;
  m_textures.push_back(t);
  return (uint32_t)m_textures.size();
}

bool VulkanRenderer::createTexPipeline(VkRenderPass pass, uint32_t w,
                                       uint32_t h, VkPipeline& out) {
  out = VK_NULL_HANDLE;
  if (!m_device || !pass || !w || !h) return false;
  VkShaderModule vsm = VK_NULL_HANDLE, fsm = VK_NULL_HANDLE;
  VkShaderModuleCreateInfo sci{};
  sci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  sci.codeSize = sizeof(kTriTexVertSpv);
  sci.pCode = kTriTexVertSpv;
  if (vkCreateShaderModule(m_device, &sci, nullptr, &vsm) != VK_SUCCESS)
    return false;
  sci.codeSize = sizeof(kTriTexFragSpv);
  sci.pCode = kTriTexFragSpv;
  if (vkCreateShaderModule(m_device, &sci, nullptr, &fsm) != VK_SUCCESS) {
    vkDestroyShaderModule(m_device, vsm, nullptr);
    return false;
  }
  VkPipelineShaderStageCreateInfo stages[2]{};
  stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = vsm;
  stages[0].pName = "main";
  stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = fsm;
  stages[1].pName = "main";
  VkVertexInputBindingDescription bind{};
  bind.stride = sizeof(VkTexVert);
  bind.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  VkVertexInputAttributeDescription attrs[3]{};
  attrs[0].location = 0;
  attrs[0].format = VK_FORMAT_R32G32_SFLOAT;
  attrs[0].offset = 0;
  attrs[1].location = 1;
  attrs[1].format = VK_FORMAT_R32G32_SFLOAT;
  attrs[1].offset = 8;
  attrs[2].location = 2;
  attrs[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attrs[2].offset = 16;
  VkPipelineVertexInputStateCreateInfo vii{};
  vii.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vii.vertexBindingDescriptionCount = 1;
  vii.pVertexBindingDescriptions = &bind;
  vii.vertexAttributeDescriptionCount = 3;
  vii.pVertexAttributeDescriptions = attrs;
  VkPipelineInputAssemblyStateCreateInfo iai{};
  iai.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  iai.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkViewport vp{0, 0, (float)w, (float)h, 0.0f, 1.0f};
  VkRect2D sc{{0, 0}, {w, h}};
  VkPipelineViewportStateCreateInfo vpi{};
  vpi.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vpi.viewportCount = 1;
  vpi.pViewports = &vp;
  vpi.scissorCount = 1;
  vpi.pScissors = &sc;
  VkPipelineRasterizationStateCreateInfo rsi{};
  rsi.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rsi.polygonMode = VK_POLYGON_MODE_FILL;
  rsi.cullMode = VK_CULL_MODE_NONE;
  rsi.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rsi.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo msi{};
  msi.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  msi.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineColorBlendAttachmentState ba{};
  ba.blendEnable = VK_TRUE;
  ba.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  ba.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  ba.colorBlendOp = VK_BLEND_OP_ADD;
  ba.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  ba.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  ba.alphaBlendOp = VK_BLEND_OP_ADD;
  ba.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  VkPipelineColorBlendStateCreateInfo bci{};
  bci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  bci.attachmentCount = 1;
  bci.pAttachments = &ba;
  if (!m_texPipeLayout) {
    VkPipelineLayoutCreateInfo pli{};
    pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pli.setLayoutCount = 1;
    pli.pSetLayouts = &m_texLayout;
    if (vkCreatePipelineLayout(m_device, &pli, nullptr, &m_texPipeLayout) != VK_SUCCESS) {
      vkDestroyShaderModule(m_device, vsm, nullptr);
      vkDestroyShaderModule(m_device, fsm, nullptr);
      return false;
    }
  }
  VkGraphicsPipelineCreateInfo gpi{};
  gpi.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  gpi.stageCount = 2;
  gpi.pStages = stages;
  gpi.pVertexInputState = &vii;
  gpi.pInputAssemblyState = &iai;
  gpi.pViewportState = &vpi;
  gpi.pRasterizationState = &rsi;
  gpi.pMultisampleState = &msi;
  gpi.pColorBlendState = &bci;
  gpi.layout = m_texPipeLayout;
  gpi.renderPass = pass;
  bool ok = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &gpi,
                                      nullptr, &out) == VK_SUCCESS;
  vkDestroyShaderModule(m_device, vsm, nullptr);
  vkDestroyShaderModule(m_device, fsm, nullptr);
  return ok;
}

bool VulkanRenderer::uploadTexBatch() {
  VkDeviceSize need = (VkDeviceSize)m_texBatch.size() * 6 * sizeof(VkTexVert);
  if (need > m_texCap) {
    if (m_texBuf) vkDestroyBuffer(m_device, m_texBuf, nullptr);
    if (m_texMem) vkFreeMemory(m_device, m_texMem, nullptr);
    m_texBuf = VK_NULL_HANDLE;
    m_texMem = VK_NULL_HANDLE;
    m_texCap = 0;
    if (need) {
      VkPhysicalDeviceMemoryProperties mp{};
      vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &mp);
      VkBufferCreateInfo bci{};
      bci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bci.size = need;
      bci.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
      if (vkCreateBuffer(m_device, &bci, nullptr, &m_texBuf) != VK_SUCCESS)
        return false;
      VkMemoryRequirements mr{};
      vkGetBufferMemoryRequirements(m_device, m_texBuf, &mr);
      uint32_t mi = findMemType(mp, mr.memoryTypeBits,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      if (mi == UINT32_MAX) return false;
      VkMemoryAllocateInfo mai{};
      mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      mai.allocationSize = mr.size;
      mai.memoryTypeIndex = mi;
      if (vkAllocateMemory(m_device, &mai, nullptr, &m_texMem) != VK_SUCCESS)
        return false;
      vkBindBufferMemory(m_device, m_texBuf, m_texMem, 0);
      m_texCap = need;
    }
  }
  if (need) {
    void* dst = nullptr;
    if (vkMapMemory(m_device, m_texMem, 0, need, 0, &dst) != VK_SUCCESS)
      return false;
    VkTexVert* w = (VkTexVert*)dst;
    for (size_t i = 0; i < m_texBatch.size(); i++)
      for (int k = 0; k < 6; k++) *w++ = m_texBatch[i].v[k];
    vkUnmapMemory(m_device, m_texMem);
  }
  return true;
}

void VulkanRenderer::PushTri3D(uint32_t tex, const VkMeshVert v[3]) {
  if (!tex || !v) return;
  MeshTri t;
  t.tex = tex;
  t.v[0] = v[0];
  t.v[1] = v[1];
  t.v[2] = v[2];
  m_meshBatch.push_back(t);
  DrawItem it;
  it.tex = false;
  it.mesh = true;
  it.idx = (uint32_t)(m_meshBatch.size() - 1);
  m_order.push_back(it);
}

void VulkanRenderer::SetViewProj(const float m[16]) {
  if (!m) return;
  memcpy(m_viewProj, m, sizeof(m_viewProj));
}

size_t VulkanRenderer::PendingMeshTris() const { return m_meshBatch.size(); }

bool VulkanRenderer::createMeshPipeline(VkRenderPass pass, uint32_t w,
                                        uint32_t h, VkPipeline& out) {
  out = VK_NULL_HANDLE;
  if (!m_device || !pass || !w || !h) return false;
  const bool hasDepth = (m_depthFormat != VK_FORMAT_UNDEFINED);
  VkShaderModule vsm = VK_NULL_HANDLE, fsm = VK_NULL_HANDLE;
  VkShaderModuleCreateInfo sci{};
  sci.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  sci.codeSize = sizeof(kMesh3dVertSpv);
  sci.pCode = kMesh3dVertSpv;
  if (vkCreateShaderModule(m_device, &sci, nullptr, &vsm) != VK_SUCCESS)
    return false;
  sci.codeSize = sizeof(kMesh3dFragSpv);
  sci.pCode = kMesh3dFragSpv;
  if (vkCreateShaderModule(m_device, &sci, nullptr, &fsm) != VK_SUCCESS) {
    vkDestroyShaderModule(m_device, vsm, nullptr);
    return false;
  }
  if (!ensureTexObjects()) {
    vkDestroyShaderModule(m_device, vsm, nullptr);
    vkDestroyShaderModule(m_device, fsm, nullptr);
    return false;
  }
  VkPipelineShaderStageCreateInfo stages[2]{};
  stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
  stages[0].module = vsm;
  stages[0].pName = "main";
  stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  stages[1].module = fsm;
  stages[1].pName = "main";
  VkVertexInputBindingDescription bind{};
  bind.stride = sizeof(VkMeshVert);
  bind.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  VkVertexInputAttributeDescription attrs[3]{};
  attrs[0].location = 0;
  attrs[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attrs[0].offset = 0;
  attrs[1].location = 1;
  attrs[1].format = VK_FORMAT_R32G32_SFLOAT;
  attrs[1].offset = 12;
  attrs[2].location = 2;
  attrs[2].format = VK_FORMAT_R32G32B32A32_SFLOAT;
  attrs[2].offset = 20;
  VkPipelineVertexInputStateCreateInfo vii{};
  vii.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vii.vertexBindingDescriptionCount = 1;
  vii.pVertexBindingDescriptions = &bind;
  vii.vertexAttributeDescriptionCount = 3;
  vii.pVertexAttributeDescriptions = attrs;
  VkPipelineInputAssemblyStateCreateInfo iai{};
  iai.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  iai.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  VkViewport vp{0, 0, (float)w, (float)h, 0.0f, 1.0f};
  VkRect2D sc{{0, 0}, {w, h}};
  VkPipelineViewportStateCreateInfo vpi{};
  vpi.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  vpi.viewportCount = 1;
  vpi.pViewports = &vp;
  vpi.scissorCount = 1;
  vpi.pScissors = &sc;
  VkPipelineRasterizationStateCreateInfo rsi{};
  rsi.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rsi.polygonMode = VK_POLYGON_MODE_FILL;
  rsi.cullMode = VK_CULL_MODE_NONE;
  rsi.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rsi.lineWidth = 1.0f;
  VkPipelineMultisampleStateCreateInfo msi{};
  msi.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  msi.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  VkPipelineDepthStencilStateCreateInfo dsi{};
  dsi.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  dsi.depthTestEnable = hasDepth ? VK_TRUE : VK_FALSE;
  dsi.depthWriteEnable = hasDepth ? VK_TRUE : VK_FALSE;
  dsi.depthCompareOp = VK_COMPARE_OP_LESS;
  VkPipelineColorBlendAttachmentState ba{};
  ba.blendEnable = VK_TRUE;
  ba.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  ba.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  ba.colorBlendOp = VK_BLEND_OP_ADD;
  ba.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  ba.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  ba.alphaBlendOp = VK_BLEND_OP_ADD;
  ba.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  VkPipelineColorBlendStateCreateInfo bci{};
  bci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  bci.attachmentCount = 1;
  bci.pAttachments = &ba;
  if (!m_meshPipeLayout) {
    VkPushConstantRange pcr{};
    pcr.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pcr.offset = 0;
    pcr.size = sizeof(m_viewProj);
    VkPipelineLayoutCreateInfo pli{};
    pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pli.setLayoutCount = 1;
    pli.pSetLayouts = &m_texLayout;
    pli.pushConstantRangeCount = 1;
    pli.pPushConstantRanges = &pcr;
    if (vkCreatePipelineLayout(m_device, &pli, nullptr, &m_meshPipeLayout) != VK_SUCCESS) {
      vkDestroyShaderModule(m_device, vsm, nullptr);
      vkDestroyShaderModule(m_device, fsm, nullptr);
      return false;
    }
  }
  VkGraphicsPipelineCreateInfo gpi{};
  gpi.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  gpi.stageCount = 2;
  gpi.pStages = stages;
  gpi.pVertexInputState = &vii;
  gpi.pInputAssemblyState = &iai;
  gpi.pViewportState = &vpi;
  gpi.pRasterizationState = &rsi;
  gpi.pMultisampleState = &msi;
  gpi.pDepthStencilState = &dsi;
  gpi.pColorBlendState = &bci;
  gpi.layout = m_meshPipeLayout;
  gpi.renderPass = pass;
  // Subpass 0 carries the depth attachment exactly when hasDepth is true,
  // matching how createRenderPass / InitHeadless built this pass.
  gpi.subpass = 0;
  bool ok = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &gpi,
                                      nullptr, &out) == VK_SUCCESS;
  vkDestroyShaderModule(m_device, vsm, nullptr);
  vkDestroyShaderModule(m_device, fsm, nullptr);
  return ok;
}

bool VulkanRenderer::uploadMeshBatch() {
  VkDeviceSize need = (VkDeviceSize)m_meshBatch.size() * 3 * sizeof(VkMeshVert);
  if (need > m_meshCap) {
    if (m_meshBuf) vkDestroyBuffer(m_device, m_meshBuf, nullptr);
    if (m_meshMem) vkFreeMemory(m_device, m_meshMem, nullptr);
    m_meshBuf = VK_NULL_HANDLE;
    m_meshMem = VK_NULL_HANDLE;
    m_meshCap = 0;
    if (need) {
      VkPhysicalDeviceMemoryProperties mp{};
      vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &mp);
      VkBufferCreateInfo bci{};
      bci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bci.size = need;
      bci.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
      if (vkCreateBuffer(m_device, &bci, nullptr, &m_meshBuf) != VK_SUCCESS)
        return false;
      VkMemoryRequirements mr{};
      vkGetBufferMemoryRequirements(m_device, m_meshBuf, &mr);
      uint32_t mi = findMemType(mp, mr.memoryTypeBits,
                                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      if (mi == UINT32_MAX) return false;
      VkMemoryAllocateInfo mai{};
      mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      mai.allocationSize = mr.size;
      mai.memoryTypeIndex = mi;
      if (vkAllocateMemory(m_device, &mai, nullptr, &m_meshMem) != VK_SUCCESS)
        return false;
      vkBindBufferMemory(m_device, m_meshBuf, m_meshMem, 0);
      m_meshCap = need;
    }
  }
  if (need) {
    void* dst = nullptr;
    if (vkMapMemory(m_device, m_meshMem, 0, need, 0, &dst) != VK_SUCCESS)
      return false;
    VkMeshVert* w = (VkMeshVert*)dst;
    for (size_t i = 0; i < m_meshBatch.size(); i++)
      for (int k = 0; k < 3; k++) *w++ = m_meshBatch[i].v[k];
    vkUnmapMemory(m_device, m_meshMem);
  }
  return true;
}

// Replays flat and textured items in submission order, switching pipelines
// as needed. Vulkan executes draws in command order with no implicit
// cross-pipeline ordering, unlike the GL/DX immediate paths this mirrors.
bool VulkanRenderer::drawOrdered(VkCommandBuffer cmd, VkRenderPass pass) {
  if (m_order.empty()) {
    m_batch.clear();
    m_texBatch.clear();
    m_meshBatch.clear();
    return true;
  }
  if (!m_pipeline) {
    if (pass == m_renderPass && m_renderPass) {
      if (!createWindowPipeline()) return false;
    } else {
      return false; // headless always builds m_pipeline in InitHeadless
    }
  }
  VkPipeline* slot = (pass == m_renderPass && m_renderPass) ? &m_texPipeWin
                                                            : &m_texPipeOff;
  uint32_t ew, eh;
  if (slot == &m_texPipeWin) { ew = m_swapExtent.width; eh = m_swapExtent.height; }
  else { ew = m_offExtent.width; eh = m_offExtent.height; }
  bool needTex = false;
  bool needMesh = false;
  for (size_t i = 0; i < m_order.size(); i++) {
    if (m_order[i].mesh) needMesh = true;
    else if (m_order[i].tex) needTex = true;
  }
  if (needTex) {
    if (!*slot && !createTexPipeline(pass, ew, eh, *slot)) return false;
    if (!uploadTexBatch()) return false;
  }
  VkPipeline* mslot = (pass == m_renderPass && m_renderPass) ? &m_meshPipeWin
                                                             : &m_meshPipeOff;
  if (needMesh) {
    if (!*mslot && !createMeshPipeline(pass, ew, eh, *mslot)) return false;
    if (!uploadMeshBatch()) return false;
  }
  if (!uploadBatch()) return false;
  VkPipeline cur = VK_NULL_HANDLE;
  VkDeviceSize off = 0;
  uint32_t ff = 0, tf = 0, mf = 0;
  for (size_t i = 0; i < m_order.size(); i++) {
    if (m_order[i].mesh) {
      if (cur != *mslot) {
        cur = *mslot;
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, cur);
        vkCmdBindVertexBuffers(cmd, 0, 1, &m_meshBuf, &off);
        vkCmdPushConstants(cmd, m_meshPipeLayout,
                           VK_SHADER_STAGE_VERTEX_BIT, 0,
                           (uint32_t)sizeof(m_viewProj), m_viewProj);
      }
      uint32_t id = (m_order[i].idx < m_meshBatch.size())
                        ? m_meshBatch[m_order[i].idx].tex
                        : 0;
      if (id && id <= m_textures.size()) {
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_meshPipeLayout, 0, 1,
                                &m_textures[id - 1].set, 0, nullptr);
        vkCmdDraw(cmd, 3, 1, mf, 0);
      }
      mf += 3;
    } else if (!m_order[i].tex) {
      if (cur != m_pipeline) {
        cur = m_pipeline;
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, cur);
        vkCmdBindVertexBuffers(cmd, 0, 1, &m_vertBuf, &off);
      }
      vkCmdDraw(cmd, 3, 1, ff, 0);
      ff += 3;
    } else {
      if (cur != *slot) {
        cur = *slot;
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, cur);
        vkCmdBindVertexBuffers(cmd, 0, 1, &m_texBuf, &off);
      }
      uint32_t id = (m_order[i].idx < m_texBatch.size())
                        ? m_texBatch[m_order[i].idx].tex
                        : 0;
      if (id && id <= m_textures.size()) {
        vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_texPipeLayout, 0, 1,
                                &m_textures[id - 1].set, 0, nullptr);
        vkCmdDraw(cmd, 6, 1, tf, 0);
      }
      tf += 6;
    }
  }
  m_batch.clear();
  m_texBatch.clear();
  m_meshBatch.clear();
  m_order.clear();
  return true;
}

#endif
