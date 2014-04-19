/*
 * Phong shader,provides ambient, diffuse and specular lightning per vertex.
 * Taken more or less from the OpenGL4.0 shading language cookbook.
 */
#version 130
in vec3 vertex_position;
in vec3 vertex_normal;

out vec3 light_intensity;


// Light characteristics:
uniform vec4 light_position;
// Diffuse reflectivity
uniform vec3 Kd;
// Light source intensity
uniform vec3 Ld;

// Matrices:
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 proj_matrix;
uniform mat4 mpv_matrix;
// Inverse transpose of the model_matrix.
uniform mat3 norm_matrix;


void main(){

	vec4 pos4 = vec4(vertex_position, 1.0);
	// Transform normals to eye space.
	vec3 tnorm = normalize(norm_matrix * vertex_normal);

	// Transform positions to eye space
	vec4 eye_positions = view_matrix * model_matrix * pos4;
	vec3 s = normalize(vec3(light_position - eye_positions));

	// Diffuse shading equation
	light_intensity = Ld * Kd * max(dot(s, tnorm), 0.0);
	gl_Position = proj_matrix * view_matrix * model_matrix * pos4;
}