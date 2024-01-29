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
			BoardSave::getInstance().saveBoard(board, "save.asc");

			Vector2i* tmp = new Vector2i[1];
			tmp[0].x = 0;
			tmp[0].y = 0;
			Vector2i pinPos = {3, 3 };
			
			Level* level = loadLevelSTART();
			level->load();
			Component** components = level->getComponents();
			board->placeComponent(new Resistor(dynamic_cast<Resistor*>(components[0])), pinPos);
			delete[] tmp;
			
			BoardSave::getInstance().saveBoard(board, "save.asc");
			Level::saveRealizedLevel(level->getId(), 0);
			User::getInstance().syncSavesFile();

			User::getInstance().getSavesFile();
			Level::extractRelizedLevel(level->getId());
			
			Board* newBoard = BoardSave::getInstance().loadBoard("save.asc", level);

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
			string saveData = R"(
Version 4
SHEET 1 88 148
WIRE 64 0 16 0
WIRE 0 48 0 0
WIRE 48 48 0 48
WIRE 64 48 64 0
FLAG 16 0 0
SYMBOL _app\\myvoltage 0 0 R270
SYMATTR InstName V1
SYMATTR Value 12
SYMBOL _app\\appres 48 48 R0
SYMATTR InstName res0id0
SYMATTR Value 1
TEXT -32 -24 Left 2 !.tran 10

)";
			fstream file;
			file.open("save/save.asc", ios::out);
			file.write(saveData.c_str(), saveData.size());

			SimulationEngine sim;
			sim.simulate();
			float val = sim.getComponentValue("res0id0");
			Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage(to_string(val).c_str());
			Assert::IsTrue(val != 0.f);
		}
		TEST_METHOD(TestLevel)
		{
			Level* level = loadLevelSTART();
			level->load();

			Component** components = level->getComponents();
			Board* board = new Board(level->getBoardDimension().x, level->getBoardDimension().y, level->getBoardDimension().z);
			level->initBoard(board);
			
			Vector2i pos = { 0,1 };
			board->placeComponent(new Resistor(dynamic_cast<Resistor*>(components[0])), pos);
			board->addRoute({ 0,0 }, { 0,1 });
			board->addRoute({ 1,0 }, { 1,1 });


			BoardSave::getInstance().saveBoard(board, "save.asc");
			SimulationEngine sim;
			sim.simulate();
			Assert::IsTrue(level->checkSimulation(board, &sim));
		}
	};
}
