#pragma once
#include "Cell.h"
#include "Vehicles/Vehicle.h"
namespace ZPR {
	class CreatorObserver
	{
	public:
		virtual void UpdateSelectedCell(sf::Vector2i coords) = 0;
		virtual void UpdateCells(std::vector<Cell> cells) = 0;
        virtual void SaveToFile() = 0;
		virtual void UpdateIsDrawingRoad(bool isDrawingRoad) = 0;
        virtual void UpdateIsDeletingRoad(bool isDeletingRoad) = 0;
		virtual ~CreatorObserver() {}
	};
}

