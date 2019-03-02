R""(
#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;

out vec3 vert_pos;
out vec3 vert_color;

void main(){

	vert_pos = pos;
	vert_color = color;
	gl_Position = vec4(pos, 1.0);

}

)""