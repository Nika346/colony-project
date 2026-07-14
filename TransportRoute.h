#pragma once
#include "Modules.h"
#include <memory>
#include <random>
using namespace std;

enum class RouteState {
    OPERATIONAL,      // исправен
    DAMAGED,          // частично повреждён
    OVERLOADED,       // перегружен
    UNDER_REPAIR,     // ремонтируется
    DESTROYED         // разрушен
};

class TransportRoute {
private:
    int id;
    ColonyModule* startModule;  // указатель на начальный модуль
    ColonyModule* endModule;    // указатель на конечный модуль
    double length;              // длина
    double travelTime;          // не было этого поля
    int capacity;               // пропускная способность
    RouteState state;           // состояние перехода
    int maxHealth;              // максимальная прочность
    int currentHealth;          // текущая прочность
    bool usableByRobots;        // можно использовать роботами
    bool usableByColonists;     // можно использовать колонистами
public:
    TransportRoute(int id, ColonyModule* start, ColonyModule* end, double length);
    // Вычислить время прохождения для конкретного робота в конкретных условиях
    double calculateTravelTime(const Robot& robot, const Weather& weather) const;
    bool isAccessible() const;   // Проверка, доступен ли путь (не разрушен и не перегружен)
    void takeDamage(int amount);
    void repair(int amount);
    bool isPassable(bool forRobots=true) const;

    int getId() const { return id; }
    ColonyModule* getStartModule() const { return startModule; }
    ColonyModule* getEndModule() const { return endModule; }
    double getLength() const { return length; }
    double getTravelTime() const { return travelTime; }
    int getCapacity() const { return capacity; }
    RouteState getState() const { return state; }
    int getMaxHealth() const { return maxHealth; }
    int getCurrentHealth() const { return currentHealth; }
    bool isUsableByRobots() const { return usableByRobots; }
    bool isUsableByColonists() const { return usableByColonists; }

    void setState(RouteState newState) { state = newState; }
    void setCapacity(int cap) { capacity = cap; }
    void setUsableByRobots(bool usable) { usableByRobots = usable; }
    void setUsableByColonists(bool usable) { usableByColonists = usable; }
};

