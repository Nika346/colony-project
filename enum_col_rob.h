//
// Created by Вероника on 02.07.2026.
//
#ifndef MYPROJECT_ENUM_COL_ROB_H
#define MYPROJECT_ENUM_COL_ROB_H
#include <string>
using namespace std;
// Специализации колонистов - определяют, какие задачи может выполнять колонист
enum Colonist_spec {
    SPEC_ENGINEER,          // Инженер - ремонт и строительство
    SPEC_BIOLOGIST,         // Биолог - работа в теплице, исследование флоры
    SPEC_MINER,             // Шахтёр - добыча ресурсов
    SPEC_DOCTOR,            // Доктор - лечение колонистов
    SPEC_ENERGY_OPERATOR,   // Оператор энергосистем - управление энергоснабжением
    SPEC_TECHNICIAN,        // Техник - обслуживание оборудования
    SPEC_REGULAR            // Обычный житель - может выполнять базовые задачи
};
// Состояния колонистов - отражают текущую активность
enum Colonist_state {
    STATE_RESTING,          // Отдыхает - восстанавливает силы
    STATE_WORKING,          // Работает - выполняет задачу
    STATE_MOVING,           // Перемещается - между модулями
    STATE_REPAIRING,        // Ремонтирует - оборудование или модули
    STATE_TREATMENT,        // Лечится - в медицинском модуле
    STATE_WAITING,          // Ожидает назначения - свободен
    STATE_IN_DANGER,        // В опасности - требует немедленной помощи
    STATE_DEAD,             // Погиб - исключён из модели
};
// Типы задач, которые могут выполнять колонисты и роботы
enum Task_type {
    TASK_MINING,            // Добыча руды и полезных ископаемых
    TASK_GREENHOUSE,        // Обслуживание теплицы и выращивание пищи
    TASK_ENERGY,            // Производство и распределение энергии
    TASK_REPAIR,            // Ремонт модулей и оборудования
    TASK_MEDICAL,           // Лечение колонистов
    TASK_EMERGENCY,         // Аварийные работы (пожары, утечки и т.д.)
    TASK_CARGO,             // Перевозка грузов между модулями
    TASK_MAINTENANCE        // Обслуживание оборудования
};
// Типы роботов
enum Robot_type {
    ROBOT_MINING,           // Добывающий робот - работает в шахте
    ROBOT_REPAIR,           // Ремонтный робот - чинит оборудование
    ROBOT_MEDICAL,          // Медицинский робот - помогает докторам
    ROBOT_CARGO             // Грузовой робот - перевозит ресурсы
};
// Состояния роботов
enum Robot_state {
    ROBOT_STATE_WORKING,        // Работает - выполняет задачу
    ROBOT_STATE_MOVING,         // Перемещается - между точками
    ROBOT_STATE_CHARGING,       // Заряжается - пополняет энергию
    ROBOT_STATE_DAMAGED,        // Повреждён - требует ремонта
    ROBOT_STATE_WAITING_FOR_TASK, // Ожидает задачи
    ROBOT_STATE_DESTROYED,      // Уничтожен - исключён из модели
    ROBOT_STATE_MAINTENANCE     // На обслуживании - плановый ремонт
};
// Функции для преобразования перечислений в строки (для вывода на экран)
string spec_to_str(Colonist_spec spec);
string state_to_str(Colonist_state state);
string task_type_to_str(Task_type task);
string robot_type_to_str(Robot_type robot);
string robot_state_to_str(Robot_state state);
#endif //MYPROJECT_ENUM_COL_ROB_H