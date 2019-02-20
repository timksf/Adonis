#pragma once

#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "glm/glm.hpp"

namespace Adonis {

	namespace render {

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

			inline RenderDevice(const Color& clear_color) : clear_color(clear_color) {};

			/**
			*	@brief				This function is defined by the different implementations, of which only one will be active
			*						Constructs a renderer
			*	@param clear_color	The color used to clear the framebuffers
			*	@return				A pointer to the constructed object
			*/
			static auto create(const Color& clear_color = { {0.0f, 0.0f, 0.0f, 0.0f} })->std::unique_ptr<RenderDevice>;

			virtual ~RenderDevice() {};

			ON_EVENT_DECL_P_V(PreRenderEvent);
			ON_EVENT_DECL_P_V(RenderEvent);
			ON_EVENT_DECL_P_V(UpdateEvent);
			ON_EVENT_DECL_P_V(WindowResizeEvent);

			virtual auto clear()->void = 0;

			/**
			*	@brief			Change the currently active rendering pipeline
			*	@param pipe		A pointer to the rendering pipeline which should be acitvated
			*/
			virtual auto set_pipeline(std::shared_ptr<RenderPipeline> pipe)->void = 0;

			/**
			*	@return	A string containing name of the rendering device
			*/
			virtual auto renderer()->std::string = 0;

			/**
			*	@return	A string containing the version of the rendering software
			*/
			virtual auto version()->std::string = 0;

			/**
			*	@return	A string containing the version of the shading language
			*/
			virtual auto sl_language_version()->std::string = 0;

			/**
			*	@return	A string containing the name of the vendor of the rendering device
			*/
			virtual auto vendor()->std::string = 0;

			Color clear_color;
		};

		class ADONIS_API Shader {
		public:

			/**
			*	@brief				Loads shader code from a file
			*	@param file_name	The name of the file containing the shader code
			*	@return				A string containing the shader code
			*/
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

			/**
			*	@brief			Construct a shader object with all necessary underlying operations 
			*	@param code		A string containing the shader code
			*/
			Shader(const std::string& code) {};

			/**
			*	@brief	Properly release the underlying object
			*/
			virtual ~Shader() {};

			virtual auto id()->unsigned int = 0;

		};

		class ADONIS_API VertexShader: public Shader{
		public:
			
			/**
			*	@brief			This function is defined by the different implementations, of which only one will be active
			*					Constructs a fragment shader object
			*	@param code		A string containing the shader code
			*	@return			A pointer to the constructed object
			*/
			static auto create(const std::string& code)->std::unique_ptr<VertexShader>;
			VertexShader(const std::string& code) :Shader(code){};
		};

		class ADONIS_API FragmentShader : public Shader {
		public:

			/**
			*	@brief			This function is defined by the different implementations, of which only one will be active
			*					Constructs a vertex shader object
			*	@param code		A string containing the shader code
			*	@return			A pointer to the constructed object
			*/
			static auto create(const std::string& code)->std::unique_ptr<FragmentShader>;
			FragmentShader(const std::string& code) :Shader(code) {};
		};

		class ADONIS_API PipelineParam {
		public:
			virtual ~PipelineParam() {};

			/**
			*	@brief			Set the value of the shader uniform as an int
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_int(int32_t)		->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as an unsigned int
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_uint(uint32_t)		->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a float
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_float(float)		->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a double
			*	@param value	The value which the uniform will be assigned to			
			*/
			virtual auto set_double(double)		->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a boolean
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_bool(bool)			->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a vector of two booleans
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2b(glm::bvec2)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as vector of two integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2i(glm::ivec2)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a vector of two unsigned integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2u(glm::uvec2)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a vector of two floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec2f(glm::fvec2)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a vector of three booleans
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3b(glm::bvec3)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a vector of three integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3i(glm::ivec3)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a vector of htree unsigned integers
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3u(glm::uvec3)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a vector of three floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_vec3f(glm::fvec3)	->void = 0;

			/**
			*	@brief		Set the value of the shader uniform as a 3x3 matrix of floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_mat3f(glm::fmat3)	->void = 0;

			/**
			*	@brief			Set the value of the shader uniform as a 4x4 matrix of floats
			*	@param value	The value which the uniform will be assigned to
			*/
			virtual auto set_mat4f(glm::fmat4)	->void = 0;

			/**
			*	@return	The name of the uniform
			*/
			virtual auto name()->std::string = 0;

		};

		class ADONIS_API RenderPipeline {
		public: 

			/**
			*	@brief					This function is defined by the different implementations, of which only one will be active
			*							Constructs a rednering pipeline from the provided shaders
			*	@param vertex_shader	A pointer to a vertex shader object, which is wrapped around the actual shader code
			*	@param fragment_shader	A pointer to a fragment shader object, which is wrapped around the actual shader code
			*	@return					A pointer to the constructed object
			*/
			static auto create(std::unique_ptr<VertexShader> vertex_shader, std::unique_ptr<FragmentShader> fragment_shader)->std::shared_ptr<RenderPipeline>;

			/**
			*	@brief	This function is defined by the different implementations, of which only one will be active
			*			Provides a default rendering pipeline for 2D rendering
			*	@return A pointer to a pre-defined pipeline
			*/
			static auto test_pipeline_2D()->std::unique_ptr<RenderPipeline>;

			virtual ~RenderPipeline() {};

			/**
			*	@brief	Activate the pipeline
			*/
			virtual auto activate()const ->void = 0;

			/**
			*	@return	A pointer to a pipeline param, which is wrapped around a shader uniform and it's properties
			*/
			virtual auto get_param(const std::string& name)->std::shared_ptr<PipelineParam> = 0;
		private:
		};

		enum class ADONIS_API VertexType: uint32_t {
			FLOAT = 0,
			HALF_FLOAT = 1,
			DOUBLE = 2,
			BYTE = 3,
			UNSIGNED_BYTE = 4,
			SHORT = 5,
			UNSIGNED_SHORT = 6,
			INTEGER = 7,
			UNSIGNED_INTEGER = 8,
			NORMALIZED_BYTE = 9,
			NORMALIZED_UNSIGNED_BYTE = 10,
			NORMALIZED_SHORT = 11,
			NORMALIZED_UNSIGNED_SHORT = 12,
			NORMALIZED_INTEGER = 13,
			NORMALIZED_UNSIGNED_INTEGER = 14
		};

		enum class ADONIS_API BufferBit : uint32_t {
			DYNAMIC_STORAGE = 0,
			MAP_READ,
			MAP_WRITE
		};

		inline BufferBit operator|(BufferBit a, BufferBit b) {
			return static_cast<BufferBit>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
		}

		inline BufferBit operator&(BufferBit a, BufferBit b) {
			return static_cast<BufferBit>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
		}

		inline BufferBit operator^(BufferBit a, BufferBit b) {
			return static_cast<BufferBit>(static_cast<uint32_t>(a) ^ static_cast<uint32_t>(b));
		}

		class ADONIS_API Buffer {
		public:

			static constexpr uint8_t NUMBER_OF_BUFFER_BITS = 3;
			static uint32_t buffer_bit_lookup[NUMBER_OF_BUFFER_BITS];

			virtual ~Buffer() {};
			virtual auto id()->uint32_t = 0;
			virtual auto bind()->void = 0;
			//virtual auto update(int64_t size, const void* data, uint32_t offset)->void = 0;
		};

		class ADONIS_API VertexBuffer: public Buffer {
		public:

			static constexpr uint8_t NUMBER_OF_VERTEX_TYPES = 15;
			static uint32_t vertex_type_mapping[NUMBER_OF_VERTEX_TYPES];

			/**
			*	@brief			This function is defined by the different implementations, of which only omne will be active
			*					A wrapper for an underlying vertex buffer
			*	@param size		The size of the buffer in bytes
			*	@param data		A pointer to the data, which should be used for the initialization of the buffer, may be nullptr
			*	@param flags	A bitfield specifying the intended use of the buffers data storage
			*	@return			A pointer to the constructed object
			*/
			static auto create(int64_t size, const void* data, BufferBit flags)->std::unique_ptr<VertexBuffer>;
		};

		class ADONIS_API IndexBuffer : public Buffer {
		public:

			/**
			*	@brief			This function is defined by the different implementations, of which only omne will be active
			*					A wrapper for an underlying index buffer
			*	@param size		The size of the buffer in bytes
			*	@param data		A pointer to the data, which should be used for the initialization of the buffer, may be nullptr
			*	@param flags	A bitfield specifying the intended use of the buffers data storage
			*/
			static auto create(int64_t size, const void* data, BufferBit flags)->std::unique_ptr<IndexBuffer>;
		};

		class ADONIS_API VertexAttrib {
		public:

			/*
			*	@brief				Create a vertex attribute object
			*	@param index		The index of the attribute in the buffer
			*	@param offset		The distance in bytes from the beginning of a vertex to the attribute
			*	@param type			The type of the attribute, also specifies if the values should be normalized
			*	@param size			The number of values 
			*/
			static auto create(uint32_t index, uint32_t offset, VertexType type, uint32_t size)->std::unique_ptr<VertexAttrib>;

			virtual ~VertexAttrib() {};
		};

		
		class ADONIS_API VertexArrayDesc {
		public:

			static auto create(std::vector<std::unique_ptr<VertexAttrib>>&& attribs, uint32_t baseoffset, uint32_t stride)->std::unique_ptr<VertexArrayDesc>;

			virtual ~VertexArrayDesc() {};

			/**
			*	@return	A reference to the attribute formats, the description consists of
			*/
			virtual auto attribs()->std::vector<std::unique_ptr<VertexAttrib>>& = 0;

			/**
			*	@return	The offset of the actual vertex data relative to the beginning of the buffer
			*/
			virtual auto baseoffset()->uint32_t = 0;

			/**
			*	@return	The number of bytes between the vertex elements
			*/
			virtual auto stride()->uint32_t = 0;
		};

		class ADONIS_API VertexArray {
		public:

			static auto create()->std::unique_ptr<VertexArray>;

			virtual ~VertexArray() {};

			/**
			*	@brief Enable underlying vertex array object 
			*/
			virtual auto bind()->void = 0;

			/**
			*	@brief					Add a buffer to the vertex array object with a corresponding description of the buffer's structure
			*
			*	@param	vbo				the name/id of the vertex buffer object
			*	@param	attrib_desc		the description of the buffer's structure, basically a list of attribute formats
			*/
			virtual auto add_buffer(uint32_t id, std::unique_ptr<VertexArrayDesc> attrib_desc)->bool = 0;

		};


	}

}
