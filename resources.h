//
// Created by Вероника on 06.07.2026.
//

#ifndef MYPROJECT_RESOURSES_H
#define MYPROJECT_RESOURSES_H
#pragma once
#include "modules.h"
#include <map>
#include <string>
struct ResourcesAvailability {
        bool oxygen_ok;
        bool water_ok;
        bool food_ok;
};

class Resource {
private:
        ResourceType type;
        std::string name;
        double currentAmount;
        double maxCapacity;
        double productionRate;      // скорость производства за шаг
        double consumptionRate;     // скорость потребления за шаг
        double minAllowedLevel;     // минимально допустимый уровень
        double criticalLevel;       // критический уровень
public:
        Resource();
        Resource(ResourceType type, string name, double maxCap,
                          double minLevel = 0, double criticalLevel = 10);
        bool consume(double amount);
        bool produce(double amount);
        // Проверки
        bool isCritical() const;
        bool isBelowMinimum() const;
        bool isEmpty() const;
        bool isFull() const;


        ResourceType getType() const { return type; }
        string getName() const { return name; }
        double getCurrentAmount() const { return currentAmount; }
        double getMaxCapacity() const { return maxCapacity; }
        double getProductionRate() const { return productionRate; }
        double getConsumptionRate() const { return consumptionRate; }
        double getMinAllowedLevel() const { return minAllowedLevel; }
        double getCriticalLevel() const { return criticalLevel; }


        void setCurrentAmount(double amount);
        void setMaxCapacity(double capacity);
        void setProductionRate(double rate);
        void setConsumptionRate(double rate);
        double getPercentage() const;  // процент заполнения
        void resetRates();             // сбросить скорости на 0
};


// Менеджер ресурсов колонии
class ColonyResourceManager {
private:
        map<ResourceType, Resource> resources;
public:
        ColonyResourceManager();
        void initializeResources();
        Resource& get_resource(ResourceType type){return resources[type];} // мне нужна эта строчка для получения ресурсов
        ResourcesAvailability checkAndConsumeForGroup(double oxygenNeeded, double waterNeeded, double foodNeeded);
};


#endif //MYPROJECT_RESOURSES_H
