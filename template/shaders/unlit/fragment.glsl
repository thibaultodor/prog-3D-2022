#version 330 core

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec4 FragColor;

uniform vec4 color;
uniform sampler2D colorTexture;
uniform sampler2D normalMap;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 textureDir;
uniform samplerCube skybox;

void main() {
    //FragColor = color;


    // Take from LearnOpenGL //
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(normalMap,o_uv0).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);
    // get diffuse color
    vec3 color = texture(colorTexture,o_uv0).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    // specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec;
    FragColor = vec4(ambient + diffuse + specular, 1.0);


    //float dU = 0.5 * ( (texture(bumpMap,vec2(o_uv0[0]+0.01,o_uv0[1]))[0] - texture(bumpMap,vec2(o_uv0[0]-0.01,o_uv0[1]))[0]) );
    //float dV = 0.5 * ( (texture(bumpMap,vec2(o_uv0[0],o_uv0[1]+0.01))[0] - texture(bumpMap,vec2(o_uv0[0],o_uv0[1]-0.01))[0]) );
    //vec3 dirU = vec3(1,0,dU);
    //vec3 dirV = vec3(0,1,dV);
    //vec4 m = vec4(normalize(dirU*dirV),0);
    //FragColor = texture(colorTexture, o_uv0) * vec4(rgb_normal,0);
    //FragColor = texture(colorTexture, o_uv0);
    //FragColor = vec4(rgb_normal,0);
    // DEBUG: position
    //FragColor = vec4(o_positionWorld, 1.0);
    // DEBUG: normal
    //FragColor = vec4(0.5 * o_normalWorld + vec3(0.5) , 1.0);
    // DEBUG: uv0
    //FragColor = vec4(o_uv0, 1.0);
}
