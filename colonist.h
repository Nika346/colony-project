
#ifndef MYPROJECT_COLONIST_H
#define MYPROJECT_COLONIST_H
#pragma once
#include "enum_col_rob.h"
#include "modules.h"
#include "resources.h"
#include <iostream>
#include <string>
#include <algorithm>
using namespace std;
 // Структура для хранения норм потребления ресурсов одним колонистом
struct Resources_consumption {
    double oxygen; 
    double water;  
    double food;  
    // Конструктор со значениями по умолчанию
    Resources_consumption(double o = 0.05, double w = 0.12, double f = 0.06):
        oxygen(o), water(w), food(f) {};
};

class ColonistGroup {
private:
    string group_id;
    int count;
    Colonist_spec specialization;
    double health;    // (в процентах, 0-100)
    double fatigue;     // Уровень усталости (в процентах, 0-100, растет при работе)
    Colonist_state state;
    Task_type current_task;
    ModuleType current_module;
    ColonyModule* module_cur;
    Resources_consumption resources_consumption;    // Нормы потребления ресурсов для одного колониста
    bool opportunity_to_work;    // (зависит от здоровья и усталости)
    Resources_consumption consumptionRatePerPerson;
    Robot* transportRobot;
    ColonyModule* start_module;
    ColonyModule* end_module;

public:
    ColonistGroup(string& group_id, Colonist_spec specialization, int count,
                  ColonyModule* moduleptr, Task_type current_task,
                  double health = 100.0, double fatigue = 0.0,
                  const Resources_consumption& consumption = Resources_consumption());
    // ГЕТТЕРЫ
    string get_group_id() const { return group_id; }
    int get_count() const { return count; }
    Colonist_spec get_specialization() const { return specialization; }
    ModuleType get_module() const { return current_module; }
    ColonyModule* get_module_cur() const {return module_cur;}
    Task_type get_cur_task() const { return current_task; }
    double get_health() const { return health; }
    double get_fatigue() const { return fatigue; }
    Colonist_state get_state() const { return state; }
    bool get_opportunity_to_work() const { return opportunity_to_work; }
    Resources_consumption get_consumption() const { return resources_consumption; }
    Resources_consumption getConsumptionRate() const { return consumptionRatePerPerson; }
    Robot* get_transportRobot() const {return transportRobot;};

    // СЕТТЕРЫ
    void set_group_id(string group_i) { group_id = group_i; }
    void set_count(int c) { count = c; }
    void set_health(double h) { health = max(0.0, min(100.0, h)); }
    void set_fatigue(double fat) { fatigue = max(0.0, min(100.0, fat)); }
    void set_state(Colonist_state st) { state = st; }
    void set_cur_task(Task_type current_t) { current_task = current_t; }
    void set_specialization(Colonist_spec spec) { specialization = spec; }
    void set_module(ModuleType current_m) { current_module = current_m; }
    void set_opportunity_to_work(bool opportunity) { opportunity_to_work = opportunity; }
    void set_transportRobot(Robot* r){transportRobot=r;};
    void set_start_module(ColonyModule* mod){ start_module = mod; }
    ColonyModule* get_start_module() const { return start_module; }
    void move_to_module(ColonyModule* mod);
    ColonyModule* get_end_module() const { return end_module; }
    void set_end_module(ColonyModule* mod) { end_module = mod; }
     //Здоровье снижается при недостатке ресурсов или высоком уровне усталости
     //_available - достаточно ли кислорода/воды/пищи
     int update_health(const ResourcesAvailability& availability);
     //Усталость растет при работе и снижается во время отдыха
    bool update_fatigue(bool is_working);
     //Проверка, может ли группа выполнять указанную задачу.
     //Зависит от специализации и текущего состояния
    bool can_perform_task(Task_type task) const;
     //Получение общего количества потребляемых ресурсов группой
     //return структура с общим потреблением (умножается на количество колонистов)
    Resources_consumption get_total_consumption() const;
     //Добавление новых колонистов в группу
    void add_colonists(int amount);
     //Удаление колонистов из группы (в случае гибели
    void remove_colonists(int amount);
};
#endif //MYPROJECT_COLONIST_H
