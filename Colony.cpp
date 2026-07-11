#include "Colony.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <utility>
using namespace std;

// ==================== Colony ====================
Colony::Colony() : rng(42) {
    createModules();
    int n;
    cout<<"Введите количество транспортных путей: "<<endl;
    cin>>n;
    createTransportNetwork(n);
    cout<<"Колония успешно создана!"<<endl;
}
void Colony::createModules(){
    int habitatCount, greenhouseCount, solarCount, nuclearCount, mineCount, waterRecyclerCount, storageCount, medicalCount, repairBayCount;
    int currentId = 0;
    cout<<"Введите количество Жилых модулей: "<<endl;
    cin>>habitatCount;
    cout<<"Введите количество Теплиц: "<<endl;
    cin>>greenhouseCount;
    cout<<"Введите количество Солнечных электростанций: "<<endl;
    cin>>solarCount;
    cout<<"Введите количество Атомных электростанций: "<<endl;
    cin>>nuclearCount;
    cout<<"Введите количество Шахт: "<<endl;
    cin>>mineCount;
    cout<<"Введите количество Систем очистки воды: "<<endl;
    cin>>waterRecyclerCount;
    cout<<"Введите количество Складов: "<<endl;
    cin>>storageCount;
    cout<<"Введите количество Медицинских модулей: "<<endl;
    cin>>medicalCount;
    cout<<"Введите количество Ремонтных цехов: "<<endl;
    cin>>repairBayCount;
    for (int i = 0; i < habitatCount; i++) {
        modules.push_back(make_shared<HabitatModule>(
            currentId++, "Жилой " + to_string(i+1), 10));
    }
    for (int i = 0; i < greenhouseCount; i++) {
        modules.push_back(make_shared<Greenhouse>(
            currentId++, "Теплица " + to_string(i+1)));
    }
    for (int i = 0; i < solarCount; i++) {
        modules.push_back(make_shared<SolarPowerPlant>(
            currentId++, "Солнечная электростанция " + to_string(i+1)));
    }
    for (int i = 0; i < nuclearCount; i++) {
        modules.push_back(make_shared<NuclearPowerPlant>(
            currentId++, "Атомная электростанция" + to_string(i+1)));
    }
    for (int i = 0; i < mineCount; i++) {
        modules.push_back(make_shared<Mine>(
            currentId++, "Шахта " + to_string(i+1)));
    }
    for (int i = 0; i < waterRecyclerCount; i++) {
        modules.push_back(make_shared<WaterRecycler>(
            currentId++, "Система очистки воды " + to_string(i+1)));
    }
    for (int i = 0; i < storageCount; i++) {
        modules.push_back(make_shared<Storage>(
            currentId++, "Склад " + to_string(i+1)));
    }
    for (int i = 0; i < medicalCount; i++) {
        modules.push_back(make_shared<MedicalModule>(
            currentId++, "Медблок " + to_string(i+1)));
    }
    for (int i = 0; i < repairBayCount; i++) {
        modules.push_back(make_shared<RepairBay>(
            currentId++, "Ремонтный цех " + to_string(i+1)));
    }
    cout << "Создано модулей: " << currentId << "\n";
}
void Colony::createTransportNetwork(int totalRoutes) {
    uniform_real_distribution<double> lengthDist(10.0, 100.0); // Генерируем случайную длину (10-100 метров)
    uniform_int_distribution<int> capacityDist(5, 20);

    int minRoutes = modules.size() - 1;
    if (totalRoutes < minRoutes) {
        cout << "Слишком мало путей! Минимум: " << minRoutes << "\n";
        cout << "Задайте количество транспортных путей: \n";
        cin >> totalRoutes;
    }
    // Этап 1: Создаём основную цепочку
    // Создаём массив индексов модулей
    vector<int> moduleIndices;
    for (size_t i = 0; i < modules.size(); i++) {   
        moduleIndices.push_back(i);
    }
    shuffle(moduleIndices.begin(), moduleIndices.end(), rng); // Перемешиваем
    for (size_t i = 0; i < moduleIndices.size() - 1; i++) {    
        int j1 = moduleIndices[i];
        int j2 = moduleIndices[i + 1];
        double length = lengthDist(rng);
        auto route = make_shared<TransportRoute>(
            routes.size(), modules[j1].get(), modules[j2].get(), length); //указатель на объект класса пути
        route->setCapacity(capacityDist(rng)); // случайная пропускная способность от 5 до 20
        route->setState(RouteState::OPERATIONAL); // изначально все пути исправны и доступны колонистам и роботам
        route->setUsableByRobots(true);
        route->setUsableByColonists(true);
        routes.push_back(route); // добавляем в список путей
        modules[j1]->ModulesRoutes.push_back(modules[j2]); //добавляем указатели на соседние модули     
        modules[j2]->ModulesRoutes.push_back(modules[j1]);

    }
    // Этап 2: Добавляем случайные дополнительные пути
    int extraRoutes = totalRoutes - routes.size();
    if (extraRoutes > 0) {
        for(int i =0; i<extraRoutes; i++){
            uniform_int_distribution<int> index (0, moduleIndices.size()-1);
            int module1 = index(rng);
            int module2 = index(rng);
            //пока модуль с индексом module2 есть в списке соседних модулей модуля module1, мы меняем его индекс
            while(find(modules[module1]->ModulesRoutes.begin(),modules[module1]->ModulesRoutes.end(),
            modules[module2])!=modules[module1]->ModulesRoutes.end()){
                module2 = index(rng);
            }
        double length = lengthDist(rng);
        auto route = make_shared<TransportRoute>(
            routes.size(), modules[module1].get(), modules[module2].get(), length);
        uniform_int_distribution<int> capacityDist(5, 20);
        route->setCapacity(capacityDist(rng));
        route->setState(RouteState::OPERATIONAL);
        route->setUsableByRobots(true);
        route->setUsableByColonists(true);
        routes.push_back(route);
        modules[module1]->ModulesRoutes.push_back(modules[module2]);
        modules[module2]->ModulesRoutes.push_back(modules[module1]);
        }
    }

    cout << "\nВсего создано путей: " << routes.size() << "\n";
}




