#pragma once
#include "Task.h"
#include <queue>
#include <vector>
#include <iostream>

class PriorityTaskQueue {
private:
    // Компаратор(функция для сравнения двух величин) для приоритетной очереди
    struct TaskComparator {
        bool operator()(const Task& a, const Task& b) const {
            return a.getPriority() < b.getPriority();  // более высокий приоритет = выше в очереди
        }
    };
    priority_queue<Task, vector<Task>, TaskComparator> pq;
    //priority_queue это шаблоный класс, который автоматически сортирует элементы по убыванию
    //(содержит в себе объекты класса задач, хранит их в виде вектора, сортирует по принципу TaskComparator)
public:
    void addTask(const Task& task);
    Task getHighestPriority();     // извлечь самую важную задачу
    Task peek() const;      // посмотреть самую важную, не удаляя    исправила сигнатуру, потому что функция не может возвращать пустоту
    bool isEmpty() const;
    size_t size() const;
    void printAll() const;         // для отладки
    bool has_task(ColonyModule* mod) const;
};
