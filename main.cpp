#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <iostream>
#include "demofw.h"
#include "assert.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	/* Create scene and add shaders */
    Scene world;
	world.program = Program("/home/per/code/nomad/basic.vert",
							"/home/per/code/nomad/basic.frag");

	/* Add models to the scene. */
    world.models.push_back(Model("/home/per/code/models/cube.dae"));

	/* Check that the vertex position always get location 0. */
	//TODO: Shouldn't be restricted, fix.
	assert(glGetAttribLocation(world.program.handle, "vPosition") == 0);

    glfwSetKeyCallback(window, world.key_callback);

    /* Set global gles settings. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	world.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
