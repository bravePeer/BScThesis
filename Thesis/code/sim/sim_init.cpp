SimulationEngine::SimulationEngine()
{
	logger = new applogger::Logger("Simulation");
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
	return l.get_data('I(' + componentName  + ')', time=0)
	)END");
	logger->Info("Python initialized");
}