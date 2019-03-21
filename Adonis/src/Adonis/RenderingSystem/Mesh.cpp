#include "pch.h"
#include "Mesh.h"

namespace Adonis::rendersystem {

	MeshSpecification::MeshSpecification(render::DrawMode mode, render::DrawMethod method) :
		m_draw_mode(mode),
		m_draw_method(method) {

	};

	bool MeshSpecification::operator==(const MeshSpecification& rhs) const {
		return (m_draw_mode == rhs.m_draw_mode) && (m_draw_method == rhs.m_draw_method);
	}


}