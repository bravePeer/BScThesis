#include "SimulationEngine.h"

SimulationEngine::SimulationEngine()
{
	board = nullptr;
	Py_Initialize();
	

	PyObject* moduleMainString = PyUnicode_FromString("__main__");
	moduleMain = PyImport_Import(moduleMainString);
	Py_DECREF(moduleMainString);

	PyRun_SimpleString("import string");
	PyRun_SimpleString("import ltspice");
	PyRun_SimpleString("from PyLTSpice import SimCommander");

	PyRun_SimpleString(
		"def mul(a, b):                                 \n"\
		"   return a * b                                \n"\
	);

	PyRun_SimpleString(R"END(
def getSimVal(componentName):
	print(componentName)
	return l.get_data('I(' + componentName  + ')', time=0)
	)END");

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
	PyObject* moduleMainString = PyUnicode_FromString("__main__");
	moduleMain = PyImport_Import(moduleMainString);

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
	/*PyObject* func = PyObject_GetAttrString(this->moduleMain, "mul");
	PyObject* args = PyTuple_Pack(2, PyFloat_FromDouble(3.0), PyFloat_FromDouble(4.0));

	PyObject* result = PyObject_CallObject(func, args);
	double resultd = PyFloat_AsDouble(result);
	return resultd;*/

	PyObject* func = PyObject_GetAttrString(this->moduleMain, "getSimVal");
	PyObject* args = PyTuple_Pack(1, PyUnicode_FromString(name.c_str()));

	PyObject* result = PyObject_CallObject(func, args);
	double resultd = PyFloat_AsDouble(result);

	cout << resultd;
	return resultd;

//    
//
//
//
//	cout << name.c_str() << endl;
//	PyObject* pArgs = PyTuple_Pack(1, PyBytes_FromString(name.c_str()));
//
//	if (PyRun_SimpleString(pythonCode) != 0) {
//		PyErr_Print();
//		Py_DECREF(pArgs);
//		Py_Finalize();
//		return 1;
//	}
//
//	// Wywo³anie funkcji za pomoc¹ PyObject_CallMethod
//	PyObject* pValue = PyObject_CallObject(PyObject_GetAttrString(moduleMain, "getSimVal"), pArgs);
//
//	if (pValue != NULL) {
//		// Pobranie wyniku i wypisanie go
//		double result = PyFloat_AsDouble(pValue);
//		printf("Wynik z Pythona: %.2f\n", result);
//		Py_DECREF(pValue);
//		return result;
//
//	}
//	else {
//		PyErr_Print();
//	}
//
//	Py_DECREF(pArgs);
//	throw "awd";
//	return 0.0f;
}
