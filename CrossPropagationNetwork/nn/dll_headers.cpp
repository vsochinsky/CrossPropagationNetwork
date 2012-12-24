#include "stdafx.h"

#include "dll_headers.h"
#include "IntSensors.h"
#include "NetClass.h"

  NeuralNet net;
  sensor_layer SL;
  int n_of_sensors = 0;
  std::vector<int> heights;
  std::ofstream  save_file("log.txt", std::ofstream::out);
  std::vector<bool> command;
  
extern "C" __declspec(dllexport) void __stdcall InitNet(){
	
	save_file << "InitNet() started OK" << std::endl;
	int i;
	for(i = 0; i < net.input_layer_size; i++){
		net.input_layer[i].old_sensor_excitement = net.init_threshold;   // new error
	}
	save_file << "InitNet() finished OK" << std::endl;
}

extern "C" __declspec(dllexport) void  __stdcall Iterate(int sensorIters, int actuatorIters){

	save_file << "Iterate() started OK" << std::endl;
	int z;
	//printf("Istarting for\n");
	for(z = 0; z <= sensorIters; z++){
	  //	cout << "NEW ITERATION " << z << endl; //<-may be buggy
		//std::cout << net.input_layer.size() << std::endl;
		//std::cout << "size ok" << std::endl;
		//net.GetSensors(heights);
		net.WorkingIterate(1.2);  //TODO
		//std::cout << z << " WI OK" << std::endl;
	}


  ///teaching
  for(z = 0; z < actuatorIters; z++){
	  net.SensorIterate(1);
	  net.ActuatorIterate(1);
	  //printf("teach ok\n");
  }
  save_file << "Iterate() finished OK" << std::endl;
}

extern "C" __declspec(dllexport) void  __stdcall save(char* filename){
	save_file << "save() started OK" << std::endl;
	net.Save(filename);
	save_file << "save() finished OK" << std::endl;
}

extern "C" __declspec(dllexport) void  __stdcall load(char* filename){
	save_file << "load() started OK" << std::endl;
	net.Load(filename);
	save_file << "load() finished OK" << std::endl;
}

extern "C" __declspec(dllexport) int  __stdcall sendCommand(){

  unsigned short i;
  int cmd=0;
  save_file << "sendCommand() started OK" << std::endl;
  //std::cout<< "sendCommand() started" << std::endl;
  command = net.Get_Actuator_State();

  save_file << "recieved command " << command[0] <<  command[1] <<  command[2] <<  command[3]<< std::endl;

  for(i=3; i>0; i--){
	  //std::cout << "for" << std::endl;
	  if(command.back()==true) cmd+=1;
	  cmd<<=1;
	  command.pop_back();
  }

  save_file << "sendCommand()" << cmd << "finished OK" << std::endl;
  return cmd;
}

extern "C" __declspec(dllexport)  void  __stdcall sendData(double speed, int angle, int xt, int yt, int xa, int ya){
	save_file << "sendData() started OK" << std::endl;
	
	std::cout << "sendData() started OK" << std::endl;
	n_of_sensors=SL.get_height(speed, angle, xt, yt, xa, ya, heights);
	//printf("N of sensors = %d\n", n_of_sensors);

	save_file << "sendData() finished OK" << std::endl;

}
