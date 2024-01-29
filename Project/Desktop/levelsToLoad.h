#pragma once
#include "level.h"
#include <functional>
#include "simpdata.h"

//Level* loadExampleLevel()
//{
//	Level* level = new Level(L"Identyfikator", L"Nazwa", "Opis", true);
//
//	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
//	level->setPrevLevelsIds({ }); 
//
//	level->setBoardDimension({ 15, 15, 1 });
//
//	//Dodanie podstawowych elementów na płtytkę
//	level->setInitBoardFun([](Board* board)->bool {
//		Vector2i* tmp = new Vector2i[1];
//		tmp[0].x = 0;
//		tmp[0].y = 0;
//		Vector2i pinPos = { 0, 0 };
//		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
//		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
//		gnd->rotate();
//		delete[] tmp;
//		board->placeComponentForce(vcc, pinPos);
//		pinPos.x++;
//		board->placeComponentForce(gnd, pinPos);
//		return true; });
//
//	//Ustawienie, które grafiki są wykorzystywane
//	level->setComponentsGraphicsToLoad([]()->void {
//		GraphicManager& graphics = GraphicManager::GetInstance();
//		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
//	graphics.addGraphicToLoad("resistor", RESISTOR_200_GRAPHIC_PATH); });
//
//	//Dodanie komponentów, które mogą być ułożone na płytkę
//	level->setGenerateComponents(([](int* componentsCount)->Component** {
//		*componentsCount = 1;
//		Component** components = new Component * [*componentsCount];
//		Vector2i* tmp = new Vector2i[2];
//		tmp[0].x = 0;
//		tmp[0].y = 0;
//		tmp[1].x = 1;
//		tmp[1].y = 0;
//		components[0] = new Resistor(L"Rezystor\n1kΩ", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id0");
//		components[0]->setSimValue("1000R");
//		delete[] tmp;
//		return components; }));
//
//	//Dodanie warunków sprawdzających płytkę
//	level->setCheckBoard([](Board* board)->bool {
//		map<std::string, int> componentsCount = board->getComponentsCountById();
//		if (componentsCount.find("id0")->second != 1)
//			return false;
//		return true; });
//
//	//Dodanie warunków sprawdzających wynik symulacji
//	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
//		float val = sim->getComponentValue("res0id0");
//		if (abs(sim->getComponentValue("res0id0")) > 0.1)
//			return true;
//		return false; });
//
//	return level;
//}

Level* loadLevelExampleOfImplemetation();

Level* loadLevel0();

Level* loadLevelSTART();
Level* loadLevelTest();

Level* loadMicrocontrollerLevel();

Level* loadLevelRES0();
Level* loadLevelRES1();
Level* loadLevelRES2();

Level* loadLevelCAP0();
Level* loadLevelCAP1();
Level* loadLevelCAP2();

Level* loadLevelIND0();

Level* loadLevelTRANB0();
Level* loadLevelTRANM0();

Level* loadLevelLED0();

Level* emptyLevel(sf::String id, sf::String name, sf::String desc = "");
