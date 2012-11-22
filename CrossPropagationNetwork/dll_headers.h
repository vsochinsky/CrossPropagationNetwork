#ifndef DLL_HEADERS_H
#define DLL_HEADERS_H

#include <iostream>
#include <stdio.h>
#include <Windows.h>
//typedef void (_stdcall *FunPtr)(INT32 x);

extern "C"{
 __declspec(dllexport) void __stdcall InitNet();
 __declspec(dllexport) void __stdcall Iterate(int sensorIters, int actuatorIters);
 __declspec(dllexport) void __stdcall load(char* filename);
 __declspec(dllexport) void __stdcall save(char* filename);
 __declspec(dllexport) void __stdcall sendData(double speed, int angle, int xt, int yt, int xa, int ya);
 __declspec(dllexport) int __stdcall sendCommand();
}



#endif // DLL_HEADERS_H


