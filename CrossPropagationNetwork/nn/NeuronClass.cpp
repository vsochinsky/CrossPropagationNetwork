#include "stdafx.h"

#include "NeuronClass.h"

    Neuron::Neuron(){        
      old_sensor_excitement = 0;
      old_actuator_excitement = 0;
      actuator_excitement = 0;
      sensor_excitement = 0;
      threshold = 0;
      
    }

  
    void Neuron::SetCoefRand(double range_of_coef, int num_axons){///no changes since old version
      int i;
      for(i = 0; i < num_axons; i++){
		  out_axon_coef.push_back(std::make_pair(range_of_coef * (double)rand() / (double)RAND_MAX, 0));
      }
    }




  
    void Neuron::AddAxonConnection(Neuron *exit){//to be tested, because real number of axons may be wrong (don't match num_axons)      
      out_neuron.push_back(exit);
	  
	  exit->in_neuron.push_back(this);

	  exit->in_axon_coef.push_back(std::make_pair(out_axon_coef[out_neuron.size() - 1].first,  out_neuron.size() - 1));
	  out_axon_coef[out_neuron.size() - 1].second = exit->in_neuron.size() - 1;
    }
    
    

    void Neuron::ExcitementDecrease(double decrease_speed){

	  actuator_excitement = actuator_excitement - (100 - actuator_excitement) * decrease_speed / 100; // *= pow(decrease_speed, (-1) * fabs((double)actuator_excitement));
      sensor_excitement = sensor_excitement - (100 - sensor_excitement) * decrease_speed / 100;// *= pow(decrease_speed, (-1) * fabs((double)sensor_excitement));
    }
  
  
    int Neuron::SensorSpike(double teach_force){				
      unsigned int i;
      
      if(old_sensor_excitement >= threshold){
	      for(i = 0; i < out_neuron.size(); i++){
	        out_neuron[i]->sensor_excitement = out_neuron[i]->sensor_excitement + (100 - out_neuron[i]->sensor_excitement)*out_axon_coef[i].first/100;	// TODO normalize all the network by 100
	        if(out_neuron[i]->sensor_excitement >= out_neuron[i]->threshold){
	          out_neuron[i]->in_axon_coef[out_axon_coef[i].second].first += teach_force;  
	        }
	  
	      }
        old_sensor_excitement = 0;    ///discharging neuron after spike
	      
        return 1;
      }
      
      return 0;
    }
    
    int Neuron::Spike(){
      unsigned int i;
      if(old_sensor_excitement >= threshold){
	  for(i = 0; i < out_neuron.size(); i++){
	        out_neuron[i]->sensor_excitement += out_axon_coef[i].second;
	  }
	  old_sensor_excitement = 0;
	  return 1;
      }
      return 0;
    }
    
    int Neuron::ActuatorSpike(double teach_force){
      unsigned int i;
      
      if(old_actuator_excitement >= threshold){
	      for(i = 0; i < in_neuron.size(); i++){

			in_neuron[i]->actuator_excitement = in_neuron[i]->actuator_excitement + (100 - in_neuron[i]->actuator_excitement) * in_axon_coef[i].first / 100;
	        
			  if(in_neuron[i]->actuator_excitement >= in_neuron[i]->threshold){
				  in_neuron[i]->out_axon_coef[in_axon_coef[i].second].first += teach_force;
				}
	      }
	      
        old_actuator_excitement = 0;///discharging neuron after spike
	      return 1;
      
      }
      
      return 0;
    }

    void Neuron::Forgetting(double forgetting_force){
      unsigned int i;
		for(i = 0; i < out_axon_coef.size(); i++){
			out_axon_coef[i].first *= forgetting_force;
      }
    for(i = 0; i < in_axon_coef.size(); i++){
			in_axon_coef[i].first *= forgetting_force;
	}
	}