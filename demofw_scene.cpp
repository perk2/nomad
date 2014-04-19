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
	/* Set default light source */
	light_pos = glm::vec4(0.0f, 0.0f, 6.0f, 1.0f);
	// TODO: WHat does these values mean physically?
	diffuse_reflectivity = glm::vec3(1.0f, 0.0f, 0.0f);
	light_intensity = glm::vec3(1.0f, 1.0f, 1.0f);

}

void Scene::update_vp_matrix(){
	view_matrix  = glm::lookAt(cam_loc, cam_lookat, cam_upvector);
	proj_matrix  = glm::perspective(50.0f, 4.0f/3.0f, 0.5f, 20.0f);
	vp_matrix = proj_matrix * view_matrix;
}

glm::mat4 Scene::calc_model_view_matrix(const Model& m){
	return view_matrix * m.model_matrix;

}

glm::mat4 Scene::calc_mvp_matrix(const Model& m){
	return (proj_matrix * view_matrix * m.model_matrix);
}

/*
 * Lets the user control the camera in the scene.
 * FPS-style controls: WASD for strafing, Arrowkeys or mouse looks around.
 */
void Scene::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if(key == GLFW_KEY_W){
		cam_loc.z -=1.0f;
	}
	else if(key == GLFW_KEY_S){
		cam_loc.z +=1.0f;
	}
	else if(key == GLFW_KEY_A){
		cam_loc.x -=1.0f;
		cam_lookat.x -=1.0f;

	}
	else if(key == GLFW_KEY_D){
		cam_loc.x +=1.0f;
		cam_lookat.x +=1.0f;
	}
	else if(key == GLFW_KEY_UP){
		cam_loc.y += 1.0f;
	}
	else if(key == GLFW_KEY_DOWN){
		cam_loc.y -= 1.0f;
	}
	update_vp_matrix();
}

void Scene::render(){
	GLuint view_matrix_loc;
	GLuint model_matrix_loc;
	GLuint proj_matrix_loc;
	GLuint mvp_matrix_loc;
	GLuint norm_matrix_loc;
	glm::mat4 mvp_matrix;

	GLuint light_pos_loc;
	GLuint diffuse_reflectivity_loc;
	GLuint light_intensity_loc;
	/* Iterate through the models and render them. */
	for(Model m : this->models){
		view_matrix_loc = glGetUniformLocation(m.program.handle, "view_matrix");
		glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, &view_matrix[0][0]);

		model_matrix_loc = glGetUniformLocation(m.program.handle, "model_matrix");
		glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &(m.model_matrix[0][0]));

		proj_matrix_loc = glGetUniformLocation(m.program.handle, "proj_matrix");
		glUniformMatrix4fv(proj_matrix_loc, 1, GL_FALSE, &(proj_matrix[0][0]));

		mvp_matrix = calc_mvp_matrix(m);
		mvp_matrix_loc = glGetUniformLocation(m.program.handle, "mvp_matrix");
		glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, &(mvp_matrix[0][0]));

		norm_matrix_loc = glGetUniformLocation(m.program.handle, "norm_matrix");
		glUniformMatrix3fv(norm_matrix_loc, 1, GL_FALSE, &(m.norm_matrix[0][0]));


		light_pos_loc = glGetUniformLocation(m.program.handle, "light_position");
		glUniform4fv(light_pos_loc, 1, &(light_pos[0]));

		diffuse_reflectivity_loc = glGetUniformLocation(m.program.handle,
														"Kd");
		glUniform3fv(diffuse_reflectivity_loc, 1, &(diffuse_reflectivity[0]));

		light_intensity_loc = glGetUniformLocation(m.program.handle, "Ld");
		glUniform3fv(light_intensity_loc, 1, &(light_intensity[0]));
		//m.print_vertex_data();
		m.render();
	}
}
