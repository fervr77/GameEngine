#ifndef PANEL_H
#define PANEL_H

#include "Scene/Camera.h"
#include "glm/glm.hpp"
#include <string>
#include "Scene/Entity.h"



	
namespace qvt
{
    class Entity;
	class Scene;

	class  Panel
	{
	public:
		Panel() = default;
		Panel(Camera* editor,Scene* scene);

		

		void OnImGuiRender();
		Entity GetSelectedEntity() { return m_selectionContext; }
		void SetSelectedEntity(Entity object) { m_selectionContext = object; }
		Entity CopySelectedEntity();
		Entity CopyModel(Entity& original);
		void FocusCameraOnObject();
		void DeleteEntity();

	private:
		bool DrawEntityNode(Entity* object);
		void DrawComponents(Entity* object);
		static glm::vec3 DrawVec3Control(const std::string& name, glm::vec3& value, glm::vec3 resetValue = glm::vec3(0), float columnWidth = 100.0f);
		void FilterHierarchy(std::string name);

	private:
        static Entity m_selectionContext;

		static std::vector<std::string> gameObjectList;
		Camera* m_editorCamera;
		static unsigned int pointNumber;
		std::string hierarchyNodeName;
		bool newMaterialWindow = false;
		std::string newMaterialName;
		unsigned int fpscount = 0;
		float totalTime = 0;
		float fpsaverage;
		Scene* scene;
		


	};

}



#endif

