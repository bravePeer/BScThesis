#pragma once
#include "board.h"

//Wykonuje symulacje 
//Wykorzystując kod pythonowy?
class SimulationEngine 
{
public:
	SimulationEngine();
	~SimulationEngine() {}

	void setBoard();
	void convertBoard();
	void simulate();

	//getComponentValue(componentName, time?)

private:
	Board* board;
};