#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Simulator.h"
#include "Button.h"

#include <vector>

#include "Button.h"
#include "MapView.h"
#include "ToolsView.h"
#include "CamerasView.h"


namespace ZPR{
    
    class CreatorState: public State{
    
    public:
        CreatorState (SimulatorDataRef data, int gridSize);
        
        void Init();
        
        void HandleInput();
        void Update(float dt);
        void Draw(float dt);
    private:
        SimulatorDataRef _data;
        std::unique_ptr<MapView> _mapView;
        std::unique_ptr<ToolsView> _toolsView;
        std::unique_ptr<CamerasView> _camerasView;
        int _gridSize;
    };
}
