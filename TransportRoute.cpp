#include "TransportRoute.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <utility>
#include <limits>
#include "robot.h"
#include "Weather.h"
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
// Вычисление времени прохождения
double TransportRoute::calculateTravelTime(const Robot& robot, const Weather& weather) const {
    // Если путь недоступен — возвращаем бесконечность (не вызывает переполнения), те по этому пути не стоит ехать
    if (!isPassable()) {
        return numeric_limits<double>::infinity();
    }
    // Получаем скорость робота с учётом погоды
    double robotSpeed = robot.get_now_speed(weather);
    // Если робот не может двигаться (скорость = 0)
    if (robotSpeed <= 0.0) {
        return numeric_limits<double>::infinity();
    }
    // Базовое время = длина пути / скорость
    double baseTime = length / robotSpeed;
    // Коэффициент замедления в зависимости от состояния пути
    double damageModifier = 1.0;
    switch (state) {
        case RouteState::OPERATIONAL:
            damageModifier = 1.0;   // без замедления
            break;
        case RouteState::DAMAGED:
            damageModifier = 1.5;   // на 50% медленнее
            break;
        case RouteState::UNDER_REPAIR:
            damageModifier = 2.0;   // в 2 раза медленнее
            break;
        case RouteState::OVERLOADED:
        case RouteState::DESTROYED:
            return numeric_limits<double>::infinity();  // недоступен
            break;
    }
    return baseTime * damageModifier;
}
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
bool TransportRoute::isPassable(bool forRobots) const {
    // Разрушен или перегружен — недоступен
    if (state == RouteState::DESTROYED || state == RouteState::OVERLOADED) {
        return false;
    }
    // Проверяем, доступен ли для конкретного пользователя (true - робот, false - солонист)
    if (forRobots) {
        return usableByRobots;
    } else {
        return usableByColonists;
    }
}

