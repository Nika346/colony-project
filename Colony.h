#pragma once
#include "Modules.h"
#include "TransportRoute.h"
#include <memory>
#include <random>
using namespace std;

class Colony{
private:
    vector<shared_ptr<ColonyModule>> modules;
    vector<shared_ptr<TransportRoute>> routes;
    mt19937 rng;  // генератор случайных чисел 
public:
    Colony();
    void createModules();
    void createTransportNetwork(int totalRoutes); 
    const vector<shared_ptr<ColonyModule>>& getModules() const { return modules; }
    const vector<shared_ptr<TransportRoute>>& getRoutes() const { return routes; }

    // доп задания 12,13,14
    void save(const string& file_name) const;
    void download(const string& file_name);
    void export_csv(const string& file_name) const;
    void export_dot(const string& file_name) const;
};
