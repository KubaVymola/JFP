#version 330 core

layout (location = 0) in vec3 Pos_vert;
layout (location = 1) in vec3 Color_vert;

out vec3 Color_frag;

uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * vec4(Pos_vert, 1.0);
    Color_frag = Color_vert;
}