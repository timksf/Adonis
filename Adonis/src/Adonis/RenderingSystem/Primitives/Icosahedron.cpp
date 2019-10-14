#include "pch.h"

#include "Icosahedron.h"
#include "Adonis/Math/Math.h"


namespace Adonis::rendersystem::primitives {

	uint32_t Icosahedron::s_n_indices = 60u;
	uint32_t Icosahedron::s_n_vertices = 36u;

	Icosahedron::Icosahedron(float radius): m_radius(radius) {

		using namespace math;
		using namespace math::constants;

		std::vector<float> vertices(36);

		vertices[0] = 0.0f;
		vertices[1] = m_radius;
		vertices[2] = 0.0f;

		vertices[11 * 3] = 0.0f;
		vertices[11 * 3 + 1] = -m_radius;
		vertices[11 * 3 + 2] = 0.0f;

		static float vertical_angle = atanf(1.0f / 2.0f);
		float h_angle = 0.0f;

		for (size_t i = 1; i <= 5; i++) {

			float xz, y;
			xz = m_radius * cosf(vertical_angle);
			y = m_radius * sinf(vertical_angle);

			//second row
			vertices[i * 3ULL] = xz * cos(h_angle);
			vertices[i * 3ULL + 1] = y;
			vertices[i * 3ULL + 2u] = xz * sin(h_angle);

			//third row
			vertices[(i + 5ULL) * 3ULL] = xz * cos(h_angle + ((-(PI_F / 180.0f * 72.0f)) / 2.0f)	);
			vertices[(i + 5ULL) * 3ULL + 1u] = -y;
			vertices[(i + 5ULL) * 3ULL + 2u] = xz * sin(h_angle + ((-(PI_F / 180.0f * 72.0f)) / 2.0f));

			h_angle -= (PI_F / 180.0f * 72.0f);
		}

		uint32_t indices[] = {

			0u, 1u, 2u,
			0u, 2u, 3u,
			0u, 3u, 4u,
			0u, 4u, 5u,
			0u, 5u, 1u,
			11u, 10u, 9u,
			11u, 9u, 8u,
			11u, 8u, 7u,
			11u, 7u, 6u,
			11u, 6u, 10u,
			1u, 6u, 2u,
			6u, 7u, 2u,
			7u, 3u, 2u,
			7u, 8u, 3u,
			8u, 4u, 3u,
			8u, 9u, 4u,
			9u, 5u, 4u,
			9u, 10u, 5u,
			10u, 1u, 5u,
			10u, 6u, 1u

		};

		this->set_data(vertices.data(), s_n_vertices * 4u, indices, s_n_indices * 4u);
		this->set_specs(render::VertexArrayDesc::default_only_pos_desc(), render::DrawMode::Triangles, render::DrawMethod::Indexed);

	}


}