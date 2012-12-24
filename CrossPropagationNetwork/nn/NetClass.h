#ifndef NET_CLASS_H
#define NET_CLASS_H

#pragma once

#include "headers.h"
#include "NeuronClass.h"

using namespace System;
 

public class NeuralNet{
	private:
		int current_position_in_sensors_logfile;
		double decrease_speed;
		double forgetting_force;
		double teach_force;

	public:
		std::ofstream  actuators_out;

		typedef std::vector<Neuron> Neuron_layers;//by this typedef will be maked hidden_layer, output_layer and input_layer

		Neuron_layers hidden_layer, input_layer, output_layer;
		int hidden_layer_size, neurons_excited, num_axons, input_layer_size, output_layer_size;
		double range_of_coef, init_threshold;
		int net_iteration, net_excitement_sensors, net_excitement_actuators;
	//FILE *sensors_file;


	//for sensors
	int input_layer_set_size;
	int input_layer_set_number;


	std::ifstream sensors_file;
	NeuralNet();
	void SensorIterate();
	void con_NetStat();
	void GetSensors();
	void GetSensors(int*heights);
	std::vector<bool> Get_Actuator_State();
	void Save(char *file_name);
	void Load(char * file_name);
	void ActuatorIterate();
	void WorkingIterate(double decrease_speed);
	void Teach(double teaching_force, double forgetting_force);
	};

#endif //NET_CLASS_H