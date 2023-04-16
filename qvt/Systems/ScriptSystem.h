#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H
#include <unordered_map>
#include <vector>
#include <functional>

namespace qvt
{
    class Scene;
    class ScriptableEntity;

    class ScriptSystem
    {
        public:
            ScriptSystem(Scene* sc);
            ~ScriptSystem();
            void Update(float ts);

            static std::vector<std::string> scriptList;
            static std::unordered_map<std::string,std::function<ScriptableEntity*()>> map;

        private:
           

        private:
            Scene* scene;
            
            static float prevTimeStep;



    };

}
#endif