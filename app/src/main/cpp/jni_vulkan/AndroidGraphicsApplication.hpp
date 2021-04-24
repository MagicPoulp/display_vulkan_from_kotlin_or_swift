#ifndef ANDROID_SURFACE_VIEW_WITH_VULKAN_ANDROIDGRAPHICSAPPLICATION_HPP
#define ANDROID_SURFACE_VIEW_WITH_VULKAN_ANDROIDGRAPHICSAPPLICATION_HPP


#include <vector>
#include <string>
#include <android/asset_manager.h>
#include <android/native_window.h>
#include <util_init.hpp>
#define VOLK_IMPLEMENTATION
#include "volk_setup.hpp"
#include "GraphicsApplication.hpp" // Base class shared with iOS/macOS/...

class AndroidGraphicsApplication : public GraphicsApplication {
private:
    AAssetManager* mAssetManager;
    ANativeWindow* mWindow;
    uint32_t width;
    uint32_t height;
    VkInstance instance;
    VkSurfaceKHR surface;
    //struct sample_info info;

public:
    bool isResizeNeeded;

public:
    AndroidGraphicsApplication(AAssetManager* assetManager, ANativeWindow* window);
    ~AndroidGraphicsApplication();
    void createSurface();
    std::vector<char> readFile(const std::string& filename);
    void setSize(uint32_t w, uint32_t h);
    void drawFrame();
    void sampleMain();
    void init_window_size_patched(struct sample_info &info);
};

#endif //ANDROID_SURFACE_VIEW_WITH_VULKAN_ANDROIDGRAPHICSAPPLICATION_HPP
