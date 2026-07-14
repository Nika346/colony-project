#ifndef WEATHER_H_INCLUDED
#define WEATHER_H_INCLUDED

#pragma once
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

enum Weather_type {Clear, Cloudy, Weak_storm, Strong_storm, Extremely_cold, Radioactive_outbreak, Meteor_shower, Calm};

struct Weathereffect{                    // эффекты погоды
    double solar_productivity;
    bool moving;
    double risk_of_break;
    bool work_out;
    double speed;                     // предположительно для поботов

};

class Weather{
private:
    Weather_type weath;
public:
    Weather();
    ~Weather();

    void rand_weather();
    Weather_type get_type() const;
    double get_solar_productivity() const;
    bool get_moving() const;
    double get_risk_of_break() const;
    bool get_work_out() const;
    double get_speed() const;

    void print() const;

};

extern const Weathereffect weatherarr[];

#endif // WEATHER_H_INCLUDED
