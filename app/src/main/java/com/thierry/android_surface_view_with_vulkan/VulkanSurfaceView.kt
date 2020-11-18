package com.thierry.android_surface_view_with_vulkan

import android.content.Context
import android.util.AttributeSet
import android.view.SurfaceHolder
import android.view.SurfaceView

class VulkanSurfaceView: SurfaceView, SurfaceHolder.Callback2 {

    private var vulkanApp = VulkanAppBridge()

    constructor(context: Context): super(context) {
    }

    constructor(context: Context, attrs: AttributeSet): super(context, attrs) {
    }

    constructor(context: Context, attrs: AttributeSet, defStyle: Int): super(context, attrs, defStyle) {
    }

    constructor(context: Context, attrs: AttributeSet, defStyle: Int, defStyleRes: Int): super(context, attrs, defStyle, defStyleRes) {
    }

    init {
        alpha = 1F
        holder.addCallback(this)
    }

    // ...
    // Implementation code similar to one in GLSurfaceView is skipped.
    // See: https://android.googlesource.com/platform/frameworks/base/+/master/opengl/java/android/opengl/GLSurfaceView.java
    // ...

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        vulkanApp.resize(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        vulkanApp.destroy()
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        holder.let { h ->
            vulkanApp.create(h.surface, resources.assets)
        }
    }

    override fun surfaceRedrawNeeded(holder: SurfaceHolder) {
        vulkanApp.draw()
    }
}
