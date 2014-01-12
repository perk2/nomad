#include "demofw.h"

/*
 * File implementing the Scene class.
 */
Scene::Scene(){
    glGenVertexArrays(1,vao);
	glBindVertexArray(vao[0]);
	/* Set default camera settings. */
	cam_loc 	 = glm::vec3(0.0f, 0.0f, 5.5f);
	cam_lookat 	 = glm::vec3(0.0f, 0.0f, 0.0f);
	cam_upvector = glm::vec3(0.0f, 1.0f, 0.0f);
	view_matrix  = glm::lookAt(cam_loc, cam_lookat, cam_upvector);
	proj_matrix  = glm::perspective(60.0f, 4.0f/3.0f, 0.2f, 100.0f);
	vp_matrix = proj_matrix * view_matrix;

}

void Scene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void Scene::render(){
	int view_loc = glGetUniformLocation(program.handle, "view_matrix");

	glUniformMatrix4fv(view_loc, 1, GL_FALSE, &vp_matrix[0][0]);
	/* Iterate through the models and render them. */
	for(Model m : this->models){
		assert(glGetError()==GL_NO_ERROR);
		m.print_vertex_data();
		m.render();
	}
}
