#pragma once

#include "Adonis/RenderingSystem/Mesh.h"
#include "Adonis/RenderingSystem/Model.h"


namespace Adonis {

	namespace rendersystem {

		namespace primitives {

			class ADONIS_API Icosahedron : public Mesh {
			public:

				Icosahedron(float radius = 1.0f);

			private:

				float m_radius;

				static uint32_t s_n_vertices;
				static uint32_t s_n_indices;
			};


		}


	}

}