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
private:
	bool import_collada(const std::string& filename);
	bool process_scene(const aiScene* scene);
	void bind_buffers();
	std::vector<GLfloat> vertex_data;
	std::vector<GLushort> vertex_indices;
	/* Handles to the vertex buffer and indices buffer. */
	GLuint vbo_handle;
	GLuint ebo_handle;
};


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
 * A scene is a collection of models. It also handles camera location and movement.
 */
class Scene{
public:
	Scene();
	/* Renders all the models in the scene. */
	void render();
	/* Handles input from the user, */
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	std::vector<Model> models;
	Program program;
	glm::vec3 cam_loc;
	glm::vec3 cam_lookat;
	glm::vec3 cam_upvector;
	glm::mat4 view_matrix;
	glm::mat4 proj_matrix;
	glm::mat4 vp_matrix;
private:
	GLuint vao[1];
};




#endif /* DEMOFW_H_ */


