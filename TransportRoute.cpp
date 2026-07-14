#include "TransportRoute.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <utility>
using namespace std;

// ==================== TransportRoute ====================
TransportRoute::TransportRoute(int id, ColonyModule* start, ColonyModule* end, double length)
    : id(id), startModule(start), endModule(end), length(length),
      travelTime(length / 10.0),          // инициализация скорости 10 м/ шаг
      capacity(10),  // пропускная способность по умолчанию
      state(RouteState::OPERATIONAL),
      maxHealth(100),
      currentHealth(100),
      usableByRobots(true),
      usableByColonists(true) {}
void TransportRoute::takeDamage(int amount) {
    if (state == RouteState::DESTROYED) return;
    currentHealth = max(0, currentHealth - amount);
    if (currentHealth == 0) {
        state = RouteState::DESTROYED;
        usableByRobots = false;
        usableByColonists = false;
        cout << "Переход "<< id << " РАЗРУШЕН!\n";
    }
    else if (currentHealth < maxHealth * 0.7) {
        state = RouteState::DAMAGED;
    }
}
void TransportRoute::repair(int amount) {
    currentHealth = min(maxHealth, currentHealth + amount);
    if (currentHealth >= maxHealth*0.7) {
        state = RouteState::OPERATIONAL;
    }
}
bool TransportRoute::isPassable() const {
    return state != RouteState::DESTROYED && (usableByRobots || usableByColonists);
}

