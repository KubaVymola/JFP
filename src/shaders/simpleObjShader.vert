#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

// out vec3 ourColor;
out vec3 FragPos;
out vec3 Normal;
out float logz;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
   // const float near = 0.001;
   const float far = 12000000.0;
   const float C = 0.1;
   // const float Fcoef = 2.0/log2(far + 1.0);
   const float FC = 1.0/log(far*C + 1);
   
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   
   // Outerra 1: https://outerra.blogspot.com/2013/07/logarithmic-depth-buffer-optimizations.html
   // gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w))*Fcoef - 1.0;

   // Outerra 2: https://outerra.blogspot.com/2012/11/maximizing-depth-buffer-range-and.html
   // gl_Position.z = 2.0*log(gl_Position.w*C + 1)/log(far*C + 1) - 1;
   // gl_Position.z *= gl_Position.w;

   // Outerra 3: https://outerra.blogspot.com/2012/11/maximizing-depth-buffer-range-and.html
   logz = log(gl_Position.w*C + 1)*FC;
   gl_Position.z = (2*logz - 1)*gl_Position.w;

   // N-F: http://tulrich.com/geekstuff/log_depth_buffer.txt
   // gl_Position.z = 2.0*log(gl_Position.w/near)/log(far/near) - 1; 
   // gl_Position.z *= gl_Position.w;

   FragPos = vec3(model * vec4(aPos, 1.0));
   Normal = mat3(transpose(inverse(model))) * aNormal;
   
   // ourColor = aColor;
   // TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}