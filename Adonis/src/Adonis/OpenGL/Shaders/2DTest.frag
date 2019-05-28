R""(
#version 450

layout(location = 0) out vec4 output_color;

in vec3 vert_pos;
in vec3 vert_color;

void main(){
	
	//output_color = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	output_color = vec4(vert_color, 1.0f);

}

)""