#pragma once
#include <iostream>
#include "logger.h"
#include "databaseConnector.h"
#include "SHA256.h"

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
		prepareLogin(login);
		preparePassword(login, password);
		//this->password = password;
		if (database->checkUser(this->loginPrepared, this->password))
		{
			loggedIn = true;
			logger->Info("User: " + login + " successfuly logged in");
			return true;
		}
		logger->Info("User NOT logged in");
		return false;
	}

	//Register of new user
	bool createNewUser(string login, string password)
	{
		if (login.find(' ') != std::string::npos)
			throw std::string("Field `login` can't contain space!");
		if (password.find(' ') != std::string::npos)
			throw std::string("Field `pass` can't contain space!");

		prepareLogin(login);

		if (database->isUserExists(this->loginPrepared))
		{
			logger->Error("This login is in use!");
			return false; //Mayby add throw?
		}
		preparePassword(login, password);

		if (database->insertUser(this->loginPrepared, this->password))
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
		loginPrepared = "";
		password = "";

		loggedIn = false;
	}

	void getSavesFile()
	{
		database->getSaves(loginPrepared, password);
	}

	void syncSavesFile()
	{
		database->updateSaves(loginPrepared, password);
	}

	std::string getUserName()
	{
		return login;
	}

private:
	User() {
		logger = new applogger::Logger("User");
		database = new DatabaseConnector;
	}
	User(User&) = delete;
	void operator=(User const&) = delete;
	
	void prepareLogin(std::string login)
	{
		SHA256 sha;
		sha.update(login);

		this->loginPrepared = SHA256::toString(sha.digest());
	}

	void preparePassword(std::string login, std::string password)
	{
		SHA256 sha;
		if (password[password.length() - 1] % 2 == 0)
			sha.update(login + password);
		else
			sha.update(password + login);
		this->password = SHA256::toString(sha.digest());
	}


	std::string loginPrepared = "";
	std::string login = "";
	std::string password = "";
	bool loggedIn = false;

	applogger::Logger* logger = nullptr;
	DatabaseConnector* database = nullptr;
};