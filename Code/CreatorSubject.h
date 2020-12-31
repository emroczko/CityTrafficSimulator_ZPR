#pragma once
#include "CreatorObserver.h"
#include "Cell.h"
#include <vector>

namespace ZPR {
	class CreatorSubject
	{
	public:
		void add(std::shared_ptr<CreatorObserver> obs);
		void NotifySelectedCell(sf::Vector2i coords);
		void NotifyCells(std::vector<Cell> cells);
		void NotifyIsDrawingRoad(bool isDrawingRoad);
        void NotifyIsDeletingRoad(bool isDeletingRoad);
        void NotifyIsSimulating(bool isSimulating);
		void NotifyVehicles(std::vector<std::shared_ptr<Vehicle>> vehicles);
		virtual ~CreatorSubject() {}
	private:
		std::vector<std::shared_ptr<CreatorObserver> > _observers;
	};
}

