#include "aspch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Astro {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;	// 3*3
			case ShaderDataType::Mat4:		return GL_FLOAT;	// 4*4
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}
		AS_CORE_ASSERT(false, "Shader DataType not supported");
		return 0;

	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		AS_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,											// uint index				=> x, in the layout(location = x) in the shader
				element.GetComponentCount(),					// uint size				=> count of types per attribute (eg. 3 float per pos) in bound vertex buffer
				ShaderDataTypeToOpenGLBaseType(element.Type), 	// GLenum type				=> type of each element of attribute (eg. GL_FLOAT in pos)
				element.Normalized ? GL_TRUE : GL_FALSE, 		// GLboolean normalized		=> normalized (not really sure, GL_FALSE for floats)
				layout.GetStride(),								// GLsizei stride			=> where should we start reading the next attribute, 
				(const void*)element.Offset						// const void* pointer		=> where does the component start in the submitted buffer
			);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}