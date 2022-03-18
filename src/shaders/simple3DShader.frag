#version 330 core

in vec3 Color_frag;

out vec4 Color_out;

void main() {
    Color_out = vec4(Color_frag, 1.0);
}