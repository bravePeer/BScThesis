float SimulationEngine::getComponentValue(std::string name, float time)
{
	PyObject* func = PyObject_GetAttrString(this->moduleMain, "getSimVal");
	if (func == NULL) {
		logger->Error("Cant get simulated value!");
		throw std::string("Simulation error");
	}

	PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(name.c_str()));
	if (func == NULL) {
		logger->Error("Cant get simulated value!");
		throw std::string("Simulation error");
	}

	PyObject* result = PyObject_CallObject(func, args);
	if (result == NULL) {
		logger->Error("Cant get simulated value!");
		throw std::string("Simulation error");
	}
	return static_cast<float>(PyFloat_AsDouble(result));
}