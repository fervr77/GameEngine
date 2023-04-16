cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)


project(qvt)





add_subdirectory(vendor/glfw)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/entt)

include_directories(${CMAKE_SOURCE_DIR} vendor/glfw/include 
vendor/glad/include vendor/spdlog/include vendor/entt/single_include 
vendor/tinyobjloader vendor/stb_image vendor/imgui 
vendor/ImGuizmo 
vendor/implot)





add_definitions("-O3")

add_executable(${PROJECT_NAME} App.cpp
Core/Application.cpp 
Core/Window.cpp 
Core/Logger.cpp 
Events/Event.cpp 
Core/Input.cpp 
Core/Layer.cpp 
Core/LayerStack.cpp 
Game/GameLayer.cpp 
Scene/Scene.cpp 
Scene/Entity.cpp 
Systems/RendererSystem.cpp 
Renderer/ShaderLibrary.cpp 
Renderer/ShaderLoader.cpp 
Renderer/MeshLoader.cpp 
Renderer/MeshLibrary.cpp 
Renderer/TextureLoader.cpp 
Renderer/TextureLibrary.cpp 
Renderer/MaterialLibrary.cpp 
Systems/CameraSystem.cpp 
Scene/Camera.cpp 
Components/Components.cpp 
Systems/LightSystem.cpp 
Systems/TerrainNode.cpp 
Systems/TerrainQuadTree.cpp 
Systems/TerrainSystem.cpp 
vendor/imgui/backends/imgui_impl_opengl3.cpp 
vendor/imgui/backends/imgui_impl_glfw.cpp 
vendor/imgui/imgui.cpp
vendor/imgui/imgui_demo.cpp 
vendor/imgui/imgui_draw.cpp 
vendor/imgui/imgui_widgets.cpp 
vendor/imgui/imgui_tables.cpp
ImGui/ImGuiBuild.cpp 
ImGui/ImGuiLayer.cpp 
Game/Panel.cpp 
Renderer/Framebuffer.cpp 
vendor/ImGuizmo/ImGuizmo.cpp 
vendor/ImGuizmo/ImSequencer.cpp 
vendor/ImGuizmo/ImGradient.cpp 
vendor/ImGuizmo/ImCurveEdit.cpp 
vendor/implot/implot.cpp 
vendor/implot/implot_items.cpp 
vendor/implot/implot_demo.cpp 
Applications/App1/App1.cpp
Applications/App1/Scripts/Test.cpp  
Systems/ScriptSystem.cpp 
Systems/CollisionSystem.cpp 
GameObjects/GameObject.cpp 
GameObjects/Cube.cpp 
GameObjects/LineSegment.cpp 
GameObjects/HoverBike.cpp 
GameObjects/Cylinder.cpp 
GameObjects/HoverCar.cpp 
GameObjects/Monkey.cpp 
GameObjects/Sphere0.cpp 
GameObjects/Sphere1.cpp 
GameObjects/Sphere2.cpp 
GameObjects/Statue.cpp 
GameObjects/UVSphere.cpp 
GameObjects/Bunny.cpp 
GameObjects/Teapot.cpp 
GameObjects/Cone.cpp 
GameObjects/Torus.cpp 
GameObjects/Tree.cpp 
GameObjects/Point.cpp 
GameObjects/Plane.cpp 
GameObjects/Arrow.cpp 
GameObjects/Rectangle.cpp 
GameObjects/Triangle.cpp 
GameObjects/Tetrahedron.cpp 
GameObjects/Line.cpp 
GameObjects/Ray.cpp 
vendor/glad/src/glad.c

)

target_include_directories(${PROJECT_NAME} PUBLIC vendor/imgui vendor/ImGuizmo vendor/implot)
target_link_libraries(${PROJECT_NAME} glfw  spdlog)