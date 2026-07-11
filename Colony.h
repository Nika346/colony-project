#pragma once
#include "Modules.h"
#include "TransportRoute.h"
#include <memory>
#include <random>
#include <vector>
using namespace std;

class Colony{
private:
    vector<shared_ptr<ColonyModule>> modules;
    vector<shared_ptr<TransportRoute>> routes;
    mt19937 rng;  // генератор случайных чисел
public:
    Colony();
    void createModules(
        int habitatCount,
        int greenhouseCount,
        int solarCount,
        int nuclearCount,
        int mineCount,
        int waterRecyclerCount,
        int storageCount,
        int medicalCount,
        int repairBayCount
    );
    void createTransportNetwork(int totalRoutes);
    const vector<shared_ptr<ColonyModule>>& getModules() const { return modules; }
    const vector<shared_ptr<TransportRoute>>& getRoutes() const { return routes; }
};
