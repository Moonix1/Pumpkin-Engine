#pragma once

#include "Core/Window.hpp"
#include "Renderer/RenderingContext.hpp"

#include <GLFW/glfw3.h>

namespace Pumpkin {
    class CWindow : public Window {
    public:
        CWindow(const WindowProps& props);
		virtual ~CWindow();
		
		void OnUpdate() override;
		
		inline unsigned int GetWidth() const override { return m_Data.width; }
		inline unsigned int GetHeight() const override { return m_Data.height; }
		
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.eventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		
		inline virtual void *GetNativeWindow() const override { return m_Window; };
    private:
        virtual void Init(const WindowProps &props);
        virtual void Shutdown();
    private:
        GLFWwindow *m_Window;
        RenderingContext *m_Context;
        
        struct WindowData {
            std::string title;
            unsigned int width, height;
            bool vsync;
            
            EventCallbackFn eventCallback;
        };
        
        WindowData m_Data;
    };
}