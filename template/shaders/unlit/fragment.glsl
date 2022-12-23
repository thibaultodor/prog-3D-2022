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
uniform sampler2D metalMap;
uniform sampler2D roughMap;
uniform sampler2D metalTexture;
uniform sampler2D metalNormal;
uniform sampler2D AOmap;
uniform vec3 cameraPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

in vec3 textureDir;
uniform samplerCube skybox;

void main() {
    //FragColor = color;

    /*
    // ROUGH
    vec3 N = normalize(o_normalWorld);
    vec3 I = normalize(o_positionWorld - cameraPos);
    vec3 R = reflect(I,N);
    vec4 refle = vec4(texture(skybox,R).rgb, 1.0);

    vec3 NRough = normalize(o_normalWorld);
    vec4 refleRough1 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0]*1.05,cameraPos[1],cameraPos[2])),NRough)).rgb, 1.0);
    vec4 refleRough2 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0],cameraPos[1]*1.05,cameraPos[2])),NRough)).rgb, 1.0);
    vec4 refleRough3 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0],cameraPos[1],cameraPos[2]*1.05)),NRough)).rgb, 1.0);
    vec4 refleRough4 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0]*1.05,cameraPos[1]*1.05,cameraPos[2])),NRough)).rgb, 1.0);
    vec4 refleRough5 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0],cameraPos[1],cameraPos[2])),NRough)).rgb, 1.0);
    vec4 refleRough6 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0]*0.95,cameraPos[1],cameraPos[2])),NRough)).rgb, 1.0);
    vec4 refleRough7 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0],cameraPos[1]*0.95,cameraPos[2])),NRough)).rgb, 1.0);
    vec4 refleRough8 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0],cameraPos[1],cameraPos[2]*0.95)),NRough)).rgb, 1.0);
    vec4 refleRough9 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0]*0.95,cameraPos[1],cameraPos[2])),NRough)).rgb, 1.0);
    vec4 refleRough10 = vec4(texture(skybox,reflect(normalize(o_positionWorld - vec3(cameraPos[0],cameraPos[1]*0.95,cameraPos[2]*0.95)),NRough)).rgb, 1.0);

    vec4 refleRough = refleRough1*0.10 + refleRough2*0.10 + refleRough3*0.10 + refleRough4*0.10 + refleRough5*0.10 + refleRough6*0.10 + refleRough7*0.10 + refleRough8*0.10 + refleRough9*0.10 + refleRough10*0.10;

    vec3 roughColor = texture(roughMap,o_uv0).rgb;

    FragColor = roughColor[0]*refleRough/2 + (1-roughColor[0])*refle;
    */


    // AO
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
    vec4 fcolor = vec4(ambient + diffuse + specular, 1.0);
    vec3 AOcolor = texture(AOmap,o_uv0).rgb;
    FragColor = AOcolor[0]*fcolor + (1-AOcolor[0])*vec4(0,0,0,1);

    //FragColor = fcolor;

    /*
    //METAL
    vec3 N = normalize(o_normalWorld);
    vec3 I = normalize(o_positionWorld - cameraPos);
    vec3 R = reflect(I,N);
    vec4 refle = vec4(texture(skybox,R).rgb, 1.0);
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(metalNormal,o_uv0).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);
    // get diffuse color
    vec3 color = texture(metalTexture,o_uv0).rgb;
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
    vec4 fcolor = vec4(ambient + diffuse + specular, 1.0);


    vec3 metalcolor = texture(metalMap,o_uv0).rgb;
    FragColor = metalcolor[0]*refle + (1-metalcolor[0])*fcolor;
    */


    /*
    // Phong
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
    */


    /*
    // Miroir
    vec3 N = normalize(o_normalWorld);
    vec3 I = normalize(o_positionWorld - cameraPos);
    vec3 R = reflect(I,N);
    FragColor = vec4 ( texture ( skybox, R ).rgb , 1.0);
    */
}
