#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "glm/glm.hpp"

namespace Adonis {

	namespace render {

		template<typename T, typename... Args>
		auto create(Args&&... args)->std::unique_ptr<T>;

		struct Color {

			glm::fvec4 data = glm::fvec4(1.0f);
			float& r() {
				return data[0];
			}
			float& g() {
				return data[1];
			}
			float& b() {
				return data[2];
			}
			float& a() {
				return data[3];
			}
		};

		class RenderPipeline;

		class ADONIS_API RenderDevice : public EventListener {
		public:

			RenderDevice(const Color& clear_color) : clear_color(clear_color) {};

			static auto create(const Color& clear_color = { {0.0f, 0.0f, 0.0f, 0.0f} })->std::unique_ptr<RenderDevice>;

			virtual ~RenderDevice() {};

			ON_EVENT_DECL_P_V(PreRenderEvent);
			ON_EVENT_DECL_P_V(RenderEvent);
			ON_EVENT_DECL_P_V(UpdateEvent);

			virtual auto clear()->void = 0;
			virtual auto set_pipeline(std::shared_ptr<RenderPipeline> pipe)->void = 0;

			virtual auto renderer()->std::string = 0;
			virtual auto version()->std::string = 0;
			virtual auto sl_language_version()->std::string = 0;
			virtual auto vendor()->std::string = 0;

			Color clear_color;
		};

		class ADONIS_API Shader {
		public:

			static inline auto load_shader(const std::string& file_name)->std::string {

				std::ifstream src(file_name);

				if (!src.good()) {
					AD_CORE_WARN("Failed to load shader: {0}", file_name);
				}
				else {
					std::stringstream ss;
					ss << src.rdbuf();
					return ss.str();
				}

				return std::string();
			}

			Shader(const std::string& code) {};
			virtual ~Shader() {};

			virtual auto id()->unsigned int = 0;

		};

		class ADONIS_API VertexShader: public Shader{
		public:
			
			static auto create(const std::string& code)->std::unique_ptr<VertexShader>;
			VertexShader(const std::string& code) :Shader(code){};
		};

		class ADONIS_API FragmentShader : public Shader {
		public:
			static auto create(const std::string& code)->std::unique_ptr<FragmentShader>;
			FragmentShader(const std::string& code) :Shader(code) {};
		};

		class ADONIS_API PipelineParam {
		public:
			virtual ~PipelineParam() {};

			virtual auto set_int(int32_t)		->void = 0;
			virtual auto set_uint(uint32_t)		->void = 0;
			virtual auto set_float(float)		->void = 0;
			virtual auto set_double(double)		->void = 0;
			virtual auto set_bool(bool)			->void = 0;
			virtual auto set_vec2b(glm::bvec2)	->void = 0;
			virtual auto set_vec2i(glm::ivec2)	->void = 0;
			virtual auto set_vec2u(glm::uvec2)	->void = 0;
			virtual auto set_vec2f(glm::fvec2)	->void = 0;
			virtual auto set_vec3b(glm::bvec3)	->void = 0;
			virtual auto set_vec3i(glm::ivec3)	->void = 0;
			virtual auto set_vec3u(glm::uvec3)	->void = 0;
			virtual auto set_vec3f(glm::fvec3)	->void = 0;
			virtual auto set_mat3f(glm::fmat3)	->void = 0;
			virtual auto set_mat4f(glm::fmat4)	->void = 0;

			virtual auto name()->std::string = 0;

		};

		class ADONIS_API RenderPipeline {
		public: 
			static auto create(std::unique_ptr<VertexShader>, std::unique_ptr<FragmentShader>)->std::shared_ptr<RenderPipeline>;
			static auto test_pipeline_2D()->std::unique_ptr<RenderPipeline>;

			virtual ~RenderPipeline() {};
			virtual auto activate()const ->void = 0;
			virtual auto get_param(const std::string& name)->std::shared_ptr<PipelineParam> = 0;
		private:
		};

		class ADONIS_API Buffer {
		public:

			virtual ~Buffer() {};

		};

		class ADONIS_API VertexBuffer: public Buffer {
		public:
			static auto create(int64_t size, const void* data, uint32_t flags)->std::unique_ptr<VertexBuffer>;


		};

		class ADONIS_API IndexBuffer : public Buffer {
		public:
			static auto create(int64_t size, const void* data, uint32_t flags)->std::unique_ptr<IndexBuffer>;


		};

		class ADONIS_API VertexAttrib {
		public:
			virtual ~VertexAttrib() {};
		};

		class ADONIS_API VertexArrayDesc {
		public:

			virtual ~VertexArrayDesc() {};

			virtual auto attribs()->std::vector<std::unique_ptr<VertexAttrib>>& = 0;
			virtual auto baseoffset()->uint32_t = 0;
		};

		class ADONIS_API VertexArray {
		public:

			virtual ~VertexArray() {};
			virtual auto add_buffer(uint32_t id, std::shared_ptr<VertexArrayDesc> attrib_desc)->bool = 0;

		};


		template<typename RenderDevice, typename... Args>
		auto create<RenderDevice, Args...>(Args&&... args)->std::unique_ptr<RenderDevice> {
			return RenderDevice::create(std::forward<Args>(args)...);
		}




	}

}
