/**
 * vehicle.hpp
 * Header of Vehicle class.
 */

#pragma once
#include "SFML/Graphics.hpp"
#include "../definitions.hpp"
#include "../components/cell.hpp"
#include <chrono>


namespace zpr {

    /**
     * Class responsible for handling vehicles actions eg. moving, stopping, checking collisions. It is base class for Car and Truck.
     */

	class Vehicle : public sf::Drawable
	{
	public:
		virtual ~Vehicle() {};
		sf::RectangleShape getShape();
		void move();
		void checkOnWhichCell();
		void checkTurn();
		void stopVehicle();
		void checkVehicleStopped();
		void unblockVehicle();
		void noColision();
		bool checkColision(std::shared_ptr<Vehicle> vehicle);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		int x_, y_, speed_;
		int roadSize_, sidewalkSize_, roadStripesSize_;
		int cellSize_;
		int stopCounter_;
		bool seenByCamera_[3];
		std::vector<sf::RectangleShape> roads_;
		std::string direction_, previousDirection_;
		sf::RectangleShape shape_, colisionBox_;
		std::shared_ptr<sf::RectangleShape> currentRoad_, previousRoad_;

	private:
		void updatePosition();
		void updateColisionBoxPosition();
		bool canTurnBack();
		void turnBack();
		void choseFromOneRoads(std::shared_ptr<sf::RectangleShape> north, std::shared_ptr<sf::RectangleShape> south, std::shared_ptr<sf::RectangleShape> east, std::shared_ptr<sf::RectangleShape> west);
		void choseFromTwoRoads(std::shared_ptr<sf::RectangleShape> north, std::shared_ptr<sf::RectangleShape> south, std::shared_ptr<sf::RectangleShape> east, std::shared_ptr<sf::RectangleShape> west);
		void choseFromThreeRoads(std::shared_ptr<sf::RectangleShape> north, std::shared_ptr<sf::RectangleShape> south, std::shared_ptr<sf::RectangleShape> east, std::shared_ptr<sf::RectangleShape> west);
		void updateDirection(std::string direction);
	};
}
