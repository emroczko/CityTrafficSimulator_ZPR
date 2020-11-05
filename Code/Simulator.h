#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

#include "StateMachine.h"
#include "AssetManager.h"
#include "InputManager.h"

namespace ZPR{


    struct SimulatorData {
        StateMachine machine;
        sf::RenderWindow window;
        AssetManager assets;
        InputManager input;
    }

    typedef std::shared_ptr<SimulatorData> SimulatorDataRef;

    class Simulator{
    public:
        Simulator(int width, int height, std::string title);
    
    private:
        const float dt = 1.0f / 60.0f;
        sf::Clock _clock;
    
        SimulatorDataRef _data = std::make_shared<SimulatorData>();
    
        void Run();
    }
 
}