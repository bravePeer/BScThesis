#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class Logger
{
public:
	Logger(string _who = "")
	{
		who = _who;
		if (loggerInConsole)
		{
			fun = &Logger::LoggerInConsole;
		}
		else
		{
			fun = &Logger::LoggerInFile;
		}
	}
	~Logger()
	{
		(this->*fun)("END [ " + who + " ] ");
	}
	static void InitializeFileLogging()
	{
		file.open("Log", ios::out);
		if (file.good())
		{

		}
	}
	static void EndFileLogging()
	{
		file.close();
	}


	void LoggerInConsole(string s)
	{
		cout << s << endl;
	}

	void LoggerInFile(string s)
	{
		file << s << endl;
	}


	void Info(string info)
	{
		(this->*fun)("Info [ " + who + " ]: " + info);
	}
	void Error(string error)
	{
		(this->*fun)("Error [ " + who + " ]: " + error);
	}

	static bool loggerInConsole;
	void (Logger::* fun)(string);

private:
	string who; // np engine, ai, state
	static fstream file;
};

bool Logger::loggerInConsole = true;
fstream Logger::file;