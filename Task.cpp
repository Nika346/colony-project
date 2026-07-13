#include "Task.h"
#include <algorithm>
#include "Modules.h"
#include "TransportRoute.h"
#include "robot.h"
#include "colonist.h"
using namespace std;

Task::Task(Task_type t, ColonyModule* targetModule, int moduleImportance, 
           double damageLevel, int complexity)
    : type(t),
      complexity(complexity),
      status("pending") {

    target = targetModule;
    if (targetModule != nullptr) {
        nameTarget = "Module_" + to_string(targetModule->getId())+ "_" + targetModule->getName();
    }
    priority = static_cast<double>(moduleImportance) * damageLevel;
}

Task::Task(Task_type t, TransportRoute* targetRoute, int importance, 
           double damageLevel, int complexity)
    : type(t),
      complexity(complexity),
      status("pending") {

    target = targetRoute;
    if (targetRoute != nullptr) {
        nameTarget = "Route_" + to_string(targetRoute->getId());
    }
    priority = static_cast<double>(importance) * damageLevel;
}

ColonyModule* Task::getTargetModule() const {
    if (auto* mod = get_if<ColonyModule*>(&target)) { //проверяет, что target это указатель на модуль
        return *mod;
    }
    return nullptr;
}
TransportRoute* Task::getTargetRoute() const {
    if (auto* route = get_if<TransportRoute*>(&target)) { // проверяет, что target это указатель на путь
        return *route;
    }
    return nullptr;
}

void Task::assignTo(Robot* robot) {
    if (robot != nullptr) {
        assigned = robot;
        status = "in_progress";
    }
}
void Task::assignTo(ColonistGroup* group) {
    if (group != nullptr) {
        assigned = group;
        status = "in_progress";
    }
}

Robot* Task::getAssignedRobot() const {
    if (auto* r = get_if<Robot*>(&assigned)) {
        return *r;
    }
    return nullptr;
}
ColonistGroup* Task::getAssignedColonistGroup() const {
    if (auto* g = get_if<ColonistGroup*>(&assigned)) {
        return *g;
    }
    return nullptr;
}

bool Task::isAssigned() const {
    return !holds_alternative<monostate>(assigned); //если задача кому-то назначена - true
}
void Task::setStatus(const string& newStatus) {
    status = newStatus;
}


