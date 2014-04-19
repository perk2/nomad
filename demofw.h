#ifndef DEMOFW_H_
#define DEMOFW_H_

#include <string>
#include <iostream>
#include <vector>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <fstream>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assert.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Shader{
public:
	Shader();
	Shader(GLenum shader_type, std::string filename);
	/* Reads shader string from file and stores it in shader_string. */
	void load_shader(std::string filename);
	GLuint id;
private:
	std::string shader_string;
	GLenum shader_type;
	bool debug;
	const GLchar* string_test;
};

/*
 * Encapsulates gles program object.
 * Implemented in shader.cpp.
 */
class Program{
public:
	Program();
	Program(std::string vs_filename, std::string fs_filename);
	/* The program handle used by gles. */
	GLuint handle;
private:
	Shader vshader;
	Shader fshader;
};

/*
 * The model class is used to import and render individual 3d models created in
 * e.g. Blender.
 */
class Model{
public:
	Model();
	Model(const std::string& filename);
	void render();
	void print_vertex_data();
	void print_vertex_indices();
	GLuint num_indices;
	glm::mat4 model_matrix;
	glm::mat3 norm_matrix;
	Program program;
private:
	bool import_collada(const std::string& filename);
	bool process_scene(const aiScene* scene);
	void bind_buffers();
	std::vector<GLfloat> vertex_data;
	std::vector<GLfloat> vertex_normals;
	std::vector<GLushort> vertex_indices;
	/* Handles to the vertex buffer and indices buffer. */
	GLuint vbo_handle;
	GLuint vbo_normals_handle;
	GLuint ebo_handle;
};


/*
 * A scene is a collection of models. It also handles camera location and movement.
 */
class Scene{
public:
	Scene();
	/* Renders all the models in the scene. */
	void render();
	std::vector<Model> models;
	/* Handles input from the user, */
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	/*
	 * Static since we only have one camera, and couldn't get the callback
	 * function to work otherwise.
	 */
	void update_vp_matrix();
	glm::vec3 cam_loc;
	glm::vec3 cam_lookat;
	glm::vec3 cam_upvector;
	glm::mat4 view_matrix;
	glm::mat4 proj_matrix;
	glm::mat4 vp_matrix;
	glm::mat4 calc_model_view_matrix(const Model&);
	glm::mat4 calc_mvp_matrix(const Model&);
	glm::vec4 light_pos;
	glm::vec3 diffuse_reflectivity;
	glm::vec3 light_intensity;
private:
	GLuint vao[1];
};




#endif /* DEMOFW_H_ */


