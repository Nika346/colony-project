//
// Created by Вероника on 06.07.2026.
//
#include "Modules.h"
#include "colonist.h"
#include "robot.h"
#include "enum_col_rob.h"
#include <iostream>
#include <algorithm>
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
bool ColonyModule::startRepair(Robot* repairRobot) {
    if (state == ModuleState::DESTROYED || state == ModuleState::WORKING) {
        return false;
    }
    state = ModuleState::UNDER_REPAIR;
    cout << "Модуль " << name << " начал ремонт (робот " << repairRobot->get_id() << ")" << endl;
    return true;
}

bool ColonyModule::performRepairStep(Robot* repairRobot) {
    if (state != ModuleState::UNDER_REPAIR) return false;
    repair(8);  // чиним по 8 единиц здоровья за шаг
    // Если полностью отремонтирован
    if (currentHealth >= maxHealth) {
        state = ModuleState::WORKING;
        cout << "Модуль " << name << " полностью отремонтирован!" << endl;
        return true; // ремонт завершён
    }
    return false;
}

// HabitatModule
HabitatModule::HabitatModule(int id, string name, int capacity)
    : ColonyModule(id, name, ModuleType::HABITAT, 100, 6),
      colonistCapacity(capacity), currentColonists(0) {
    setConsumption(ResourceType::OXYGEN, 5.0);
    setConsumption(ResourceType::WATER, 3.0);
    setConsumption(ResourceType::ENERGY, 10.0);
    spec = {};
}
void HabitatModule::addColonists(int count) {
    currentColonists = min(colonistCapacity, currentColonists + count);
}
void HabitatModule::removeColonists(int count) {
    currentColonists = max(0, currentColonists - count); // тут добавила присваивание
}


// Greenhouse
Greenhouse::Greenhouse(int id, string name)
    : ColonyModule(id, name, ModuleType::GREENHOUSE, 80, 9),
      efficiency(1.0f) {
    setConsumption(ResourceType::WATER, 8.0);
    setConsumption(ResourceType::ENERGY, 5.0);
    setProduction(ResourceType::FOOD, 9.0);
    setProduction(ResourceType::OXYGEN, 10.0);
    spec = {SPEC_BIOLOGIST, SPEC_REGULAR};
}
void Greenhouse::setWeatherEffect(float modifier) {
    efficiency = modifier;
}


// SolarPowerPlant
SolarPowerPlant::SolarPowerPlant(int id, string name, float baseProd)
    : ColonyModule(id, name, ModuleType::SOLAR_POWER, 60, 7),
      baseProduction(baseProd), weatherEfficiency(1.0f) {
    setProduction(ResourceType::ENERGY, baseProd * 2.0f);
    spec = {SPEC_ENGINEER};
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
    setProduction(ResourceType::ENERGY, baseProd * 2.0f);
    setConsumption(ResourceType::FUEL, fuelConsumptionRate);
    spec = {SPEC_ENGINEER};
}
void NuclearPowerPlant::addFuel(double amount) {
    fuelAmount = min(maxFuel, fuelAmount + amount);
}


// Mine
Mine::Mine(int id, string name, int productionRate)
    : ColonyModule(id, name, ModuleType::MINE, 90, 9),
      miningSpeed(productionRate) {
    setConsumption(ResourceType::ENERGY, 15.0);
    setProduction(ResourceType::ORE, productionRate);
    setProduction(ResourceType::FUEL, productionRate);
    spec = {SPEC_MINER, SPEC_REGULAR};
}


// WaterRecycler
WaterRecycler::WaterRecycler(int id, string name)
    : ColonyModule(id, name, ModuleType::WATER_RECYCLER, 70, 10),
      recyclingEfficiency(1.0f) {
    setConsumption(ResourceType::ENERGY, 10.0);
    setProduction(ResourceType::WATER, 36.0 * 2);
    spec = {SPEC_ENGINEER};
}


//Storage
Storage::Storage(int id, string name)
    : ColonyModule(id, name, ModuleType::STORAGE, 100, 5) {
    setConsumption(ResourceType::ENERGY, 2.0);
    spec = {};
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

//MedicalModule
MedicalModule::MedicalModule(int id, string name)
    : ColonyModule(id, name, ModuleType::MEDICAL, 80, 10),
      healingRate(5), patientsCount(0) {
    setConsumption(ResourceType::ENERGY, 8.0);
    setConsumption(ResourceType::MEDICINES, 0.5);
    spec = {SPEC_DOCTOR};
}

void MedicalModule::add_patient(shared_ptr<ColonistGroup> group) {
    patients.push_back(group);
}

void MedicalModule::treat_patients() {
    for (auto it = patients.begin(); it != patients.end(); ) {
        auto group = *it;
        if (group->get_state() == STATE_DEAD || group->get_count() == 0) {
            it = patients.erase(it);
            continue;
        }
        double newHealth = min(100.0, group->get_health() + 2.0);
        group->set_health(newHealth);
        if (newHealth >= 100.0) {
            group->set_state(STATE_WAITING);
            group->set_opportunity_to_work(true);
            if (group->get_start_module()) {
                group->move_to_module(group->get_start_module());
            }
            it = patients.erase(it);
        } else {
            ++it;
        }
    }
}

bool MedicalModule::has_patients() const {
    return !patients.empty();
}


// RepairBay
RepairBay::RepairBay(int id, string name, int capacity)
    : ColonyModule(id, name, ModuleType::REPAIR_BAY, 90, 8),
      robotsInRepair(), repairCapacity(capacity), repairSpeed(30) {
    setConsumption(ResourceType::ENERGY, 12.0);
    setConsumption(ResourceType::SPARE_PARTS, 2.0);
    spec = {SPEC_ENGINEER};
}
bool RepairBay::acceptRobotForRepair(Robot* robot) {
    if (!robot) return false;
    if (robotsInRepair.size() >= static_cast<size_t>(repairCapacity)) {
        return false;  // Нет места
    }
    robot->set_state(ROBOT_STATE_MAINTENANCE);
    robotsInRepair.push_back(robot);
    cout << "Робот принят в ремонтный цех. Всего в ремонте: "<< robotsInRepair.size() << "/" << repairCapacity << endl;
    return true;
}
int RepairBay::repairAllRobots() {
    if (robotsInRepair.empty()) return 0;
    int repairedCount = 0;
    // Идём по списку роботов и ремонтируем каждого
    for (auto it = robotsInRepair.begin(); it != robotsInRepair.end(); ) {
        Robot* robot = *it;
        // Уменьшаем износ на repairSpeed
        double newWear = max(0.0, robot->get_wear_level() - repairSpeed);
        robot->repair(repairSpeed);  // Вызываем метод робота
        // Если износ стал достаточно низким — отпускаем робота
        if (newWear < 30) {
            // МЕНЯЕМ СТАТУС НА "ОЖИДАЕТ ЗАДАЧУ"
            robot->set_state(ROBOT_STATE_WAITING_FOR_TASK);
            it = robotsInRepair.erase(it);  // Удаляем из списка
            repairedCount++;
        } else {
            ++it;  // Робот остаётся в ремонте
        }
    }
    return repairedCount;
}

void RepairBay::removeRobotFromRepair(Robot* robot) {
    if (!robot) return;
    auto it = find(robotsInRepair.begin(), robotsInRepair.end(), robot);
    if (it != robotsInRepair.end()) {
        robotsInRepair.erase(it);
    }
}
