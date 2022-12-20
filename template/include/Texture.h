#ifndef TEXTURE_H_DEFINED
#define TEXTURE_H_DEFINED

#include <string>
#include <GL/glew.h>
#include <vector>

GLuint loadTexture2DFromFilePath(const std::string& path);

unsigned int loadCubemap ( std::vector<std::string> faces );

void setDefaultTexture2DParameters(GLuint texture);

#endif
