#pragma once
#include <variant>
#include <string>
#include "enum_col_rob.h"

class ColonyModule;
class TransportRoute;
class Robot;
class ColonistGroup;

class Task {
private:
    Task_type type;
    string nameTarget;               // название места в котором нужно выполнить задачу
    variant<monostate, ColonyModule*, TransportRoute* > target;     // автоматически определяется тип места(модуль или путь)
    int complexity;                 // сложность/время выполнения
    double priority;                // рассчитанный приоритет
    string status;                  // статус задачи
    variant<monostate, Robot*, ColonistGroup* > assigned;         // указатель на робота или группу колонистов

public:
    Task() : type(TASK_MINING), nameTarget(""), complexity(0), priority(0), status("empty") {} // добавила конструктор по умолчанию

    Task(Task_type t, ColonyModule* target, int moduleImportance, 
         double damageLevel, int complexity = 1);
     Task(Task_type t, TransportRoute* targetRoute, int importance, 
         double damageLevel, int complexity = 1);
    double getPriority() const { return priority; }
    Task_type getType() const { return type; }
    string getNameTarget() const { return nameTarget; }
    int getComplexity() const { return complexity; }
    string getStatus() const { return status; }

    ColonyModule* getTargetModule() const;
    TransportRoute* getTargetRoute() const;

    // Назначение исполнителя
    void assignTo(Robot* robot);
    void assignTo(ColonistGroup* group);

    Robot* getAssignedRobot() const;
    ColonistGroup* getAssignedColonistGroup() const;
    bool isAssigned() const;
    void setStatus(const string& newStatus);
    }; 
