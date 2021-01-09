#pragma once
#include "Vehicles/Vehicle.h"

namespace ZPR {
    class SimulationObserver
    {
    public:
        virtual void UpdateVehicles(std::vector<std::shared_ptr<Vehicle>> vehicles) = 0;
        virtual void UpdateIsSimulating(bool isSimulating) = 0;
        virtual void UpdateCarsLabel(int whichLabel) = 0;
        virtual void UpdateTrucksLabel(int whichLabel) = 0;
        virtual ~SimulationObserver() {}
    };
}

