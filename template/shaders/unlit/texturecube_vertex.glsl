# version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main (){
    mat3 normalMatrix = mat3 (transpose (inverse (model)));
    Normal = normalMatrix * aNormal;// world normal
    Position = vec3 (model * vec4 (aPos, 1.0));// world Position
    gl_Position = projection * view * vec4 (Position, 1.0);
}