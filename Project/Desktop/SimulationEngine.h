#pragma once
#include "board.h"
#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#include <Python.h>
#include <numpy/arrayobject.h>

//Wykonuje symulacje 
//Wykorzystuj¹c kod pythonowy?
class SimulationEngine 
{
public:
	SimulationEngine();
	~SimulationEngine();

	void setBoard();
	void convertBoard();
	void simulate();

	//getComponentValue(componentName, time?)

private:
	Board* board;
};