#include "Panel.h"
#include <string>
#include "imgui.h"
#include "imgui_internal.h"
#include "Renderer/MeshLibrary.h"
#include "Systems/RendererSystem.h"
#include "Systems/ScriptSystem.h"
#include "Renderer/TextureLibrary.h"
#include "Renderer/MeshLibrary.h"
#include "Game/GameLayer.h"
#include "Scene/ScriptableEntity.h"
#include "Components/Components.h"
#include "Renderer/MaterialLibrary.h"
#include <unordered_map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Scene/Scene.h"

#include <string.h>
#include <iostream>
#include <implot.h>
#include <implot_internal.h>
#include "GameObjects/Cube.h"
#include "GameObjects/LineSegment.h"
#include "GameObjects/HoverBike.h"
#include "GameObjects/Cylinder.h"
#include "GameObjects/HoverCar.h"
#include "GameObjects/Monkey.h"
#include "GameObjects/Sphere0.h"
#include "GameObjects/Sphere1.h"
#include "GameObjects/Sphere2.h"
#include "GameObjects/Statue.h"
#include "GameObjects/UVSphere.h"
#include "GameObjects/Bunny.h"
#include "GameObjects/Teapot.h"
#include "GameObjects/Cone.h"
#include "GameObjects/Torus.h"
#include "GameObjects/Tree.h"
#include "GameObjects/Point.h"
#include "GameObjects/Plane.h"
#include "GameObjects/Arrow.h"
#include "GameObjects/Rectangle.h"
#include "GameObjects/Triangle.h"
#include "GameObjects/Tetrahedron.h"
#include "GameObjects/Line.h"
#include "GameObjects/Ray.h"



namespace qvt
{

	unsigned int Panel::pointNumber;
    Entity Panel::m_selectionContext;
	 std::vector<std::string> Panel::gameObjectList;

	
	Panel::Panel(Camera* editor,Scene* sc)
	{
		m_editorCamera = editor;
		scene = sc;

		gameObjectList.push_back("Cube");
		gameObjectList.push_back("LineSegment");
		gameObjectList.push_back("HoverBike");
		gameObjectList.push_back("Cylinder");
		gameObjectList.push_back("HoverCar");
		gameObjectList.push_back("Monkey");
		gameObjectList.push_back("Sphere0");
		gameObjectList.push_back("Sphere1");
		gameObjectList.push_back("Sphere2");
		gameObjectList.push_back("Statue");
		gameObjectList.push_back("UVSphere");
		gameObjectList.push_back("Bunny");
		gameObjectList.push_back("Teapot");
		gameObjectList.push_back("Cone");
		gameObjectList.push_back("Torus");
		gameObjectList.push_back("Tree");
		gameObjectList.push_back("Point");
		gameObjectList.push_back("Plane");
		gameObjectList.push_back("Arrow");
		gameObjectList.push_back("Rectangle");
		gameObjectList.push_back("Triangle");
		gameObjectList.push_back("Tetrahedron");
		gameObjectList.push_back("Line");
		gameObjectList.push_back("Ray");
	
	}

	

	void Panel::OnImGuiRender()
	{

        ImGui::Begin("Scene Panel");
		
        char buff[512];
		memset(buff,0,512);
		
		if(ImGui::InputText("Search",buff,512))
		{
			hierarchyNodeName = std::string(buff);

		}

		FilterHierarchy(hierarchyNodeName);

		if(ImGui::Button("End Search",ImVec2{100,20}))
		{
			hierarchyNodeName = "";
		}

		if(ImGui::Button("Add new object",ImVec2{125,20}))
		{
                ImGui::OpenPopup("NewObjectSettings");
		}


		if(ImGui::BeginPopup("NewObjectSettings"))
		{
			for(int i=0;i<gameObjectList.size();i++)
			{
				std::string name = gameObjectList[i];

				if(ImGui::MenuItem(name.c_str()))
				{
					if(strcmp(name.c_str(),"Cube")==0)
					{
						Cube c;
					}

					if(strcmp(name.c_str(),"LineSegment")==0)
					{
						LineSegment l(glm::vec3(0,0,0),glm::vec3(10,10,10));
					}

					if(strcmp(name.c_str(),"HoverBike")==0)
					{
						HoverBike hb;
					}

					if(strcmp(name.c_str(),"Cylinder")==0)
					{
						Cylinder c;
					}

					if(strcmp(name.c_str(),"HoverCar")==0)
					{
						HoverCar c;
					}

					if(strcmp(name.c_str(),"Monkey")==0)
					{
						Monkey m;
					}

					if(strcmp(name.c_str(),"Sphere0")==0)
					{
						Sphere0 s;
					}

					if(strcmp(name.c_str(),"Sphere1")==0)
					{
						Sphere1 s;
					}

					if(strcmp(name.c_str(),"Sphere2")==0)
					{
						Sphere2 s;
					}

					if(strcmp(name.c_str(),"Statue")==0)
					{
						Statue s;
					}

					if(strcmp(name.c_str(),"UVSphere")==0)
					{
						UVSphere s;
					}

					if(strcmp(name.c_str(),"Bunny")==0)
					{
						Bunny b;
					}

					if(strcmp(name.c_str(),"Teapot")==0)
					{
						Teapot t;
					}

					if(strcmp(name.c_str(),"Cone")==0)
					{
						Cone c;
					}

					if(strcmp(name.c_str(),"Torus")==0)
					{
						Torus t;
					}

					if(strcmp(name.c_str(),"Tree")==0)
					{
						Tree t;
					}

					if(strcmp(name.c_str(),"Point")==0)
					{
						Point p(glm::vec3(0,0,0));
					}

					if(strcmp(name.c_str(),"Plane")==0)
					{
						Plane p(glm::vec3(0,0,0));
					}

					if(strcmp(name.c_str(),"Arrow")==0)
					{
						Arrow a(glm::vec3(0,0,0));
					}

					if(strcmp(name.c_str(),"Rectangle")==0)
					{
						Rectangle r(glm::vec3(0,0,0));
					}

					if(strcmp(name.c_str(),"Triangle")==0)
					{
						Triangle t(glm::vec3(0,0,0));
					}

					if(strcmp(name.c_str(),"Tetrahedron")==0)
					{
						Tetrahedron t(glm::vec3(0,0,0));
					}

					if(strcmp(name.c_str(),"Line")==0)
					{
						Line l(glm::vec3(0,0,0));
					}

					if(strcmp(name.c_str(),"Ray")==0)
					{
						Ray r(glm::vec3(0,0,0));
					}

				}


			}

			ImGui::EndPopup();
		}

		

		

		ImGui::End();

		ImGui::Begin("Properties");


        if(scene->m_registry.valid(m_selectionContext.GetEntityID()))
        {
			auto& tag = m_selectionContext.GetComponent<TagComponent>();

            std::string name = m_selectionContext.GetComponent<TagComponent>().name;
            ImGui::Text("Object Type: %s",name.c_str());
			char buff[512];
			memset(buff,0,512);
			strcpy(buff,tag.name.c_str());

			if(ImGui::InputText("Name",buff,sizeof(buff)))
			{

				tag.name = std::string(buff);
				

			}

			auto& tr = m_selectionContext.GetComponent<TransformComponent>();

			if(ImGui::Checkbox("Static",&tr.isStatic))
			{
				
			}

			


			if(!tr.isStatic)
			{
				glm::vec3 pos = tr.position;
				DrawVec3Control("Translation",pos);
				tr.position = pos;
				
				glm::vec3 rot = glm::degrees(tr.rotation);
				DrawVec3Control("Rotation",rot);
				tr.rotation = glm::radians(rot);



				glm::vec3 scale = tr.scale;
				DrawVec3Control("Scale",scale);
				tr.scale = scale;
			}

			if(m_selectionContext.HasComponent<PbrRendererComponent>())
			{
				auto& mc  = m_selectionContext.GetComponent<PbrRendererComponent>();

				MaterialLibrary::PbrMaterial* material = MaterialLibrary::GetPbrMaterial(mc.m_name);
			if(material)
			{
				ImGui::Text("Material Name: %s",mc.m_name.c_str());
				ImGui::ColorEdit3("Albedo",glm::value_ptr(material->albedo));
				ImGui::DragFloat("Metallic",&material->metallic);
				ImGui::DragFloat("Roughness",&material->roughness);
				ImGui::DragFloat("Ao",&material->ao);
				if(ImGui::Button("Change Material",ImVec2{120,20}))
				{
                      ImGui::OpenPopup("MaterialList");
				}

				if(ImGui::BeginPopup("MaterialList"))
				{
					for(int i=0;i<MaterialLibrary::pbrMaterialList.size();i++)
					{
						std::string name = MaterialLibrary::pbrMaterialList[i];

						if(ImGui::MenuItem(name.c_str()))
						{
							RendererSystem::RemoveFromPbrRenderMap(mc.m_name,(uint32_t)m_selectionContext.GetEntityID());
							mc.material = *MaterialLibrary::GetPbrMaterial(name);
							mc.m_name = name;
							RendererSystem::AddToPbrRenderMap(mc.m_name,(uint32_t)m_selectionContext.GetEntityID());
							
                            

							
							

						}
					}

					ImGui::EndPopup();
				}

				if(ImGui::Button("Create New Material",ImVec2{140,20}))
				{
				    newMaterialWindow = true;
				}

                if(newMaterialWindow)
				{
					ImGui::Begin("Create new material");

					ImGui::Text("Enter material name: ");
					char buffer[256];
					memset(buffer,0,sizeof(buffer));
					if(ImGui::InputText("Name",buffer,sizeof(buffer)))
					{
				        newMaterialName = std::string(buffer); 
					}

					


					if(ImGui::Button("Save",ImVec2{40,20}))
					{
						
						MaterialLibrary::AddNewPbrMaterial(newMaterialName);
						newMaterialWindow = false;
					}

					if(ImGui::Button("Close",ImVec2{40,20}))
					{
						newMaterialWindow = false;
					}




					ImGui::End();
				}
				

			}
			}

			if(m_selectionContext.HasComponent<MeshComponent>())
			{
				auto& mc = m_selectionContext.GetComponent<MeshComponent>();

				std::string s = std::string()+"Mesh name:"+mc.meshName;

				ImGui::Text(s.c_str());

				if(ImGui::Button("Change mesh",ImVec2{120,20}))
				{
					ImGui::OpenPopup("Mesh settings");
				}

				if(ImGui::BeginPopup("Mesh settings"))
				{
					for(int i=0;i<MeshLibrary::meshList.size();i++)
					{
						std::string name = MeshLibrary::meshList[i];

						if(ImGui::MenuItem(name.c_str()))
						{
							 
							 std::pair<unsigned int,unsigned int> mesh = MeshLibrary::GetMesh(name);

							

								mc.VAO = mesh.first;
								mc.numIndices = mesh.second;
								mc.meshName = name;
								mc.radius = MeshLibrary::GetMeshRadius(name);

								if(m_selectionContext.HasComponent<AABBoxComponent>())
								{
									auto& box = m_selectionContext.GetComponent<AABBoxComponent>();
									box.meshName = name;
									CollisionSystem::UpdateAABBoxes(true);
								}

								if(m_selectionContext.HasComponent<OBBoxComponent>())
								{
									auto& box = m_selectionContext.GetComponent<OBBoxComponent>();
									box.meshName = name;
									CollisionSystem::UpdateOBBoxes(true);
								}

								if(m_selectionContext.HasComponent<SphereBVComponent>())
								{
									auto& s = m_selectionContext.GetComponent<SphereBVComponent>();
									s.meshName = name;
									CollisionSystem::UpdateSphereBVs(true);
								}

								if(m_selectionContext.HasComponent<CapsuleBVComponent>())
								{
									auto& s =m_selectionContext.GetComponent<CapsuleBVComponent>();
									s.meshName = name;
									CollisionSystem::UpdateCapsuleBVs(true);
								}

								
							
						}
					}
					ImGui::EndPopup();
				}
			}

			if(m_selectionContext.HasComponent<AABBoxComponent>())
			{
				ImGui::Text("AABox Collider");

				if(ImGui::Button("Change collider",ImVec2{120,20}))
				{
					ImGui::OpenPopup("ColliderSettings");

				}

				if(ImGui::BeginPopup("ColliderSettings"))
				{
					for(int i=0;i<CollisionSystem::colliderList.size();i++)
					{
						std::string name = CollisionSystem::colliderList[i];

						if(ImGui::MenuItem(name.c_str()))
						{
							if(strcmp("OBBox",name.c_str())==0)
							{
								
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<AABBoxComponent>();
								m_selectionContext.AddComponent<OBBoxComponent>(mesh.meshName);
								CollisionSystem::UpdateOBBoxes(true);
							}

							if(strcmp("SphereBV",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<AABBoxComponent>();
								m_selectionContext.AddComponent<SphereBVComponent>(mesh.meshName);
								CollisionSystem::UpdateSphereBVs(true);

							}

							if(strcmp("CapsuleBV",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<AABBoxComponent>();
								m_selectionContext.AddComponent<CapsuleBVComponent>(mesh.meshName);
								CollisionSystem::UpdateCapsuleBVs(true);
							}

						}
					}
					ImGui::EndPopup();
				}
			}

			if(m_selectionContext.HasComponent<OBBoxComponent>())
			{
				ImGui::Text("OBBox Collider");

				if(ImGui::Button("Change collider",ImVec2{120,20}))
				{
					ImGui::OpenPopup("ColliderSettings");

				}

				if(ImGui::BeginPopup("ColliderSettings"))
				{
					for(int i=0;i<CollisionSystem::colliderList.size();i++)
					{
						std::string name = CollisionSystem::colliderList[i];

						if(ImGui::MenuItem(name.c_str()))
						{
							if(strcmp("AABBox",name.c_str())==0)
							{
								
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<OBBoxComponent>();
								m_selectionContext.AddComponent<AABBoxComponent>(mesh.meshName);
								CollisionSystem::UpdateAABBoxes(true);
							}

							if(strcmp("SphereBV",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<OBBoxComponent>();
								m_selectionContext.AddComponent<SphereBVComponent>(mesh.meshName);
								CollisionSystem::UpdateSphereBVs(true);

							}

							if(strcmp("CapsuleBV",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<OBBoxComponent>();
								m_selectionContext.AddComponent<CapsuleBVComponent>(mesh.meshName);
								CollisionSystem::UpdateCapsuleBVs(true);

							}

						}
					}
					ImGui::EndPopup();
				}
			}

			if(m_selectionContext.HasComponent<SphereBVComponent>())
			{
				ImGui::Text("Sphere Collider");

				if(ImGui::Button("Change collider",ImVec2{120,20}))
				{
					ImGui::OpenPopup("ColliderSettings");

				}

				if(ImGui::BeginPopup("ColliderSettings"))
				{
					for(int i=0;i<CollisionSystem::colliderList.size();i++)
					{
						std::string name = CollisionSystem::colliderList[i];

						if(ImGui::MenuItem(name.c_str()))
						{
							if(strcmp("AABBox",name.c_str())==0)
							{
								
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<SphereBVComponent>();
								m_selectionContext.AddComponent<AABBoxComponent>(mesh.meshName);
								CollisionSystem::UpdateAABBoxes(true);
							}

							if(strcmp("OBBox",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<SphereBVComponent>();
								m_selectionContext.AddComponent<OBBoxComponent>(mesh.meshName);
								CollisionSystem::UpdateOBBoxes(true);

							}

							if(strcmp("CapsuleBV",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<SphereBVComponent>();
								m_selectionContext.AddComponent<CapsuleBVComponent>(mesh.meshName);
								CollisionSystem::UpdateCapsuleBVs(true);
								
							}

						}
					}
					ImGui::EndPopup();
				}
			}

			if(m_selectionContext.HasComponent<CapsuleBVComponent>())
			{
				ImGui::Text("Capsule Collider");

				if(ImGui::Button("Change collider",ImVec2{120,20}))
				{
					ImGui::OpenPopup("ColliderSettings");

				}

				if(ImGui::BeginPopup("ColliderSettings"))
				{
					for(int i=0;i<CollisionSystem::colliderList.size();i++)
					{
						std::string name = CollisionSystem::colliderList[i];

						if(ImGui::MenuItem(name.c_str()))
						{
							if(strcmp("AABBox",name.c_str())==0)
							{
								
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<CapsuleBVComponent>();
								m_selectionContext.AddComponent<AABBoxComponent>(mesh.meshName);
								CollisionSystem::UpdateAABBoxes(true);
							}

							if(strcmp("OBBox",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<CapsuleBVComponent>();
								m_selectionContext.AddComponent<OBBoxComponent>(mesh.meshName);
								CollisionSystem::UpdateOBBoxes(true);

							}

							if(strcmp("SphereBV",name.c_str())==0)
							{
								auto& mesh = m_selectionContext.GetComponent<MeshComponent>();

								m_selectionContext.RemoveComponent<CapsuleBVComponent>();
								m_selectionContext.AddComponent<SphereBVComponent>(mesh.meshName);
								CollisionSystem::UpdateSphereBVs(true);
								
							}

						}
					}
					ImGui::EndPopup();
				}

			}

			if(m_selectionContext.HasComponent<LineSegmentPrimitiveComponent>())
			{
				auto& sl = m_selectionContext.GetComponent<LineSegmentPrimitiveComponent>();
				auto& p = m_selectionContext.GetComponent<ParentComponent>();

				auto& Atr = p.children[0].GetComponent<TransformComponent>();
				auto& Btr = p.children[1].GetComponent<TransformComponent>();

				DrawVec3Control("Point A",Atr.position);
				DrawVec3Control("Point B",Btr.position);
				ImGui::ColorEdit3("Line color",glm::value_ptr(sl.color));

			}

			if(m_selectionContext.HasComponent<PointPrimitiveComponent>())
			{
				auto& p = m_selectionContext.GetComponent<PointPrimitiveComponent>();

				ImGui::Text("Point primitive");
				ImGui::ColorEdit3("Point color",glm::value_ptr(p.color));
			}

			if(m_selectionContext.HasComponent<PlanePrimitiveComponent>())
			{
				auto& p = m_selectionContext.GetComponent<PlanePrimitiveComponent>();

				ImGui::Text("Plane primitive");
				ImGui::ColorEdit3("Plane color",glm::value_ptr(p.color));
				DrawVec3Control("Plane normal",p.normal);
			}

			if(m_selectionContext.HasComponent<Rectangle3DPrimitiveComponent>())
			{
				auto& r = m_selectionContext.GetComponent<Rectangle3DPrimitiveComponent>();

				ImGui::Text("Rectangle primitive");
				ImGui::ColorEdit3("Rectangle color",glm::value_ptr(r.color));
				ImGui::DragFloat("X extent",&r.e[0]);
				ImGui::DragFloat("Y extent",&r.e[1]);
			}

			if(m_selectionContext.HasComponent<TrianglePrimitiveComponent>())
			{
				auto& t = m_selectionContext.GetComponent<TrianglePrimitiveComponent>();

				ImGui::Text("Triangle primitive");
				ImGui::ColorEdit3("Triangle color",glm::value_ptr(t.color));
			}

			if(m_selectionContext.HasComponent<TetrahedronPrimitiveComponent>())
			{
				auto& t = m_selectionContext.GetComponent<TetrahedronPrimitiveComponent>();

				ImGui::Text("Tetrahedron primitive");
				ImGui::ColorEdit3("Tetrahedron color",glm::value_ptr(t.color));

			}

			if(m_selectionContext.HasComponent<LinePrimitiveComponent>())
			{
				auto& t = m_selectionContext.GetComponent<LinePrimitiveComponent>();

				ImGui::Text("Line primitive");
				ImGui::ColorEdit3("Line color",glm::value_ptr(t.color));
				DrawVec3Control("Point a",t.a);
				DrawVec3Control("Point b",t.b);
			}

			if(m_selectionContext.HasComponent<RayPrimitiveComponent>())
			{
				auto& t = m_selectionContext.GetComponent<RayPrimitiveComponent>();

				ImGui::Text("Ray primitive");
				ImGui::ColorEdit3("Ray color",glm::value_ptr(t.color));
				DrawVec3Control("Point a",t.a);
				DrawVec3Control("Point b",t.b);
			}


			if(m_selectionContext.HasComponent<CameraComponent>())
			{
				auto& cam = m_selectionContext.GetComponent<CameraComponent>();

				DrawVec3Control("Position",cam.camera->position);
				ImGui::DragFloat("Speed",&cam.camera->cameraSpeed);
				ImGui::DragFloat("ZNear",&cam.camera->zNear);
				ImGui::DragFloat("ZFar",&cam.camera->zFar);
				ImGui::ColorEdit3("Frustum",glm::value_ptr(cam.camera->frustumColor));
				if(ImGui::Checkbox("Primary",&cam.camera->primary))
				{
					if(cam.camera->primary)
					{
						

						cam.camera->SetAsPrimary();

						

						
					}

					GameLayer::framebuffer->Resize(GameLayer::m_viewportSize.x,GameLayer::m_viewportSize.y);
					GameLayer::mainCamera->UpdateAspectRatio((float)GameLayer::m_viewportSize.x/(float)GameLayer::m_viewportSize.y);
					



				}
				if(ImGui::Checkbox("Show frustum",&cam.camera->showFrustum))
				{
					if(cam.camera->showFrustum)
					{
						RendererSystem::viewFrustumCounter++;
					}
					else
					{
						RendererSystem::viewFrustumCounter--;
					}
				}

				if(ImGui::Checkbox("XYView",&cam.camera->xyView))
				{
					if(cam.camera->xyView)
					{
						cam.camera->yaw = -90;
						cam.camera->pitch = 0;
						cam.camera->position.y = 0;
						cam.camera->position.x = 0;

						cam.camera->UpdateVectors();

						cam.camera->yzView = false;
						cam.camera->xzView = false;
					}
				}

				if(ImGui::Checkbox("YZView",&cam.camera->yzView))
				{
					if(cam.camera->yzView)
					{
						cam.camera->yaw = 0;
						cam.camera->pitch = 0;
						cam.camera->position.y = 0;
						cam.camera->position.z = 0;

						cam.camera->UpdateVectors();

						cam.camera->xzView = false;
						cam.camera->xyView = false;
					}
				}

				if(ImGui::Checkbox("XZView",&cam.camera->xzView))
				{
					if(cam.camera->xzView)
					{
						cam.camera->yaw = 90;
						cam.camera->pitch = -90;
						cam.camera->position.z = 0;
						cam.camera->position.x = 0;

						cam.camera->UpdateVectors();

						cam.camera->yzView = false;
						cam.camera->xyView = false;
					}
					else
					{
						cam.camera->pitch = -89.0f;
						cam.camera->yaw = -90.0f;
						cam.camera->UpdateVectors();
					}
				}

				if(ImGui::Checkbox("Opposite View",&cam.camera->oppositeView))
				{
					if(cam.camera->xzView)
					{
						cam.camera->pitch*=-1;
						cam.camera->position.y*=-1;
						cam.camera->UpdateVectors();

					}

					if(cam.camera->yzView)
					{
						if(cam.camera->yaw==0)
						{
							cam.camera->yaw = 180;
						}
						else
						{
							cam.camera->yaw = 0;
						}

						cam.camera->position.x *= -1;

						cam.camera->UpdateVectors();

					}

					if(cam.camera->xyView)
					{
						cam.camera->yaw*=-1;
						cam.camera->position.z*=-1;
						cam.camera->UpdateVectors();

					}
				}

				


			}

			if(m_selectionContext.HasComponent<Live2DGraphComponent>())
			{
				auto& g = m_selectionContext.GetComponent<Live2DGraphComponent>();

				

				if(ImGui::Checkbox("Show live graphs",&g.showGraphs))
				{
					
				}

				if(g.showGraphs)
				{
					

						for(auto& b:g.boolMap)
						{
							if(ImGui::Checkbox(b.first.c_str(),&b.second))
							{

							}
						}

				}

				g.OnImGuiRenderer();
				
			}

			if(m_selectionContext.HasComponent<ScriptComponent>())
			{
				auto& sc = m_selectionContext.GetComponent<ScriptComponent>();

				for(auto& script:sc.scripts)
				{
					script.Instance->OnImGuiRender();
				}

				if(ImGui::Button("Add script",ImVec2{120,20}))
				{
					ImGui::OpenPopup("AddScriptSettings");
				}

				if(ImGui::BeginPopup("AddScriptSettings"))
				{
					for(int i=0;i<ScriptSystem::scriptList.size();i++)
					{
						std::string name = ScriptSystem::scriptList[i];

						if(ImGui::MenuItem(name.c_str()))
						{
							NativeScriptComponent script;
							script.behaviorName = name;
							sc.scripts.push_back(script);
						}

					}
					ImGui::EndPopup();
				}

				if(ImGui::Button("Delete script",ImVec2{120,20}))
				{
					ImGui::OpenPopup("DeleteScriptSettings");
				}

				if(ImGui::BeginPopup("DeleteScriptSettings"))
				{
					for(int i=0;i<sc.scripts.size();i++)
					{
						std::string name = sc.scripts[i].behaviorName;

						if(ImGui::MenuItem(name.c_str()))
						{
							int count = 0;
							for(auto& scr:sc.scripts)
							{
								if(strcmp(name.c_str(),sc.scripts[count].behaviorName.c_str())==0)
								{
									delete sc.scripts[count].Instance;
									sc.scripts[count].Instance = nullptr;
									sc.scripts.erase(sc.scripts.begin()+count);
									break;
								}
								count++;
							}
						}
					}
					ImGui::EndPopup();
				}


				for(auto& script:sc.scripts)
				{
					std::string name = script.behaviorName;

					ImGui::Text(("Script name: "+name).c_str());
				}
			}

			if(ImGui::Button("Add new component"))
			{
					ImGui::OpenPopup("AddComponent");
			}

			if(ImGui::BeginPopup("AddComponent"))
			{
				if(ImGui::MenuItem("Mesh Component"))
				{
					if(!m_selectionContext.HasComponent<MeshComponent>())
					{
						m_selectionContext.AddComponent<MeshComponent>("cube");
					}


					ImGui::CloseCurrentPopup();

				}

				

				if(ImGui::MenuItem("Pbr Component"))
				{
					if(!m_selectionContext.HasComponent<PbrRendererComponent>())
					{
						m_selectionContext.AddComponent<PbrRendererComponent>("cyan",(uint32_t)m_selectionContext.GetEntityID());

					}
					ImGui::CloseCurrentPopup();

				}

				if(ImGui::MenuItem("Camera Component"))
				{
					m_selectionContext.AddComponent<CameraComponent>(glm::vec3(0,0,0));
				}

				if(ImGui::MenuItem("Script component"))
				{
					m_selectionContext.AddComponent<ScriptComponent>();
				}

				if(ImGui::MenuItem("CollisionComponent"))
				{
					if(m_selectionContext.HasComponent<MeshComponent>())
					{
						auto& mc = m_selectionContext.GetComponent<MeshComponent>();

					   m_selectionContext.AddComponent<AABBoxComponent>(mc.meshName);
					}
					

				}


				ImGui::EndPopup();
			}

		
        }

        ImGui::End();

        


        


		ImGui::Begin("Editor Camera");

		DrawVec3Control("Position",m_editorCamera->position);
		ImGui::DragFloat("Speed",&m_editorCamera->cameraSpeed);
		ImGui::DragFloat("ZNear",&m_editorCamera->zNear);
		ImGui::DragFloat("ZFar",&m_editorCamera->zFar);
		ImGui::ColorEdit3("Frustum",glm::value_ptr(m_editorCamera->frustumColor));
		if(ImGui::Checkbox("Primary",&GameLayer::editorCamera->primary))
		{
			if(GameLayer::editorCamera->primary)
			{
			
				GameLayer::editorCamera->SetAsPrimary();

			}

			GameLayer::framebuffer->Resize(GameLayer::m_viewportSize.x,GameLayer::m_viewportSize.y);
			GameLayer::mainCamera->UpdateAspectRatio((float)GameLayer::m_viewportSize.x/(float)GameLayer::m_viewportSize.y);



		}

		if(ImGui::Checkbox("Show frustum",&m_editorCamera->showFrustum))
		{
			if(m_editorCamera->showFrustum)
			{
				RendererSystem::viewFrustumCounter++;

			}
			else
			{
				RendererSystem::viewFrustumCounter--;
			}
		}

		if(ImGui::Checkbox("XYView",&m_editorCamera->xyView))
				{
					if(m_editorCamera->xyView)
					{
						m_editorCamera->yaw = -90;
						m_editorCamera->pitch = 0;
						m_editorCamera->position.y = 0;
						m_editorCamera->position.x = 0;

						m_editorCamera->UpdateVectors();

						m_editorCamera->yzView = false;
						m_editorCamera->xzView = false;
					}
				}

				if(ImGui::Checkbox("YZView",&m_editorCamera->yzView))
				{
					if(m_editorCamera->yzView)
					{
						m_editorCamera->yaw = 0;
						m_editorCamera->pitch = 0;
						m_editorCamera->position.y = 0;
						m_editorCamera->position.z = 0;

						m_editorCamera->UpdateVectors();

						m_editorCamera->xzView = false;
						m_editorCamera->xyView = false;
					}
				}

				if(ImGui::Checkbox("XZView",&m_editorCamera->xzView))
				{
					if(m_editorCamera->xzView)
					{
						m_editorCamera->yaw = 90;
						m_editorCamera->pitch = -90;
						m_editorCamera->position.z = 0;
						m_editorCamera->position.x = 0;

						m_editorCamera->UpdateVectors();

						m_editorCamera->yzView = false;
						m_editorCamera->xyView = false;
					}
					else
					{
						m_editorCamera->pitch = -89.0f;
						m_editorCamera->yaw = -90.0f;
						m_editorCamera->UpdateVectors();
					}
				}

				if(ImGui::Checkbox("Opposite View",&m_editorCamera->oppositeView))
				{
					if(m_editorCamera->xzView)
					{
						m_editorCamera->pitch*=-1;
						m_editorCamera->position.y*=-1;
						m_editorCamera->UpdateVectors();

					}

					if(m_editorCamera->yzView)
					{
						if(m_editorCamera->yaw==0)
						{
							m_editorCamera->yaw = 180;
						}
						else
						{
							m_editorCamera->yaw = 0;
						}

						m_editorCamera->position.x*=-1;

						m_editorCamera->UpdateVectors();

					}

					if(m_editorCamera->xyView)
					{
						m_editorCamera->yaw *= -1;
						m_editorCamera->position.z*= -1;
						m_editorCamera->UpdateVectors();
					}
				}




		ImGui::End();

		
		

		


		
		
		


	}


	glm::vec3 Panel::DrawVec3Control(const std::string& name, glm::vec3& value,glm::vec3 resetValue, float columnWidth)
	{
		glm::vec3 offset = value;

		ImGui::PushID(name.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(name.c_str());

		ImGui::NextColumn();
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f,1.0f });
		if (ImGui::Button("X", buttonSize))
		{
			
			value.x = resetValue.x;
		}
			
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &value.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.8f,0.3f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.2f,1.0f });
		if (ImGui::Button("Y", buttonSize))
			value.y = resetValue.y;

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Y", &value.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();


		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.2f,0.8f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.3f,0.9f,1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.2f,0.8f,1.0f });
		if (ImGui::Button("Z", buttonSize))
			value.z = resetValue.z;
		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##Z", &value.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
		ImGui::PopID();
		ImGui::Columns(1);

		return value - offset;

	}


	void Panel::DrawComponents(Entity* object)
	{
		

		

	}

    bool Panel::DrawEntityNode(Entity* object)
    {
		auto tag = object->GetComponent<TagComponent>().name;
	

		ImGuiTreeNodeFlags flags = ((m_selectionContext == *object) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)(object->GetEntityID()), flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_selectionContext = *object;
			
			
		}
		bool entityDeleted = false;

		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
			{
				entityDeleted = true;
			}

			ImGui::EndPopup();
		}


		if (opened)
		{
			
		
			ImGui::TreePop();
		}

		if (entityDeleted)
		{

			if (m_selectionContext == *object)
			{

			    DeleteEntity();	
				
				
				GameLayer::gizmotype = -1;

			    hierarchyNodeName = "";

				return false;
				
			}



				
			

			

				
			
		}

		return true;



    }




	void Panel::FilterHierarchy(std::string name)
	{
		if(std::strcmp(name.c_str(),"new")==0)
		{
			auto view = scene->m_registry.view<TagComponent>();

			for(auto entityID:view)
			{
		          Entity entity{entityID,scene};

				  auto& tag = entity.GetComponent<TagComponent>();

				  if(strcmp(tag.name.c_str(),"new entity")==0)
				  {
					  DrawEntityNode(&entity);
				  }

				  

			}

		}

		

		if(std::strcmp(name.c_str(),"all")==0)
		{
			auto view = scene->m_registry.view<TagComponent>();

			for(auto entityID:view)
			{
		          Entity entity{entityID,scene};

				  auto& tag = entity.GetComponent<TagComponent>();

				  
				   DrawEntityNode(&entity);
				  

				  

			}
			
		}
	
		
	}

		

	void Panel::DeleteEntity()
	{
		if(!scene->m_registry.valid(m_selectionContext.GetEntityID()))
		{
			return;
		}
		
        if(m_selectionContext.HasComponent<PbrRendererComponent>())
		{
			
			auto& rc = m_selectionContext.GetComponent<PbrRendererComponent>();

			RendererSystem::RemoveFromPbrRenderMap(rc.m_name,(uint32_t)m_selectionContext.GetEntityID());
		}

		if(m_selectionContext.HasComponent<PbrStaticRendererComponent>())
		{
			
			auto& rc = m_selectionContext.GetComponent<PbrStaticRendererComponent>();

			RendererSystem::RemoveFromPbrStaticRenderMap(rc.m_name,(uint32_t)m_selectionContext.GetEntityID());
		}

		if(m_selectionContext.HasComponent<ParentComponent>())
		{
			auto& p = m_selectionContext.GetComponent<ParentComponent>();

			for(int i=0;i<p.children.size();i++)
			{
				scene->DestroyEntity(p.children[i]);
			}
		}

		scene->DestroyEntity(m_selectionContext);

		m_selectionContext = Entity(entt::null,scene);

		

    }

	Entity Panel::CopySelectedEntity()
	{
		Entity entity;

		if(m_selectionContext.HasComponent<ParentComponent>() || m_selectionContext.HasComponent<ChildComponent>())
		{
			entity = CopyModel(m_selectionContext);
		}
		else
		{
			entity = scene->CreateEntity("copy");
		}
		


		auto& tag = m_selectionContext.GetComponent<TagComponent>();
		auto& ctag = entity.GetComponent<TagComponent>();
		ctag.name = tag.name;

		auto& tr = m_selectionContext.GetComponent<TransformComponent>();
		auto& ctr = entity.GetComponent<TransformComponent>();

		ctr.position = tr.position;
		ctr.angle = tr.angle;
		ctr.rotation = tr.rotation;
		ctr.scale = tr.scale;

		if(m_selectionContext.HasComponent<MeshComponent>())
		{
			auto& mc = m_selectionContext.GetComponent<MeshComponent>();
			
			entity.AddComponent<MeshComponent>(mc.meshName);
			
			
		}

		if(m_selectionContext.HasComponent<PbrRendererComponent>())
		{
			auto& pc = m_selectionContext.GetComponent<PbrRendererComponent>();
			entity.AddComponent<PbrRendererComponent>(pc.m_name,(uint32_t)entity.GetEntityID());
		}

		if(m_selectionContext.HasComponent<PbrStaticRendererComponent>())
		{
			auto& pc = m_selectionContext.GetComponent<PbrStaticRendererComponent>();
			entity.AddComponent<PbrStaticRendererComponent>(pc.m_name,(uint32_t)entity.GetEntityID());
		}

		if(m_selectionContext.HasComponent<SimpleRendererComponent>())
		{
			auto& sc = m_selectionContext.GetComponent<SimpleRendererComponent>();
			entity.AddComponent<SimpleRendererComponent>(sc.m_name,(uint32_t)entity.GetEntityID());
		}

		if(m_selectionContext.HasComponent<AABBoxComponent>())
		{
			auto& box = m_selectionContext.GetComponent<AABBoxComponent>();
			entity.AddComponent<AABBoxComponent>(box.meshName);

		}

		if(m_selectionContext.HasComponent<SphereBVComponent>())
		{
			auto& s  = m_selectionContext.GetComponent<SphereBVComponent>();
			entity.AddComponent<SphereBVComponent>(s.meshName);
		}

		if(m_selectionContext.HasComponent<OBBoxComponent>())
		{
			auto& s = m_selectionContext.GetComponent<OBBoxComponent>();
			entity.AddComponent<OBBoxComponent>(s.meshName);
		}

		if(m_selectionContext.HasComponent<CapsuleBVComponent>())
		{
			auto& s = m_selectionContext.GetComponent<CapsuleBVComponent>();
			entity.AddComponent<CapsuleBVComponent>(s.meshName);
		}

		if(m_selectionContext.HasComponent<LineSegmentPrimitiveComponent>())
		{
			auto& ls = m_selectionContext.GetComponent<LineSegmentPrimitiveComponent>();
			auto& otherls = entity.AddComponent<LineSegmentPrimitiveComponent>(ls.a,ls.b,ls.color);
		}

		if(m_selectionContext.HasComponent<PointPrimitiveComponent>())
		{
			auto& p = m_selectionContext.GetComponent<PointPrimitiveComponent>();
			auto& otherp = entity.AddComponent<PointPrimitiveComponent>(p.color);
		}

		if(m_selectionContext.HasComponent<PlanePrimitiveComponent>())
		{
			auto& p = m_selectionContext.GetComponent<PlanePrimitiveComponent>();
			auto& otherp = entity.AddComponent<PlanePrimitiveComponent>(p.normal,p.color);

		}

		if(m_selectionContext.HasComponent<Rectangle3DPrimitiveComponent>())
		{
			auto& p = m_selectionContext.GetComponent<Rectangle3DPrimitiveComponent>();

			auto& otherp = entity.AddComponent<Rectangle3DPrimitiveComponent>(p.color);
		}

		if(m_selectionContext.HasComponent<TrianglePrimitiveComponent>())
		{
			auto& t = m_selectionContext.GetComponent<TrianglePrimitiveComponent>();
			auto& othert = entity.AddComponent<TrianglePrimitiveComponent>(t.color);
		}

		if(m_selectionContext.HasComponent<TetrahedronPrimitiveComponent>())
		{
			auto& t = m_selectionContext.GetComponent<TetrahedronPrimitiveComponent>();
			auto& othert = entity.AddComponent<TetrahedronPrimitiveComponent>(t.color);
		}

		if(m_selectionContext.HasComponent<LinePrimitiveComponent>())
		{
			auto& t = m_selectionContext.GetComponent<LinePrimitiveComponent>();
			auto& othert = entity.AddComponent<LinePrimitiveComponent>(t.a,t.b,t.color);
		}

		if(m_selectionContext.HasComponent<RayPrimitiveComponent>())
		{
			auto& t = m_selectionContext.GetComponent<RayPrimitiveComponent>();
			auto& othert = entity.AddComponent<RayPrimitiveComponent>(t.a,t.b,t.color);
		}

		if(m_selectionContext.HasComponent<GameObjectNameComponent>())
		{
			auto& n = m_selectionContext.GetComponent<GameObjectNameComponent>();
			entity.AddComponent<GameObjectNameComponent>(n.objectName);


		}

		

		

		m_selectionContext = entity;

		return entity;
	}

	Entity Panel::CopyModel(Entity& original)
	{
		auto& name = original.GetComponent<GameObjectNameComponent>().objectName;
		auto& tr =  original.GetComponent<TransformComponent>();

		if(strcmp(name.c_str(),"Cube")==0)
		{
			Cube other(tr.position);

			return other.GetEntity();
		}

		if(strcmp(name.c_str(),"LineSegment")==0)
		{
			auto& ls = original.GetComponent<LineSegmentPrimitiveComponent>();
			

			LineSegment l(ls.a+glm::vec3(0,5,0),ls.b+glm::vec3(0,5,0),ls.color);

			return l.GetEntity();
		}

		if(strcmp(name.c_str(),"HoverBike")==0)
		{
			HoverBike hb(tr.position);

			return hb.GetEntity();

			
		}

		if(strcmp(name.c_str(),"Cylinder")==0)
		{
			Cylinder c(tr.position);

			return c.GetEntity();
		}

		if(strcmp(name.c_str(),"HoverCar")==0)
		{
			HoverCar c(tr.position);

			return c.GetEntity();
		}

		if(strcmp(name.c_str(),"Monkey")==0)
		{
			Monkey m(tr.position);

			return m.GetEntity();
		}

		if(strcmp(name.c_str(),"Sphere0")==0)
		{
			Sphere0 s(tr.position);

			return s.GetEntity();
		}

		if(strcmp(name.c_str(),"Sphere1")==0)
		{
			Sphere1 s(tr.position);

			return s.GetEntity();
		}

		if(strcmp(name.c_str(),"Sphere2")==0)
		{
			Sphere2 s(tr.position);

			return s.GetEntity();
		}

		if(strcmp(name.c_str(),"Statue")==0)
		{
			Statue s(tr.position);

			return s.GetEntity();
		}

		if(strcmp(name.c_str(),"UVSphere")==0)
		{
			UVSphere s(tr.position);

			return s.GetEntity();
		}

		if(strcmp(name.c_str(),"Bunny")==0)
		{
			Bunny s(tr.position);

			return s.GetEntity();
		}

		if(strcmp(name.c_str(),"Teapot")==0)
		{
			Teapot t(tr.position);

			return t.GetEntity();
		}

		if(strcmp(name.c_str(),"Cone")==0)
		{
			Cone c(tr.position);

			return c.GetEntity();
		}

		if(strcmp(name.c_str(),"Torus")==0)
		{
			Torus t(tr.position);

			return t.GetEntity();
		}

		if(strcmp(name.c_str(),"Tree")==0)
		{
			Tree t(tr.position);

			return t.GetEntity();
		}

		if(strcmp(name.c_str(),"Point")==0)
		{
			Point p(tr.position);

			return p.GetEntity();
		}

		if(strcmp(name.c_str(),"Plane")==0)
		{
			Plane p(tr.position);

			return p.GetEntity();
		}

		if(strcmp(name.c_str(),"Arrow")==0)
		{
			Arrow a(tr.position);

			return a.GetEntity();
		}

		if(strcmp(name.c_str(),"Rectangle")==0)
		{
			Rectangle r(tr.position);

			return r.GetEntity();
		}

		if(strcmp(name.c_str(),"Triangle")==0)
		{
			Triangle t(tr.position);

			return t.GetEntity();
		}

		if(strcmp(name.c_str(),"Tetrahedron")==0)
		{
			Tetrahedron t(tr.position);

			return t.GetEntity();
		}

		if(strcmp(name.c_str(),"Line")==0)
		{
			Line l(tr.position);

			return l.GetEntity();
		}

		if(strcmp(name.c_str(),"Ray")==0)
		{
			Ray r(tr.position);

			return r.GetEntity();
		}
	}

	void Panel::FocusCameraOnObject()
	{
		if(scene->m_registry.valid(m_selectionContext.GetEntityID()))
		{
			auto& tr = m_selectionContext.GetComponent<TransformComponent>();

			CameraSystem::mainCamera->position = tr.position + glm::vec3(5,5,5);
			CameraSystem::mainCamera->yaw = 225.0f;
			CameraSystem::mainCamera->pitch = -45.0f;
			CameraSystem::mainCamera->UpdateVectors();
		}
	}

}
