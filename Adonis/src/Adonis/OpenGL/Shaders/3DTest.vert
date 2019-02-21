R""(

#version 450

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 col;

out vec3 vert_col;

//uniform float scale;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main(){
	
	vert_col = col;
	gl_Position = projection * view * model * vec4(pos, 1.0);

}


)""	