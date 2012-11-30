#ifndef NEURON_CLASS_H
#define NEURON_CLASS_H

#pragma once

#include "headers.h"


class Neuron{
  public:
    typedef std::vector< Neuron *> Neuron_lists;
    typedef std::vector<double> Coef_lists;
    Neuron_lists out_neuron, in_neuron;
    Coef_lists axon_coef;
    double old_sensor_excitement, old_actuator_excitement, sensor_excitement, actuator_excitement, threshold, mark_of_sensor, mark_of_actuator; 

    Neuron();

  
    void SetCoefRand(double range_of_coef, int num_axons);


    void AddDendriteConnection(Neuron *enter);

  
    void AddAxonConnection(Neuron *exit);
    
    

    void ExcitementDecrease(double decrease_speed);
  
  
    int SensorSpike();
    
    int Spike();
    
    int ActuatorSpike();

    void Forgetting(double forgetting_force);
};

#endif //NEURON_CLASS_H