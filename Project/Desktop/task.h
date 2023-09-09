#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Task
{
public:
	Task(std::wstring name, std::wstring description)
		:name(name), description(description)
	{ }
	~Task()
	{ }
	std::wstring& getName()
	{
		return name;
	}
	std::wstring& getDescription()
	{
		return description;
	}
private:
	std::wstring name;
	std::wstring description;
};
