set(SOURCES
        Device.cpp
        Renderer.cpp

        Vulkan/VkBuffer.cpp
        Vulkan/VkDevice.cpp
        Vulkan/VkShader.cpp
        Vulkan/VkSwapchain.cpp
        Vulkan/VkTexture.cpp

        Metal/MTLBuffer.cpp
        Metal/MTLDevice.cpp
        Metal/MTLShader.cpp
        Metal/MTLSwapchain.cpp
        Metal/MTLTexture.cpp

        D3D12/DX12Buffer.cpp
        D3D12/DX12Device.cpp
        D3D12/DX12Shader.cpp
        D3D12/DX12Swapchain.cpp
        D3D12/DX12Texture.cpp
)

omnia_lib(LibGraphics
        SOURCES ${SOURCES}
        LIBS LibUI
)
