#include "PriorityTaskQueue.h"
#include <iostream>

void PriorityTaskQueue::addTask(const Task& task) {
    pq.push(task);
    cout << "Задача добавлена в очередь: " <<  task.getNameTarget()<< " | Приоритет: " << task.getPriority() << endl;
}

Task PriorityTaskQueue::getHighestPriority() {
    if (pq.empty()) {
        cout << "Приоритетная очередь задач пуста!";
        return Task();          // нужно вернуть
    }
    Task top = pq.top(); //указатель на задачу с наибольшим приоритетом
    pq.pop(); //удаляем первый элемент, те top
    return top;
}
Task PriorityTaskQueue::peek() const {
    if (pq.empty()) {
        cout << "Приоритетная очередь задач пуста!";
        return Task();
    }
    return pq.top();
}

bool PriorityTaskQueue::isEmpty() const {
    return pq.empty();
}

size_t PriorityTaskQueue::size() const {
    return pq.size();
}







