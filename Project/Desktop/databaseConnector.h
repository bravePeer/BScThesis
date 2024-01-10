#pragma once
#include <iostream>
#include <mysqlx/xdevapi.h>
#include "logger.h"
#define PATH_TO_SAVES_FOLDER ".\\save"

/*
Run database:
	docker run --name mysqlBCsThesis -v bscThesis_db_volume:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=my-secret-pw -d -p 33060:33060 mysql:latest

Go in:
	docker exec -it mysqlBCsThesis mysql

Delete container:
	 docker stop mysqlBCsThesis ; docker rm mysqlBCsThesis


Clear Tables:
	SET FOREIGN_KEY_CHECKS = 0;
	TRUNCATE TABLE Users;
	TRUNCATE TABLE SAVES;
	SET FOREIGN_KEY_CHECKS = 1;


	delimiter $$
	create trigger insertEmptyFile after insert on Users
	for each row
	begin
	
	insert into Saves(userId, )

	end$$
	delimiter;
*/

class DatabaseConnector
{
public:
	DatabaseConnector()
	{
		logger = new applogger::Logger("DatabaseConnector");
		try
		{
			connectDB();
		}
		catch (const mysqlx::Error& err)
		{
			logger->Error(err.what());
		}
	}
	~DatabaseConnector()
	{
		delete logger;
		delete session;
	}

	bool insertUser(std::string login, std::string pass)
	{

		if (login.find(' ') != std::string::npos)
			throw std::string("Field `login` can't contain space!");
		if (pass.find(' ') != std::string::npos)
			throw std::string("Field `pass` can't contain space!");

		try
		{
			this->connectDB();
			mysqlx::Schema schema = session->getSchema("BScThesisDatabase", true);
			mysqlx::Table table = schema.getTable("Users");

			mysqlx::Row row;
			row.set(0, mysqlx::Value());
			row.set(1, login);
			row.set(2, pass);

			table.insert().rows(row).execute();

			mysqlx::RowResult result = table.select().where("login = :login").bind("login", login).execute();

			mysqlx::Table saveTable = schema.getTable("Saves");
			mysqlx::Row saveRow;
			saveRow.set(0, mysqlx::Value());
			saveRow.set(1, result.fetchOne().get(0));
			
			//TODO generate initialized file, header

			mysqlx::byte data[64] = { 0 };
			for (int i = 0; i < 64; i++)
				data[i] = (i < 60) ? 'a': 0;
			
			

			mysqlx::bytes datas(data, 64);

			saveRow.set(2, datas);

			saveTable.insert().rows(saveRow).execute();
		}
		catch (const mysqlx::Error& err)
		{
			logger->Error(err.what());
			throw std::string(err.what());
		}


		return true;
	}

	bool removeUser(std::string login, std::string pass)
	{
		
		try
		{
			this->connectDB();
			mysqlx::Schema schema = session->getSchema("BScThesisDatabase", true);
			mysqlx::Table table = schema.getTable("Users");

			mysqlx::RowResult result = table.select().where("login = :login").bind("login", login).execute();

			mysqlx::Result resultRemove = schema.getTable("Saves").remove().where("userId = :userId").bind("userId", result.fetchOne().get(0)).execute();
			if (resultRemove.getAffectedItemsCount() == 0)
				logger->Error("Can't remove data from Saves Table!");

			resultRemove = table.remove().where("login = :login").bind("login", login).execute();
			if (resultRemove.getAffectedItemsCount() > 0)
				return true;

		}
		catch (const mysqlx::Error& err)
		{
			logger->Error(err.what());
			throw std::string(err.what());
		}

		return false;
	}

	bool isUserExists(std::string login)
	{

		try
		{
			this->connectDB();
			mysqlx::Schema schema = session->getSchema("BScThesisDatabase", true);
			mysqlx::Table table = schema.getTable("Users");
			mysqlx::RowResult result = table.select().where("login = :login").bind("login", login).execute();
			
			if(result.count() > 0)
				return true;
			
		}
		catch (const mysqlx::Error& err)
		{
			logger->Error(err.what());
			throw std::string(err.what());
		}

		return false;
	}

	bool checkUser(std::string login, std::string pass)
	{

		try
		{
			this->connectDB();
			
			mysqlx::Schema schema = session->getSchema("BScThesisDatabase", true);
			mysqlx::Table table = schema.getTable("Users");
			mysqlx::RowResult result = table.select().where("login = :login AND password = :password").bind("login", login).bind("password", pass).execute();

			if (result.count() > 0)
				return true;

		}
		catch (const mysqlx::Error& err)
		{
			logger->Error(err.what());
			throw std::string(err.what());
		}

		return false;
	}

	bool updateSaves(std::string login, std::string pass)
	{
		return true;
	}

	bool getSaves(std::string login, std::string pass)
	{
		this->connectDB();

		try
		{
			mysqlx::Schema schema = session->getSchema("BScThesisDatabase", true);
			mysqlx::Table table = schema.getTable("Users");
			mysqlx::RowResult result = table.select().where("login = :login").bind("login", login).execute();
			
			mysqlx::RowResult resultSaves = schema.getTable("Saves").select().where("userId = :userId").bind("userId", result.fetchOne().get(0)).execute();
			mysqlx::Value saves = resultSaves.fetchOne().get(2); // data
			
			char* data = new char[saves.getRawBytes().size()];
			std::fstream file(PATH_TO_SAVES_FOLDER "\\saves", std::ios::out | std::ios::binary);
			if (!file.good())
				throw std::string("Error with save file");

			for (mysqlx::byte bb : saves.getRawBytes())
			{
				file << bb;
			}

			file.close();
			delete[] data;
		}
		catch (const mysqlx::Error& err)
		{
			logger->Error(err.what());
			throw std::string(err.what());
		}

		return false;
	}

	std::string getTest()
	{
		return "test";
	}
private:
	void connectDB()
	{
		if (session != nullptr)
			return;

		//try
		//{
			session = new mysqlx::Session("mysqlx://root@127.0.0.1:33060");
		//}
		//catch (const mysqlx::Error& err)
		//{
		//	/*logger->Error(err.what());*/
		//	throw err;
		//}

		logger->Info("Connected to database");
	}

	applogger::Logger* logger;
	mysqlx::Session* session;
};