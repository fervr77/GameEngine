#include "Framebuffer.h"
#include "glad/glad.h"
#include "Core/Logger.h"

namespace qvt
{


	Framebuffer::Framebuffer(const FramebufferSpecification& spec) :m_specification(spec)
	{
		Invalidate();
	}
	Framebuffer::~Framebuffer()
	{

		glDeleteFramebuffers(1, &m_rendererID);
		glDeleteTextures(1, &m_colorattachment);
		glDeleteTextures(1, &m_entityIDattachment);
		glDeleteTextures(1, &m_depthattachment);

	}

	void Framebuffer::Invalidate()
	{
		
		if (m_rendererID)
		{
			glDeleteFramebuffers(1, &m_rendererID);
			glDeleteTextures(1, &m_colorattachment);
			glDeleteTextures(1, & m_entityIDattachment);
			glDeleteTextures(1, &m_depthattachment);

		}

		glCreateFramebuffers(1, &m_rendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_colorattachment);
		glBindTexture(GL_TEXTURE_2D, m_colorattachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_specification.width, m_specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorattachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_entityIDattachment);
		glBindTexture(GL_TEXTURE_2D, m_entityIDattachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, m_specification.width, m_specification.height, 0,GL_RED_INTEGER, GL_INT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_entityIDattachment, 0);


		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthattachment);
		glBindTexture(GL_TEXTURE_2D, m_depthattachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_specification.width, m_specification.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthattachment, 0);
		
		GLenum buffers[2] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(2, buffers);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_rendererID);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}
	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{

		m_specification.width = width;
		m_specification.height = height;

		Invalidate();

	}
	int Framebuffer::ReadPixel(int x, int y)
	{
		glReadBuffer(GL_COLOR_ATTACHMENT1);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

}
