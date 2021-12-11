#include <util_init.hpp>
#include "AndroidGraphicsApplication.hpp"
#include "cube_data.h"

AndroidGraphicsApplication::AndroidGraphicsApplication(AAssetManager* assetManager, ANativeWindow* window): GraphicsApplication() {
    mAssetManager = assetManager;
    mWindow = window;

    //info = {};
    char sample_title[] = "android_surface_view_with_vulkan";
    //init_global_layer_properties(info);
    //init_instance(info, sample_title);
    createSurface();
}

AndroidGraphicsApplication::~AndroidGraphicsApplication() {
    //vkDestroyInstance(instance, nullptr);
}

void AndroidGraphicsApplication::createSurface() {
    __android_log_print(ANDROID_LOG_VERBOSE, "AndroidGraphicsApplication", "createSurface");
    sampleMain();
}

// Used to setup shaders.
std::vector<char> AndroidGraphicsApplication::readFile(const std::string& filename) {
    AAsset* file = AAssetManager_open(mAssetManager, filename.c_str(), AASSET_MODE_BUFFER);
    size_t size = AAsset_getLength(file);
    std::vector<char> data(size);
    AAsset_read(file, data.data(), size);
    AAsset_close(file);
    return data;
}

void AndroidGraphicsApplication::setSize(uint32_t w, uint32_t h) {
    width = w;
    height = h;
}

void AndroidGraphicsApplication::drawFrame() {
    // not done yet
    // redraw after SurfaceView.surfaceRedrawNeeded()
}

// from the code samples, the sample 15-draw_cube
// git clone --recursive https://github.com/LunarG/VulkanSamples.git
void AndroidGraphicsApplication::sampleMain() {
    VkResult U_ASSERT_ONLY res;
    struct sample_info info = {};
    char sample_title[] = "Draw Cube";
    const bool depthPresent = true;

    //process_command_line_args(info, argc, argv);
    if (volkInitialize())
    {
        throw std::runtime_error("Failed to initialize volk.");
    }
    init_global_layer_properties(info);
    init_instance_extension_names(info);
    init_device_extension_names(info);
    init_instance(info, sample_title);
    init_enumerate_device(info);
    //init_window_size(info, 500, 500);
    init_window_size_patched(info); // PATCHED
    init_connection(info);
    init_window(info);
    //init_swapchain_extension(info);
    init_swapchain_extension(info, mWindow); // PATCHED
    init_device(info);

    init_command_pool(info);
    init_command_buffer(info);
    execute_begin_command_buffer(info);
    init_device_queue(info);
    init_swap_chain(info);
    init_depth_buffer(info);
    init_uniform_buffer(info);
    init_descriptor_and_pipeline_layouts(info, false);
    init_renderpass(info, depthPresent);
    // we could compile our own shaders, but here we use pre-made data
    // https://vulkan-tutorial.com/Drawing_a_triangle/Graphics_pipeline_basics/Shader_modules
#include "15-draw_cube.vert.h"
#include "15-draw_cube.frag.h"
    VkShaderModuleCreateInfo vert_info = {};
    VkShaderModuleCreateInfo frag_info = {};
    vert_info.sType = frag_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    vert_info.codeSize = sizeof(__draw_cube_vert);
    vert_info.pCode = __draw_cube_vert;
    frag_info.codeSize = sizeof(__draw_cube_frag);
    frag_info.pCode = __draw_cube_frag;
    init_shaders(info, &vert_info, &frag_info);
    init_framebuffers(info, depthPresent);
    init_vertex_buffer(info, g_vb_solid_face_colors_Data, sizeof(g_vb_solid_face_colors_Data),
                       sizeof(g_vb_solid_face_colors_Data[0]), false);
    init_descriptor_pool(info, false);
    init_descriptor_set(info, false);
    init_pipeline_cache(info);
    init_pipeline(info, depthPresent);

    /* VULKAN_KEY_START */

    VkClearValue clear_values[2];
    clear_values[0].color.float32[0] = 0.2f;
    clear_values[0].color.float32[1] = 0.2f;
    clear_values[0].color.float32[2] = 0.2f;
    clear_values[0].color.float32[3] = 0.2f;
    clear_values[1].depthStencil.depth = 1.0f;
    clear_values[1].depthStencil.stencil = 0;

    VkSemaphore imageAcquiredSemaphore;
    VkSemaphoreCreateInfo imageAcquiredSemaphoreCreateInfo;
    imageAcquiredSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    imageAcquiredSemaphoreCreateInfo.pNext = NULL;
    imageAcquiredSemaphoreCreateInfo.flags = 0;

    res = vkCreateSemaphore(info.device, &imageAcquiredSemaphoreCreateInfo, NULL, &imageAcquiredSemaphore);
    assert(res == VK_SUCCESS);

    // Get the index of the next available swapchain image:
    res = vkAcquireNextImageKHR(info.device, info.swap_chain, UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE,
                                &info.current_buffer);
    // TODO: Deal with the VK_SUBOPTIMAL_KHR and VK_ERROR_OUT_OF_DATE_KHR
    // return codes
    assert(res == VK_SUCCESS);

    VkRenderPassBeginInfo rp_begin;
    rp_begin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rp_begin.pNext = NULL;
    rp_begin.renderPass = info.render_pass;
    rp_begin.framebuffer = info.framebuffers[info.current_buffer];
    rp_begin.renderArea.offset.x = 0;
    rp_begin.renderArea.offset.y = 0;
    rp_begin.renderArea.extent.width = info.width;
    rp_begin.renderArea.extent.height = info.height;
    rp_begin.clearValueCount = 2;
    rp_begin.pClearValues = clear_values;

    vkCmdBeginRenderPass(info.cmd, &rp_begin, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline);
    vkCmdBindDescriptorSets(info.cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, info.pipeline_layout, 0, NUM_DESCRIPTOR_SETS,
                            info.desc_set.data(), 0, NULL);

    const VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(info.cmd, 0, 1, &info.vertex_buffer.buf, offsets);

    init_viewports(info);
    init_scissors(info);

    vkCmdDraw(info.cmd, 12 * 3, 1, 0, 0);
    vkCmdEndRenderPass(info.cmd);
    res = vkEndCommandBuffer(info.cmd);
    const VkCommandBuffer cmd_bufs[] = {info.cmd};
    VkFenceCreateInfo fenceInfo;
    VkFence drawFence;
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.pNext = NULL;
    fenceInfo.flags = 0;
    vkCreateFence(info.device, &fenceInfo, NULL, &drawFence);

    VkPipelineStageFlags pipe_stage_flags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit_info[1] = {};
    submit_info[0].pNext = NULL;
    submit_info[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info[0].waitSemaphoreCount = 1;
    submit_info[0].pWaitSemaphores = &imageAcquiredSemaphore;
    submit_info[0].pWaitDstStageMask = &pipe_stage_flags;
    submit_info[0].commandBufferCount = 1;
    submit_info[0].pCommandBuffers = cmd_bufs;
    submit_info[0].signalSemaphoreCount = 0;
    submit_info[0].pSignalSemaphores = NULL;

    /* Queue the command buffer for execution */
    res = vkQueueSubmit(info.graphics_queue, 1, submit_info, drawFence);
    assert(res == VK_SUCCESS);

    /* Now present the image in the window */

    VkPresentInfoKHR present;
    present.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present.pNext = NULL;
    present.swapchainCount = 1;
    present.pSwapchains = &info.swap_chain;
    present.pImageIndices = &info.current_buffer;
    present.pWaitSemaphores = NULL;
    present.waitSemaphoreCount = 0;
    present.pResults = NULL;

    /* Make sure command buffer is finished before presenting */
    do {
        res = vkWaitForFences(info.device, 1, &drawFence, VK_TRUE, FENCE_TIMEOUT);
    } while (res == VK_TIMEOUT);

    assert(res == VK_SUCCESS);
    res = vkQueuePresentKHR(info.present_queue, &present);
    assert(res == VK_SUCCESS);

    wait_seconds(20);
    /* VULKAN_KEY_END */
    if (info.save_images) write_ppm(info, "15-draw_cube");

    vkDestroySemaphore(info.device, imageAcquiredSemaphore, NULL);
    vkDestroyFence(info.device, drawFence, NULL);
    destroy_pipeline(info);
    destroy_pipeline_cache(info);
    destroy_descriptor_pool(info);
    destroy_vertex_buffer(info);
    destroy_framebuffers(info);
    destroy_shaders(info);
    destroy_renderpass(info);
    destroy_descriptor_and_pipeline_layouts(info);
    destroy_uniform_buffer(info);
    destroy_depth_buffer(info);
    destroy_swap_chain(info);
    destroy_command_buffer(info);
    destroy_command_pool(info);
    destroy_device(info);
    destroy_window(info);
    destroy_instance(info);
}

void AndroidGraphicsApplication::init_window_size_patched(struct sample_info &info) {
    int32_t default_width = ANativeWindow_getWidth(mWindow);
    int32_t default_height = ANativeWindow_getHeight(mWindow);
    info.width = default_width;
    info.height = default_height;
}
