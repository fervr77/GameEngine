#include "GameObject.h"
#include "Scene/Scene.h"
#include "Game/GameLayer.h"

namespace qvt
{
    GameObject::GameObject()
    {
        scene = GameLayer::scene;
    }

    GameObject::~GameObject()
    {
        
    }
}