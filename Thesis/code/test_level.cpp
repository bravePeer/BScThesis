TEST_METHOD(TestLevel){
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