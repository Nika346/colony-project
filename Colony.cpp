#include "Colony.h"
#include "robot.h"
#include "Weather.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <set>
#include <utility>
#include <queue>
#include <limits>
#include <map>
#include <random>
#include <chrono>
#include "Colony.h"
using namespace std;

// ==================== Colony ====================
Colony::Colony() : currentHour(0) {
    random_device rd;
    rng.seed(rd());
    createModules();
    int n;
    cout << "Введите количество транспортных путей: " << endl;
    cin >> n;
    createTransportNetwork(n);
    cout << "Колония успешно создана!" << endl;
    //СОЗДАНИЕ РОБОТОВ
    cout << "\n--- СОЗДАНИЕ РОБОТОВ ---" << endl;
    int robotCount;
    cout << "Сколько роботов вы хотите создать? ";
    cin >> robotCount;
    for (int i = 0; i < robotCount; i++) {
        cout << "\nРобот №" << (i + 1) << ":" << endl;
        // Выбор типа робота
        int typeChoice;
        cout << "Выберите тип робота:" << endl;
        cout << "1. Ремонтный" << endl;
        cout << "2. Грузовой" << endl;
        cout << "3. Добывающий" << endl;
        cout << "Ваш выбор: ";
        cin >> typeChoice;
        Robot_type robotType;
        if (typeChoice == 1) robotType = ROBOT_REPAIR;
        else if (typeChoice == 2) robotType = ROBOT_CARGO;
        else robotType = ROBOT_MINING; // По умолчанию добывающий
        // Выбор модуля для размещения
        cout << "В каком модуле разместить робота? (введите ID модуля): ";
        int moduleId;
        cin >> moduleId;
        // Ищем модуль по ID
        ColonyModule* targetModule = nullptr;
        for (const auto& mod : modules) { // modules — это вектор умных указателей shared_ptr
            if (mod->getId() == moduleId) {
                targetModule = mod.get(); // Чтобы превратить умный указатель в обычный, вызывается метод .get()
                break;
            }
        }
        if (targetModule == nullptr) {
            cout << "Модуль с ID " << moduleId << " не найден! Робот размещен в первом модуле." << endl;
            targetModule = modules[0].get(); // Если ошиблись, ставим в первый модуль
        }
        // Создаем робота и добавляем в список
        int robotId = i + 1; // ID робота начинается с 1
        double robotSpeed = speedDist(rng);
        robots.push_back(make_shared<Robot>(robotId, robotType, targetModule, robotSpeed, 100.0));
        cout << "Робот " << robotId << " (" << robot_type_to_str(robotType) << ") создан в модуле "
             << targetModule->getName() << "! (скорость " << robotSpeed <<")" << endl;
    }
    //СОЗДАНИЕ ГРУПП КОЛОНИСТОВ
    cout << "\n--- СОЗДАНИЕ ГРУПП КОЛОНИСТОВ ---" << endl;
    int groupCount;
    cout << "Сколько групп колонистов вы хотите создать? ";
    cin >> groupCount;
    for (int i = 0; i < groupCount; i++) {
        cout << "\nГруппа №" << (i + 1) << ":" << endl;
        // Выбор профессии
        int specChoice;
        cout << "Выберите специализацию группы:" << endl;
        cout << "1. Инженеры" << endl;
        cout << "2. Шахтеры" << endl;
        cout << "3. Биологи" << endl;
        cout << "4. Доктора" << endl;
        cout << "5. Обычные жители" << endl;
        cout << "Ваш выбор: ";
        cin >> specChoice;
        Colonist_spec spec;
        string groupName;
        if (specChoice == 1) { spec = SPEC_ENGINEER; groupName = "Инженеры"; }
        else if (specChoice == 2) { spec = SPEC_MINER; groupName = "Шахтеры"; }
        else if (specChoice == 3) { spec = SPEC_BIOLOGIST; groupName = "Биологи"; }
        else if (specChoice == 4) { spec = SPEC_DOCTOR; groupName = "Доктора"; }
        else { spec = SPEC_REGULAR; groupName = "Обычные жители"; }
        // Количество человек в группе
        int personCount;
        cout << "Сколько колонистов в этой группе? ";
        cin >> personCount;
        // Выбор модуля для размещения
        cout << "В каком модуле разместить группу? (введите ID модуля): ";
        int moduleId;
        cin >> moduleId;
        // Ищем модуль по ID
        ColonyModule* targetModule = nullptr;
        for (const auto& mod : modules) {
            if (mod->getId() == moduleId) {
                targetModule = mod.get();
                break;
            }
        }
        if (targetModule == nullptr) {
            cout << "Модуль с ID " << moduleId << " не найден! Группа размещена в первом модуле." << endl;
            targetModule = modules[0].get();
        }
        // Создаем группу колонистов и добавляем в список
        double ranHel = dist75_120(rng);
        string fullGroupName = groupName + " " + to_string(i + 1);
        colonistGroups.push_back(make_shared<ColonistGroup>(
            fullGroupName, spec, personCount,
            targetModule, TASK_MAINTENANCE,
            ranHel, 0.0, Resources_consumption()
        ));
        cout << "Группа " << fullGroupName << " (" << personCount << " чел.) создана в модуле "
             << targetModule->getName() << "!" << endl;
    }
    cout << "\nЗаселение завершено! Начинаем симуляцию..." << endl;
    reset_statistics();
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
    // 1. ПРОИЗВОДСТВО РЕСУРСОВ (с проверкой наличия работников)
    for (const auto& mod : modules) {
        // Модуль должен работать
        if (!mod->isOperational()) continue;
        // Проверяем, есть ли в этом модуле группа колонистов, которая может работать
        bool hasWorkersHere = false;
        for (const auto& group : colonistGroups) {
            // Группа жива, находится в этом модуле и может работать
            if (group->get_state() == STATE_WORKING &&
                group->get_count() > 0 &&
                group->get_opportunity_to_work() ) {
                // Сравниваем типы модулей (группа находится в жилом, а модуль - рабочий)
                hasWorkersHere = true;
                group->update_fatigue(true);
                }
        }
        // Если в модуле нет работников, он не производит ресурсы
        if (!hasWorkersHere) continue;
        // Если работники есть — производим
        for (auto const& [ResType, amount] : mod->getProduction()) {
            if (amount > 0) {
                resources.get_resource(ResType).produce(amount);
                switch (ResType) {
                    case ResourceType::FOOD: stats.food_produced += amount; break;
                    case ResourceType::OXYGEN: stats.oxygen_produced += amount; break;
                    case ResourceType::WATER: stats.water_produced += amount; break;
                    case ResourceType::ENERGY: stats.energy_produced += amount; break;
                    default: break;
                }
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
                switch (ResType) {
                    case ResourceType::FOOD: stats.food_consumed += amount; break;
                    case ResourceType::OXYGEN: stats.oxygen_consumed += amount; break;
                    case ResourceType::WATER: stats.water_consumed += amount; break;
                    case ResourceType::ENERGY: stats.energy_consumed += amount; break;
                    default: break;
                }
            }
        }
    }

    // востановление отключенных модулей
    for (auto& mod : modules) {
        if (mod->getState() == ModuleState::OFFLINE) {
            bool has_resources = true;
            for (auto const& [ResType, amount] : mod->getConsumption()) {
                if (amount > 0) {
                    Resource& res = resources.get_resource(ResType);
                    if (res.getCurrentAmount() < (amount * 4)) {
                        has_resources = false;
                        break;
                    }
                }
            }
            if (has_resources) {
                mod->setState(ModuleState::WORKING);
                cout << "Час " << currentHour << ": Модуль " << mod->getName() << " включен (ресурсы восстановлены)." << endl;
            }
        }
    }

    // 3. генерация аварии(каждый час, шанс 3%)
    if (rand() % 100 < 3) {
        generateAccident();
    }
    // 4. Обновление активных аварий
    updateActiveAccidents();

    // лечение в медицинских модулях
    process_medical_modules();

    // 5. ПРОВЕРКА КРИТИЧЕСКИХ УРОВНЕЙ
    vector<ResourceType> types = {ResourceType::OXYGEN, ResourceType::WATER, ResourceType::FOOD, ResourceType::ENERGY};
    for (ResourceType type : types) {
        Resource& res = resources.get_resource(type);
        if (res.isCritical()) {
            cout << "Час " << currentHour << ": КРИТИЧЕСКИЙ УРОВЕНЬ! Ресурс " << res.getName() << " на исходе! (" << res.getCurrentAmount() << "/" << res.getMaxCapacity() << ")" << endl;
        }
    }
    // 6. ПОТРЕБЛЕНИЕ РЕСУРСОВ КОЛОНИСТАМИ
    for (const auto& group : colonistGroups) {
        // Если группа мертва или пуста — пропускаем её
        if (group->get_state() == STATE_DEAD || group->get_count() == 0) continue;
        // "Сколько нужно кислорода, воды и еды на этот час группе?"
        Resources_consumption need = group->get_total_consumption();
        // Пытаемся списать эти ресурсы со склада. Если не хватило — возвращается false.
        ResourcesAvailability avail;
        avail.oxygen_ok = resources.get_resource(ResourceType::OXYGEN).consume(need.oxygen);
        avail.water_ok = resources.get_resource(ResourceType::WATER).consume(need.water);
        avail.food_ok = resources.get_resource(ResourceType::FOOD).consume(need.food);
        // Передаём результаты (чего хватило, а чего нет) в группу.
        // Если чего-то не хватило, здоровье снизится.
        int dead = group->update_health(avail);
        if (avail.oxygen_ok) stats.oxygen_consumed += need.oxygen;
        if (avail.water_ok) stats.water_consumed += need.water;
        if (avail.food_ok) stats.food_consumed += need.food;
        // Если кто-то умер — пишем в консоль
        if (dead > 0) {
            cout << "Час " << currentHour << ": Группа " << group->get_group_id()
                 << " потеряла " << dead << " колонистов!" << endl;
        }
    }
    // 7. ЗАДАЧИ И РОБОТЫ
    // Проверяем, могут ли роботы двигаться в текущую погоду
    bool canRobotsMove = weather.get_speed() > 0.0;
    // Проверка на наличие грузовых роботов в целом
    bool hasCargoRobot = false;
    for (const auto& r : robots) {
        if (r->get_type() == ROBOT_CARGO && r->get_state() != ROBOT_STATE_DESTROYED) {
            hasCargoRobot = true;
            break;
        }
    }
        // Анализ всех модулей, если робот есть и он может двигаться
    if (hasCargoRobot && canRobotsMove) {
        map<ModuleType, int> jobOpenings;
        map<ModuleType, vector<ColonyModule*>> modulesByType;
        for (const auto& mod : modules) {
            if (!mod->isOperational()) continue;
            if (mod->getType() == ModuleType::HABITAT) continue;
            jobOpenings[mod->getType()] += 1;
            modulesByType[mod->getType()].push_back(mod.get());
        }
        // Проверяем все группы колонистов
        for (auto& group : colonistGroups) {
            if (group->get_state() == STATE_DEAD || group->get_count() == 0) continue;
            if (group->get_transportRobot() != nullptr) continue;  // Уже перемещаются

            ModuleType neededModuleType;
            bool needsTransfer = false;
            // Сопоставляем специализацию с типом модуля
            // 1. ПРИОРИТЕТ 1: УСТАВШИЕ -> В ЖИЛОЙ МОДУЛЬ
            if (group->get_fatigue() > 70 && group->get_state() == STATE_WORKING) {
                neededModuleType = ModuleType::HABITAT;
                needsTransfer = true;
                }
            // 2. ОБЫЧНАЯ ЛОГИКА: ОВДЫХНУВШИЕ -> НА РАБОТУ
             else if(group->get_state() == STATE_WAITING &&  group->get_fatigue() < 30) {
                switch (group->get_specialization()) {
                    case SPEC_MINER:
                        if (jobOpenings[ModuleType::MINE] > 0) {
                            neededModuleType = ModuleType::MINE;
                            needsTransfer = true;
                        }
                        break;
                    case SPEC_BIOLOGIST:
                        if (jobOpenings[ModuleType::GREENHOUSE] > 0) {
                            neededModuleType = ModuleType::GREENHOUSE;
                            needsTransfer = true;
                        }
                        break;
                    case SPEC_ENGINEER:
                    case SPEC_TECHNICIAN:
                        if (jobOpenings[ModuleType::SOLAR_POWER] > 0 ||
                            jobOpenings[ModuleType::NUCLEAR_POWER] > 0 ||
                            jobOpenings[ModuleType::REPAIR_BAY] > 0 ||
                            jobOpenings[ModuleType::WATER_RECYCLER] > 0) {
                            if (jobOpenings[ModuleType::SOLAR_POWER] > 0) {
                                neededModuleType = ModuleType::SOLAR_POWER;
                            } else if (jobOpenings[ModuleType::REPAIR_BAY] > 0) {
                                neededModuleType = ModuleType::REPAIR_BAY;
                            } else if (jobOpenings[ModuleType::WATER_RECYCLER] > 0) {
                                neededModuleType = ModuleType::WATER_RECYCLER;
                            } else {
                                neededModuleType = ModuleType::NUCLEAR_POWER;
                            }
                            needsTransfer = true;
                        }
                        break;
                    case SPEC_DOCTOR:
                        if (jobOpenings[ModuleType::MEDICAL] > 0) {
                            neededModuleType = ModuleType::MEDICAL;
                            needsTransfer = true;
                        }
                        break;
                    case SPEC_ENERGY_OPERATOR:
                        if (jobOpenings[ModuleType::SOLAR_POWER] > 0 ||
                            jobOpenings[ModuleType::NUCLEAR_POWER] > 0) {
                            neededModuleType = ModuleType::SOLAR_POWER;
                            needsTransfer = true;
                        }
                        break;
                    case SPEC_REGULAR:
                        if (jobOpenings[ModuleType::MINE] > 0) {
                            neededModuleType = ModuleType::MINE;
                            needsTransfer = true;
                        } else if (jobOpenings[ModuleType::GREENHOUSE] > 0) {
                            neededModuleType = ModuleType::GREENHOUSE;
                            needsTransfer = true;
                        }
                        break;
                    default:
                        break;
                }
            }
            if (needsTransfer && !modulesByType[neededModuleType].empty()) {
                if (group->get_module() == neededModuleType) continue;// Группа уже здесь, пропускаем

                // Берём первый подходящий модуль этого типа
                ColonyModule* targetMod = modulesByType[neededModuleType][0];

                // Создаём задачу на перевозку этой конкретной группы
                Task moveTask(TASK_CARGO, targetMod, targetMod->getImportanceLevel(), 1.0, 10);
                taskQueue.addTask(moveTask);

                // Защита от отрицательных значений
                if (jobOpenings[neededModuleType] > 0) {
                    jobOpenings[neededModuleType] -= 1;
                }
                modulesByType[neededModuleType].erase(modulesByType[neededModuleType].begin());
            }
        }
    }
    // Роботы берут задачи из очереди
    for (const auto& robot : robots) {
        // Если робот не свободен — пропускаем
        if (robot->get_state() != ROBOT_STATE_WAITING_FOR_TASK) continue;
        if (robot->needs_maintenance()) continue;
        if (!canRobotsMove) continue;
        if (!taskQueue.isEmpty()) {
            Task currentTask = taskQueue.peek();
            if (!robot->can_perform_task(currentTask.getType())) continue;  // Этот робот не может выполнять такую задачу — пропускаем
            currentTask = taskQueue.getHighestPriority();// Теперь безопасно забираем задачу из очереди
            ColonyModule* targetMod = currentTask.getTargetModule();
            if (targetMod != nullptr && robot->assign_task(currentTask.getType(), currentTask.getComplexity(), weather)) {
                // Находим группу, которую нужно везти к этому модулю
                ColonistGroup* groupToTransport = nullptr;
                if (robot->get_type() == ROBOT_CARGO && currentTask.getType() == TASK_CARGO) {
                    for (auto& group : colonistGroups) {
                    if (group->get_module() != targetMod->getType() &&group->get_state() != STATE_DEAD &&group->get_transportRobot() == nullptr) {
                        // Проверяем, подходит ли специализация
                        bool matches = false;
                        switch (group->get_specialization()) {
                            case SPEC_MINER:
                                matches = (targetMod->getType() == ModuleType::MINE);
                                break;
                            case SPEC_BIOLOGIST:
                                matches = (targetMod->getType() == ModuleType::GREENHOUSE);
                                break;
                            case SPEC_ENGINEER:
                            case SPEC_TECHNICIAN:
                                matches = (targetMod->getType() == ModuleType::REPAIR_BAY ||
                                          targetMod->getType() == ModuleType::SOLAR_POWER ||
                                          targetMod->getType() == ModuleType::NUCLEAR_POWER ||
                                          targetMod->getType() == ModuleType::WATER_RECYCLER);
                                break;
                            case SPEC_DOCTOR:
                                matches = (targetMod->getType() == ModuleType::MEDICAL);
                                break;
                            case SPEC_ENERGY_OPERATOR:
                                matches = (targetMod->getType() == ModuleType::SOLAR_POWER ||
                                          targetMod->getType() == ModuleType::NUCLEAR_POWER);
                                break;
                            case SPEC_REGULAR:
                                matches = (targetMod->getType() == ModuleType::MINE ||
                                          targetMod->getType() == ModuleType::GREENHOUSE);
                                break;
                            default:
                                break;
                        }
                    if (matches) {
                            groupToTransport = group.get();
                            break;
                        }
                    }
                }
            }
            else if ((currentTask.getType() == TASK_REPAIR || currentTask.getType() == TASK_EMERGENCY) && robot->get_type() == ROBOT_REPAIR) {
                cout << "Ремонтный робот " << robot->get_id() << " направлен чинить модуль " << targetMod->getName() << endl;
                targetMod->setState(ModuleState::UNDER_REPAIR);
            }
            // Робот назначил задачу. Теперь ищем путь!
            vector<int> path = findShortestPath(robot->get_module()->getId(), targetMod->getId(), *robot, weather);
            if (!path.empty()) {
                robot->setRoute(path, targetMod);
                if (groupToTransport) {
                    robot->setPassengers(groupToTransport);
                    groupToTransport->set_transportRobot(robot.get());
                    groupToTransport->set_state(STATE_MOVING);
                    groupToTransport->set_opportunity_to_work(false);
                    cout << "Группа " << groupToTransport->get_group_id() << " села в робота " << robot->get_id() << endl;
                }
                cout << "Робот " << robot->get_id() << " начал движение к " << targetMod->getName() << endl;
            }
            else {
                cout << "Робот " << robot->get_id() << " не может найти путь!" << endl;
                robot->set_state(ROBOT_STATE_WAITING_FOR_TASK); // возвращаем в ожидание
            }
        }
    }
}

// 8 ДВИЖЕНИЕ РОБОТОВ
    for (auto& robot : robots) {
        if (robot->get_state() != ROBOT_STATE_MOVING) continue;
        ColonyModule* currentTarget = robot->getTargetModule();
        if (!currentTarget) {
            robot->set_state(ROBOT_STATE_WAITING_FOR_TASK);
            continue;
        }
        // Проверяем, прибыл ли робот
        if (robot->hasArrived()) {
            robot->set_state(ROBOT_STATE_WORKING);
            if (robot->get_type() == ROBOT_REPAIR) {
                // Начинаем/продолжаем ремонт
                currentTarget->startRepair(robot.get());
            } else if (robot->get_type() == ROBOT_CARGO) {
                robot->dropOffPassengers();
                robot->set_state(ROBOT_STATE_WAITING_FOR_TASK);
            }
            continue;
        }
        // Если ещё не прибыл — двигаемся
        robot->moveOneStep(modules);
    }
    // ==================== 9. ПРОДОЛЖЕНИЕ РЕМОНТА ====================
    for (auto& robot : robots) {
        if (robot->get_state() == ROBOT_STATE_WORKING && robot->get_type() == ROBOT_REPAIR) {
            ColonyModule* target = robot->getTargetModule();
            if (target && target->isUnderRepair()) {
                bool finished = target->performRepairStep(robot.get());
                if (finished) {
                    cout << "Робот " << robot->get_id() << " завершил ремонт модуля " << target->getName() << endl;
                    robot->set_state(ROBOT_STATE_WAITING_FOR_TASK);
                }
            }
        }
    }

    // Обновляем состояние каждого робота (тратит энергию, стареет)
    for (const auto& robot : robots) {
        robot->update();
    }
    // ОТПРАВЛЯЕМ ПОВРЕЖДЁННЫХ РОБОТОВ В РЕМЦЕХ
     // Находим все ремонтные цеха
    vector<RepairBay*> repairBays;
    for (const auto& mod : modules) {
        if (mod->getType() == ModuleType::REPAIR_BAY && mod->isOperational()) {
            repairBays.push_back(dynamic_cast<RepairBay*>(mod.get()));
        }
    }
    // Ищем роботов с высоким износом, Если износ > 70% — отправляем в ремонт
    for (auto& robot : robots) {
        // Пропускаем уже ремонтирующихся или уничтоженных
        if (robot->get_state() != ROBOT_STATE_MAINTENANCE && robot->get_state() != ROBOT_STATE_DESTROYED && robot->get_wear_level() > 70){
            // Ищем цех со свободным местом
            for (auto* bay : repairBays) {
                if (bay->hasCapacity()) {
                    bay->acceptRobotForRepair(robot.get());
                    break;
                }
            }
        }
    }
    // РЕМОНТИРУЕМ РОБОТОВ В ЦЕХЕ
    repairRobotsInBay();

    update_statistics();

    currentHour++; // Увеличиваем счётчик часов
}
// Запуск цикла симуляции на несколько дней
void Colony::run(){
    for (int i = 0; i < 5; i++) {
        weather.rand_weather();
        currentHour = 0;
        cout << "--- День " << (i + 1) << " ---" << endl;
        //int weat = weather.get_type();
        cout << "Погода " ;
        weather.print();
        cout << endl;
        for (int hour = 0; hour < 24; hour++) {
            tick();
        }
    }
    print_statistics();
    save_statistics("statistics.txt");
}

void Colony::generateAccident() {
    // Выбираем случайный модуль
    uniform_int_distribution<int> modDist(0, modules.size() - 1);
    int modIndex = modDist(rng);
    ColonyModule* targetModule = modules[modIndex].get();
    // Генерируем аварию (шанс уже проверили выше, поэтому probability = 100)
    Accident* accident = Accident_generator::generate_accident( weather, 100, targetModule);
    if (accident) {
        accident->apply_effect(targetModule, resources, colonistGroups, routes); // Применяем эффекты (урон модулю, потеря ресурсов)
        activeAccidents.push_back(accident); // Добавляем в список активных
        stats.total_accidents++;
        cout << "Час " << currentHour << ": АВАРИЯ! " << accident->get_aftereffect() << endl;
        if (accident->get_type() == Accident_type::Illness) {
            MedicalModule* medModule = nullptr;
            for (auto& mod : modules) {
                if (mod->getType() == ModuleType::MEDICAL) {
                    medModule = dynamic_cast<MedicalModule*>(mod.get());
                    break;
                }
            }
            if (medModule) {
                for (auto& group : colonistGroups) {
                    if (group->get_state() == STATE_TREATMENT && group->get_health() < 30 && group->get_count() > 0 && group->get_end_module() != medModule) {
                            if (!group->get_start_module()) {
                                group->set_start_module(group->get_end_module());
                            }
                        group->move_to_module(medModule);
                        medModule->add_patient(group);
                        cout << "Группа " << group->get_group_id() << " направлена в медблок." << endl;
                        }
                    }
            } else {
                cout << "Нет медицинского модуля для лечения!" << endl;
            }
        }

        // Создаём задачу на ремонт
        Task_type taskType = TASK_REPAIR;  // по умолчанию — ремонт
        switch (accident->get_type()) {
            case Accident_type::Oxyden_leakege:
            case Accident_type::Brake_water_system:
                taskType = TASK_EMERGENCY;  // аварийные работы
                break;
            case Accident_type::Illness:
                taskType = TASK_MEDICAL;  // медицинская помощь
                break;
            default:
                taskType = TASK_REPAIR;  // обычный ремонт
                break;
        }
        // Рассчитываем приоритет
        int moduleImportance = targetModule->getImportanceLevel();
        double damageLevel = accident->get_force() / 10.0;  // сила аварии (0.1 - 0.5)
        Task task(taskType, targetModule, moduleImportance, damageLevel, accident->get_force());

        // Добавляем в приоритетную очередь
        taskQueue.addTask(task);
        stats.total_repairs++;
    }
}

void Colony::updateActiveAccidents() {
    // Идём по всем активным авариям
    for (auto it = activeAccidents.begin(); it != activeAccidents.end(); ) {
        Accident* acc = *it;  // Получаем указатель на аварию
        acc->update();// Вызываем update() — он уменьшает time_to_end на 1
        // Проверяем, завершилась ли авария
        if (!acc->isActive()) {
            delete acc;  // Освобождаем память!
            it = activeAccidents.erase(it);  // Удаляем элемент из вектора
        } else {
            // Авария ещё активна — переходим к следующей
            ++it;
        }
    }
}

void Colony::repairRobotsInBay() {
    // Находим все ремонтные цеха
    for (const auto& mod : modules) {
        if (mod->getType() != ModuleType::REPAIR_BAY) continue;
        if (!mod->isOperational()) continue;
        RepairBay* bay = dynamic_cast<RepairBay*>(mod.get());
        if (!bay) continue;
        // Ремонтируем всех роботов в этом цехе
        int repaired = bay->repairAllRobots();
        stats.robots_repaired += repaired;
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

void Colony::show_graph(){
    export_dot("colony_graph.dot");
    int res = system("dot -Tpng colony_graph.dot -o colony_graph.png");
    if (res == 0){
        system("start colony_graph.png");
    } else {
        cout << "нет проги" << endl;
    }
}

void Colony::process_medical_modules() {
    for (auto& mod : modules) {
        if (mod->getType() == ModuleType::MEDICAL) {
            MedicalModule* med = dynamic_cast<MedicalModule*>(mod.get());
            if (med) {
                med->treat_patients();
            }
        }
    }
}


void Colony::reset_statistics(){
    stats = Statistics();
    stats.initial_colonists = 0;
    for (const auto& group : colonistGroups) {
        stats.initial_colonists += group->get_count();
    }
    stats.current_colonists = stats.initial_colonists;
}

void Colony::update_statistics(){
    stats.current_colonists = 0;
    for (const auto& group : colonistGroups) {
        stats.current_colonists += group->get_count();
    }
    stats.colonists_died = stats.initial_colonists - stats.current_colonists;

    double total_health = 0.0;
    int module_count = 0;
    for (const auto& mod : modules) {
        if (mod->isOperational()) {
            total_health += mod->getCurrentHealth();
            module_count++;
        }
    }
    stats.avg_modules_health = (module_count > 0) ? total_health / module_count : 0.0;

    stats.total_hours++;
    if (stats.total_hours % 24 == 0) stats.total_days++;
}

void Colony::print_statistics() const {
    cout << "\n========== СТАТИСТИКА КОЛОНИИ ==========\n";
    cout << "Количество дней симуляции: " << stats.total_days << endl;
    cout << "Количество часов: " << stats.total_hours << endl;
    cout << "Начальное количество колонистов: " << stats.initial_colonists << endl;
    cout << "Текущее количество колонистов: " << stats.current_colonists << endl;
    cout << "Количество умерших колонистов: " << stats.colonists_died << endl;
    cout << "Количество аварий: " << stats.total_accidents << endl;
    cout << "Количество ремонтов модулей: " << stats.total_repairs << endl;
    cout << "Количество отремонтированных роботов: " << stats.robots_repaired << endl;
    cout << "Среднее здоровье модулей: " << stats.avg_modules_health << endl;
    cout << "\n========== ПРОИЗВОДСТВО РЕСУРСОВ ==========\n";
    cout << "Количество произведенной пищи: " << stats.food_produced << endl;
    cout << "Количество произведенного кислорода: " << stats.oxygen_produced << endl;
    cout << "Количество произведенной воды: " << stats.water_produced << endl;
    cout << "Количество произведенной энергии: " << stats.energy_produced << endl;
    cout << "\n========== ПОТРЕБЛЕНИЕ РЕСУРСОВ ==========\n";
    cout << "Количество потребленной пищи: " << stats.food_consumed << endl;
    cout << "Количество потребленного кислорода: " << stats.oxygen_consumed << endl;
    cout << "Количество потребленной воды: " << stats.water_consumed << endl;
    cout << "Количество потребленной энергии: " << stats.energy_consumed << endl;
    cout << "============================================\n";
}


void Colony::save_statistics(const string& file_name) const {
    ofstream out(file_name);
    if (!out) {
        cerr << "Ошибка открытия файла!" << endl;
        return;
    }
    out << "Количество дней симуляции: " << stats.total_days << endl;
    out << "Количество часов: " << stats.total_hours << endl;
    out << "Начальное количество колонистов: " << stats.initial_colonists << endl;
    out << "Текущее количество колонистов: " << stats.current_colonists << endl;
    out << "Количество умерших колонистов: " << stats.colonists_died << endl;
    out << "Количество аварий: " << stats.total_accidents << endl;
    out << "Количество ремонтов модулей: " << stats.total_repairs << endl;
    out << "Количество отремонтированных роботов: " << stats.robots_repaired << endl;
    out << "Среднее здоровье модулей: " << stats.avg_modules_health << endl;
    out << "Количество произведенной пищи: " << stats.food_produced << endl;
    out << "Количество произведенного кислорода: " << stats.oxygen_produced << endl;
    out << "Количество произведенной воды: " << stats.water_produced << endl;
    out << "Количество произведенной энергии: " << stats.energy_produced << endl;
    out << "Количество потребленной пищи: " << stats.food_consumed << endl;
    out << "Количество потребленного кислорода: " << stats.oxygen_consumed << endl;
    out << "Количество потребленной воды: " << stats.water_consumed << endl;
    out << "Количество потребленной энергии: " << stats.energy_consumed << endl;
    out.close();
    cout << "Статистика сохранена в " << file_name << endl;
}

// ==================== АЛГОРИТМ ДЕЙКСТРЫ ====================
shared_ptr<ColonyModule> Colony::findModuleById(int id) const { // Найти модуль по ID
    for (const auto& mod : modules) { //mod - умный указатель на модуль, проходит по всему вектору modules
        if (mod->getId() == id) {
            return mod; // возращает указатель на модуль
        }
    }
    return nullptr;
}
vector<shared_ptr<TransportRoute>> Colony::getRoutesForModule(int moduleId) const { // Найти все пути, связанные с модулем
    vector<shared_ptr<TransportRoute>> result; ///путой вектор путей (будет содержать пути с искомым модулем)
    for (const auto& route : routes) { // route - умный указатель на путь
        int startId = route->getStartModule()->getId(); //id первого модуля
        int endId = route->getEndModule()->getId();     //id второго модуля
        if (startId == moduleId || endId == moduleId) { // Если модуль является началом или концом пути
            result.push_back(route);
        }
    }
    return result; //возращаем вектор путей
}
// Алгоритм Дейкстры — поиск кратчайшего пути по времени
vector<int> Colony::findShortestPath(int startModuleId, int endModuleId, const Robot& robot, const Weather& weather) {
    // Если старт и конец совпадают
    if (startModuleId == endModuleId) {
        return {startModuleId};
    }
    // Проверка, что модули существуют
    if (!findModuleById(startModuleId) || !findModuleById(endModuleId)) {
        cout << "Ошибка: один из модулей не найден!\n";
        return {};
    }
    map<int, double> dist; // Расстояния (время) от старта до каждого модуля
    map<int, int> parent; // Откуда пришли в каждый модуль (для восстановления пути)
    // Обработанные модули
    set<int> visited;    // Обработанные модули, set - тоже самое, что и вектор, но в него нельзя добавить два одинаковых значений

    // Инициализация: все расстояния = бесконечность
    for (const auto& mod : modules) {
        dist[mod->getId()] = numeric_limits<double>::infinity();
    }

    // Расстояние до старта = 0
    dist[startModuleId] = 0.0;
    parent[startModuleId] = -1;

    // Приоритетная очередь (min-heap по времени)
    priority_queue<PathNode, vector<PathNode>, greater<PathNode>> pq; //greater<PathNode> вызывает operator>, те сортирует по возрастанию времени прохождения
    pq.push({startModuleId, 0.0}); // добавляем стартовый узел графа
    while (!pq.empty()) { //пока очередь не пустая
        int currentId = pq.top().moduleId;// Берём модуль с наименьшим временем
        double currentTime = pq.top().time;
        pq.pop(); // удаляем его из очереди
        if (visited.count(currentId)) {// Если уже обработали — пропускаем
            continue;
        }
        visited.insert(currentId); //добавляем в конец
        // Если достигли цели — выходим
        if (currentId == endModuleId) {
            break;
        }
        // Получаем все пути, связанные с текущим модулем
        auto connectedRoutes = getRoutesForModule(currentId);
        for (const auto& route : connectedRoutes) {
            // Определяем соседний модуль
            int neighborId;
            if (route->getStartModule()->getId() == currentId) {//если текущий модуль начало пути - сосед это конец пути
                neighborId = route->getEndModule()->getId();
            } else {
                neighborId = route->getStartModule()->getId();
            }
            if (visited.count(neighborId)) { // Если уже обработали — пропускаем
                continue;
            }
            if (!route->isPassable()) { // Проверяем, доступен ли путь
                continue;
            }
            // Вычисляем время прохождения пути (учитывает робота, погоду и состояние пути)
            double travelTime = route->calculateTravelTime(robot, weather);

            // Если путь непроходим (время = бесконечность) — пропускаем
            if (travelTime == numeric_limits<double>::infinity()) {
                continue;
            }

            // Новое время = текущее + время перехода
            double newTime = currentTime + travelTime;

            // Если нашли более быстрый путь — обновляем
            if (newTime < dist[neighborId]) {
                dist[neighborId] = newTime;
                parent[neighborId] = currentId;
                pq.push({neighborId, newTime});
            }
        }
    }
    // Проверяем, достижима ли цель
    if (dist[endModuleId] == numeric_limits<double>::infinity()) {
        cout << "Путь от модуля " << startModuleId
             << " к модулю " << endModuleId << " не найден!\n";
        return {};
    }
    // Восстанавливаем путь от конца к началу
    vector<int> path;
    for (int at = endModuleId; at != -1; at = parent[at]) {
        path.push_back(at);
    }
    // Переворачиваем (от начала к концу)
    reverse(path.begin(), path.end());
    // Вывод информации о пути
    cout << "Найден путь от модуля " << startModuleId << " к модулю " << endModuleId << ":\n";
    cout << "Время прохождения: " << dist[endModuleId] << " шагов\n";
    cout << "Маршрут: ";
    for (size_t i = 0; i < path.size(); i++) {
        auto mod = findModuleById(path[i]);
        if (mod) {
            cout << mod->getName();
            if (i < path.size() - 1) cout << " -> ";
        }
    }
    cout << "\n";
    return path;
}
