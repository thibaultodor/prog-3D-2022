# version 330 core

out vec4 FragColor;
in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main (){
    vec3 I = cross(cameraPos,Position);// camera to position
    vec3 R = reflect(cameraPos,I);
    FragColor = vec4(texture(skybox,R).rgb,1.0);
}