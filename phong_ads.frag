
#version 130 
in vec3 light_intensity;
out vec4 fColor;
void main(){
	fColor = vec4(light_intensity, 1.0);
}