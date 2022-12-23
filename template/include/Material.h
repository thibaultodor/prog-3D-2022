#ifndef MATERIAL_H_DEFINED
#define MATERIAL_H_DEFINED
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <stdexcept>

#include "Texture.h"

struct Material {
	// Shader program
	GLuint m_program;
    GLuint m_cube;

	// Material parameters
	glm::vec4 m_color;
	GLint m_texture;
    GLint m_normalmap;
    GLint m_metalmap;
    GLint m_roughmap;
    GLint m_AOmap;
    GLint m_metaltexture;
    GLint m_metalnormalmap;
    GLint m_skybox;

    unsigned int cubeVAO;
    unsigned int skyboxVAO;

	inline void check() {
		if (m_program == 0 || m_cube == 0) {
			throw std::runtime_error("Shader program not initialized");
		}
	}

	Material(): m_program(0),m_cube(0) {

	}

	virtual ~Material();

	virtual void init();

	virtual void clear();

	void bind(int shader);

	virtual void internalBind();

	void setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix);

	GLint getAttribute(const std::string& in_attributeName);

	GLint getUniform(const std::string& in_uniformName);

    void internalBindCube();
};

#endif
