#ifndef IMGUILAYER_H
#define IMGUILAYER_H
#include "Core/Layer.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"

namespace qvt
{





	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach()override;
		virtual  void OnDetach()override;
		virtual void OnImGuiRender()override;
		virtual void OnEvent(Event& e)override;
		void Begin();
		void End();
		void SetDarkThemeColors();
		void BlockEvents(bool block) { m_blockEvents = block; }

	private:
		float m_Time = 0.0f;
		bool m_blockEvents = true;



	};

}



#endif
