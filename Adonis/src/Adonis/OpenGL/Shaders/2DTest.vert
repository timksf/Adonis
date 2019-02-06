R""(
#version 450

layout(location = 0) in vec3 pos;

out vec3 vert_pos;
uniform int x;

void main(){

	vert_pos = pos;
	gl_Position = vec4(pos, 1.0 + x);
}

)""