//
// Created by Вероника on 06.07.2026.
//

#ifndef MYPROJECT_ACCIDENT_H
#define MYPROJECT_ACCIDENT_H
#include "weather.h"
#include "resources.h"
#include "modules.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>


using namespace std;


enum class Accident_type {Oxyden_leakege, Break_solar_panel, Damage_transition, Brake_water_system, Fire, Mine_collapse, Illness, Damage_robot, New_deposit, Successful_research, Communication_failure, Meteorite_fall, Sudden_storm, Yield_growth};
//кислород, панели, переход, вода, огонь, обрушение шахты, повреждение робота, новое месторождение, успешное исслелование, сбой связи, падение метеорита, внезапная буря, рост урожайности


class Accident{
private:
        Accident_type accid;
        int force;
        int long_time;
        string aftereffect;     // последствия
        int urgency;
        bool is_active;
        int time_to_end;
        ColonyModule* Mod;
public:
        Accident(Accident_type t, int f, int dur, const string& desc, int urg, ColonyModule* module = nullptr);


        void apply_effect(ColonyModule* mod, ColonyResourceManager& resourceManager);
        void update(); // обновление собфтий
        bool isActive() const;
        Accident_type get_type() const;
        int get_force() const;
        int get_time_to_end() const;
        string get_aftereffect() const;
        ColonyModule* get_module() const;
        // приоритет
        int get_priority(const ColonyModule* mod) const;
        int get_urgency() const;


        ~Accident();



};


class Accident_generator {
public:
        static Accident* generate_accident(const Weather& weather, int probability, ColonyModule* Mod = nullptr);
};
#endif //MYPROJECT_ACCIDENT_H