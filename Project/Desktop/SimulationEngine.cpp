#include "SimulationEngine.h"

SimulationEngine::SimulationEngine()
{
	logger = new applogger::Logger("Simulation");
	//board = nullptr;
	Py_Initialize();
	
	PyObject* moduleMainString = PyUnicode_FromString("__main__");
	if (moduleMainString == NULL)
		throw std::string("SimulationEngine error");

	moduleMain = PyImport_Import(moduleMainString);
	Py_DECREF(moduleMainString);

	PyRun_SimpleString("import string");
	PyRun_SimpleString("import ltspice");
	PyRun_SimpleString("from PyLTSpice import SimCommander");

	PyRun_SimpleString(R"END(
def getSimVal(componentName):
	data = l.get_data('I(' + componentName  + ')', time=0)
	if data is None:
		return 0
	return data
	)END");

	logger->Info("Python initialized");
}

SimulationEngine::~SimulationEngine()
{
	Py_Finalize(); // 4
	delete logger;
}


void SimulationEngine::simulate()
{
	PyObject* moduleMainString = PyUnicode_FromString("__main__");
	if (moduleMainString == NULL)
	{
		logger->Error("Cant load main module!");
		throw std::string("Simulation error");
	}

	moduleMain = PyImport_Import(moduleMainString);
	logger->Info("Simulation log:");
	PyRun_SimpleString(R"END(
LTC = SimCommander("save/save.asc")
idx = len(LTC.netlist) - 1

# LTC.netlist.insert(idx, '.dc VIN')
LTC.run()
LTC.wait_completion()

l = ltspice.Ltspice("save/save_1.raw")
l.parse()	
	)END");
}

float SimulationEngine::getComponentValue(std::string name, float time)
{
	PyObject* func = PyObject_GetAttrString(this->moduleMain, "getSimVal");
	if (func == NULL)
	{
		logger->Error("Cant get simulated value!");
		throw std::string("Simulation error");
	}

	PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(name.c_str()));
	if (func == NULL)
	{
		logger->Error("Cant get simulated value!");
		throw std::string("Simulation error");
	}

	PyObject* result = PyObject_CallObject(func, args);
	if (result == NULL)
	{
		logger->Error("Cant get simulated value!");
		throw std::string("Simulation error");
	}

	//double resultd = PyFloat_AsDouble(result);

	return static_cast<float>(PyFloat_AsDouble(result));
}
