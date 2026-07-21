// Created by Вероника on 06.07.2026.
#include "robot.h"
#include "colonist.h"
#include <algorithm>  // для std::max, std::min
#include <cmath>
#include <climits>
using namespace std;
/*
 * Конструктор робота
 * Инициализирует все поля начальными значениями
 */
Robot::Robot(int id, Robot_type type, ColonyModule* current_module,
             double speed, double max_energy)
    : currentPath(),
      pathIndex(0),
      id(id),
      type(type),
      current_module(current_module),
      passengers(nullptr),
      energy_charge(max_energy * 0.9),  // Начинает с 90% заряда
      max_energy(max_energy),
      speed(speed),
      state(ROBOT_STATE_WAITING_FOR_TASK),  // Изначально ожидает задачу
      current_task(TASK_CARGO),  // Задача по умолчанию
      time_to_complete_task(0),
      efficiency(1.0),
      wear_level(0.0)
{
    // Проверка валидности входных данных
    if (max_energy <= 0) {
        this->max_energy = 100.0;
        energy_charge = 90.0;
    }
    if (speed <= 0) {
        this->speed = 1.0;
    }
}
/*
 * Назначение задачи роботу
 * Проверяет, может ли робот выполнить задачу, и назначает её
 */
bool Robot::assign_task(Task_type task, int duration, const Weather& weather) {
    // Проверяем, может ли робот выполнять такую задачу
    if (!can_perform_task(task)) {
        cout << "Робот " << id << " не может выполнять задачу "
             << task_type_to_str(task) << endl;
        return false;
    }
    // Проверяем состояние робота
    if (state == ROBOT_STATE_DESTROYED || state == ROBOT_STATE_DAMAGED) {
        cout << "Робот " << id << " не может работать (повреждён или уничтожен)" << endl;
        return false;
    }
    if (energy_charge < 10) {
        cout << "Робот " << id << " слишком мало заряда для работы" << endl;
        return false;
    }
    // проверка погоды
    if (task == TASK_REPAIR || task == TASK_EMERGENCY || task == TASK_CARGO || task == TASK_MINING) {
        if (weather.get_speed() <= 0.0) {
            cout << "Робот " << id << " не может двигаться из-за погоды (скорость = 0)" << endl;
            return false;
        }
        if (weather.get_speed() < 0.3) {
            cout << "Предупреждение: погода сильно замедляет робота " << id << endl;
        }
    }
    // Назначаем задачу
    current_task = task;
    time_to_complete_task = duration;
    state = ROBOT_STATE_WORKING;
    // Учитываем эффективность при выполнении задачи
    // Чем выше износ, тем медленнее работает робот
    double speed_penalty = 1.0 + (wear_level / 200.0);  // До 1.5x медленнее
    time_to_complete_task = static_cast<int>(duration * speed_penalty);
    cout << "Роботу " << id << " назначена задача " << task_type_to_str(task)
         << " на " << time_to_complete_task << " шагов" << endl;
    return true;
}
/*
 * Обновление состояния робота (вызывается каждый шаг симуляции)
 */
bool Robot::update() {
    // Проверяем, не уничтожен ли робот
    if (state == ROBOT_STATE_DESTROYED) {
        return false;
    }
    // Если робот повреждён, он не может работать
    if (state == ROBOT_STATE_DAMAGED) {
        return false;
    }
    // Если робот на обслуживании, он не работает
    if (state == ROBOT_STATE_MAINTENANCE) {
        // Во время обслуживания износ уменьшается
        wear_level = max(0.0, wear_level - 2.0);
        if (wear_level < 10) {
            state = ROBOT_STATE_WAITING_FOR_TASK;
        }
        return true;
    }
    // Если робот заряжается
    if (state == ROBOT_STATE_CHARGING) {
        energy_charge = min(max_energy, energy_charge + 5.0);  // Зарядка 5% за шаг
        if (energy_charge >= max_energy * 0.95) {
            state = ROBOT_STATE_WAITING_FOR_TASK;
        }
        return true;
    }
    // Если робот перемещается
    if (state == ROBOT_STATE_MOVING) {
        // Расход энергии при перемещении
        energy_charge = max(0.0, energy_charge - 0.5);
        if (energy_charge <= 0) {
            state = ROBOT_STATE_CHARGING;
            cout << "Робот " << id << " разрядился во время перемещения" << endl;
            return false;
        }
        return true;
    }
    // Если робот работает над задачей
    if (state == ROBOT_STATE_WORKING) {
        // Расход энергии при работе
        double energy_consumption = 1.0;
        if (type == ROBOT_MINING || type == ROBOT_REPAIR) {
            energy_consumption = 2.0;  // Тяжёлые работы требуют больше энергии
        }
        energy_charge = max(0.0, energy_charge - energy_consumption);
        // Износ при работе
        wear_level = min(100.0, wear_level + 0.5);
        // Если заряд упал до 0 - робот перестаёт работать
        if (energy_charge <= 0) {
            state = ROBOT_STATE_CHARGING;
            cout << "Робот " << id << " разрядился, отправлен на зарядку" << endl;
            return false;
        }
        // Если износ стал критическим
        if (wear_level >= 90) {
            state = ROBOT_STATE_DAMAGED;
            cout << "Робот " << id << " получил повреждения из-за износа" << endl;
            return false;
        }
        // Уменьшаем оставшееся время
        time_to_complete_task--;
        // Если задача выполнена
        if (time_to_complete_task <= 0) {
            state = ROBOT_STATE_WAITING_FOR_TASK;
            cout << "Робот " << id << " завершил задачу " << task_type_to_str(current_task) << endl;
            return true;
        }
    }
    // Если робот свободен или ожидает задачу
    if ( state == ROBOT_STATE_WAITING_FOR_TASK) {
        // Если заряд низкий - отправляем на зарядку
        if (energy_charge < 20) {
            state = ROBOT_STATE_CHARGING;
            return true;
        }
        // Если износ высокий - отправляем на обслуживание
        if (wear_level > 70) {
            state = ROBOT_STATE_MAINTENANCE;
            return true;
        }
    }
    return state == ROBOT_STATE_WORKING || state == ROBOT_STATE_MOVING;
}
/*
 * Зарядка робота (восстановление энергии)
 */
bool Robot::charge(double amount) {
    if (state == ROBOT_STATE_DESTROYED) {
        return false;
    }
    energy_charge = min(max_energy, energy_charge + amount);
    state = ROBOT_STATE_CHARGING;
    return true;
}
/*
 * Повреждение робота (авария, удар и т.д.)
 */
bool Robot::take_damage(double damage) {
    if (state == ROBOT_STATE_DESTROYED) {
        return false;
    }
    wear_level = min(100.0, wear_level + damage);
    if (wear_level >= 100) {
        state = ROBOT_STATE_DESTROYED;
        cout << "Робот " << id << " уничтожен!" << endl;
        return false;
    } else if (wear_level >= 70) {
        state = ROBOT_STATE_DAMAGED;
        cout << "Робот " << id << " повреждён (износ " << wear_level << "%)" << endl;
    }
    return state != ROBOT_STATE_DESTROYED;
}
/*
 * Ремонт робота (восстановление после повреждений)
 */
void Robot::repair(double repair_amount) {
    if (state == ROBOT_STATE_DESTROYED) {
        return;
    }
    wear_level = max(0.0, wear_level - repair_amount);
    state = ROBOT_STATE_MAINTENANCE;
    if (wear_level < 30) {
        cout << "Робот " << id << " отремонтирован (износ " << wear_level << "%)" << endl;
    }
}
/*
 * Проверка возможности выполнения задачи
 * Зависит от типа робота
 */
bool Robot::can_perform_task(Task_type task) const {
    // Проверяем состояние
    if (state == ROBOT_STATE_DESTROYED || state == ROBOT_STATE_DAMAGED) {
        return false;
    }
    // Проверяем соответствие типа робота и задачи
    switch (type) {
        case ROBOT_MINING:
            return (task == TASK_MINING || task == TASK_CARGO);
        case ROBOT_REPAIR:
            return (task == TASK_REPAIR || task == TASK_EMERGENCY);
        case ROBOT_MEDICAL:
            return (task == TASK_MEDICAL);
        case ROBOT_CARGO:
            return (task == TASK_CARGO);
        default:
            return false;
    }
}
/*
 * Может ли робот двигаться
 */
bool Robot::can_move() const {
    return state != ROBOT_STATE_DESTROYED &&
           state != ROBOT_STATE_DAMAGED &&
           state != ROBOT_STATE_CHARGING &&
           energy_charge > 5;
}

bool Robot::can_move(const Weather& weather) const{
    if (!can_move()) return false;
    return weather.get_speed() > 0.0;
}

double Robot::get_now_speed(const Weather& weather) const{
    return speed * weather.get_speed();
}


void Robot::setRoute(const vector<int>& path, ColonyModule* target) {
    currentPath = path;
    targetModule = target;
    pathIndex = 0;
    isMoving = true;
    state = ROBOT_STATE_MOVING; // Переводим робота в состояние движения
}
void Robot::moveOneStep(const vector<shared_ptr<ColonyModule>>& allModules) {
    if (!isMoving || currentPath.empty() || hasArrived()) return;
    // Если мы уже находимся в следующем модуле маршрута, переходим к следующему шагу
    if (current_module->getId() == currentPath[pathIndex]) {
        pathIndex++;
        if (hasArrived()) {
            state = ROBOT_STATE_WAITING_FOR_TASK; // Прибыли
            isMoving = false;
            cout << "Робот " << id << " прибыл в пункт назначения!" << endl;
            dropOffPassengers();
            // Если это грузовой робот и он выполняет перевозку
            if (type == ROBOT_CARGO && current_task == TASK_CARGO) {
                cout << "Грузовой робот " << id << " доставил колонистов на рабочее место!" << endl;
            }
            return;
        }    
    }
    // Ищем следующий модуль по ID в списке всех модулей колонии
    int nextModuleId = currentPath[pathIndex];
    for (const auto& mod : allModules) {
        if (mod->getId() == nextModuleId) {
            current_module = mod.get(); // Перемещаем робота в следующий модуль
            cout << "Робот " << id << " переместился в модуль " << mod->getName() << endl;
            break;
        }
    }
}
bool Robot::hasArrived() const {
    return static_cast<size_t>(pathIndex) >= currentPath.size();
}


void Robot::setPassengers(ColonistGroup* group) {
    passengers = group;
    if (group) {
        cout << "Робот " << id << " взял на борт группу " << group->get_group_id() << endl;
    }
}


void Robot::dropOffPassengers() {
    if (passengers) {
        cout << "Робот " << id << " высадил группу " << passengers->get_group_id() << " в модуле " << current_module->getName() << endl;
        passengers->set_module(current_module->getType());
        passengers->set_opportunity_to_work(true);
        passengers->set_transportRobot(nullptr);

        ModuleType modType = current_module->getType();
        bool isHabitat = (modType == ModuleType::HABITAT);

        if (isHabitat) {
            passengers->set_state(STATE_RESTING);
            passengers->set_cur_task(TASK_MAINTENANCE);
        } else {
            Task_type task = TASK_MAINTENANCE;
            Colonist_spec spec = passengers->get_specialization();
            switch (modType) {
                case ModuleType::MINE:
                    task = TASK_MINING;
                    break;
                case ModuleType::GREENHOUSE:
                    task = TASK_GREENHOUSE;
                    break;
                case ModuleType::SOLAR_POWER:
                case ModuleType::NUCLEAR_POWER:
                    task = TASK_ENERGY;
                    break;
                case ModuleType::MEDICAL:
                    task = TASK_MEDICAL;
                    break;
                case ModuleType::REPAIR_BAY:
                case ModuleType::WATER_RECYCLER:
                    task = TASK_REPAIR;
                    break;
                default:
                    task = TASK_MAINTENANCE;
                    break;
            }
            if (passengers->can_perform_task(task)) {
                passengers->set_cur_task(task);
                passengers->set_state(STATE_WORKING);
                cout << "Группа " << passengers->get_group_id() << " начала работу в модуле " << current_module->getName() << endl;
            } else {
                passengers->set_state(STATE_WAITING);
                passengers->set_cur_task(TASK_MAINTENANCE);
            }
        }
        passengers->set_end_module(current_module);
        passengers = nullptr;
    }
}

