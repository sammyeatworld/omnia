set(SOURCES
        Device.cpp
)

if (APPLE)
    list(APPEND SOURCES
            Metal/MTLBuffer.cpp
            Metal/MTLDevice.cpp
            Metal/MTLShader.cpp
            Metal/MTLSwapchain.cpp
            Metal/MTLTexture.cpp
    )
elseif (WIN32)
    list(APPEND SOURCES
            D3D12/DX12Buffer.cpp
            D3D12/DX12Device.cpp
            D3D12/DX12Shader.cpp
            D3D12/DX12Swapchain.cpp
            D3D12/DX12Texture.cpp
    )
endif()

set(THIRD_PARTY_LIBS)

if (LINUX OR WIN32)
    list(APPEND SOURCES
            Vulkan/VkBuffer.cpp
            Vulkan/VkCommandBuffer.cpp
            Vulkan/VkDevice.cpp
            Vulkan/VkPhysicalDevice.cpp
            Vulkan/VkPipeline.cpp
            Vulkan/VkRenderPass.cpp
            Vulkan/VkRenderTarget.cpp
            Vulkan/VkShader.cpp
            Vulkan/VkSwapchain.cpp
            Vulkan/VkTexture.cpp
    )

    find_package(Vulkan REQUIRED)
    find_package(VulkanMemoryAllocator CONFIG REQUIRED)
    list(APPEND THIRD_PARTY_LIBS Vulkan::Vulkan GPUOpen::VulkanMemoryAllocator)

    if (WIN32)
        add_compile_definitions(VK_USE_PLATFORM_WIN32_KHR)
    else()
        add_compile_definitions(VK_USE_PLATFORM_XCB_KHR)
    endif()
endif()

omnia_lib(LibRHI
        SOURCES ${SOURCES}
        LIBS LibUI
        THIRD_PARTY PRIVATE ${THIRD_PARTY_LIBS}
)
