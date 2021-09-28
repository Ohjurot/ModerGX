#pragma once

#include <ModernGX.h>
#include <ModernGX/Util/EasyHWND.h>
#include <ModernGX/Foundation/HRException.h>
#include <ModernGX/Core/GPU/GPUResource.h>
#include <ModernGX/Core/GPU/GPUDescriptorHeap.h>

#include <exception>

namespace MGX::Core {
    // Window class
    class Window : private EasyHWND::Window 
    {
        public:
            // Construct
            Window() = delete;
            Window(const Window&) = delete;
            Window(LPCWSTR title, ID3D12Device* ptrDevice, ID3D12CommandQueue* ptrCommandQueue, GPU::DescriptorRange rtvRange, bool borderless = false, bool trippleBuffering = false);

            // Destructor
            ~Window();

            // Window functions
            bool ProcessWindowEvents() noexcept;

            // Swap chain sizing
            bool NeedsResizing() noexcept;
            void ResizeNow(ID3D12Device* ptrDevice) noexcept;

            // Present window
            void Present(bool vsync = false) noexcept;

            // Buffers
            unsigned int GetCurrentBufferIndex() noexcept;
            GPU::Resource* GetBuffer(unsigned int idx) noexcept;
            inline unsigned int GetBufferCount() const noexcept 
            {
                return m_bufferCount;
            }

            // Rtv Handle
            D3D12_CPU_DESCRIPTOR_HANDLE GetRtvCpuHanlde(unsigned int idx) noexcept;

        private:
            // Buffer release and construct
            void __getBuffers(ID3D12Device* ptrDevice);
            void __releaseBuffers() noexcept;

        protected:
            // Window message handler
            virtual bool handleWindowMessage(LRESULT* ptrLRESULT, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;

        private:
            // Window class
            static EasyHWND::WindowClass s_wndCls;

            // Swap chain pointer
            ComPointer<IDXGISwapChain1> m_ptrSwapChain;

            // Buffers
            GPU::Resource m_ptrBuffers[3];
            unsigned int m_bufferIndex = 0;
            const unsigned int m_bufferCount;

            // RTV Descriptors (Size >= 3)
            GPU::DescriptorRange m_rtvRange;

            // Size
            UINT m_width = 0;
            UINT m_height = 0;
            bool m_needsResize = false;
    };
}