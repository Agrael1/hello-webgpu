/**
 * \file webgpu.h
 * WebGPU/Dawn wrapper.
 */
#pragma once

#ifdef __EMSCRIPTEN__
#include <webgpu/webgpu.h>
#else
#include <dawn/webgpu_cpp.h>
#endif

#include "window.h"

namespace webgpu {
WGPUDevice create(HWND window, WGPUBackendType type = WGPUBackendType_Force32);

WGPUSwapChain createSwapChain(WGPUDevice device);

/**
 * See \c #createSwapChain();
 */
WGPUTextureFormat getSwapChainFormat(WGPUDevice device);
}
