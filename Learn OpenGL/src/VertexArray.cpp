#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCheckError(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCheckError(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vbLayout)
{
	Bind();
	vertexBuffer.Bind();

	const auto& elements = vbLayout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];

		GLCheckError(glEnableVertexAttribArray(i));
		GLCheckError(glVertexAttribPointer(i, element.count, element.type, element.normalized, vbLayout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCheckError(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCheckError(glBindVertexArray(0));
}
