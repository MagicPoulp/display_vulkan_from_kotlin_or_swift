# to run manually:
# c; cmake .. -DANDROID_NDK=$ANDROID_NDK

cmake_minimum_required(VERSION 3.10.2)
# message(WARNING ${ANDROID_NDK})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/utils)

include_directories(${ANDROID_NDK}//sources/android/native_app_glue)

if (ANDROID)
    set(VOLK_STATIC_DEFINES VK_USE_PLATFORM_ANDROID_KHR)
elseif()
endif()

include_directories(volk)

add_library(
        jni_vulkan
        SHARED
        VulkanAppBridge.cpp AndroidGraphicsApplication.cpp GraphicsApplication.cpp utils/util_init.cpp utils/util.cpp)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive")

add_subdirectory(volk)
target_link_libraries(jni_vulkan PRIVATE log android volk)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fpermissive")
