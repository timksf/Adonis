#pragma once
#include "Adonis/Core.h"
#include "Adonis/Layers/Layer.h"



namespace Adonis {

	class LayerStack {
	public:

		LayerStack() {};
		~LayerStack() {
			for (auto& layer : m_layers)
				layer->detach();
		};

		template<typename LayerType, typename... Args>
		inline auto push_layer(Args&& ...args) {
			if (!std::is_base_of_v<Layer, LayerType>) {
				AD_CORE_CRITICAL("{0} is not derived from Layer class", AD_NAME_FROM_TYPE(LayerType));
			}
			m_layers.push_back( std::make_unique<LayerType>(std::forward<Args>(args)...) );
		};



	private:
		std::vector<std::unique_ptr<Layer>> m_layers;
	};

}
