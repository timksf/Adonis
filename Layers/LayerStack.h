#pragma once
#include "Adonis/Core.h"
#include "Adonis/Layers/Layer.h"



namespace Adonis {

	class LayerStack {
	public:


		~LayerStack();

		template<typename LayerType, typename... Args>
		inline auto push_layer(Args&& ...args) {
			if (!std::is_base_of_v<Layer, LayerType>) {
				AD_CORE_CRITICAL("{0} is not derived from Layer class", AD_NAME_FROM_TYPE(LayerType));
			}
		};
	


	private:
		std::vector<std::unique_ptr<Layer>> m_layers;
	};

}
