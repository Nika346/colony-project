#include "PriorityTaskQueue.h"
#include <iostream>

void PriorityTaskQueue::addTask(const Task& task) {
    pq.push(task);
    cout << "Задача добавлена в очередь: " <<  task.getNameTarget()<< " | Приоритет: " << task.getPriority() << endl;
}

Task PriorityTaskQueue::getHighestPriority() {
    if (pq.empty()) {
        cout << "Приоритетная очередь задач пуста!";
        return;
    }
    Task top = pq.top(); //указатель на задачу с наибольшим приоритетом
    pq.pop(); //удаляем первый элемент, те top
    return top;
}
const Task& PriorityTaskQueue::peek() const {
    if (pq.empty()) {
        cout << "Приоритетная очередь задач пуста!";
        return;
    }
    return pq.top();
}

bool PriorityTaskQueue::isEmpty() const {
    return pq.empty();
}

size_t PriorityTaskQueue::size() const {
    return pq.size();
}

void PriorityTaskQueue::printAll() const {
    cout << "В очереди задач: " << pq.size() << endl;
}






