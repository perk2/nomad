#version 130
in vec3 vertex_position;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
void main(){
	vec4 pos4 = vec4(vertex_position, 1.0);
	gl_Position = view_matrix * model_matrix * pos4;
}