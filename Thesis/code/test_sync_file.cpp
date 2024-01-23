TEST_CLASS(TestSaveSynchronization)
{
	TEST_METHOD(TestFileSaving)
	{
		// Login user
		...
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
		...
	}
};