#include "Application.hpp"
#include "Defines.hpp"

#include <glad/glad.h>

#include "Input.hpp"

namespace Pumpkin {
    
    Application *Application::s_Instance = nullptr;

    Application::Application() {
        PUMPKIN_CORE_ASSERT(s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(PUMPKIN_BIND_EVENT_FN(Application::OnEvent));
    }
    
    Application::~Application() {
        
    }
    
    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>(PUMPKIN_BIND_EVENT_FN(Application::OnWindowClose));
        
        for (auto it = m_LayerStack.end() ; it != m_LayerStack.begin();) {
            (*--it)->OnEvent(event);
            if (event.handled)
                break;
        }
    }
    
    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }
    
    void Application::PushOverlay(Layer *overlay) {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }
    
    bool Application::OnWindowClose(WindowCloseEvent &event) {
        m_Running = false;
        return true;
    }
    
    void Application::Run() {
        while (m_Running) {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            
            for (Layer *layer : m_LayerStack)
                layer->OnUpdate();
                
            m_Window->OnUpdate();
        }
    }
}