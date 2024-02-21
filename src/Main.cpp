#include <iostream>
#include "Game/Game.h"

int main(int argc, char* argv[]) {
    /*Game game;
    game.Initialize();
    game.Run();
    game.Destroy();*/
    Registry registry;
    Entity entity(1);
    registry.AddComponent<TransformComponent>(entity, glm::vec2{ 10.0f,10.0f }, glm::vec2{10.0f, 10.0f}, 0.0);
    return 0;
}
