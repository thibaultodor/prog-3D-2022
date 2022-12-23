// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>
// STD includes
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <float.h>
#include <queue>
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
// Local includes
#include "Camera.h"
#include "Shader.h"
#include "Material.h"
#include "Context.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "SceneLoader.h"

#include "Texture.h"


// -------------------------------------------
// OpenGL/GLUT application code.
// -------------------------------------------

static GLint window;

void init() {
	// Context::camera.initPos();
    Context::camera.resize(SCREENWIDTH, SCREENHEIGHT);
	glCullFace (GL_BACK);
	glEnable (GL_CULL_FACE);
	glDepthFunc (GL_LESS);
	glEnable (GL_DEPTH_TEST);
	glClearColor (0.2f, 0.2f, 0.3f, 1.0f);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}


}

void internalBindSkybox(Material * m){
    float cubeVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
    float skyboxVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    // cube VAO
    unsigned int cubeVBO;
    glGenVertexArrays(1, &m->cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindVertexArray(m->cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // skybox VAO
    unsigned int skyboxVBO;
    glGenVertexArrays(1, &m->skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(m->skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void beforeLoop() {
	// Take first instance center as target
	if (!Context::instances.empty()) {
		glm::vec3 centerOfInstance0 = Context::instances.at(0).mesh->center;
		std::cerr << glm::to_string(centerOfInstance0) << std::endl;

		glm::vec3 target(Context::instances.at(0).matrix * glm::vec4(centerOfInstance0, 1.0));
		Context::camera.position = glm::vec3(0.5,0.5,0.5);
		Context::camera.target = target;//lookat
		Context::camera.forward = glm::normalize(target - Context::camera.position);
		Context::camera.view = Context::camera.getViewMatrix();
	}

    for (Material * m:Context::materials) {
        /*
        m->m_texture = loadTexture2DFromFilePath("./data/BoomBox_baseColor.png");
        m->m_normalmap = loadTexture2DFromFilePath("./data/BoomBox_normal.png");
        m->m_metalmap = loadTexture2DFromFilePath("./data/rustediron2_metallic.png");
        m->m_roughmap = loadTexture2DFromFilePath("./data/rustediron2_roughness.png");
        m->m_metaltexture = loadTexture2DFromFilePath("./data/rustediron2_basecolor.png");
        m->m_metalnormalmap = loadTexture2DFromFilePath("./data/rustediron2_normal.png");
        */
        m->m_texture = loadTexture2DFromFilePath("./data/BarramundiFish_baseColor.png");
        //m->m_texture = loadTexture2DFromFilePath("./data/stylized-beast-fur-bl/stylized-beast-fur_albedo.png");
        m->m_normalmap = loadTexture2DFromFilePath("./data/BarramundiFish_normal.png");
        m->m_metalmap = loadTexture2DFromFilePath("./data/stylized-beast-fur-bl/stylized-beast-fur_metallic.png");
        //m->m_roughmap = loadTexture2DFromFilePath("./data/stylized-beast-fur-bl/stylized-beast-fur_roughness.png");
        m->m_roughmap = loadTexture2DFromFilePath("./data/rustediron2_roughness.png");
        m->m_AOmap = loadTexture2DFromFilePath("./data/BarramundiFish_occlusionRoughnessMetallic.png");

        std::vector<std::string> faces
                {
                        "./data/skybox/frozen_bk.png",
                        "./data/skybox/frozen_ft.png",
                        "./data/skybox/frozen_up.png",
                        "./data/skybox/frozen_dn.png",
                        "./data/skybox/frozen_lf.png",
                        "./data/skybox/frozen_rt.png"
                };
        m->m_skybox = loadCubemap(faces);
        internalBindSkybox(m);
    }

    /*
    std::vector<std::string> faces
    {
        "./data/skybox/arctic_bk.png",
        "./data/skybox/arctic_ft.png",
        "./data/skybox/arctic_up.png",
        "./data/skybox/arctic_dn.png",
        "./data/skybox/arctic_lf.png",
        "./data/skybox/arctic_rt.png"
    };
    m->m_skybox = loadCubemap(faces);
    */

    /*
    std::vector<std::string> faces
    {
        "./data/skybox/frozen_bk.png",
        "./data/skybox/frozen_ft.png",
        "./data/skybox/frozen_up.png",
        "./data/skybox/frozen_dn.png",
        "./data/skybox/frozen_lf.png",
        "./data/skybox/frozen_rt.png"
    };
    m->m_skybox = loadCubemap(faces);
    */

    /*
    std::vector<std::string> faces
    {
        "./data/skybox/midnight-silence_bk.png",
        "./data/skybox/midnight-silence_ft.png",
        "./data/skybox/midnight-silence_up.png",
        "./data/skybox/midnight-silence_dn.png",
        "./data/skybox/midnight-silence_lf.png",
        "./data/skybox/midnight-silence_rt.png"
    };
    m->m_skybox = loadCubemap(faces);
    */

    std::vector<std::string> faces
    {
        "./data/skybox/midnight-silence_bk.png",
        "./data/skybox/midnight-silence_ft.png",
        "./data/skybox/midnight-silence_up.png",
        "./data/skybox/midnight-silence_dn.png",
        "./data/skybox/midnight-silence_lf.png",
        "./data/skybox/midnight-silence_rt.png"
    };
}

void draw() {
	if (Context::refreshMatrices) {
		// Context::camera.refreshMatrices();
		Context::camera.view = Context::camera.getViewMatrix();
		Context::camera.projection = Context::camera.getProjectionMatrix();
		Context::refreshMatrices = false;
	}
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < Context::instances.size(); ++i) {
		Instance& inst = Context::instances[i];
		Material* material = inst.material;
		Mesh* mesh = inst.mesh;
		material->bind(0);
		material->setMatrices(Context::camera.projection, Context::camera.view, inst.matrix);
		mesh->draw();
        material->bind(1);
        material->setMatrices(Context::camera.projection, Context::camera.view, inst.matrix);
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw();
	glFlush();
	glutSwapBuffers();
}

int main (int argc, char ** argv) {
	if (argc < 2) {
		std::cerr << "Missing parameter: <path-to-model>" << std::endl;
		exit (EXIT_FAILURE);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
	window = glutCreateWindow("TP");
	init();
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutReshapeFunc(reshape);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	key('?', 0, 0);

	std::string path(argv[1]);
	loadDataWithAssimp(path);
	beforeLoop();

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glutMainLoop();
	clearContext();
	return EXIT_SUCCESS;
}
