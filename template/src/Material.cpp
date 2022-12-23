// Local includes
#include "Material.h"
#include "Shader.h"
#include "Context.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
    if (m_cube != 0) {
        glDeleteProgram(m_cube);
    }
}

void Material::init() {
	// TODO : Change shader by your
	m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
    m_cube = load_shaders("shaders/unlit/cubemap_vertex.glsl", "shaders/unlit/cubemap_frag.glsl");
    check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};
	m_texture = 0;
    m_normalmap = 0;
    m_metalmap = 0;
    m_metaltexture = 0;
    m_metalnormalmap = 0;
    m_roughmap = 0;
    m_AOmap = 0;
}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind(int shader) {
	check();
    glm::vec3 cam_pos = Context::camera.position;
    glUniform3f( getUniform("cameraPos"), cam_pos[0], cam_pos[1], cam_pos[2] );
    if(shader==0){
        glUseProgram(m_program);
        internalBind();
    }
    else if(shader==1){
        glUseProgram(m_cube);
        internalBindCube();
    }
}

void Material::internalBind() {
	// bind parameters
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	if (m_texture != 0) {
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_texture);
		glUniform1i(getUniform("colorTexture"), 0);
	}
    if (m_normalmap != 0) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_normalmap);
        glUniform1i(getUniform("normalMap"), 1);
    }
    if (m_metalmap != 0) {
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_metalmap);
        glUniform1i(getUniform("metalMap"), 2);
    }
    if (m_metaltexture != 0) {
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, m_metaltexture);
        glUniform1i(getUniform("metalTexture"), 3);
    }
    if (m_metalnormalmap != 0) {
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, m_metalnormalmap);
        glUniform1i(getUniform("metalNormal"), 4);
    }
    if (m_roughmap != 0) {
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, m_roughmap);
        glUniform1i(getUniform("roughMap"), 5);
    }
    if (m_AOmap != 0) {
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_2D, m_AOmap);
        glUniform1i(getUniform("AOmap"), 6);
    }
}

void Material::internalBindCube() {
    // cubes
    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
