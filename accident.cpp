//
// Created by Вероника on 06.07.2026.
//
#include "Accident.h"
// события
Accident::Accident(Accident_type t, int f, int dur, const string& desc, int urg, ColonyModule* module): accid(t), force(f), long_time(dur), aftereffect(desc), urgency(urg), is_active(true), time_to_end(dur), Mod(module) {}
Accident::~Accident() {}
void Accident::apply_effect(ColonyModule* mod, ColonyResourceManager& resourceManager) {
    // Если переданный модуль равен nullptr, берём сохранённый (поле Mod)
    if (!mod) mod = Mod;
    if (!mod) {
        cout << "Ошибка: модуль не задан для события." << endl;
        return;
    }
    switch (accid) {
        case Accident_type::Oxyden_leakege:
            resourceManager.get_resource(ResourceType::OXYGEN).consume(force * 10);
            mod->takeDamage(force * 2);
            cout << "Утечка кислорода в модуле " << mod->getName() << "!" << endl;
            break;
        case Accident_type::Break_solar_panel:
            mod->takeDamage(force * 5);
            cout << "Поломка солнечных панелей в модуле " << mod->getName() << "!" << endl;
            break;
        case Accident_type::Damage_transition:
            cout << "Повреждён переход между модулями!" << endl;
            break;
        case Accident_type::Brake_water_system:
            resourceManager.get_resource(ResourceType::WATER).consume(force * 15);
            mod->takeDamage(force * 2);
            cout << "Отказ системы очистки воды в модуле " << mod->getName() << "!" << endl;
            break;
        case Accident_type::Fire:
            mod->takeDamage(force * 8);
            cout << "Пожар в модуле " << mod->getName() << "!" << endl;
            break;
        case Accident_type::Mine_collapse:
            mod->takeDamage(force * 10);
            cout << "Обрушение в шахте " << mod->getName() << "!" << endl;
            break;
        case Accident_type::Illness:
            resourceManager.get_resource(ResourceType::FOOD).consume(force * 5);
            resourceManager.get_resource(ResourceType::WATER).consume(force * 3);
            cout << "Вспышка болезни! Требуется медицинская помощь." << endl;
            break;
        case Accident_type::Damage_robot:
            cout << "Робот повреждён! Требуется ремонт." << endl;
            break;
        case Accident_type::New_deposit:
            resourceManager.get_resource(ResourceType::BUILDING_MATERIALS).produce(force * 30);
            cout << "Обнаружено новое месторождение! Добыто " << force * 30 << " материалов." << endl;
            break;
        case Accident_type::Successful_research:
            cout << "Успешное исследование! Эффективность повышена." << endl;
            break;
        case Accident_type::Communication_failure:
            cout << "Сбой связи! Временная изоляция." << endl;
            break;
        case Accident_type::Meteorite_fall:
            mod->takeDamage(force * 12);
            cout << "Метеорит попал в модуль " << mod->getName() << "!" << endl;
            break;
        case Accident_type::Sudden_storm:
            mod->takeDamage(force * 3);
            resourceManager.get_resource(ResourceType::ENERGY).consume(force * 10);
            cout << "Сильная пылевая буря повредила оборудование!" << endl;
            break;
        case Accident_type::Yield_growth:
            resourceManager.get_resource(ResourceType::FOOD).produce(force * 20);
            cout << "Внезапный рост урожайности! Собрано " << force * 20 << " еды." << endl;
            break;
        default:
            mod->takeDamage(force * 2);
            cout << "Неизвестное событие." << endl;
            break;
    }
}
// Обновление
void Accident::update() {
    if (is_active) {
        time_to_end--;
        if (time_to_end <= 0) {
            is_active = false;
            cout << "Событие \"" << aftereffect << "\" завершено." << endl;
        }
    }
}
bool Accident::isActive() const { return is_active; }
Accident_type Accident::get_type() const { return accid; }
int Accident::get_force() const { return force; }
int Accident::get_time_to_end() const { return time_to_end; }
string Accident::get_aftereffect() const { return aftereffect; }
ColonyModule* Accident::get_module() const { return Mod; }
int Accident::get_urgency() const { return urgency; }
// Приоритет
int Accident::get_priority(const ColonyModule* mod) const {
    if (!mod) return urgency;
    return (mod->getImportanceLevel() * 2 + urgency + force) / 2;
}
// генератор
Accident* Accident_generator::generate_accident(const Weather& weather, int probability, ColonyModule* Mod) {
    if (rand() % 100 >= probability) return nullptr;
    Weather_type w = weather.get_type();
    Accident_type type;
    int force = rand() % 5 + 1;
    int long_time = rand() % 3 + 1;
    string desc;
    int urgency = 5;
    switch (w) {
        case Weather_type::Strong_storm:
            if (rand() % 2 == 0) {
                type = Accident_type::Sudden_storm;
                desc = "Сильная пылевая буря! Оборудование повреждено.";
                force += 2;
                urgency = 8;
            } else {
                type = Accident_type::Break_solar_panel;
                desc = "Солнечные панели забиты песком и повреждены.";
                urgency = 7;
            }
            break;
        case Weather_type::Meteor_shower:
            type = Accident_type::Meteorite_fall;
            desc = "Метеоритный дождь! Попадание в модуль.";
            force += 3;
            urgency = 9;
            long_time = 1;
            break;
        case Weather_type::Radioactive_outbreak:
            type = Accident_type::Damage_robot;
            desc = "Радиационная вспышка вызвала сбой электроники.";
            urgency = 8;
            break;
        default:
            int r = rand() % 10;
            switch (r) {
        case 0: type = Accident_type::Oxyden_leakege; desc = "Утечка кислорода"; urgency = 10; break;
                case 1: type = Accident_type::Brake_water_system; desc = "Отказ системы воды"; urgency = 9; break;
                case 2: type = Accident_type::Fire; desc = "Пожар"; urgency = 8; break;
                case 3: type = Accident_type::Mine_collapse; desc = "Обрушение в шахте"; urgency = 8; break;
                case 4: type = Accident_type::Illness; desc = "Вспышка болезни"; urgency = 9; break;
                case 5: type = Accident_type::Damage_robot; desc = "Повреждение робота"; urgency = 6; break;
                case 6: type = Accident_type::Communication_failure; desc = "Сбой связи"; urgency = 5; break;
                case 7: type = Accident_type::Break_solar_panel; desc = "Поломка панелей"; urgency = 6; break;
                case 8: type = Accident_type::Successful_research; desc = "Успешное исследование"; urgency = 2; break;
                case 9: type = Accident_type::New_deposit; desc = "Новое месторождение"; urgency = 3; break;
            }
            break;
    }
    if (type == Accident_type::New_deposit || type == Accident_type::Successful_research || type == Accident_type::Yield_growth) {
        long_time = 1;
        force = rand() % 3 + 1;
    }
    return new Accident(type, force, long_time, desc, urgency, Mod);
}