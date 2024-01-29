TEST_METHOD(TestInsertUser) {
	DatabaseConnector dc;
	std::string login = "user1";
	std::string pass = "pass";
	
	try {
		Assert::IsTrue(dc.insertUser(login, pass));
	}
	catch (const std::string&) {
		Microsoft::VisualStudio::CppUnitTestFramework::Logger::WriteMessage("User exists!");
	}

	auto func = [&] {dc.insertUser(login, pass); };
	Assert::ExpectException<std::string>(func);

	login = "us er1";
	pass = "pa ss";
	auto func2 = [&] {dc.insertUser(login, pass); };
	Assert::ExpectException<std::string>(func2);
}