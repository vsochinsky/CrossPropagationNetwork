// This is the main DLL file.

//#include "NeuronClass.cpp"
#include "NetClass.h"






NeuralNet::NeuralNet()
{
  input_layer_set_size = 10;
  
	  Neuron neuron_for_pushback;
	  int i, e, a; // <= Matvey?
	  char temp[100];

	  neurons_excited = 0;
	  srand(time(NULL));
		net_iteration = 0;

	  std::ifstream  config_file("net_config.txt", std::ifstream::in);
	  sensors_file.open("sensors.txt", std::ifstream::in);
	  actuators_out.open("actuators_out.txt", std::ofstream::out);


	  if(!config_file.is_open()){
		  printf("!!!error opening net_config.txt!!!\n");
		 
		  exit(1);
	  }

	  config_file >> temp >> hidden_layer_size;
	  config_file >> temp >> num_axons;
	  config_file >> temp >> range_of_coef;
	  config_file >> temp >> init_threshold;       
	  config_file >> temp >> input_layer_size;
	  config_file >> temp >> output_layer_size;
	  config_file >> temp >> input_layer_set_size;

      std::cout << "from config: input layer size = " << input_layer_size << std::endl;

	  if(input_layer_size % input_layer_set_size != 0){
		std::cout << "warning: incorrect input layer!" << std::endl;
	  }
	  input_layer_set_number = input_layer_size / input_layer_set_size;

	  for(i = 0; i < input_layer_size; i++){///creating input_layer
		  input_layer.push_back(neuron_for_pushback);
		  input_layer[i].threshold = init_threshold;
		  input_layer[i].mark_of_sensor = 1;
		  input_layer[i].SetCoefRand(range_of_coef, num_axons);

	  }

	  for(i = 0; i < output_layer_size; i++){///creating output_layer
		  output_layer.push_back(neuron_for_pushback);
		  output_layer[i].mark_of_actuator = 1;
		  output_layer[i].threshold = init_threshold;
		  output_layer[i].SetCoefRand(range_of_coef, num_axons);
	  }

	  for(i = 0; i < hidden_layer_size; i++){///creating hidden_layer
		  hidden_layer.push_back(neuron_for_pushback);
		hidden_layer[i].threshold = init_threshold;
		hidden_layer[i].SetCoefRand(range_of_coef, num_axons);
	  }




	  for(i = 0; i < hidden_layer_size; i++){///connecting hidden_layer and output_layer  <= WTF?!

		if(i < num_axons * output_layer_size){///conecting output_layer
		  a = (int)((double)( output_layer_size * rand() ) / (double)RAND_MAX);     /// deleted "-1"

		  hidden_layer[i].AddAxonConnection(&output_layer[a]);
			output_layer[a].AddDendriteConnection(&hidden_layer[i]);

		  for(e = 1; e < num_axons; e++){
			a = (int)((double)(hidden_layer_size - 1) * ((double)rand() / (double)RAND_MAX));
			hidden_layer[i].AddAxonConnection(&hidden_layer[a]);
			hidden_layer[a].AddDendriteConnection(&hidden_layer[i]);
		  }

		  continue;

		}
		  for(e = 0; e < num_axons; e++){///conecting rest of hidden_layer
		  a = (int)((double)(hidden_layer_size - 1) * ((double)rand() / (double)RAND_MAX));

		  hidden_layer[i].AddAxonConnection(&hidden_layer[a]);
			hidden_layer[a].AddDendriteConnection(&hidden_layer[i]);

		}
	  }


	  for(i = 0; i < input_layer_size; i++){///connecting input_layer
		for(e = 0; e < num_axons; e++){
			a = (int)((double)(hidden_layer_size - 1) * ((double)rand() / (double)RAND_MAX));

		  input_layer[i].AddAxonConnection(&hidden_layer[a]);
			hidden_layer[a].AddDendriteConnection(&input_layer[i]);
		}
	  }

}


void NeuralNet::ActuatorIterate(){

	  int i;
	  net_excitement_actuators = 0;

	  
		  output_layer[1].ActuatorSpike();
	  std::cout << "1 ok"<<std::endl;

	  for(i = 0; i < output_layer_size; i++){
		  output_layer[i].old_actuator_excitement += output_layer[i].actuator_excitement;
		  output_layer[i].actuator_excitement = 0;
		  //output_layer[i].ExcitementDecrease(2);///decreasing BOTH excitements
	  }
	  std::cout << "2 ok"<<std::endl;
	  for(i = 0; i < hidden_layer_size; i++){
		  if(hidden_layer[i].ActuatorSpike()){
		  net_excitement_actuators++;
		}
	  }
	  std::cout << "3 ok"<<std::endl;
	  for(i = 0; i < hidden_layer_size; i++){
		  hidden_layer[i].old_actuator_excitement += hidden_layer[i].actuator_excitement;
		  hidden_layer[i].actuator_excitement = 0;
		  //hidden_layer[i].ExcitementDecrease(0);///decreasing BOTH excitements
	  }
	  std::cout << "4 ok"<<std::endl;
}



void NeuralNet::SensorIterate(){ ///this function will be spammed dy main
		int i;
			net_iteration++;
			net_excitement_sensors = 0;

		for(i = 0; i < input_layer_size; i++){
			input_layer[i].SensorSpike();
		}

		//switching excitements
		for(i = 0; i < input_layer_size; i++){
	        input_layer[i].old_sensor_excitement += input_layer[i].sensor_excitement;
	        input_layer[i].sensor_excitement = 0;
//	        input_layer[i].ExcitementDecrease(2);///decreasing BOTH excitements
        }
      
        for(i = 0; i < hidden_layer_size; i++){
	        if( hidden_layer[i].SensorSpike() )
		        net_excitement_sensors++;
        }
      
        for(i = 0; i < hidden_layer_size; i++){
	        hidden_layer[i].old_sensor_excitement += hidden_layer[i].sensor_excitement;
	        hidden_layer[i].sensor_excitement = 0;
	        hidden_layer[i].ExcitementDecrease(0);///decreasing BOTH excitements
        }
      
}

void NeuralNet::NetStat(){
		int i;
		std::cout << "===========================" << std::endl;
		std::cout << "Net iteration (by sensors) " << net_iteration << ":" << std::endl;
		std::cout << "Sensors: ";
		for(i = 0; i < input_layer.size(); i++){
			std::cout << input_layer[i].sensor_excitement << "  ";
		}
		std::cout << std::endl;
		std::cout << "Actuators: ";
    actuators_out << net_iteration << "  ";
		for(i = 0; i < output_layer.size(); i++){
			std::cout << output_layer[i].sensor_excitement << "  ";
      actuators_out << output_layer[i].sensor_excitement << " ";
      
		}
   std::cout <<std::cout;
    actuators_out <<std::cout;
		std::cout << "Neurons spiked (sensors side, only hidden): " << net_excitement_sensors << std::cout;
   std::cout << "Neurons spiked (actuators side, only hidden): " << net_excitement_actuators << std::cout;
}
    
  void NeuralNet::GetSensors(){ //from file
    int i;
	double musor;
	double value;

	int current_set_begining=0;
	int which_neuron_of_set;

	for(i=0; i<6; i++){
	  sensors_file >> musor;
	}
	for(i=0; i < input_layer_set_number; i++){
	  sensors_file >>  value;
	  /*input_layer[i].sensor_excitement*/ value = value / 255; //normirovka

	  which_neuron_of_set = (int) (value * (double) input_layer_set_size);
	 std::cout << "debug: value = " << value << " which_neuron = " << which_neuron_of_set <<std::cout;
	  input_layer[ current_set_begining + which_neuron_of_set ] . sensor_excitement = 1; //maximum possible value

	  current_set_begining += input_layer_set_size;

	}
  }

void NeuralNet::GetSensors(int*heights){ // interactive
	int i;
	double musor;
	double value;

	int current_set_begining=0;
	int which_neuron_of_set;

	for(i=0; i < input_layer_set_number; i++){
      printf("debug:height= %lf\n", heights[i]);
	  value = heights[i] / 255; //normirovka

	  which_neuron_of_set = (int) (value * (double) input_layer_set_size);
	 std::cout << "debug: value = " << value << " which_neuron = " << which_neuron_of_set <<std::cout;
	  input_layer[ current_set_begining + which_neuron_of_set ] . sensor_excitement = 1; //maximum possible value

	  current_set_begining += input_layer_set_size;

	}
  }

  
std::vector<bool> NeuralNet::Get_Actuator_State(){ //TODO: add ability to end program
    unsigned short i;
	std::vector<bool> command; 
	
    
    //checking sensors
	printf("checking sensors\n");
    for(i=0; i<4; i++){
      if (output_layer[i].sensor_excitement >= output_layer[i].threshold)
	//*command[i]='1';
		command.push_back(true);
      else
	//*command[i]='0';
		command.push_back(false);
    }
	printf(":Get_Actuator_State OK\n");
	return command;
    
}
void NeuralNet::Save(char *file_name){
    std::ofstream  save_file(file_name, std::ofstream::out);
    int i, e, f, n;
	
    for(i = 0; i < hidden_layer_size; i++){
		save_file << " "<< hidden_layer[i].threshold << " " << hidden_layer[i].sensor_excitement << " " << hidden_layer[i].old_sensor_excitement << " " << hidden_layer[i].actuator_excitement << " " << hidden_layer[i].old_actuator_excitement << " " << hidden_layer[i].mark_of_actuator << " " << hidden_layer[i].mark_of_sensor << " ";
      for(e = 0; e < num_axons; e++){
	save_file << " " << hidden_layer[i].axon_coef[e];
      }
      save_file << " ";
    }
    for(i=0; i < output_layer_size; i++){
      save_file << output_layer[i].threshold << " " << output_layer[i].sensor_excitement << " " << output_layer[i].old_sensor_excitement << " " << output_layer[i].actuator_excitement << " " << output_layer[i].old_actuator_excitement << " " << output_layer[i].mark_of_actuator << " " << output_layer[i].mark_of_sensor << " ";
      
    }
    for(i = 0; i < input_layer_size; i++){
      save_file << " " << input_layer[i].threshold << " " << input_layer[i].sensor_excitement << " " << input_layer[i].old_sensor_excitement << input_layer[i].actuator_excitement << " " << input_layer[i].old_actuator_excitement << " " << input_layer[i].mark_of_actuator << " " << input_layer[i].mark_of_sensor << " ";
      for(e = 0; e < num_axons; e++){
	save_file << " " << input_layer[i].axon_coef[e];
      }
      save_file << " ";
    }
    
    for(i = 0; i < hidden_layer_size; i++){
      for(e = 0; e < num_axons; e++){
	if(hidden_layer[i].out_neuron[e]->out_neuron.size() == 0){
	  for(f = 0; hidden_layer[i].out_neuron[e] != &output_layer[f]; f++){} ///be carefull!!!!!!!! it may be easier))))
	  save_file << hidden_layer_size + f << " ";
	}
	else{
	  for(f = 0; hidden_layer[i].out_neuron[e] != &hidden_layer[f]; f++){} ///be carefull!!!!!!!!
	  save_file << f << " ";
	}
    }
  }
   for(i = 0; i < input_layer_size; i++){
      for(e = 0; e < num_axons; e++){
	for(f = 0; input_layer[i].out_neuron[e] != &hidden_layer[f]; f++){} ///be carefull!!!!!!!!
	save_file << f << " ";
 }
   }
   save_file.close();
 }
  
  

void NeuralNet::Load(char * file_name){
   std::ifstream load_file(file_name, std::ifstream::in);
   int i, e, n;
   for(i = 0; i < hidden_layer_size; i++){
	 load_file >> hidden_layer[i].threshold >> hidden_layer[i].sensor_excitement >> hidden_layer[i].old_sensor_excitement >> hidden_layer[i].actuator_excitement >> hidden_layer[i].old_actuator_excitement >> hidden_layer[i].mark_of_actuator >> hidden_layer[i].mark_of_sensor;
	 for(e = 0; e < num_axons; e++){
	   load_file >> hidden_layer[i].axon_coef[e];
	}
	hidden_layer[i].out_neuron.clear();
	hidden_layer[i].in_neuron.clear();
  }
  for(i=0; i < output_layer_size; i++){
	  load_file >> output_layer[i].threshold >> output_layer[i].sensor_excitement >> output_layer[i].old_sensor_excitement >> output_layer[i].actuator_excitement >> output_layer[i].old_actuator_excitement >> output_layer[i].mark_of_actuator >> output_layer[i].mark_of_sensor;
	  output_layer[i].in_neuron.clear();
  }
  for(i = 0; i < input_layer_size; i++){
	  load_file >> input_layer[i].threshold >> input_layer[i].sensor_excitement >> input_layer[i].old_sensor_excitement >> input_layer[i].actuator_excitement >> input_layer[i].old_actuator_excitement >> input_layer[i].mark_of_actuator >> input_layer[i].mark_of_sensor;
	  for(e = 0; e < num_axons; e++){
	load_file >> input_layer[i].axon_coef[e];
	  }
	  input_layer[i].out_neuron.clear();
  }
   for(i = 0; i < hidden_layer_size; i++){
	  for(e = 0; e < num_axons; e++){
	load_file >> n;
	if(n >= hidden_layer_size){
	  hidden_layer[i].AddAxonConnection(&output_layer[n - hidden_layer_size]);
	  output_layer[n - hidden_layer_size].AddDendriteConnection(&hidden_layer[i]);
	}
	else{
	  hidden_layer[i].AddAxonConnection(&hidden_layer[n]);
	  hidden_layer[n].AddDendriteConnection(&hidden_layer[i]);
	}
	}
   }
   for(i = 0; i < input_layer_size; i++){
	 for(e = 0; e < num_axons; e++){
	   load_file >> n;
	   input_layer[i].AddAxonConnection(&hidden_layer[n]);
	   hidden_layer[n].AddDendriteConnection(&input_layer[i]);
	}
  }
}

void NeuralNet::WorkingIterate(double decrease_speed){
	  int i;

	  for(i = 0; i < input_layer_size; i++){
	input_layer[i].Spike();
	  } 
	  for(i = 0; i < hidden_layer_size; i++){
	  //cout << hidden_layer[i].old_sensor_excitement <<endl;
	  //scanf("%d", &i);
	NeuralNet::hidden_layer[i].Spike();
	//cout<<hidden_layer[i].old_sensor_excitement<<endl;
	  }
	  for(i = 0; i < hidden_layer_size; i++){
	hidden_layer[i].old_sensor_excitement += hidden_layer[i].sensor_excitement;
	hidden_layer[i].sensor_excitement = 0;

	  }
}

void NeuralNet::Teach(double teaching_force, double forgetting_force){
	int i, e;
	for(i = 0; i < input_layer_size; i++){
		for(e=0; e < input_layer[i].axon_coef.size(); e++){
			if(input_layer[i].out_neuron[e]->mark_of_actuator != 0 && input_layer[i].out_neuron[e]->mark_of_sensor != 0){
				input_layer[i].axon_coef[e] += input_layer[i].mark_of_actuator * teaching_force;///??????????
			}
		}
		input_layer[i].mark_of_actuator = 0;
	//	input_layer[i].ExcitementDecrease(2);///decreasing BOTH excitements and marks
		input_layer[i].Forgetting(forgetting_force);  ///?????????
	}
	for(i = 0; i < hidden_layer_size; i++){
		if(hidden_layer[i].mark_of_actuator != 0 && hidden_layer[i].mark_of_sensor != 0){
			for(e = 0; e < hidden_layer[i].axon_coef.size(); e++){
				if(hidden_layer[i].out_neuron[e]->mark_of_actuator != 0 && hidden_layer[i].out_neuron[e]->mark_of_sensor != 0){
					hidden_layer[i].axon_coef[e] += hidden_layer[i].mark_of_sensor * hidden_layer[i].mark_of_actuator * teaching_force;///????
				}
			}
			hidden_layer[i].mark_of_actuator = 0;
			hidden_layer[i].mark_of_sensor = 0;
		}
		hidden_layer[i].Forgetting(forgetting_force);///?????????????
	//	hidden_layer[i].ExcitementDecrease(2);///decreasing BOTH excitements and marks
	}

}