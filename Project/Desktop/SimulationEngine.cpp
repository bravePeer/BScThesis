#include "SimulationEngine.h"

SimulationEngine::SimulationEngine()
{
	board = nullptr;
	Py_Initialize();
	
	PyRun_SimpleString("import ltspice");
}

SimulationEngine::~SimulationEngine()
{
	Py_Finalize(); // 4
}

void SimulationEngine::convertBoard()
{

}

void SimulationEngine::simulate()
{
	PyRun_SimpleString(R"END(

from PyLTSpice import SimCommander
import ltspice

LTC = SimCommander("save/save.asc")
idx = len(LTC.netlist) - 1

# LTC.netlist.insert(idx, '.dc VIN')
LTC.run()
LTC.wait_completion()

l = ltspice.Ltspice("save/save_1.raw")
l.parse()	
	)END");
}
