#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;

void main() {
    color = vec4(0.2, 0.3, 0.8, 1.0);
}
