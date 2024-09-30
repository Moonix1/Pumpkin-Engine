#include "Application.hpp"
#include "Defines.hpp"

#include "Input.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Pumpkin {
    
    Application *Application::s_Instance = nullptr;

    Application::Application() {
        PUMPKIN_CORE_ASSERT(s_Instance, "Application already exists!");
        s_Instance = this;
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(PUMPKIN_BIND_EVENT_FN(Application::OnEvent));
        
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        
        glGenVertexArrays(1, &m_VertexArray);
        glBindVertexArray(m_VertexArray);
        
        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f,
        };
        
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        
        uint32_t indices[3] = { 0, 1, 2 };
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
        
        m_Shader.reset(new Shader("../Shaders/test.vert", "../Shaders/test.frag"));
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
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);
            
            m_Shader->Bind();
            glBindVertexArray(m_VertexArray);
            glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
            
            for (Layer *layer : m_LayerStack)
                layer->OnUpdate();
            
            m_ImGuiLayer->Begin();
            for (Layer *layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();
                
            m_Window->OnUpdate();
        }
    }
}