// nn_test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>

#define LCMD 5


//typedef void (WINAPI*cfunc)();

int main()
	{
  HINSTANCE libHandle = LoadLibrary("NN.DLL");
	


	typedef void (__stdcall *PFuncInitNet)();
	typedef void (__stdcall *PFuncIterate)(int, int);
	typedef void (__stdcall *PFuncSendData)(double, int, int, int, int, int);
	typedef int  (__stdcall *PFuncSendCommand)();
	typedef void (__stdcall *PFuncLoad)(char*);
	typedef void (__stdcall *PFuncSave)(char*);

	PFuncInitNet     initNet     = (PFuncInitNet)     GetProcAddress( (HMODULE)libHandle, "InitNet" );
	PFuncIterate     iterate     = (PFuncIterate)     GetProcAddress( (HMODULE)libHandle, "Iterate" );
	PFuncLoad    	 load        = (PFuncLoad)    	  GetProcAddress( (HMODULE)libHandle, "load" );
	PFuncSave        save        = (PFuncSave)    	  GetProcAddress( (HMODULE)libHandle, "save" );
	PFuncSendData	 sendData    = (PFuncSendData)    GetProcAddress( (HMODULE)libHandle, "sendData" );
	PFuncSendCommand sendCommand = (PFuncSendCommand) GetProcAddress( (HMODULE)libHandle, "sendCommand" );
	
	printf("checking library in\n");
	system("cd");
//	getch();

	if(!initNet || !iterate || !sendCommand || !sendData || !load || !save)
	{
		printf("procedure adress error:\n initNet: %d\n iterate: %d\n load: %d\n save: %d\n sendData: %d\n sendCommand: %d\n",
			initNet, iterate, load, save, sendData, sendCommand);
//		getch();
		FreeLibrary(libHandle);
		return -1;
	}

	printf("start\n");
//	getch();
	initNet();
	printf("init ok\n");
	
	save("do.sav");
	printf("save ok\n");

	load("do.sav");
	printf("load ok\n");
	
	sendData(0.000000,90,10,365,325,300);
	printf("send 1 ok\n");

	sendData(0.000000,90,10,365,325,300);
	printf("send 2 ok\n");
	sendCommand();
	printf("sending command\n");
	printf("command %d sent\n", sendCommand());

	printf("starting iterate\n");
	iterate(1, 1);
	printf("iterate ok\n");

	save("posle.sav");
	printf("save ok\n");

	load("posle.sav");
	printf("load ok\n");
	for(int j = 0; j < 1000; j++){
		iterate(1, 1);
	}
	printf("the end\n");

//	getch();

         
	FreeLibrary((HMODULE)libHandle);
//	getch();
	return 0;
}

