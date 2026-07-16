#pragma once
#include "Modules.h"
#include "colonist.h"
#include "PriorityTaskQueue.h"
#include "TransportRoute.h"
#include "resources.h"
#include "Weather.h"
#include <memory>
#include <random>
#include <queue>
#include <map>
#include <set>
using namespace std;

class Robot;
class Weather;

// Структура для приоритетной очереди в алгоритме Дейкстры, хранит информацию о модуле и времени достижения
struct PathNode {
    int moduleId;      // ID модуля
    double time;       // Время достижения от старта
    // Оператор сравнения для min-heap (приоритетная очередь по возрастанию)
    // Меньшее время = более высокий приоритет
    bool operator>(const PathNode& other) const {
        return time > other.time;
    }
};

class Colony{
private:
    vector<shared_ptr<Robot>> robots;           // Список всех роботов
    vector<shared_ptr<ColonistGroup>> colonistGroups; // Список всех групп колонистов
    PriorityTaskQueue taskQueue;                // Очередь задач, которую мы написали ранее
    vector<shared_ptr<ColonyModule>> modules;
    vector<shared_ptr<TransportRoute>> routes;
    mt19937 rng;  // генератор случайных чисел
    ColonyResourceManager resources; // Запасы ресурсов
    Weather weather;                 // Погода
    int currentHour;                 // Счётчик часов для вывода в консоль

public:
    Colony();
    void createModules();
    void createTransportNetwork(int totalRoutes);
    const vector<shared_ptr<ColonyModule>>& getModules() const { return modules; }
    const vector<shared_ptr<TransportRoute>>& getRoutes() const { return routes; }

    void tick(); // Делает один шаг (1 час)
    void run();  // Запускает цикл симуляции

    void generateAccident();
    void createRepairTask(Accident* accident);
    void updateActiveAccidents();

    // доп задания 12,13,14
    void save(const string& file_name) const;
    void download(const string& file_name);
    void export_csv(const string& file_name) const;
    void export_dot(const string& file_name) const;


    // ==================== АЛГОРИТМ ДЕЙКСТРЫ ====================
    // Основной метод: поиск кратчайшего пути между двумя модулями
    // Возвращает вектор ID модулей в порядке прохождения (от старта к цели)
    // Если путь не найден - возвращает пустой вектор
    vector<int> findShortestPath(int startModuleId, int endModuleId, const Robot& robot, const Weather& weather);
    // Вспомогательный метод: найти все пути, связанные с модулем
    // Возвращает вектор указателей на TransportRoute, где модуль является началом или концом
    vector<shared_ptr<TransportRoute>> getRoutesForModule(int moduleId) const;
    // Вспомогательный метод: найти модуль по его ID, возвращает shared_ptr на модуль или nullptr, если не найден
    shared_ptr<ColonyModule> findModuleById(int id) const;
};
