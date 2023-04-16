#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H
#include <cstdint>


namespace qvt
{
	struct FramebufferSpecification
	{
		uint32_t width;
		uint32_t height;

	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferSpecification& spec);
		virtual ~Framebuffer();
		void Invalidate();
		virtual void Bind();
		virtual void Unbind();
		virtual const FramebufferSpecification& GetSpecification()const { return m_specification; }
		virtual uint32_t GetColorAttachmentRendererID(){ return m_colorattachment; }
		virtual uint32_t GetEntityIDAttachmentRendererID() { return m_entityIDattachment; }
		virtual void Resize(uint32_t width, uint32_t height);
		virtual int ReadPixel(int x, int y);
		static Framebuffer* Create(const FramebufferSpecification& spec) { return new Framebuffer(spec); }

	private:
		FramebufferSpecification m_specification;
		uint32_t m_rendererID = 0;
		uint32_t m_colorattachment = 0;
		uint32_t m_entityIDattachment = 0;
		uint32_t m_depthattachment = 0;


	};

}

#endif

