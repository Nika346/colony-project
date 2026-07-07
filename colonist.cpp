//
// Created by Вероника on 06.07.2026.
//
#include "colonist.h"
#include <algorithm>  // для std::max, std::min
using namespace std;
/*
 * Конструктор группы колонистов.
 * Инициализирует все поля начальными значениями.
 */
ColonistGroup::ColonistGroup(string& group_id, Colonist_spec specialization, int count,
                             ModuleType current_module, Task_type current_task,
                             double health, double fatigue,
                             const Resources_consumption& consumption)
    : group_id(group_id),
      count(max(0, count)),  // Количество не может быть отрицательным
      specialization(specialization),
      health(max(0.0, min(100.0, health))),  // Здоровье в диапазоне 0-100
      fatigue(max(0.0, min(100.0, fatigue))), // Усталость в диапазоне 0-100
      state(STATE_WAITING),  // Изначально ожидают назначения
      current_task(current_task),
      current_module(current_module),
      resources_consumption(consumption),
      opportunity_to_work(true)  // Изначально могут работать
{
    // Если колонистов нет, состояние - погибшие
    if (count <= 0) {
        state = STATE_DEAD;
        opportunity_to_work = false;
    }
}
/*
 * Обновление здоровья колонистов.
 * Проверяет доступность ресурсов и уровень усталости.
 * Возвращает количество погибших (если здоровье упало до 0).
 */
int ColonistGroup::update_health(bool oxygen_available, bool water_available, bool food_available) {
    // Если группа уже мертва, ничего не делаем
    if (state == STATE_DEAD || count <= 0) {
        return 0;
    }
    // Коэффициент ухудшения здоровья из-за недостатка ресурсов
    double health_penalty = 0.0;
    // Проверяем каждый ресурс
    if (!oxygen_available) {
        health_penalty += 1.5;  // Нехватка кислорода сильно влияет
        cout << "Предупреждение: Нехватка кислорода в группе " << group_id << "!" << endl;
    }
    if (!water_available) {
        health_penalty += 1.0;
        cout << "Предупреждение: Нехватка воды в группе " << group_id << "!" << endl;
    }
    if (!food_available) {
        health_penalty += 0.8;
        cout << "Предупреждение: Нехватка пищи в группе " << group_id << "!" << endl;
    }
    // Усталость тоже влияет на здоровье
    if (fatigue > 80) {
        health_penalty += 0.5;  // Сильная усталость ухудшает здоровье
    }
    // Применяем ухудшение здоровья
    if (health_penalty > 0) {
        health = max(0.0, health - health_penalty);
        // Если здоровье упало до 0 - колонисты умирают
        if (health == 0) {
            int dead = count;  // Все колонисты в группе умирают
            state = STATE_DEAD;
            opportunity_to_work = false;
            count = 0;
            cout << "Группа колонистов " << group_id << " погибла от нехватки ресурсов!" << endl;
            return dead;
        }
    }
    // Если здоровье ниже 30%, колонисты не могут работать
    if (health < 30) {
        opportunity_to_work = false;
    } else {
        opportunity_to_work = true;
    }
    return 0;  // Погибших нет
}
/*
 * Обновление уровня усталости.
 * Усталость растёт во время работы и снижается во время отдыха.
 */
bool ColonistGroup::update_fatigue(bool is_working) {
    if (state == STATE_DEAD || count <= 0) {
        return false;
    }
    if (is_working) {
        // При работе усталость растёт (скорость зависит от состояния здоровья)
        double fatigue_increase = 1.0;
        if (health < 50) {
            fatigue_increase = 2.0;  // Больной быстрее устаёт
        }
        fatigue = min(100.0, fatigue + fatigue_increase);
        // Если усталость достигла 100%, колонисты не могут работать
        if (fatigue >= 100) {
            opportunity_to_work = false;
            state = STATE_RESTING;  // Принудительный отдых
            cout << "Группа " << group_id << " достигла предела усталости! Нужен отдых." << endl;
            return false;
        }
    } else {
        // При отдыхе усталость снижается
        fatigue = max(0.0, fatigue - 2.0);
        if (fatigue < 30) {
            opportunity_to_work = true;  // Отдохнули, могут работать
        }
    }
    return opportunity_to_work;
}
/*
 * Проверка возможности выполнения задачи.
 * Зависит от специализации колонистов.
 */
bool ColonistGroup::can_perform_task(Task_type task) const {
    // Если группа не может работать или мертва - не может выполнять задачи
    if (!opportunity_to_work || state == STATE_DEAD) {
        return false;
    }
    // Проверка соответствия специализации и задачи
    switch (specialization) {
        case SPEC_ENGINEER:
            return (task == TASK_REPAIR || task == TASK_CONSTRUCTION ||
                    task == TASK_ENERGY || task == TASK_EMERGENCY);
        case SPEC_BIOLOGIST:
            return (task == TASK_GREENHOUSE || task == TASK_RESEARCH);
        case SPEC_MINER:
            return (task == TASK_MINING || task == TASK_CARGO);
        case SPEC_DOCTOR:
            return (task == TASK_MEDICAL);
        case SPEC_RESEARCHER:
            return (task == TASK_RESEARCH || task == TASK_EXPLORATION);
        case SPEC_ENERGY_OPERATOR:
            return (task == TASK_ENERGY);
        case SPEC_TECHNICIAN:
            return (task == TASK_REPAIR || task == TASK_MAINTENANCE ||
                    task == TASK_ENERGY || task == TASK_EMERGENCY);
        case SPEC_REGULAR:
            // Обычные жители могут выполнять базовые задачи
            return (task == TASK_CARGO || task == TASK_CONSTRUCTION ||
                    task == TASK_GREENHOUSE);
        default:
            return false;
    }
}
/*
 * Получение общего потребления ресурсов группой.
 * Умножает норму потребления одного колониста на количество.
 */
Resources_consumption ColonistGroup::get_total_consumption() const {
    Resources_consumption total;
    total.oxygen = resources_consumption.oxygen * count;
    total.water = resources_consumption.water * count;
    total.food = resources_consumption.food * count;
    return total;
}
/*
 * Добавление колонистов в группу.
 */
void ColonistGroup::add_colonists(int amount) {
    if (amount > 0) {
        count += amount;
        // Если группа была мертва, оживляем
        if (state == STATE_DEAD) {
            state = STATE_WAITING;
            health = 80;  // Новые колонисты со средним здоровьем
            fatigue = 0;
            opportunity_to_work = true;
        }
    }
}
/*
 * Удаление колонистов из группы (гибель).
 */
void ColonistGroup::remove_colonists(int amount) {
    if (amount > 0) {
        count = max(0, count - amount);
        if (count == 0) {
            state = STATE_DEAD;
            opportunity_to_work = false;
            health = 0;
        }
    }
}