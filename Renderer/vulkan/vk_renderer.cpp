#include "vk_renderer.h"
#ifdef RENDERER_VULKAN
#include <stdexcept>

VulkanRenderer::VulkanRenderer() {}
VulkanRenderer::~VulkanRenderer() { Shutdown(); }

HRESULT VulkanRenderer::Init(SDL_Window* window) {
  if (!createInstance()) return E_FAIL;
  // SDL crea VkSurface
  if (!SDL_Vulkan_CreateSurface(window, m_instance, &m_surface)) return E_FAIL;
  if (!pickPhysicalDevice()) return E_FAIL;
  if (!createLogicalDevice()) return E_FAIL;
  if (!createSwapchain(window)) return E_FAIL;
  return S_OK;
}

void VulkanRenderer::Shutdown() {
  if (m_device) vkDeviceWaitIdle(m_device);
  if (m_swapchain) vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
  if (m_surface) vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
  if (m_device) vkDestroyDevice(m_device, nullptr);
  if (m_instance) vkDestroyInstance(m_instance, nullptr);
  m_instance = VK_NULL_HANDLE;
  m_device = VK_NULL_HANDLE;
}

HRESULT VulkanRenderer::BeginScene() { return S_OK; }
HRESULT VulkanRenderer::EndScene() { return S_OK; }
HRESULT VulkanRenderer::Clear(uint32_t) { return S_OK; }
HRESULT VulkanRenderer::Present() { return S_OK; }
HRESULT VulkanRenderer::DrawPrimitive(VkPrimitiveTopology, const void*, uint32_t) { return S_OK; }

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
  uint32_t count = 0;
  vkEnumeratePhysicalDevices(m_instance, &count, nullptr);
  if (count == 0) return false;
  std::vector<VkPhysicalDevice> devs(count);
  vkEnumeratePhysicalDevices(m_instance, &count, devs.data());
  m_physicalDevice = devs[0];
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

bool VulkanRenderer::createSwapchain(SDL_Window*) {
  // TODO: elegir surface format/present mode, crear swapchain real
  // Stub para compilar - el siguiente paso es implementar con VMA + renderpass
  return true;
}
#endif
