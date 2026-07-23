//
// Created by Вероника on 02.07.2026.
//
#ifndef MYPROJECT_ENUM_COL_ROB_H
#define MYPROJECT_ENUM_COL_ROB_H
#include <string>
using namespace std;
// Специализации колонистов - определяют, какие задачи может выполнять колонист
enum Colonist_spec {
    SPEC_ENGINEER,          
    SPEC_BIOLOGIST,       
    SPEC_MINER,             
    SPEC_DOCTOR,           
    SPEC_ENERGY_OPERATOR,   
    SPEC_TECHNICIAN,       
    SPEC_REGULAR           
};
// Состояния колонистов - отражают текущую активность
enum Colonist_state {
    STATE_RESTING,          
    STATE_WORKING,          
    STATE_MOVING,          
    STATE_REPAIRING,      
    STATE_TREATMENT,       
    STATE_WAITING,          
    STATE_IN_DANGER,       
    STATE_DEAD,            
};
// Типы задач, которые могут выполнять колонисты и роботы
enum Task_type {
    TASK_MINING,            
    TASK_GREENHOUSE,       
    TASK_ENERGY, 
    TASK_REPAIR,  
    TASK_MEDICAL,   
    TASK_EMERGENCY,      
    TASK_CARGO,             
    TASK_MAINTENANCE      
};
// Типы роботов
enum Robot_type {
    ROBOT_MINING,           
    ROBOT_REPAIR,          
    ROBOT_MEDICAL,       
    ROBOT_CARGO
};
// Состояния роботов
enum Robot_state {
    ROBOT_STATE_WORKING,       
    ROBOT_STATE_MOVING,        
    ROBOT_STATE_CHARGING,    
    ROBOT_STATE_DAMAGED,     
    ROBOT_STATE_WAITING_FOR_TASK, 
    ROBOT_STATE_DESTROYED,    
    ROBOT_STATE_MAINTENANCE   
// Функции для преобразования перечислений в строки (для вывода на экран)
string spec_to_str(Colonist_spec spec);
string state_to_str(Colonist_state state);
string task_type_to_str(Task_type task);
string robot_type_to_str(Robot_type robot);
string robot_state_to_str(Robot_state state);
#endif //MYPROJECT_ENUM_COL_ROB_H
