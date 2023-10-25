#pragma once
#include "board.h"

//Wykonuje symulacje 
//Wykorzystuj¹c kod pythonowy?
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