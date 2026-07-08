//
// Created by Вероника on 06.07.2026.
//
#ifndef MYPROJECT_COLONIST_H
#define MYPROJECT_COLONIST_H
#pragma once
#include "enum_col_rob.h"
#include "modules.h"
#include "resources.h"
#include <iostream>
#include <string>
using namespace std;
 // Структура для хранения норм потребления ресурсов одним колонистом
 // Каждый колонист потребляет кислород, воду и пищу в определенном количестве
struct Resources_consumption {
    double oxygen;  // Потребление кислорода за шаг симуляции
    double water;   // Потребление воды за шаг симуляции
    double food;    // Потребление пищи за шаг симуляции

    // Конструктор со значениями по умолчанию
    Resources_consumption(double o = 0.05, double w = 0.12, double f = 0.06):
        oxygen(o), water(w), food(f) {};
};
 // Класс, представляющий группу колонистов с одинаковыми характеристиками
 // Для упрощения модели колонисты объединены в группы по специализации

class ColonistGroup {
private:
    // Идентификатор группы (уникальное имя)
    string group_id;
    // Количество колонистов в группе
    int count;
    // Специализация группы (инженеры, биологи и т.д.)
    Colonist_spec specialization;
    // Состояние здоровья группы (в процентах, 0-100)
    double health;
    // Уровень усталости (в процентах, 0-100, растет при работе)
    double fatigue;
    // Текущее состояние группы (работает, отдыхает и т.д.)
    Colonist_state state;
    // Текущая задача (что делают колонисты)
    Task_type current_task;
    // Модуль, в котором сейчас находится группа
    ModuleType current_module;
    // Нормы потребления ресурсов для одного колониста
    Resources_consumption resources_consumption;
    // Может ли группа работать (зависит от здоровья и усталости)
    bool opportunity_to_work;
    Resources_consumption consumptionRatePerPerson;

public:
     // Конструктор группы колонистов
     // group_id - идентификатор группы
     // specialization - специализация
     // count - количество колонистов
     // current_module - текущий модуль
     // current_task - текущая задача
     // health - уровень здоровья (по умолчанию 100)
     // fatigue - уровень усталости (по умолчанию 0)
     // consumption - нормы потребления ресурсов

    ColonistGroup(string& group_id, Colonist_spec specialization, int count,
                  ModuleType current_module, Task_type current_task,
                  double health = 100.0, double fatigue = 0.0,
                  const Resources_consumption& consumption = Resources_consumption());

    // ГЕТТЕРЫ
    string get_group_id() const { return group_id; }
    int get_count() const { return count; }
    Colonist_spec get_specialization() const { return specialization; }
    ModuleType get_module() const { return current_module; }
    Task_type get_cur_task() const { return current_task; }
    double get_health() const { return health; }
    double get_fatigue() const { return fatigue; }
    Colonist_state get_state() const { return state; }
    bool get_opportunity_to_work() const { return opportunity_to_work; }
    Resources_consumption get_consumption() const { return resources_consumption; }
    Resources_consumption getConsumptionRate() const { return consumptionRatePerPerson; }

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
     //Метод для обновления здоровья колонистов
     //Здоровье снижается при недостатке ресурсов или высоком уровне усталости
     //oxygen_available - достаточно ли кислорода
     //water_available - достаточно ли воды
     //food_available - достаточно ли пищи
     //return количество погибших колонистов (если здоровье упало до 0)
     int update_health(const ResourcesAvailability& availability);

     //Метод для обновления уровня усталости
     //Усталость растет при работе и снижается во время отдыха
     //is_working - работают ли колонисты
     //return true, если колонисты могут продолжать работать
    bool update_fatigue(bool is_working);
     //Проверка, может ли группа выполнять указанную задачу.
     //Зависит от специализации и текущего состояния
     // task - проверяемая задача
     //return true, если могут выполнять
    bool can_perform_task(Task_type task) const;
     //Получение общего количества потребляемых ресурсов группой
     //return структура с общим потреблением (умножается на количество колонистов)
    Resources_consumption get_total_consumption() const;
     //Добавление новых колонистов в группу
     //amount - количество новых колонистов
    void add_colonists(int amount);
     //Удаление колонистов из группы (в случае гибели)
     //param amount - количество удаляемых колонистов

    void remove_colonists(int amount);
};

#endif //MYPROJECT_COLONIST_H
