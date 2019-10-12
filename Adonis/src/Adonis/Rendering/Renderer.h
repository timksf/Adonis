#pragma once
#ifndef RENDERER
#define RENDERER


#include "Adonis/Core.h"
#include "Adonis/Eventsystem/Events/Events.h"
#include "glm/glm.hpp"

#define DEFAULT_FRAMEBUFFER 0

namespace Adonis {

	namespace rendersystem {
		class Scene;
	};

	namespace render {

		namespace enums {

			enum class DrawMode : uint32_t {
				Triangles = 0,
				Points,
				Lines,
			};

			enum class DrawMethod : uint32_t {
				Classic = 0,
				Indexed
			};

			enum class TexturePixelFormatSized : uint32_t {
				R3_G3_B2 = 0,
				RGB8,
				RGBA4,
				RGBA8,
				DEPTH16
			};

			enum class TextureParameter : uint32_t {
				WRAP_X = 0,
				WRAP_Y,
				WRAP_Z,
				MIN_FILTER,
				MAG_FILTER
			};

			enum class TextureParamValue : uint32_t {
				FILTER_NEAREST = 0,
				FILTER_LINEAR,
				WRAP_CLAMP_TO_EDGE,
				WRAP_CLAMP_TO_BORDER,
				WRAP_MIRRORED_REPEAT,
				WRAP_REPEAT,
				WRAP_MIRROR_CLAMP_TO_EDGE
			};

			enum class PixelFormat : uint32_t {
				RGB = 0,
				RGBA,
				BGR,
				BGRA,
			};

			enum class PixelDataType : uint32_t {
				SHORT = 0,
				USHORT,
				BYTE,
				UBYTE,
				INT,
				UINT
			};

			enum class ADONIS_API VertexType : uint32_t {
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

			enum class ADONIS_API FramebufferStatus : uint32_t {
				UNDEFINED = 0,
				INCOMPLETE_ATTACHMENT,
				INCOMPLETE_MISSING_ATTACHMENT,
				INCOMPLETE_DRAW_BUFFER,
				INCOMPLETE_READ_BUFFER,
				UNSUPPORTED,
				INCOMPLETE_MULTISAMPLE,
				INCOMPLETE_LAYER_TARGETS,
				COMPLETE,
				UNKNOWN
			};

			enum class ADONIS_API BufferBit : uint32_t {
				DYNAMIC_STORAGE = 1 << 0,
				MAP_READ = 1 << 1,
				MAP_WRITE = 1 << 2
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

			enum class FramebufferTextureAttachment : uint32_t {
				COLOR = 0,
				DEPTH,
				STENCIL
			};
		}

		namespace lookup_tables {

			AD_LOOKUP_TABLE_HEADER_DECL_U32(drawmode, 3);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(sized_pixel_format, 5);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(tex_param, 5);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(tex_param_value, 7);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(pixel_format, 4);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(pixel_datatype, 6);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(texture_attachment_type, 3);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(buffer_bit, 3);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(vertex_type, 15);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(vertex_type_size, 15);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(drawmode_divisor, 3);
			AD_LOOKUP_TABLE_HEADER_DECL_U32(framebuffer_status, 9);

		}

		using namespace enums;
		using namespace lookup_tables;

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

			/*AD_LOOKUP_TABLE_DECL_U32(drawmode, 3);*/

			inline RenderDevice(const Color& clear_color) : clear_color(clear_color) {};

			/**
			*	@brief				This function is defined by the different implementations, of which only one will be active
			*						Constructs a renderer
			*	@param clear_color	The color used to clear the framebuffers
			*	@return				A pointer to the constructed object
			*/
			static auto create(const Color& clear_color = { {0.0f, 0.0f, 0.0f, 0.0f} })->std::unique_ptr<RenderDevice>;

			virtual ~RenderDevice() {};

			AD_ON_EVENT_DECL_P_V(WindowResizeEvent);

			virtual auto clear_color_buffer(uint32_t buffer_index = 0)->void = 0;

			virtual auto clear_depth_buffer(float depth)->void = 0;

			virtual auto draw_triangles(int offset, int count)->void = 0;

			virtual auto aspect_ratio()->float = 0;

			virtual auto set_framebuffer(uint32_t id)->void = 0;

			virtual auto toggle_wireframe()->void = 0;

			virtual auto set_viewport(int x0, int y0, uint32_t width, uint32_t height)->void = 0;

			virtual auto draw(std::shared_ptr<rendersystem::Scene> scene)->void = 0;

			virtual auto draw(DrawMethod method, DrawMode mode, int offset, int count)->void = 0;

			virtual auto last_error()->uint32_t = 0;

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

			float test = -0.1f;

			Color clear_color;
		};	

		class ADONIS_API Texture {
		public:

			virtual auto id()->uint32_t = 0;
			virtual auto resize(int width, int height)->void = 0;
			virtual auto set_param(TextureParameter param, TextureParamValue value)->void = 0;
		};

		class ADONIS_API Texture2D : public Texture{
		public:

			static auto create(int width, int height, const void* data = nullptr, TexturePixelFormatSized fmt = TexturePixelFormatSized::RGB8)->std::unique_ptr<Texture2D>;

			virtual ~Texture2D() = default;

			virtual auto resize(int width, int height)->void = 0;

			virtual auto id()->uint32_t = 0;
			virtual auto set_param(TextureParameter param, TextureParamValue value)->void = 0;
		};

		class ADONIS_API Framebuffer {
		public:

			/*AD_LOOKUP_TABLE_DECL_U32(texture_attachment_type, 3);*/	

			const int MAX_COLOR_ATTACHMENTS;

			static auto create()->std::unique_ptr<Framebuffer>;

			Framebuffer() = delete;

			Framebuffer(int x) : MAX_COLOR_ATTACHMENTS(x) {};

			virtual ~Framebuffer() {};

			virtual auto attach(uint32_t, FramebufferTextureAttachment, int attachment_index = 0)->void = 0;

			virtual auto activate_color_attachment(int = 0)->void = 0;

			virtual auto complete()->bool = 0;

			virtual auto status()->enums::FramebufferStatus = 0;

			virtual auto id()->uint32_t = 0;

			virtual auto max_color_attachments()->int = 0;

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
			static auto test_pipeline_2D()->std::shared_ptr<RenderPipeline>;

			static auto test_pipeline_3D_pos_col()->std::shared_ptr<RenderPipeline>;

			static auto test_pipeline_3D_pos()->std::shared_ptr<RenderPipeline>;

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

		class ADONIS_API Buffer {
		public:

			virtual ~Buffer() {};
			virtual auto id()->uint32_t = 0;
			virtual auto bind()->void = 0;
			//virtual auto update(int64_t size, const void* data, uint32_t offset)->void = 0;
		};

		class ADONIS_API VertexBuffer: public Buffer {
		public:

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

			static auto attrib3float(uint32_t index, uint32_t offset)->std::unique_ptr<VertexAttrib>;

			virtual ~VertexAttrib() {};

			virtual auto index()->uint32_t = 0;
			virtual auto offset()->uint32_t = 0;
			virtual auto type()->VertexType = 0;
			virtual auto api_type()->uint32_t = 0;
			virtual auto size()->uint32_t = 0;
			virtual auto normalized()->bool { return false; };
		};

		
		class ADONIS_API VertexArrayDesc {
		public:

			static auto create(std::vector<std::unique_ptr<VertexAttrib>>&& attribs, uint32_t baseoffset, uint32_t stride)->std::shared_ptr<VertexArrayDesc>;
			
			static auto create_empty()->std::shared_ptr<VertexArrayDesc>;

			/**
			*	@brief A default vertex array description object describing a buffer of the type:
			*	  POS_ATTR	 COL_ATTR
			*	| x | y | z| r | g | b
			*
			*	@return pointer to the vertex array description 
			*/
			static auto default_pos_color_desc()->std::shared_ptr<VertexArrayDesc>;

			static auto default_only_pos_desc()->std::shared_ptr<VertexArrayDesc>;
					   
			virtual ~VertexArrayDesc() {};

			virtual auto add_attrib(std::unique_ptr<VertexAttrib>&& attrib)->void = 0;

			virtual auto add_attrib(VertexType type, uint32_t number, int custom_index = -1, int custom_offset = -1)->void = 0;

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

			virtual auto initialized()->bool = 0;

			virtual auto force_init()-> void = 0;

			virtual auto compare(const std::shared_ptr<VertexArrayDesc> rhs)const->bool = 0;
		};

		class ADONIS_API VertexArray {
		public:

			static auto create(std::shared_ptr<VertexArrayDesc>)->std::unique_ptr<VertexArray>;

			virtual ~VertexArray() {};

			/**
			*	@brief Enable underlying vertex array object 
			*/
			virtual auto use()->void = 0;

			virtual auto id()->uint32_t = 0;

			/**
			*	@brief					Add a buffer to the vertex array object with a corresponding description of the buffer's structure
			*
			*	@param	vbo				the name/id of the vertex buffer object
			*	@param	attrib_desc		the description of the buffer's structure, basically a list of attribute formats
			*/
			//virtual auto add_buffer(uint32_t id, std::unique_ptr<VertexArrayDesc> attrib_desc)->bool = 0;

			virtual auto set_index_buffer(uint32_t idx_buffer_id)->void = 0;

			virtual auto set_vertex_buffer(uint32_t id, uint32_t bindingindex, int32_t custom_baseoffset = -1, int32_t custom_stride = -1)->void = 0;

			virtual auto add_desc(std::shared_ptr<VertexArrayDesc> desc, bool increase_bindingindex = false, bool overwrite_existing_attribs = false)->uint32_t = 0;

			virtual auto desc()->std::shared_ptr<VertexArrayDesc> = 0;

		protected:
			virtual auto add_attrib_to_underlying_obj(uint32_t where)->void = 0;
		};


	}

}
#endif // !RENDERER