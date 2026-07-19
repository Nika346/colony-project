//
// Created by Вероника on 06.07.2026.
//

#ifndef MYPROJECT_MODULES_H
#define MYPROJECT_MODULES_H
#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
using namespace std;

class Robot;
class ColonistGroup;


// Перечисление состояний модуля
enum class ModuleState {
    WORKING,         // исправен
    DAMAGED,         // повреждён
    UNDER_REPAIR,    // ремонтируется
    OFFLINE,         // отключён
    DESTROYED        // разрушен
};


// Перечисление типов модулей
enum class ModuleType {     //добавить ремонтный цех
    HABITAT,           // жилой модуль
    GREENHOUSE,        // теплица
    SOLAR_POWER,       // солнечная электростанция
    NUCLEAR_POWER,     // ядерная электростанция
    MINE,              // шахта
    WATER_RECYCLER,    // очистка воды
    STORAGE,           // склад
    MEDICAL,           // медицинский модуль
    REPAIR_BAY         // ремонтный цех
};

// Перечисление типов ресурсов
enum class ResourceType {
    OXYGEN,            // кислород
    WATER,             // вода
    FOOD,              // пища
    ENERGY,            // энергия
    FUEL,              // топливо
    BUILDING_MATERIALS,// стройматериалы 
    MEDICINES,         // медикоменты 
    SPARE_PARTS,       // запчасти
    ORE                // руда
};

// Базовый класс модуля
class ColonyModule {
protected:
    int id;
    string name;
    ModuleType type;
    ModuleState state;
    int maxHealth;
    int currentHealth;
    int importanceLevel;       // 1-10
    int workersCount;
    int repairTime;
    // Потребление и производство ресурсов (ключ - тип ресурса, значение - количество)
    map<ResourceType, double> consumptionRate;
    map<ResourceType, double> productionRate;
public:
    vector<shared_ptr<ColonyModule>> ModulesRoutes;
    ColonyModule(int id, string name, ModuleType type,
                 int maxHealth, int importance);
    virtual ~ColonyModule()=default;
    // Базовые методы
    virtual void takeDamage(int amount);
    virtual void repair(int amount);
    virtual bool isOperational() const;
    // Геттеры
    int getId() const { return id; }
    string getName() const { return name; }
    ModuleType getType() const { return type; }
    ModuleState getState() const { return state; }
    int getCurrentHealth() const { return currentHealth; }
    int getMaxHealth() const { return maxHealth; }
    int getImportanceLevel() const { return importanceLevel; }
    int getWorkersCount() const { return workersCount; }
    map<ResourceType, double> getConsumption() const { return consumptionRate; }
    map<ResourceType, double> getProduction() const { return productionRate; }
    // Сеттеры
    void setState(ModuleState newState) { state = newState; }
    void setWorkersCount(int count) { workersCount = count; }
    // Методы для работы с ресурсами
    void setConsumption(ResourceType type, double amount);
    void setProduction(ResourceType type, double amount);
    double getConsumptionFor(ResourceType type) const;
    double getProductionFor(ResourceType type) const;
};


// ==================== НАСЛЕДНИКИ ====================
// 1. Жилой модуль
class HabitatModule : public ColonyModule {
private:
    int colonistCapacity;
    int currentColonists;
public:
    HabitatModule(int id, string name, int capacity = 10);
    int getCurrentColonists() const { return currentColonists; }
    void addColonists(int count);
    void removeColonists(int count);
};
// 2. Теплица
class Greenhouse : public ColonyModule {
private:
    float efficiency;
public:
    Greenhouse(int id, string name);
    void setWeatherEffect(float modifier);  // 0.0 - 1.0
    float getEfficiency() const { return efficiency; }
};


// 3. Солнечная электростанция
class SolarPowerPlant : public ColonyModule {
private:
    float baseProduction;
    float weatherEfficiency;
public:
    SolarPowerPlant(int id, string name, float baseProd = 50.0f);
    void setWeatherModifier(float modifier);
    float getEnergyProduction() const;
};
// 4. Ядерная электростанция
class NuclearPowerPlant : public ColonyModule {
private:
    float baseProduction;
    double fuelAmount;


double maxFuel;
    double fuelConsumptionRate;
public:
    NuclearPowerPlant(int id, string name, float baseProd = 80.0f);
    void addFuel(double amount);
    double getFuelAmount() const { return fuelAmount; }
    bool hasFuel() const { return fuelAmount > 0; }
};
// 5. Шахта
class Mine : public ColonyModule {
private:
    int miningSpeed;
public:
    Mine(int id, string name, int speed = 5);
    int getMiningSpeed() const { return miningSpeed; }
};
// 6. Система очистки воды
class WaterRecycler : public ColonyModule {
private:
    float recyclingEfficiency;
public:
    WaterRecycler(int id, string name);
    void setEfficiency(float eff) { recyclingEfficiency = eff; }
};
// 7. Склад
class Storage : public ColonyModule {
private:
    map<ResourceType, double> storageCapacity;
    map<ResourceType, double> currentStorage;
public:
    Storage(int id, string name);
    bool storeResource(ResourceType type, double amount);
    bool withdrawResource(ResourceType type, double amount);
    double getStoredAmount(ResourceType type) const;
};
// 8. Медицинский модуль
class MedicalModule : public ColonyModule {
private:
    int healingRate;
    int patientsCount;
    vector<shared_ptr<ColonistGroup>> patients;
public:
    MedicalModule(int id, string name);
    int getPatientsCount() const { return patientsCount; }
    void addPatient(int count) { patientsCount += count; }
    void add_patient(shared_ptr<ColonistGroup> group);
    void treat_patients();
    bool has_patients() const;
};

// 9. Ремонтный цех
class RepairBay : public ColonyModule {
private:
    vector<Robot*> robotsInRepair; // список роботов в ремонте (вместо счётчика)
    int repairCapacity;           // сколько роботов можно чинить одновременно
    int repairSpeed;              // сколько здоровья восстанавливается за шаг
    
public:
    RepairBay(int id, string name, int capacity = 3);
    bool acceptRobotForRepair(Robot* robot);  // Принять робота в ремонт
    int repairAllRobots();         // Отремонтировать робота (возвращает количество восстановленного здоровья)
    bool hasCapacity() const { return robotsInRepair.size() < repairCapacity; } // Есть ли свободное место?
    int getRobotsInRepair() const { return robotsInRepair.size(); }
    int getRepairCapacity() const { return repairCapacity; }
    const vector<Robot*>& getRobotsInRepairList() const { return robotsInRepair; } // Получить список роботов в ремонте
    void removeRobotFromRepair(Robot* robot);// Удалить робота из списка (когда ремонт завершён)
};

#endif //MYPROJECT_MODULES_H
