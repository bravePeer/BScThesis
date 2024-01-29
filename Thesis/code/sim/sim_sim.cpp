void SimulationEngine::simulate()
{
	PyObject* moduleMainString = PyUnicode_FromString("__main__");
	if (moduleMainString == NULL) {
		logger->Error("Cant load main module!");
		throw std::string("Simulation error");
	}

	moduleMain = PyImport_Import(moduleMainString);
	PyRun_SimpleString(R"END(
LTC = SimCommander("save/save.asc")
LTC.run()
LTC.wait_completion()
l = ltspice.Ltspice("save/save_1.raw")
l.parse()	
	)END");
}