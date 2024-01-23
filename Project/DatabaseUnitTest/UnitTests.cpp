#include "pch.h"
#include "CppUnitTest.h"
#include "../Desktop/databaseConnector.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
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
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage("User exists!");
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
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage("User NOT exists!");
			}
		}
		
		TEST_METHOD(TestIsUserExists)
		{
			DatabaseConnector dc;
			std::string login = "userExists";
			std::string pass = "pass";

			// Be sure user not exist
			bool isExists = dc.isUserExists(login);
			if (isExists) {
				dc.removeUser(login, pass);
				isExists = dc.isUserExists(login);
			}
			Assert::IsFalse(isExists);
			
			try {
				Assert::IsTrue(dc.insertUser(login, pass));
			}
			catch (const std::string&) {
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage("User exists!");
			}

			Assert::IsTrue(dc.isUserExists(login));

			try {
				Assert::IsTrue(dc.removeUser(login, pass));
			}
			catch (const std::string&) {
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage("User NOT exists!");
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
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(err.c_str());
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
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(error.c_str());
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
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(error.c_str());
				Assert::IsTrue(false);
			}

			try
			{
				dc.removeUser(login, pass);
			}
			catch (const std::string& error)
			{
				Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(error.c_str());
			}
		}

	};
	TEST_CLASS(TestSaveSynchronization)
	{
		TEST_METHOD(TestFileSaving)
		{
			std::string login = "testSaveFileUser8";
			std::string pass = "testSaveFileUserPass";

			User::getInstance().createNewUser(login, pass);
			User::getInstance().loginExistingUser(login, pass);
			User::getInstance().getSavesFile();
			
			// Login user
			// ...

			Board* board = new Board(20, 20, 1);
			BoardSave::getInstance()->saveBoard(board, "save.asc");

			Vector2i* tmp = new Vector2i[1];
			tmp[0].x = 0;
			tmp[0].y = 0;
			Vector2i pinPos = {3, 3 };
			
			Level* level = loadLevelSTART();
			level->load();
			Component** components = level->getComponents();
			board->placeComponent(new Resistor(dynamic_cast<Resistor*>(components[0])), pinPos);
			delete[] tmp;
			
			BoardSave::getInstance()->saveBoard(board, "save.asc");
			Level::saveRealizedLevel(level->getId(), 0);
			User::getInstance().syncSavesFile();

			User::getInstance().getSavesFile();
			Level::extractRelizedLevel(level->getId());
			
			Board* newBoard = BoardSave::getInstance()->loadBoard("save.asc", level);

			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					Component* component = board->getComponentOnBoard({ i,j });
					Component* newComponent = newBoard->getComponentOnBoard({ i,j });
					if ((component != nullptr) && (newComponent != nullptr))
						Assert::IsTrue(board->getComponentOnBoard({ i,j })->getId() == board->getComponentOnBoard({ i,j })->getId());
				}
			}
		
			// Clean and remove user
			// ...
			DatabaseConnector dc;
			dc.removeUser(login, pass);

			delete board;
			delete newBoard;
			delete level;
		}
	};
	TEST_CLASS(Simulation)
	{
		TEST_METHOD(SimpleSimulation)
		{
			SimulationEngine sim;
			sim.simulate();
			float val = sim.getComponentValue("led0id1");
			Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(to_string(val).c_str());
			Assert::IsTrue(val != 0.f);
		}
		TEST_METHOD(TestLevel)
		{
			Level* level = loadLevel0();
			level->load();

			Component** components = level->getComponents();

			Board* board = new Board(level->getBoardDimension().x, level->getBoardDimension().y, level->getBoardDimension().z);
			
		}
	};
}
