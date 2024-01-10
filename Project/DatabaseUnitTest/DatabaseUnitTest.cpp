#include "pch.h"
#include "CppUnitTest.h"
#include "../Desktop/databaseConnector.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DatabaseUnitTest
{
	TEST_CLASS(DatabaseUnitTest)
	{
	public:
		TEST_METHOD(TestInsertUser)
		{
			DatabaseConnector dc;
			std::string login = "user1";
			std::string pass = "pass";
			
			try
			{
				Assert::IsTrue(dc.insertUser(login, pass));
			}
			catch (const std::string&)
			{
				Logger::WriteMessage("User exists!");
			}

			auto func = [&] {dc.insertUser(login, pass); };
			Assert::ExpectException<std::string>(func);

			login = "us er1";
			pass = "pa ss";
			auto func2 = [&] {dc.insertUser(login, pass); };
			Assert::ExpectException<std::string>(func2);
		}

		TEST_METHOD(TestRemoveUser)
		{
			DatabaseConnector dc;
			std::string login = "user1";
			std::string pass = "pass";

			try
			{
				Assert::IsTrue(dc.removeUser(login, pass));
				Assert::IsFalse(dc.removeUser(login, pass));
			}
			catch (const std::string&)
			{
				Logger::WriteMessage("User NOT exists!");
			}
		}
		
		TEST_METHOD(TestIsUserExists)
		{
			DatabaseConnector dc;
			std::string login = "userExists";
			std::string pass = "pass";

			bool isExists = dc.isUserExists(login);
			if (isExists)
			{
				dc.removeUser(login, pass);
			}

			Assert::IsFalse(isExists);
			

			try
			{
				Assert::IsTrue(dc.insertUser(login, pass));
			}
			catch (const std::string&)
			{
				Logger::WriteMessage("User exists!");
			}

			Assert::IsTrue(dc.isUserExists(login));

			try
			{
				Assert::IsTrue(dc.removeUser(login, pass));
			}
			catch (const std::string&)
			{
				Logger::WriteMessage("User NOT exists!");
			}
		}

		TEST_METHOD(TestCheckUser)
		{
			DatabaseConnector dc;
			std::string login = "userCheck";
			std::string pass = "pass";
			try 
			{
				Assert::IsFalse(dc.checkUser(login, pass));

				dc.insertUser(login, pass);

				Assert::IsTrue(dc.checkUser(login, pass));

				dc.removeUser(login, pass);
			}
			catch (const std::string& err)
			{
				Logger::WriteMessage(err.c_str());
			}
		}

		TEST_METHOD(TestGetSaves)
		{
			DatabaseConnector dc;
			std::string login = "userSaves";
			std::string pass = "pass";
			 
			
			try
			{
				dc.insertUser(login, pass);
			}
			catch (const std::string& error)
			{
				Logger::WriteMessage(error.c_str());
			}

			try
			{
				dc.getSaves(login, pass);
				std::fstream file(PATH_TO_SAVES_FOLDER "\\saves", std::ios::in | std::ios::binary);

				Assert::IsTrue(file.good());
				//file.close();
				
			}
			catch (const std::string& error)
			{
				Logger::WriteMessage(error.c_str());
				Assert::IsTrue(false);
			}

			try
			{
				dc.removeUser(login, pass);
			}
			catch (const std::string& error)
			{
				Logger::WriteMessage(error.c_str());
			}
		}

		TEST_METHOD(TestUpdateSaves)
		{
			DatabaseConnector dc;
			std::string login = "userUpdateSaves";
			std::string pass = "pass";

			//std::string dataToSave = "some Data\0";
			char dataToSave[] = { 41,42,43,44, 0, 12, 3 };


			std::fstream file(PATH_TO_SAVES_FOLDER "\\saves", std::ios::out | std::ios::binary);
			for (int i = 0; i < 7; i++)
			{
				file << dataToSave[i];
			}

			file.close();
			
			dc.updateSaves(login, pass);

			std::fstream file2(PATH_TO_SAVES_FOLDER "\\saves", std::ios::in | std::ios::binary);
			Assert::IsTrue(file2.good());

			char data[64] = { 0 };
			file2.getline(data, 64);

			for (int i = 0; i < 7; i++)
			{
				//Assert::IsFalse(data[i] == 0, L"Bad count");
				Assert::IsTrue(data[i] == dataToSave[i]);
			}
			file2.close();
		}
	};
}
