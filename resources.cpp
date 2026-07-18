//
// Created by Вероника on 06.07.2026.
//
#include "resources.h"
#include <map>
using namespace std;


// Resource
Resource::Resource()
    : type(ResourceType::OXYGEN), name(""), 
      currentAmount(0), maxCapacity(1000),
      productionRate(0), consumptionRate(0),
      minAllowedLevel(0), criticalLevel(10) {}


Resource::Resource(ResourceType type, string name, double maxCap,
                   double minLevel, double critLevel)
    : type(type), name(name), currentAmount(0), maxCapacity(maxCap),
      productionRate(0), consumptionRate(0),
      minAllowedLevel(minLevel), criticalLevel(critLevel) {}


bool Resource::consume(double amount) {
    if (currentAmount < amount) {
        return false;  // Недостаточно ресурса
    }
    currentAmount -= amount;
    return true;
}
bool Resource::produce(double amount) {
    if (currentAmount + amount > maxCapacity) {
        currentAmount = maxCapacity;
        return false;  // Переполнение
    }
    currentAmount += amount;
    return true;
}
bool Resource::isCritical() const {
    return currentAmount <= criticalLevel;
}
bool Resource::isBelowMinimum() const {
    return currentAmount < minAllowedLevel;
}
bool Resource::isEmpty() const {
    return currentAmount <= 0;
}
bool Resource::isFull() const {
    return currentAmount >= maxCapacity;
}
void Resource::setCurrentAmount(double amount) {
    currentAmount = amount;
}
void Resource::setMaxCapacity(double capacity) {
    maxCapacity = capacity;
    if (currentAmount > capacity) {
        currentAmount = capacity;
    }
}
void Resource::setProductionRate(double rate) {
    productionRate = rate;
}
void Resource::setConsumptionRate(double rate) {
    consumptionRate = rate;
}
double Resource::getPercentage() const {
    if (maxCapacity == 0) return 0.0;
    return (currentAmount / maxCapacity) * 100.0;
}
void Resource::resetRates() {
    productionRate = 0;
    consumptionRate = 0;
}


// ColonyResourceManager
ColonyResourceManager::ColonyResourceManager() {
    initializeResources();
}
void ColonyResourceManager::initializeResources() {
    // Инициализация всех типов ресурсов


    resources[ResourceType::OXYGEN] = Resource(
        ResourceType::OXYGEN, "Кислород", 
        1000.0,  // макс. запас
        100.0,   // мин. уровень
        200.0    // критический уровень
    );
    resources[ResourceType::OXYGEN].setCurrentAmount(500.0);
    resources[ResourceType::WATER] = Resource(
        ResourceType::WATER, "Вода",
        800.0, 50.0, 150.0
    );
    resources[ResourceType::WATER].setCurrentAmount(400.0);
    resources[ResourceType::FOOD] = Resource(
        ResourceType::FOOD, "Пища",
        600.0, 50.0, 100.0
    );
    resources[ResourceType::FOOD].setCurrentAmount(300.0);
    resources[ResourceType::ENERGY] = Resource(
        ResourceType::ENERGY, "Энергия",
        500.0, 0.0, 50.0
    );
    resources[ResourceType::ENERGY].setCurrentAmount(250.0);
    resources[ResourceType::FUEL] = Resource(
        ResourceType::FUEL, "Топливо",
        200.0, 0.0, 20.0
    );
    resources[ResourceType::FUEL].setCurrentAmount(100.0);
    resources[ResourceType::BUILDING_MATERIALS] = Resource(
        ResourceType::BUILDING_MATERIALS, "Стройматериалы",
        500.0, 0.0, 50.0
    );
    resources[ResourceType::BUILDING_MATERIALS].setCurrentAmount(250.0);
    resources[ResourceType::MEDICINES] = Resource(
        ResourceType::MEDICINES, "Mедекоменты",
        100.0, 0.0, 10.0
    );
    resources[ResourceType::SPARE_PARTS] = Resource(
    ResourceType::SPARE_PARTS, "Запасные части",
    300.0, 0.0, 30.0
);
    resources[ResourceType::SPARE_PARTS].setCurrentAmount(150.0);
    resources[ResourceType::MEDICINES].setCurrentAmount(50.0);
    resources[ResourceType::ORE] = Resource(
        ResourceType::ORE, "Руда",
        1000.0, 0.0, 0.0
    );
    resources[ResourceType::ORE].setCurrentAmount(500.0);
}
ResourcesAvailability ColonyResourceManager::checkAndConsumeForGroup(double oxygenNeeded, double waterNeeded, double foodNeeded) {
    ResourcesAvailability result;
    result.oxygen_ok = resources[ResourceType::OXYGEN].consume(oxygenNeeded);
    result.water_ok = resources[ResourceType::WATER].consume(waterNeeded);
    result.food_ok = resources[ResourceType::FOOD].consume(foodNeeded);
    return result;
}
