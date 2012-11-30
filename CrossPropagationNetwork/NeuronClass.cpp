

#include "NeuronClass.h"

    Neuron::Neuron(){        
      old_sensor_excitement = 0;
      old_actuator_excitement = 0;
      actuator_excitement = 0;
      sensor_excitement = 0;
      threshold = 0;
      mark_of_actuator = 0;
      mark_of_sensor = 0;
    }

  
    void Neuron::SetCoefRand(double range_of_coef, int num_axons){///no changes since old version
      int i;
      for(i = 0; i < num_axons; i++){
        axon_coef.push_back(range_of_coef * (double)rand() / (double)RAND_MAX);
      }
    }


    void Neuron::AddDendriteConnection(Neuron *enter){
     in_neuron.push_back(enter);
    }

  
    void Neuron::AddAxonConnection(Neuron *exit){//to be tested, because real number of axons may be wrong (don't match num_axons)      
      out_neuron.push_back(exit);
    }
    
    

    void Neuron::ExcitementDecrease(double decrease_speed){
	  //actuator_excitement *= pow(decrease_speed, (-1) * fabs((double)actuator_excitement));
      //sensor_excitement *= pow(decrease_speed, (-1) * fabs((double)sensor_excitement));
	  //mark_of_actuator *= 1 / decrease_speed;
      //mark_of_sensor *= 1 / decrease_speed;		
    }
  
  
    int Neuron::SensorSpike(){				
      int i;
      
      if(old_sensor_excitement >= threshold){
	      for(i = 0; i < out_neuron.size(); i++){
	        out_neuron[i]->sensor_excitement += axon_coef[i];
	        if(out_neuron[i]->sensor_excitement >= out_neuron[i]->threshold){
	          out_neuron[i]->mark_of_sensor = 1;  
	        }
	  
	      }
        old_sensor_excitement = 0;    ///discharging neuron after spike
	      
        return 1;
      }
      
      return 0;
    }
    
    int Neuron::Spike(){
      int i;
      if(old_sensor_excitement >= threshold){
	  for(i = 0; i < out_neuron.size(); i++){
	        out_neuron[i]->sensor_excitement += axon_coef[i];
	  }
	  old_sensor_excitement = 0;
	  return 1;
      }
      return 0;
    }
    
    int Neuron::ActuatorSpike(){
      int i, axon_num = 0;
      
      if(old_actuator_excitement >= threshold){
	      for(i = 0; i < in_neuron.size(); i++){
	        
          //finding axon which is connected to this neuron, his number is "axon_num"
          while(in_neuron[i]->out_neuron[axon_num] != this){
            axon_num++;
	        }
	        
          in_neuron[i]->actuator_excitement += in_neuron[i]->axon_coef[axon_num];
	        
          if(in_neuron[i]->actuator_excitement >= in_neuron[i]->threshold){
	          in_neuron[i]->mark_of_actuator = 1;
	        }
	      }
	      
        old_actuator_excitement = 0;///discharging neuron after spike
	      return 1;
      
      }
      
      return 0;
    }

    void Neuron::Forgetting(double forgetting_force){
      int i;
      for(i = 0; i < axon_coef.size(); i++){
	axon_coef[i] -= forgetting_force;
      }
    }
