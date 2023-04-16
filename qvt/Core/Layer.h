#ifndef LAYER_H
#define LAYER_H
#include <string>
#include "Events/Event.h"


namespace qvt
{
    class Layer
    {
        public:
            Layer(const std::string& name = "Layer");
            virtual ~Layer() = default;

            virtual void OnAttach(){}
            virtual void OnDetach(){}
            virtual void OnUpdate(float ts){}
            virtual void OnEvent(Event& event){}
            virtual void OnImGuiRender(){}
            const std::string GetName()const{return m_debugName;}




        protected:
            std::string m_debugName;

    };

}
#endif