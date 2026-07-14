#include "Colony.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <utility>
using namespace std;

// ==================== Colony ====================
Colony::Colony() : rng(42), currentHour(0) {
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

// Основной шаг симуляции (один час)
void Colony::tick() {
    // Обновляем погоду


    // 1. ПРОИЗВОДСТВО РЕСУРСОВ
    for (const auto& mod : modules) {
        // Пропускаем разрушенные и отключенные модули
        if (!mod->isOperational()) continue;

        for (auto const& [ResType, amount] : mod->getProduction()) {
            if (amount > 0) {
                resources.get_resource(ResType).produce(amount);
            }
        }
    }

    // 2. ПОТРЕБЛЕНИЕ РЕСУРСОВ
    for (const auto& mod : modules) {
        if (!mod->isOperational()) continue;

        for (auto const& [ResType, amount] : mod->getConsumption()) {
            if (amount > 0) {
                // Если не хватило ресурса (вернуло false) - отключаем модуль и пишем с часом
                if (!resources.get_resource(ResType).consume(amount)) {
                    mod->setState(ModuleState::OFFLINE);
                    cout << "Час " << currentHour << ": Модуль " << mod->getName() << " отключен (нехватка ресурса)!" << endl;
                    break; // Прерываем проверку для этого модуля
                }
            }
        }
    }

    // 3. ПРОВЕРКА КРИТИЧЕСКИХ УРОВНЕЙ
    vector<ResourceType> types = {ResourceType::OXYGEN, ResourceType::WATER, ResourceType::FOOD, ResourceType::ENERGY};
    for (ResourceType type : types) {
        Resource& res = resources.get_resource(type);
        if (res.isCritical()) {
            cout << "Час " << currentHour << ": КРИТИЧЕСКИЙ УРОВЕНЬ! Ресурс " << res.getName() << " на исходе! (" << res.getCurrentAmount() << "/" << res.getMaxCapacity() << ")" << endl;
        }
    }

    currentHour++; // Увеличиваем счётчик часов
}

// Запуск цикла симуляции на несколько дней
void Colony::run() {
    weather.rand_weather();
    for (int i = 0; i < 5; i++) {
        currentHour = 0;
        cout << "--- День " << (i + 1) << " ---" << endl;
        for (int hour = 0; hour < 24; hour++) {
            tick();
        }
    }
}

// доп задания 12,13,14
void Colony::save(const string& file_name) const{
    ofstream out(file_name);
    if (!out){
        cerr << "Ошибка открытия файла!";
        return;
    }
    out << modules.size() << endl;
    for (const auto& mod : modules) {
        out << mod->getId() << " " << static_cast<int>(mod->getType()) << " " << mod->getCurrentHealth() << " " << mod->getMaxHealth() << " " << mod->getImportanceLevel() << " " << static_cast<int>(mod->getState()) << " " << mod->getName() << endl;
    }
    out << routes.size() << endl;
    for (const auto& route : routes) {
        out << route->getId() << " " << route->getStartModule()->getId() << " " << route->getEndModule()->getId() << " " << route->getLength() << " " << static_cast<int>(route->getState()) << " " << route->getCurrentHealth() << " " << route->getMaxHealth() << endl;
    }
    out.close();
    cout << "Сценарий сохранён в " << file_name << endl;
}

void Colony::download(const string& file_name){
    ifstream in(file_name);
    if (!in) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }
    modules.clear();
    routes.clear();
    size_t module_count;
    in >> module_count;
    for (size_t i = 0; i < module_count; ++i) {
        int id, typeInt, health, maxHealth, importance, stateInt;
        string name;
        in >> id >> typeInt >> health >> maxHealth >> importance >> stateInt;
        getline(in, name);
        ModuleType type = static_cast<ModuleType>(typeInt);
        shared_ptr<ColonyModule> mod;
        switch (type) {
            case ModuleType::HABITAT:
                mod = make_shared<HabitatModule>(id, name, 10);
                break;
            case ModuleType::GREENHOUSE:
                mod = make_shared<Greenhouse>(id, name);
                break;
            case ModuleType::SOLAR_POWER:
                mod = make_shared<SolarPowerPlant>(id, name);
                break;
            case ModuleType::NUCLEAR_POWER:
                mod = make_shared<NuclearPowerPlant>(id, name);
                break;
            case ModuleType::MINE:
                mod = make_shared<Mine>(id, name);
                break;
            case ModuleType::WATER_RECYCLER:
                mod = make_shared<WaterRecycler>(id, name);
                break;
            case ModuleType::STORAGE:
                mod = make_shared<Storage>(id, name);
                break;
            case ModuleType::MEDICAL:
                mod = make_shared<MedicalModule>(id, name);
                break;
            case ModuleType::REPAIR_BAY:
                mod = make_shared<RepairBay>(id, name, 3);
                break;
            default:
                cerr << "Неизвестный тип модуля!" << endl;
                continue;
        }
        mod->setState(static_cast<ModuleState>(stateInt));
        if (health < maxHealth) {
            mod->takeDamage(maxHealth - health);
        }
        modules.push_back(mod);
    }
    size_t route_count;
    in >> route_count;
    for (size_t i = 0; i < route_count; ++i) {
        int id, startId, endId, stateInt, curHealth, maxHealth;
        double length;
        in >> id >> startId >> endId >> length >> stateInt >> curHealth >> maxHealth;
        ColonyModule* start = nullptr;
        ColonyModule* end = nullptr;
        for (auto& m : modules) {
            if (m->getId() == startId) start = m.get();
            if (m->getId() == endId) end = m.get();
        }
        if (!start || !end) {
            cerr << "Ошибка: не найден модуль для перехода!" << endl;
            continue;
        }
        auto route = make_shared<TransportRoute>(id, start, end, length);
        route->setState(static_cast<RouteState>(stateInt));
        if (curHealth < maxHealth) {
            route->takeDamage(maxHealth - curHealth);
        }
        routes.push_back(route);
    }
    in.close();
    cout << "Сценарий загружен из " << file_name << endl;
}

void Colony::export_csv(const string& file_name) const{
    ofstream out(file_name);
    if (!out) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }
    out << "ID;Название;Тип;Здоровье;Макс. здоровье;Важность;Состояние;Потребление;Производство" << endl;
    for (const auto& mod : modules) {
        out << mod->getId() << ";" << mod->getName() << ";" << static_cast<int>(mod->getType()) << ";" << mod->getCurrentHealth() << ";" << mod->getMaxHealth() << ";" << mod->getImportanceLevel() << ";" << static_cast<int>(mod->getState()) << ";";
        string cons, prod;
        for (auto& pair : mod->getConsumption()) {
            cons += to_string(static_cast<int>(pair.first)) + ":" + to_string(pair.second) + " ";
        }
        for (auto& pair : mod->getProduction()) {
            prod += to_string(static_cast<int>(pair.first)) + ":" + to_string(pair.second) + " ";
        }
        out << cons << ";" << prod << endl;
    }
    out.close();
    cout << "CSV отчёт сохранён в " << file_name << endl;
}

void Colony::export_dot(const string& file_name) const{
    ofstream out(file_name);
    if (!out) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }
    out << "digraph Colony {" << endl;
    out << "    node [shape=box, style=filled, fillcolor=lightblue];" << endl;
    for (const auto& mod : modules) {
        out << "    " << mod->getId() << " [label=\"" << mod->getName() << "\\n" << "HP: " << mod->getCurrentHealth() << "/" << mod->getMaxHealth() << "\"];" << endl;
    }
    for (const auto& route : routes) {
        out << "    " << route->getStartModule()->getId() << " -> "
            << route->getEndModule()->getId() << " [label=\"длина=" << route->getLength()
            << "\\nсост=" << static_cast<int>(route->getState()) << "\"];" << endl;
    }
    out << "}" << endl;
    out.close();
    cout << "DOT-файл для схемы сохранён в " << file_name << endl;
}