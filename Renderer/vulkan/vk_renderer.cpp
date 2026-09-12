#include "vk_renderer.h"
#ifdef RENDERER_VULKAN
#include "Platform/security.h"
#include <algorithm>
#include <limits>
#include <stdexcept>

VulkanRenderer::VulkanRenderer() {
  m_clearValue.color.float32[0] = 0.1f;
  m_clearValue.color.float32[1] = 0.1f;
  m_clearValue.color.float32[2] = 0.2f;
  m_clearValue.color.float32[3] = 1.0f;
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
  if (m_offFb) { vkDestroyFramebuffer(m_device, m_offFb, nullptr); m_offFb = VK_NULL_HANDLE; }
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
  rp.clearValueCount = 1;
  rp.pClearValues = &m_clearValue;
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
        return true;
      }
    }
  }
  // fallback: primer device si ninguno anuncia present (p.e. headless)
  if (!lith_validate_ptr(devs[0])) return false;
  m_physicalDevice = devs[0];
  m_graphicsQueueFamily = 0;
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
    int w = 0, h = 0;
    SDL_Vulkan_GetDrawableSize(window, &w, &h);
    if (w==0 || h==0) SDL_GetWindowSize(window, &w, &h);
    if (w==0) w = 800;
    if (h==0) h = 600;
    extent.width  = std::clamp<uint32_t>((uint32_t)w, caps.minImageExtent.width, caps.maxImageExtent.width);
    extent.height = std::clamp<uint32_t>((uint32_t)h, caps.minImageExtent.height, caps.maxImageExtent.height);
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
  VkAttachmentDescription att{};
  att.format = m_swapFormat;
  att.samples = VK_SAMPLE_COUNT_1_BIT;
  att.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  att.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  att.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  att.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference ref{};
  ref.attachment = 0;
  ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription sub{};
  sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  sub.colorAttachmentCount = 1;
  sub.pColorAttachments = &ref;

  VkSubpassDependency dep{};
  dep.srcSubpass = VK_SUBPASS_EXTERNAL;
  dep.dstSubpass = 0;
  dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dep.srcAccessMask = 0;
  dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo ci{};
  ci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  ci.attachmentCount = 1;
  ci.pAttachments = &att;
  ci.subpassCount = 1;
  ci.pSubpasses = &sub;
  ci.dependencyCount = 1;
  ci.pDependencies = &dep;

  return vkCreateRenderPass(m_device, &ci, nullptr, &m_renderPass) == VK_SUCCESS;
}

bool VulkanRenderer::createFramebuffers() {
  m_framebuffers.resize(m_swapViews.size());
  for (size_t i=0;i<m_swapViews.size();++i) {
    VkFramebufferCreateInfo ci{};
    ci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    ci.renderPass = m_renderPass;
    ci.attachmentCount = 1;
    ci.pAttachments = &m_swapViews[i];
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

#endif
