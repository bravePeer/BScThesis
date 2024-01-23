#pragma once
#include "board.h"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>

//Wykonuje symulacje 
//Wykorzystując kod pythonowy?
class SimulationEngine 
{
public:
	SimulationEngine();
	~SimulationEngine();

	void setBoard();
	void convertBoard();
	void simulate();

	float getComponentValue(std::string name, float time = 0);
	//getComponentValue(componentName, time?)

private:
	PyObject* moduleMain;
	Board* board;
};