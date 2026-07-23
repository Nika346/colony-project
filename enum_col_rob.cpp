#include "enum_col_rob.h"
// Преобразует специализацию колониста в строку для отображения
string spec_to_str(Colonist_spec spec) {
    switch (spec) {
        case SPEC_ENGINEER: return "инженер";
        case SPEC_BIOLOGIST: return "биолог";
        case SPEC_MINER: return "шахтёр";
        case SPEC_DOCTOR: return "доктор";
        case SPEC_ENERGY_OPERATOR: return "оператор энергосистем";
        case SPEC_TECHNICIAN: return "техник";
        case SPEC_REGULAR: return "обычный житель";
        default: return "неизвестно";
    }
}
// Преобразует состояние колониста в строку
string state_to_str(Colonist_state state) {
    switch (state) {
        case STATE_RESTING: return "отдыхает";
        case STATE_WORKING: return "работает";
        case STATE_MOVING: return "перемещается";
        case STATE_REPAIRING: return "ремонтирует";
        case STATE_TREATMENT: return "лечится";
        case STATE_WAITING: return "ожидает назначения";
        case STATE_IN_DANGER: return "в опасности";
        case STATE_DEAD: return "погиб";
        default: return "неизвестно";
    }
}
// Преобразует тип задачи в строку
string task_type_to_str(Task_type task) {
    switch (task) {
        case TASK_MINING: return "добыча ресурсов";
        case TASK_GREENHOUSE: return "обслуживание теплицы";
        case TASK_ENERGY: return "производство энергии";
        case TASK_REPAIR: return "ремонт модулей";
        case TASK_MEDICAL: return "лечение пострадавших";
        case TASK_EMERGENCY: return "аварийные работы";
        case TASK_CARGO: return "перевозка грузов";
        case TASK_MAINTENANCE: return "обслуживание оборудования";
        default: return "неизвестно";
    }
}
// Преобразует тип робота в строку
string robot_type_to_str(Robot_type robot) {
    switch (robot) {
        case ROBOT_MEDICAL: return "медицинский";
        case ROBOT_MINING: return "добывающий";
        case ROBOT_REPAIR: return "ремонтный";
        case ROBOT_CARGO: return "грузовой";
        default: return "неизвестно";
    }
}
// Преобразует состояние робота в строку
string robot_state_to_str(Robot_state state) {
    switch (state) {
        case ROBOT_STATE_CHARGING: return "заряжается";
        case ROBOT_STATE_DAMAGED: return "поврежден";
        case ROBOT_STATE_DESTROYED: return "уничтожен";
        case ROBOT_STATE_MAINTENANCE: return "на обслуживании";
        case ROBOT_STATE_MOVING: return "перемещается";
        case ROBOT_STATE_WAITING_FOR_TASK: return "ожидает задачи";
        case ROBOT_STATE_WORKING: return "работает";
        default: return "неизвестно";
    }
}
