#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 tangent;
layout(location = 3) in vec2 uv0;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 o_positionWorld;
out vec3 o_normalWorld;
out vec2 o_uv0;

out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;


void main() {
  mat3 normalMatrix = mat3(transpose(inverse(model)));
  o_uv0[0] = uv0[0];
  // V a besoin d'etre inversé
  o_uv0[1] = 1-uv0[1];
  vec4 positionWorld = model * vec4(position, 1.0);
  o_positionWorld = positionWorld.xyz;
  o_normalWorld = normalMatrix * normal;
  gl_Position = projection * view * positionWorld;

  // Take from LearnOpenGL //
  vec3 lightPos = vec3(-3,1,5);
  vec3 T = normalize(normalMatrix * tangent);
  vec3 N = normalize(normalMatrix * normal);
  T = normalize(T - dot(T, N) * N);
  vec3 B = cross(N, T);
  mat3 TBN = transpose(mat3(T, B, N));
  TangentLightPos = TBN * lightPos;
  TangentViewPos = TBN * position;
  TangentFragPos = TBN * position;


  //vec3 vertexNormal_cameraspace = normalMatrix * normalize(normal);
  //vec3 vertexTangent_cameraspace = normalMatrix * normalize(tangent);
}
