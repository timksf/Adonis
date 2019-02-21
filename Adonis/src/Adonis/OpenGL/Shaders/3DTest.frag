R""(

#version 450

in vec3 vert_col;

void main(){

	gl_FragColor = vec4(vert_col, 1.0);

}



)""	