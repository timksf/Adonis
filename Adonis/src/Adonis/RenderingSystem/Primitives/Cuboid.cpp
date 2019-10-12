#include "pch.h"
#include "Cuboid.h"



namespace Adonis::rendersystem::primitives {

	Cuboid::Cuboid(float height, float width, float depth) : Mesh() {

		float vertices[] = {
				width / 2.0f, -height / 2.0f, depth / 2.0f,		
				width / 2.0f,  height / 2.0f, depth / 2.0f,		
				-width / 2.0f, height / 2.0f,  depth / 2.0f,	
				-width / 2.0f, -height / 2.0f, depth / 2.0f,	
				width / 2.0f,  -height / 2.0f, -depth / 2.0f,	
				width / 2.0f,  height / 2.0f, -depth / 2.0f,	
				-width / 2.0f, -height / 2.0f, -depth / 2.0f,	
				-width / 2.0f, height / 2.0f, -depth / 2.0f,	
		};

		uint32_t indices[] = {

			0,1,4,
			4,1,5,
			0,2,1,
			0,3,2,
			0,4,6,
			0,3,6,
			1,5,2,
			2,5,7,
			2,3,6,
			2,6,7,
			5,6,7,
			5,4,6

		};

		this->set_data(vertices, sizeof(vertices), indices, sizeof(indices));
		this->set_specs(render::VertexArrayDesc::default_only_pos_desc(), render::DrawMode::Triangles, render::DrawMethod::Indexed);
	}


}