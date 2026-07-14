//
// Created by Вероника on 06.07.2026.
//
#ifndef MYPROJECT_ROBOT_H
#define MYPROJECT_ROBOT_H
#pragma once
#include "enum_col_rob.h"
#include "modules.h"
#include "Weather.h"
#include <iostream>
#include <string>
using namespace std;
/**
 * Класс, представляющий робота в колонии
 * Роботы используются для выполнения опасных, повторяющихся или технически сложных задач
 */
class Robot {
private:
    // Идентификатор робота (уникальный номер)
    int id;
    // Тип робота (добывающий, ремонтный, грузовой и т.д.)
    Robot_type type;
    // Текущий модуль, где находится робот
    ColonyModule* current_module;
    // Текущий заряд энергии (в процентах, 0-100)
    double energy_charge;
    // Максимальный заряд энергии
    double max_energy;
    // Скорость перемещения (влияет на время выполнения задач)
    double speed;
    // Текущее состояние робота (свободен, работает, заряжается и т.д.)
    Robot_state state;
    // Текущая задача, которую выполняет робот
    Task_type current_task;
    // Время, оставшееся до завершения текущей задачи (в шагах симуляции)
    int time_to_complete_task;
    // Эффективность робота (0-1, зависит от состояния и заряда)
    double efficiency;
    // Уровень износа (0-100, растёт при работе и падает при обслуживании)
    double wear_level;
public:
    /*
     * Конструктор робота
     * id - уникальный идентификатор
     * type - тип робота
     * current_module - модуль, где находится
     * speed - скорость перемещения
     * max_energy - максимальный заряд энергии
     */
    Robot(int id, Robot_type type, ColonyModule* current_module = nullptr,
          double speed = 1.0, double max_energy = 100.0);
    // ГЕТТЕРЫ
    int get_id() const { return id; }
    Robot_type get_type() const { return type; }
    ColonyModule* get_module() const { return current_module; }
    double get_energy_charge() const { return energy_charge; }
    double get_max_energy() const { return max_energy; }
    double get_speed() const { return speed; }
    Robot_state get_state() const { return state; }
    Task_type get_current_task() const { return current_task; }
    int get_time_to_complete() const { return time_to_complete_task; }
    double get_efficiency() const { return efficiency; }
    double get_wear_level() const { return wear_level; }
    double get_now_speed(const Weather& weather) const; // эта функция вычисляет реальную скорость с учетом погоды
   
    // СЕТТЕРЫ
    void set_module(ColonyModule* mod) { current_module = mod; }
    void set_state(Robot_state new_state) { state = new_state; }
    void set_current_task(Task_type task) { current_task = task; }
    void set_speed(double new_speed) { speed = new_speed; }
    /*
     * Назначить задачу роботу
     * task - тип задачи
     * duration - продолжительность в шагах симуляции
     * true, если задача назначена успешно
     */
    bool assign_task(Task_type task, int duration, const Weather& weather); // с учетом погоды
    /*
     * Обновление состояния робота (вызывается каждый шаг симуляции)
     * Обновляет заряд энергии, износ и прогресс выполнения задачи
     * return true, если робот активен и работает
     */
    bool update();
    /*
     * Зарядка робота (восстановление энергии)
     * amount - количество энергии для зарядки
     * return true, если зарядка успешна
     */
    bool charge(double amount);
    /*
     * Повреждение робота (авария, удар и т.д.)
     * damage - степень повреждения
     * return true, если робот ещё работает
     */
    bool take_damage(double damage);
    /*
     * Ремонт робота (восстановление после повреждений)
     * repair_amount - количество восстанавливаемого здоровья
     */
    void repair(double repair_amount);
    /*
     * Проверка возможности выполнения задачи
     * task - проверяемая задача
     * return true, если может выполнить
     */
    bool can_perform_task(Task_type task) const;
    /*
     * Может ли робот двигаться
     * return true, если может перемещаться
     */
    bool can_move() const;
    bool can_move(const Weather& weather) const; // для учета погоды
    /*
     * Получение текущего заряда в процентах
     */
    double get_charge_percentage() const {
        return (max_energy > 0) ? (energy_charge / max_energy) * 100.0 : 0.0;
    }
    /*
     * Проверка, нуждается ли робот в обслуживании
     */
    bool needs_maintenance() const {
        return wear_level > 70 || energy_charge < 20 || state == ROBOT_STATE_DAMAGED;
    }
};
#endif //MYPROJECT_ROBOT_H
