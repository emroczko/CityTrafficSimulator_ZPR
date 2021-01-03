
#include "CreatorSubject.h" 

namespace ZPR {
	/*Dodaje obserwatora*/
	void CreatorSubject::add(std::shared_ptr<CreatorObserver> obs)
	{
		this->_observers.push_back(obs);
	}
	/*Powiadamia obserwatorów o zmianie koordynatów obecnie wybranek komórki*/
	void CreatorSubject::NotifySelectedCell(sf::Vector2i coords)
	{
		for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
			obs->UpdateSelectedCell(coords);
		}
	}
	/*Powiadamia obserwatoró o zmianie stanu kontenera zawierającego komórki*/
	void CreatorSubject::NotifyCells(std::vector<Cell> cells)
	{
		for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
			obs->UpdateCells(cells);
		}
	}
	/*Powiadamia obserwatorów o zmianie stanu rysowania drogi (włączony/wyłączony)*/
	void CreatorSubject::NotifyIsDrawingRoad(bool isDrawingRoad)
	{
		for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
			obs->UpdateIsDrawingRoad(isDrawingRoad);
		}
	}
	/*Powiadamia obserwatorów o zmianie stanu usuwania drogi (włączony/wyłączony)*/
    void CreatorSubject::NotifyIsDeletingRoad(bool isDeletingRoad)
    {
        for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
            obs->UpdateIsDeletingRoad(isDeletingRoad);
        }
    }
}
