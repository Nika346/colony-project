//
// Created by Вероника on 06.07.2026.
//
#include "Weather.h"


//погода


const Weathereffect weatherarr[] = {
        {1.0, true, 0.01, true, 1.0},   // Clear
        {0.6, true, 0.02, true, 1.0},   // Cloudy
        {0.3, true, 0.20, true, 0.7},   // LightDustStorm
        {0.0, false, 0.50, false, 0.0}, // HeavyDustStorm
        {0.8, true, 0.15, true, 0.5},   // ExtremeCold
        {0.5, false, 0.30, false, 0.0}, // RadiationFlare
        {0.2, false, 0.80, false, 0.0}, // MeteorShower
        {0.9, true, 0.01, true, 1.0}    // Calm
};


Weather::Weather() {
        weath = static_cast<Weather_type>(rand() % 8);
}


Weather::~Weather() {}


void Weather::rand_weather() {
        weath = static_cast<Weather_type>(rand() % 8);
}


Weather_type Weather::get_type() const {
        return weath;
}


double Weather::get_solar_productivity() const {
        return weatherarr[static_cast<int>(weath)].solar_productivity;
}


bool Weather::get_moving() const {
        return weatherarr[static_cast<int>(weath)].moving;
}


double Weather::get_risk_of_break() const {
        return weatherarr[static_cast<int>(weath)].risk_of_break;
}


bool Weather::get_work_out() const {
        return weatherarr[static_cast<int>(weath)].work_out;
}


double Weather::get_speed() const {
        return weatherarr[static_cast<int>(weath)].speed;
}


void Weather::print() const {
        switch (weath) {
                case Weather_type::Clear: cout << "ясна€"; break;
                case Weather_type::Cloudy: cout << "ќблачна€"; break;
                case Weather_type::Weak_storm: cout << "—лаба€ пылева€ бур€"; break;
                case Weather_type::Strong_storm: cout << "—ильна€ пылева€ бур€"; break;
                case Weather_type::Extremely_cold: cout << "Ёкстремальный холод"; break;
                case Weather_type::Radioactive_outbreak: cout << "–адиационна€ вспышка"; break;
                case Weather_type::Meteor_shower: cout << "ћетеоритный дождь"; break;
                case Weather_type::Calm: cout << "—покойна€"; break;
        }
}