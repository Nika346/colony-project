//
// Created by Вероника on 06.07.2026.
//
#include "modules.h"
#include <iostream>
using namespace std;
ColonyModule::ColonyModule(int id, string name, ModuleType type,
                           int maxHealth, int importance)
    : id(id), name(name), type(type), state(ModuleState::WORKING),
      maxHealth(maxHealth), currentHealth(maxHealth),
      importanceLevel(importance), workersCount(0), repairTime(0) {}


void ColonyModule::takeDamage(int amount) {
    if (state == ModuleState::DESTROYED) return;
    currentHealth = max(0, currentHealth - amount);
    if (currentHealth == 0) {
        state = ModuleState::DESTROYED;
        cout<<"Модуль "<< name << " РАЗРУШЕН!";
    } else if (currentHealth < maxHealth) {
        state = ModuleState::DAMAGED;
    }
}
void ColonyModule::repair(int amount) {
    if (state == ModuleState::DESTROYED) return;
    currentHealth = min(maxHealth, currentHealth + amount);
    if (currentHealth == maxHealth) {
        state = ModuleState::WORKING;
    }
}
bool ColonyModule::isOperational() const {
    return state != ModuleState::DESTROYED && state != ModuleState::OFFLINE;
}
void ColonyModule::setConsumption(ResourceType type, double amount) {
    consumptionRate[type] = amount;
}
void ColonyModule::setProduction(ResourceType type, double amount) {
    productionRate[type] = amount;
}
double ColonyModule::getConsumptionFor(ResourceType type) const {
    auto it = consumptionRate.find(type);
    return (it != consumptionRate.end()) ? it->second : 0.0;
}
double ColonyModule::getProductionFor(ResourceType type) const {
    auto it = productionRate.find(type);
    return (it != productionRate.end()) ? it->second : 0.0;
}


// HabitatModule
HabitatModule::HabitatModule(int id, string name, int capacity)
    : ColonyModule(id, name, ModuleType::HABITAT, 100, 10),
      colonistCapacity(capacity), currentColonists(0) {
    setConsumption(ResourceType::OXYGEN, 5.0);
    setConsumption(ResourceType::WATER, 3.0);
    setConsumption(ResourceType::ENERGY, 10.0);
}
void HabitatModule::addColonists(int count) {
    currentColonists = min(colonistCapacity, currentColonists + count);
}
void HabitatModule::removeColonists(int count) {
    currentColonists = max(0, currentColonists - count); // тут добавила присваивание
}


// Greenhouse
Greenhouse::Greenhouse(int id, string name)
    : ColonyModule(id, name, ModuleType::GREENHOUSE, 80, 7),
      efficiency(1.0f) {
    setConsumption(ResourceType::WATER, 8.0);
    setConsumption(ResourceType::ENERGY, 5.0);
    setProduction(ResourceType::FOOD, 6.0);
    setProduction(ResourceType::OXYGEN, 2.0);
}
void Greenhouse::setWeatherEffect(float modifier) {
    efficiency = modifier;
}


// SolarPowerPlant
SolarPowerPlant::SolarPowerPlant(int id, string name, float baseProd)
    : ColonyModule(id, name, ModuleType::SOLAR_POWER, 60, 8),
      baseProduction(baseProd), weatherEfficiency(1.0f) {
    setProduction(ResourceType::ENERGY, baseProd);
}
void SolarPowerPlant::setWeatherModifier(float modifier) {
    weatherEfficiency = modifier;
}
float SolarPowerPlant::getEnergyProduction() const {
    return baseProduction * weatherEfficiency;
}


// NuclearPowerPlant
NuclearPowerPlant::NuclearPowerPlant(int id, string name, float baseProd)
    : ColonyModule(id, name, ModuleType::NUCLEAR_POWER, 120, 9),
      baseProduction(baseProd), fuelAmount(100.0), maxFuel(100.0),
      fuelConsumptionRate(5.0) {
    setProduction(ResourceType::ENERGY, baseProd);
    setConsumption(ResourceType::FUEL, fuelConsumptionRate);
}
void NuclearPowerPlant::addFuel(double amount) {
    fuelAmount = min(maxFuel, fuelAmount + amount);
}


// Mine
Mine::Mine(int id, string name, int productionRate)
    : ColonyModule(id, name, ModuleType::MINE, 90, 6),
      miningSpeed(productionRate) { // я тут исправила oreProductionRate(productionRate) на то что написала как в объявлении
    setConsumption(ResourceType::ENERGY, 15.0);
    setProduction(ResourceType::ORE, productionRate);
}


// WaterRecycler
WaterRecycler::WaterRecycler(int id, string name)
    : ColonyModule(id, name, ModuleType::WATER_RECYCLER, 70, 9),
      recyclingEfficiency(1.0f) {
    setConsumption(ResourceType::ENERGY, 10.0);
    setProduction(ResourceType::WATER, 12.0);
}


//Storage
Storage::Storage(int id, string name)
    : ColonyModule(id, name, ModuleType::STORAGE, 100, 5) {
    setConsumption(ResourceType::ENERGY, 2.0);
    for (int i = 0; i <= static_cast<int>(ResourceType::ORE); ++i) {
        ResourceType type = static_cast<ResourceType>(i);
        storageCapacity[type] = 1000.0;
        currentStorage[type] = 0.0;
    }
}
bool Storage::storeResource(ResourceType type, double amount) {
    if (currentStorage[type] + amount > storageCapacity[type]) {
        return false;  // Нет места
    }
    currentStorage[type] += amount;
    return true;
}
bool Storage::withdrawResource(ResourceType type, double amount) {
    if (currentStorage[type] < amount) {
        return false;  // Недостаточно ресурса
    }
    currentStorage[type] -= amount;
    return true;
}
double Storage::getStoredAmount(ResourceType type) const {
    auto it = currentStorage.find(type);
    return (it != currentStorage.end()) ? it->second : 0.0;
}
void Storage::setCapacity(ResourceType type, double capacity) {
    storageCapacity[type] = capacity;
}



MedicalModule::MedicalModule(int id, string name)
    : ColonyModule(id, name, ModuleType::MEDICAL, 80, 10),
      healingRate(5), patientsCount(0) {
    setConsumption(ResourceType::ENERGY, 8.0);
    setConsumption(ResourceType::MEDICINES, 0.5);
}
void MedicalModule::treatPatients(int count) {
    patientsCount = max(0, patientsCount - count);
}