#include "pch.h"
#include "Cuboid.h"



namespace Adonis::rendersystem::primitives {

	Cuboid::Cuboid(float height, float width, float depth) : Mesh() {

		float vertices[] = {
				-width / 2.0f, -height / 2.0f, depth / 2.0f,	1.0f, 1.0f, 1.0f,
				 width / 2.0f, -height / 2.0f, depth / 2.0f,	1.0f, 1.0f, 1.0f,
				 width / 2.0f,  height / 2.0f, depth / 2.0f,	1.0f, 1.0f, 1.0f,
				 -width / 2.0f, height / 2.0f,  depth / 2.0f,	1.0f, 1.0f, 1.0f,
				 width / 2.0f,  -height / 2.0f, -depth / 2.0f,	1.0f, 1.0f, 1.0f,
				 width / 2.0f,  height / 2.0f, -depth / 2.0f,	1.0f, 1.0f, 1.0f,
				-width / 2.0f, -height / 2.0f, -depth / 2.0f,	1.0f, 1.0f, 1.0f,
				 -width / 2.0f, height / 2.0f, -depth / 2.0f,	1.0f, 1.0f, 1.0f
		};

		uint32_t indices[] = {
			0u,1u,2u,0u,2u,3u,1u,4u,5u,1u,5u,2u,6u,0u,3u,6u,3u,7u,6u,4u,5u,6u,5u,7u,3u,2u,5u,3u,5u,7u,0u,1u,4u,0u,4u,6u 
		};

		this->set_data(vertices, sizeof(vertices), indices, sizeof(indices));
		this->set_specs(render::VertexArrayDesc::standard_pos_color_desc(), render::DrawMode::Triangles, render::DrawMethod::Indexed);
	}


}