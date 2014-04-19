#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <iostream>
#include "demofw.h"
#include "assert.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
 * This ugly wrapper is required since the glfwsetkeycallback requires a void
 * function pointer,
 * I cant pass a member function to this function(if not static), so
 * use this hack instead.
 */

Scene * callback_scene;
void key_callback_wrapper(GLFWwindow* window, int key, int scancode, int action, int mods){
	callback_scene->key_callback(window, key, scancode, action, mods);
}


int main(void){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window){
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	/* Create scene and add shaders */
    Scene world;

	/* Add models to the scene. */
    // TODO: How to easily change models and add more scenes here??
    // Need to design...
    Model cube = Model("/home/per/code/models/cube.dae");
	cube.program = Program("/home/per/code/nomad/phong_ads.vert",
							"/home/per/code/nomad/phong_ads.frag");
	// TODO: Model and normal matrices should be set by the model itself.
	cube.model_matrix = glm::translate(cube.model_matrix, glm::vec3(1.0f, 2.0f, 1.0f));
	// Cast
	cube.norm_matrix = glm::mat3(cube.model_matrix);
	cube.norm_matrix = glm::transpose(glm::inverse(cube.norm_matrix));
    world.models.push_back(cube);

	Model gulv = Model("/home/per/code/models/floor.dae");
	gulv.program = Program("/home/per/code/nomad/phong_ads.vert",
							"/home/per/code/nomad/phong_ads.frag");
	glm::vec3 xnormal = glm::vec3(1.0f, 0.0f, 0.0f);
	gulv.model_matrix = glm::rotate(gulv.model_matrix, 80.0f, xnormal);
	gulv.norm_matrix = glm::mat3(gulv.model_matrix);
	gulv.norm_matrix = glm::transpose(glm::inverse(gulv.norm_matrix));

	world.models.push_back(gulv);


	callback_scene = &world;
    glfwSetKeyCallback(window, &key_callback_wrapper);

    /* Set global gles settings. */
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

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
