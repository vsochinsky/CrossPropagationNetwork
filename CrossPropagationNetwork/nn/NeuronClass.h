#ifndef NEURON_CLASS_H
#define NEURON_CLASS_H

#pragma once

#include "headers.h"


class Neuron{
  public:
    typedef std::vector< Neuron *> Neuron_lists;
	typedef std::pair<double, int> ppair;
    typedef std::vector<ppair> Coef_lists;
    Neuron_lists out_neuron, in_neuron;
    Coef_lists out_axon_coef, in_axon_coef;
    double old_sensor_excitement, old_actuator_excitement, sensor_excitement, actuator_excitement, threshold; 

    Neuron();

  
    void SetCoefRand(double range_of_coef, int num_axons);



  
    void AddAxonConnection(Neuron *exit);
    
    

    void ExcitementDecrease(double decrease_speed);
  
  
    int SensorSpike(double teach_force);
    
    int Spike();
    
    int ActuatorSpike(double teach_force);

    void Forgetting(double forgetting_force);
};

#endif //NEURON_CLASS_H