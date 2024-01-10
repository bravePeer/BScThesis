#pragma once
#include <iostream>
#include "logger.h"
#include "databaseConnector.h"

using namespace std;

class User {
public:
	static User& getInstance()
	{
		/*if (user == nullptr)
			user = new User;*/
		static User instance;
		return instance;
	}
	~User()
	{
		delete database;
		delete logger;
	}


	bool loginExistingUser(string login, string password)
	{
		this->login = login;
		this->password = password;
		if (database->checkUser(login, password))
		{
			loggedIn = true;
			logger->Info("User successfuly loggedin");
			return true;
		}
		logger->Info("User NOT logged in");
		return false;
	}

	//Register of new user
	bool createNewUser(string login, string password)
	{
		if (database->isUserExists(login))
		{
			logger->Error("This login is in use!");
			return false; //Mayby add throw?
		}

		if (database->insertUser(login, password))
		{
			logger->Info("Account created");
			return true;
		}
		
		logger->Error("Account NOT created!");
		return false;
	}

	bool isLoggedIn() const
	{
		return loggedIn;
	}

	void logout()
	{
		login = "";
		password = "";

		loggedIn = false;
	}

	void getSavesFile()
	{
		database->getSaves(login, password);
	}

	void syncSavesFile()
	{
		database->updateSaves(login, password);
	}

private:
	User() {
		logger = new applogger::Logger("User");
		database = new DatabaseConnector;
	}
	User(User&) = delete;
	void operator=(User const&) = delete;

	string login = "";
	string password = "";
	bool loggedIn = false;

	applogger::Logger* logger = nullptr;
	DatabaseConnector* database = nullptr;
};