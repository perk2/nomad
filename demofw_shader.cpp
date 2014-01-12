#include "demofw.h"

Shader::Shader(){
	// Do nothing.
}
Shader::Shader(GLenum shader_type, std::string filename){
	debug = true;
	this->shader_type = shader_type;
	load_shader(filename);
	id = glCreateShader(shader_type);
	/* TODO: Can I avoid using this temporary char? While still reading from file using string? */
	string_test = shader_string.data();
	glShaderSource(id, 1, &string_test, NULL);
	glCompileShader(id);
	if(debug){
		char infolog[100];
		glGetShaderInfoLog(id, 1024, NULL, infolog);
		GLint id_cstatus;
		glGetShaderiv(id, GL_COMPILE_STATUS, &id_cstatus);
		if(id_cstatus != GL_TRUE){
			std::cout << "Shader compilation failed with cstatus:"
					<< id_cstatus << std::endl;
			std::cout << "Infolog:" << infolog << std::endl;
		}
		else{
			std::cout << "Shader compilation successful." << std::endl;
		}
	}

}

void Shader::load_shader(std::string filename){
	std::ifstream in(filename);
	if (in){
		in.seekg(0, std::ios::end);
		shader_string.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&shader_string[0], shader_string.size());
		in.close();
	}
	else{
		std::cout << "Could not open shader file." << std::endl;
	}
}

Program::Program(){
	// Do nothing.
}
Program::Program(std::string vs_filename, std::string fs_filename){
		vshader = Shader(GL_VERTEX_SHADER, vs_filename);
		fshader = Shader(GL_FRAGMENT_SHADER, fs_filename);
		handle = glCreateProgram();
		glAttachShader(handle, vshader.id);
		glAttachShader(handle, fshader.id);
		glLinkProgram(handle);
		glUseProgram(handle);
		assert(glGetError()==GL_NO_ERROR);
}
