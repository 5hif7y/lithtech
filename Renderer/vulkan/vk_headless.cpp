#include "vk_renderer.h"
#ifdef RENDERER_VULKAN
#include "tri_spv.h"
#include <cstdio>
#include <cstring>

namespace {
uint32_t findMem(const VkPhysicalDeviceMemoryProperties& mp,
                 uint32_t bits, VkMemoryPropertyFlags want) {
    for (uint32_t i = 0; i < mp.memoryTypeCount; i++)
        if ((bits & (1u << i)) &&
            (mp.memoryTypes[i].propertyFlags & want) == want)
            return i;
    return UINT32_MAX;
}
} // namespace

bool VulkanRenderer::InitHeadless(uint32_t w, uint32_t h) {
    Shutdown();
    m_offExtent = {w, h};

    VkApplicationInfo app{};
    app.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app.pApplicationName = "NOLF2 headless";
    app.apiVersion = VK_API_VERSION_1_2;
    VkInstanceCreateInfo ici{};
    ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    ici.pApplicationInfo = &app;
    if (vkCreateInstance(&ici, nullptr, &m_instance) != VK_SUCCESS) return false;

    uint32_t ndev = 0;
    vkEnumeratePhysicalDevices(m_instance, &ndev, nullptr);
    if (!ndev) return false;
    std::vector<VkPhysicalDevice> devs(ndev);
    vkEnumeratePhysicalDevices(m_instance, &ndev, devs.data());
    bool picked = false;
    for (auto d : devs) {
        uint32_t nq = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(d, &nq, nullptr);
        std::vector<VkQueueFamilyProperties> qp(nq);
        vkGetPhysicalDeviceQueueFamilyProperties(d, &nq, qp.data());
        for (uint32_t i = 0; i < nq; i++)
            if (qp[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_physicalDevice = d;
                m_graphicsQueueFamily = i;
                picked = true;
                break;
            }
        if (picked) break;
    }
    if (!picked) return false;
    VkPhysicalDeviceProperties pickedProps{};
    vkGetPhysicalDeviceProperties(m_physicalDevice, &pickedProps);
    printf("HOST: vulkan device=%s vendor=0x%04x device=0x%04x\n",
           pickedProps.deviceName, pickedProps.vendorID, pickedProps.deviceID);

    float prio = 1.0f;
    VkDeviceQueueCreateInfo qci{};
    qci.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    qci.queueFamilyIndex = m_graphicsQueueFamily;
    qci.queueCount = 1;
    qci.pQueuePriorities = &prio;
    VkDeviceCreateInfo dci{};
    dci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    dci.queueCreateInfoCount = 1;
    dci.pQueueCreateInfos = &qci;
    if (vkCreateDevice(m_physicalDevice, &dci, nullptr, &m_device) != VK_SUCCESS)
        return false;
    vkGetDeviceQueue(m_device, m_graphicsQueueFamily, 0, &m_graphicsQueue);

    VkPhysicalDeviceMemoryProperties mp{};
    vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &mp);

    // Offscreen color target (RGBA8, render + copy source).
    VkImageCreateInfo ii{};
    ii.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ii.imageType = VK_IMAGE_TYPE_2D;
    ii.format = VK_FORMAT_R8G8B8A8_UNORM;
    ii.extent = {w, h, 1};
    ii.mipLevels = 1;
    ii.arrayLayers = 1;
    ii.samples = VK_SAMPLE_COUNT_1_BIT;
    ii.tiling = VK_IMAGE_TILING_OPTIMAL;
    ii.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    ii.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    if (vkCreateImage(m_device, &ii, nullptr, &m_offImage) != VK_SUCCESS)
        return false;
    VkMemoryRequirements mr{};
    vkGetImageMemoryRequirements(m_device, m_offImage, &mr);
    uint32_t mi = findMem(mp, mr.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (mi == UINT32_MAX) return false;
    VkMemoryAllocateInfo mai{};
    mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    mai.allocationSize = mr.size;
    mai.memoryTypeIndex = mi;
    if (vkAllocateMemory(m_device, &mai, nullptr, &m_offMemory) != VK_SUCCESS)
        return false;
    vkBindImageMemory(m_device, m_offImage, m_offMemory, 0);

    VkImageViewCreateInfo vi{};
    vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vi.image = m_offImage;
    vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vi.format = VK_FORMAT_R8G8B8A8_UNORM;
    vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    vi.subresourceRange.levelCount = 1;
    vi.subresourceRange.layerCount = 1;
    if (vkCreateImageView(m_device, &vi, nullptr, &m_offView) != VK_SUCCESS)
        return false;

    VkAttachmentDescription att{};
    att.format = VK_FORMAT_R8G8B8A8_UNORM;
    att.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    att.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    att.finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    VkAttachmentReference ref{};
    ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription sub{};
    sub.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    sub.colorAttachmentCount = 1;
    sub.pColorAttachments = &ref;
    VkRenderPassCreateInfo rpi{};
    rpi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpi.attachmentCount = 1;
    rpi.pAttachments = &att;
    rpi.subpassCount = 1;
    rpi.pSubpasses = &sub;
    if (vkCreateRenderPass(m_device, &rpi, nullptr, &m_offPass) != VK_SUCCESS)
        return false;

    VkFramebufferCreateInfo fbi{};
    fbi.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbi.renderPass = m_offPass;
    fbi.attachmentCount = 1;
    fbi.pAttachments = &m_offView;
    fbi.width = w;
    fbi.height = h;
    fbi.layers = 1;
    if (vkCreateFramebuffer(m_device, &fbi, nullptr, &m_offFb) != VK_SUCCESS)
        return false;

    // Pipeline con shaders embebidos.
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
    gpi.renderPass = m_offPass;
    bool ok = vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &gpi,
                                        nullptr, &m_pipeline) == VK_SUCCESS;
    vkDestroyShaderModule(m_device, vsm, nullptr);
    vkDestroyShaderModule(m_device, fsm, nullptr);
    if (!ok) return false;

    // Command pool + staging buffer for readback.
    VkCommandPoolCreateInfo cpi{};
    cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cpi.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    cpi.queueFamilyIndex = m_graphicsQueueFamily;
    if (vkCreateCommandPool(m_device, &cpi, nullptr, &m_cmdPool) != VK_SUCCESS)
        return false;
    VkCommandBufferAllocateInfo cai{};
    cai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cai.commandPool = m_cmdPool;
    cai.commandBufferCount = 1;
    if (vkAllocateCommandBuffers(m_device, &cai, &m_cmdBuffer) != VK_SUCCESS)
        return false;

    VkBufferCreateInfo bci2{};
    bci2.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bci2.size = (VkDeviceSize)w * h * 4;
    bci2.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    bci2.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(m_device, &bci2, nullptr, &m_stageBuf) != VK_SUCCESS)
        return false;
    vkGetBufferMemoryRequirements(m_device, m_stageBuf, &mr);
    mi = findMem(mp, mr.memoryTypeBits,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                 VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if (mi == UINT32_MAX) return false;
    mai.allocationSize = mr.size;
    mai.memoryTypeIndex = mi;
    if (vkAllocateMemory(m_device, &mai, nullptr, &m_stageMem) != VK_SUCCESS)
        return false;
    vkBindBufferMemory(m_device, m_stageBuf, m_stageMem, 0);

    m_headless = true;
    return true;
}

void VulkanRenderer::PushTri(const VkTriVert v[3]) {
    m_batch.push_back(v[0]);
    m_batch.push_back(v[1]);
    m_batch.push_back(v[2]);
    DrawItem it;
    it.tex = false;
    it.idx = (uint32_t)(m_batch.size() / 3 - 1);
    m_order.push_back(it);
}

size_t VulkanRenderer::PendingTris() const { return m_batch.size() / 3; }

bool VulkanRenderer::SnapshotPPM(const char* path) {
    if (!m_headless || !m_device) return false;
    uint32_t w = m_offExtent.width, h = m_offExtent.height;

    VkCommandBufferBeginInfo bi{};
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkResetCommandBuffer(m_cmdBuffer, 0);
    if (vkBeginCommandBuffer(m_cmdBuffer, &bi) != VK_SUCCESS) return false;
    VkClearValue clear = m_clearValue;
    VkRenderPassBeginInfo rp{};
    rp.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp.renderPass = m_offPass;
    rp.framebuffer = m_offFb;
    rp.renderArea.extent = m_offExtent;
    rp.clearValueCount = 1;
    rp.pClearValues = &clear;
    vkCmdBeginRenderPass(m_cmdBuffer, &rp, VK_SUBPASS_CONTENTS_INLINE);
    if (!drawOrdered(m_cmdBuffer, m_offPass)) return false;
    vkCmdEndRenderPass(m_cmdBuffer);
    // Offscreen -> staging copy.
    VkBufferImageCopy cp{};
    cp.imageExtent = {w, h, 1};
    cp.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    cp.imageSubresource.layerCount = 1;
    vkCmdCopyImageToBuffer(m_cmdBuffer, m_offImage,
                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                           m_stageBuf, 1, &cp);
    if (vkEndCommandBuffer(m_cmdBuffer) != VK_SUCCESS) return false;

    VkSubmitInfo si{};
    si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    si.commandBufferCount = 1;
    si.pCommandBuffers = &m_cmdBuffer;
    VkFence fence = VK_NULL_HANDLE;
    VkFenceCreateInfo fi{};
    fi.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    vkCreateFence(m_device, &fi, nullptr, &fence);
    if (vkQueueSubmit(m_graphicsQueue, 1, &si, fence) != VK_SUCCESS) {
        vkDestroyFence(m_device, fence, nullptr);
        return false;
    }
    vkWaitForFences(m_device, 1, &fence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(m_device, fence, nullptr);

    void* px = nullptr;
    vkMapMemory(m_device, m_stageMem, 0, (VkDeviceSize)w * h * 4, 0, &px);
    FILE* f = fopen(path, "wb");
    if (!f) {
        vkUnmapMemory(m_device, m_stageMem);
        return false;
    }
    fprintf(f, "P6\n%u %u\n255\n", w, h);
    const uint8_t* row = (const uint8_t*)px;
    // NDC mapping puts screen-top at framebuffer row 0: no flip needed.
    for (uint32_t y = 0; y < h; y++) {
        const uint8_t* src = row + (size_t)y * w * 4;
        for (uint32_t x = 0; x < w; x++)
            fwrite(src + x * 4, 1, 3, f);
    }
    fclose(f);
    vkUnmapMemory(m_device, m_stageMem);
    m_batch.clear();
    return true;
}

#endif
