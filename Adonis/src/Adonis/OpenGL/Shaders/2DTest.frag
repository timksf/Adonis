R""(
#version 450

in vec3 vert_pos;
in vec3 vert_color;

void main(){

	gl_FragColor = vec4(vert_color, 1.0);

}

)""