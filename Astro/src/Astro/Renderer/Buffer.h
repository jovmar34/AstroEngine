#pragma once

namespace Astro {
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return		sizeof(float);
			case ShaderDataType::Float2:	return 2  * sizeof(float);
			case ShaderDataType::Float3:	return 3  * sizeof(float);
			case ShaderDataType::Float4:	return 4  * sizeof(float);
			case ShaderDataType::Mat3:		return 9  * sizeof(float);
			case ShaderDataType::Mat4:		return 16 * sizeof(float);
			case ShaderDataType::Int:		return		sizeof(float);
			case ShaderDataType::Int2:		return 2  *	sizeof(float);
			case ShaderDataType::Int3:		return 3  * sizeof(float);
			case ShaderDataType::Int4:		return 4  *	sizeof(float);
			case ShaderDataType::Bool:		return		sizeof(bool);
		}
		AS_CORE_ASSERT(false, "Shader DataType not supported");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Offset;
		uint32_t Size;
		ShaderDataType Type;
		bool Normalized;

		BufferElement() {}

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{ 
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 9;	// 3*3
				case ShaderDataType::Mat4:		return 16;	// 4*4
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}
			AS_CORE_ASSERT(false, "Shader DataType not supported");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_elements(elements) 
		{
			CalculateOffsetsAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElement>& GetElements()	{ return m_elements; }

		std::vector<BufferElement>::iterator begin()	{ return m_elements.begin(); }
		std::vector<BufferElement>::iterator end()		{ return m_elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
	private:
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			for (auto& element : m_elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	
	private:
		std::vector<BufferElement> m_elements;
		uint32_t m_Stride = 0;
	};

	
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
}