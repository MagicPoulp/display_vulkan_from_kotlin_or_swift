# Introduction

It is possible to shows a vulkan graphics from Kotlin or Swift, and not just in pure C/C++.

Currently, only the kotlin version works.

# Sources

the android tutorial
https://developer.android.com/ndk/guides/graphics/getting-started

the vulkan code sample
git clone --recursive https://github.com/LunarG/VulkanSamples.git

the idea to use an android SurfaceView
https://stackoverflow.com/questions/45157950/can-we-use-vulkan-with-java-activity-on-android-platform

# Links

main idea to use vulkan from kotlin/java
https://stackoverflow.com/questions/45157950/can-we-use-vulkan-with-java-activity-on-android-platform

an example using jni
https://github.com/android/ndk-samples/tree/android-mk/hello-jni

some ideas about how to compile shaders in android studio
https://developer.android.com/ndk/guides/graphics/shader-compilers
https://developer.android.com/codelabs/android-studio-cmake#0

we do not use the dynamic loading because it reaquires lots of unstable changes
as illustraded below
https://marcelbraghetto.github.io/a-simple-triangle/2019/06/16/part-17/

about the vulkan pipeline
https://vulkan-tutorial.com/Drawing_a_triangle/Presentation/Window_surface

about the concepts of swapchain and command buffers and queues
https://software.intel.com/content/www/us/en/develop/articles/api-without-secrets-introduction-to-vulkan-part-1.html
https://software.intel.com/content/www/us/en/develop/articles/api-without-secrets-introduction-to-vulkan-part-2.html

How to build on mac OS
https://www.appitventures.com/blog/how-to-setup-flutter-on-android-studio
