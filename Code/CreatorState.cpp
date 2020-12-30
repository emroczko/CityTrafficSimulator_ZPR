#include <sstream>
#include <iostream>

#include "CreatorState.h"
#include "Definitions.h"
#include "InputManager.h"

namespace ZPR {
    CreatorState::CreatorState(SimulatorDataRef data, int gridSize) : _data(data), _gridSize(gridSize) { }
    void CreatorState::Init(){
		this->_mapView = std::make_shared<MapView>(this->_data, _gridSize);
		this->_toolsView = std::make_unique<ToolsView>(this->_data);
        this->_camerasView = std::make_unique<CamerasView>(this->_data);
        this->_creatorHandler = std::make_unique<CreatorHandler>(this->_gridSize);
        this->_creatorHandler->add(this->_mapView);
        this->_creatorHandler->init();
        this->_simulationHandler = std::make_unique<SimulationHandler>();
        this->_simulationHandler->add(this->_mapView);
        this->_simulationHandler->add(this->_toolsView);
        this->_simulationHandler->add(this->_creatorHandler);
        this->_simulationHandler->init();
        this->_toolsView->add(this->_creatorHandler);
        this->_camerasView->add(this->_simulationHandler);
    }

    void CreatorState::HandleInput() {
        sf::Event event;
        while (this->_data->window.pollEvent(event))
        {
            if (sf::Event::Closed == event.type)
            {
                this->_data->window.close();
            }
            sf::Vector2i mousePosition = sf::Mouse::getPosition(this->_data->window);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                
                if(this->_mapView->isClicked(mousePosition)){
                    sf::Vector2f mousePositionRelativeToToolsView = this->_data->window.mapPixelToCoords(mousePosition, this->_mapView->GetView());
                    this->_creatorHandler->HandleInput(this->_mapView->HandleInput(mousePositionRelativeToToolsView));
                }
                
                else if (this->_toolsView->isClicked(mousePosition)) {
                    this->_toolsView->HandleInput();
                }
                else if (this->_camerasView->isClicked(mousePosition)) {
                    this->_camerasView->HandleInput();
                }
            }
            /*if (event.type == sf::Event::MouseButtonReleased ) {
                if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        this->_mapView->isReleased(mousePosition);
                }
            }*/
        }
    }

    void CreatorState::Update(float dt){

    }

    void CreatorState::Draw(float dt){
        this->_data->window.clear();
        
        this->_camerasView->Draw();
        this->_mapView->Draw();
        this->_toolsView->Draw();

        this->_data->window.display();
    }

}
