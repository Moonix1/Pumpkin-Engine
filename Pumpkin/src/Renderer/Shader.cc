#include "pheader.hpp"
#include "Shader.hpp"

#include <cstdio>
#include <fstream>
#include <sstream>

#include <glad/glad.h>

namespace Pumpkin {
    std::string GetCurrentPath() {
        std::string filePath = __FILE__;
        size_t lastSlashPos = filePath.find_last_of("/\\");
        
        return filePath.substr(0, lastSlashPos + 1);
    }
    
    std::string GetFileContents(std::string path) {
        std::ifstream file(GetCurrentPath() + path);
        if (!file) {
            throw std::runtime_error("Could not find file!");
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        
        return buffer.str();
    }
    
    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        std::string vertexSource = GetFileContents(vertexPath);
        std::string fragmentSource = GetFileContents(fragmentPath);
        
        // Create an empty vertex shader handle
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        
        // Send the vertex shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        const GLchar *source = vertexSource.c_str();
        glShaderSource(vertexShader, 1, &source, 0);
        
        // Compile the vertex shader
        glCompileShader(vertexShader);
        
        GLint isCompiled = 0;
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
            
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(vertexShader);
            
            PUMPKIN_CORE_ERROR("{0}", infoLog.data());
            PUMPKIN_CORE_ASSERT(false, "Vertex shader compilation Failed!");
        }
        
        // Create an empty fragment shader handle
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        
        // Send the fragment shader source code to GL
        // Note that std::string's .c_str is NULL character terminated.
        source = fragmentSource.c_str();
        glShaderSource(fragmentShader, 1, &source, 0);
        
        // Compile the fragment shader
        glCompileShader(fragmentShader);
        
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
            
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the shader anymore.
            glDeleteShader(fragmentShader);
            // Either of them. Don't leak shaders.
            glDeleteShader(vertexShader);
            
            PUMPKIN_CORE_ERROR("{0}", infoLog.data());
            PUMPKIN_CORE_ASSERT(false, "Fragment shader compilation Failed!");
        }
        
        // Vertex and fragment shaders are successfully compiled.
        // Now time to link them together into a program.
        // Get a program object.
        m_RendererID = glCreateProgram();
        GLuint program = m_RendererID;
        
        // Attach our shaders to our program
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        
        // Link our program
        glLinkProgram(program);
        
        // Note the different functions here: glGetProgram* instead of glGetShader*.
        GLint isLinked = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE) {
            GLint maxLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
            
            // The maxLength includes the NULL character
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
            
            // We don't need the program anymore.
            glDeleteProgram(program);
            // Don't leak shaders either.
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            
            PUMPKIN_CORE_ERROR("{0}", infoLog.data());
            PUMPKIN_CORE_ASSERT(false, "Shader link compilation Failed!");
        }
        
        // Always detach shaders after a successful link.
        glDetachShader(program, vertexShader);
        glDetachShader(program, fragmentShader);
    }
    
    Shader::~Shader() {
        glDeleteProgram(m_RendererID);
    }
    
    void Shader::Bind() const {
        glUseProgram(m_RendererID);
    }
    
    void Shader::Unbind() const {
        glUseProgram(0);
    }
    
}