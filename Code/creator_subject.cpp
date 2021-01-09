
#include "creator_subject.hpp" 

namespace ZPR {
	/*Dodaje obserwatora*/
	void CreatorSubject::add(std::shared_ptr<CreatorObserver> obs)
	{
		this->_observers.push_back(obs);
	}
	/*Powiadamia obserwator�w o zmianie koordynat�w obecnie wybranek kom�rki*/
	void CreatorSubject::NotifySelectedCell(sf::Vector2i coords)
	{
		for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
			obs->UpdateSelectedCell(coords);
		}
	}
	/*Powiadamia obserwator� o zmianie stanu kontenera zawieraj�cego kom�rki*/
	void CreatorSubject::NotifyCells(std::vector<Cell> cells)
	{
		for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
			obs->UpdateCells(cells);

		}
	}
   
	/*Powiadamia obserwator�w o zmianie stanu rysowania drogi (w��czony/wy��czony)*/
	void CreatorSubject::NotifyIsDrawingRoad(bool isDrawingRoad)
	{
		for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
			obs->UpdateIsDrawingRoad(isDrawingRoad);
		}
	}
	/*Powiadamia obserwator�w o zmianie stanu usuwania drogi (w��czony/wy��czony)*/
    void CreatorSubject::NotifyIsDeletingRoad(bool isDeletingRoad)
    {
        for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
            obs->UpdateIsDeletingRoad(isDeletingRoad);
        }
    }
    void CreatorSubject::NotifyIsAddingCamera(bool isAddingCamera, int whichCamera, int row, int col)
    {
        for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
            obs->UpdateIsAddingCamera(isAddingCamera, whichCamera, row, col);
        }
    }
    void CreatorSubject::NotifyIsDeletingCamera(int whichCamera)
    {
        for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
            obs->UpdateIsDeletingCamera(whichCamera);
        }
    }
    void CreatorSubject::NotifySave()
    {
        for (std::shared_ptr<CreatorObserver> obs : this->_observers) {
            obs->SaveToFile();
        }
    }
    
}