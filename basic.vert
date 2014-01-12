#version 130
in vec3 vPosition;
uniform mat4 view_matrix;
void main(){
	vec4 pos4 = vec4(vPosition, 1.0);
	gl_Position = view_matrix * pos4;
}