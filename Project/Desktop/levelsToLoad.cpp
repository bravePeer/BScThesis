#include "levelsToLoad.h"

Level* loadExampleLevel()
{
	Level* level = new Level("Identyfikator", L"Nazwa", L"Opis", false, L"SkróconyOpis");
	
	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "PrevLevelId" });
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true; 
	});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});
	
	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelExampleOfImplemetation()
{

	Level* level = new Level("ID", L"NAME", L"DESC");
	
	level->setPathToSave("save.asc");//This will be usless


	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor", L"Zamienia część energii elektrycznje w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LightEmittingDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		/*if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;*/

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevelTest()
{
	Level* level = new Level("Test0", L"Test", L"Poziom testujący", false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 4;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor", L"Zamienia część energii elektrycznje w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LightEmittingDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id1");
		components[2] = new Capacitor(L"", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id2");
		delete[] tmp;
		tmp = new Vector2i[8];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		tmp[2].x = 2; tmp[2].y = 0;
		tmp[3].x = 3; tmp[3].y = 0;

		tmp[4].x = 0; tmp[4].y = 3;
		tmp[5].x = 1; tmp[5].y = 3;
		tmp[6].x = 2; tmp[6].y = 3;
		tmp[7].x = 3; tmp[7].y = 3;

		components[3] = new Microcontroller(L"uc", L"awd", Vector2i(4, 4), 8, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id3");
		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		if (componentsCount[Component::CompoenetType::resistor] != 1)
			return false;

		if (componentsCount[Component::CompoenetType::led] != 1)
			throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);

		return true;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		return true;
		});

	return level;
}

Level* loadLevelKirchhoff()
{
	const wchar_t* desc = LR"END(
Poziom, w którym poznasz pierwsze prawo Kirchhoffa!


Pierwsze prawo Kirchhoffa mówi, że suma prądów wpływających 
do dowolnego węzła w obwodzie elektrycznym jest równa sumie 
prądów wypływających z tego węzła. 



Warunki zaliczenia poziomu:
Podłącz 3 różne oporniki szeregowo. Dodaj łącznie 5 amperomierzy,
3 z nich są podłączone do każdego opornika, natomiast 1 przed
rozdzieleniem węzła oraz 1 po połączeniu węzłów.
Sprawdź schemat, aby zobaczyć połączenie elementów.


Wykorzystaj woltomierz, aby sprawdzić napięcie na opornikach.

























)END";

	const wchar_t* shortDesc = LR"END(
Poziom, w którym poznasz prawo Ohma 
oraz łączenie przyrządów pomiarowych!


Pierwsze prawo Kirchhoffa mówi, że suma prądów wpływających 
do dowolnego węzła w obwodzie elektrycznym jest równa sumie 
prądów wypływających z tego węzła. 


Warunki zaliczenia poziomu:
Podłącz 3 różne oporniki szeregowo. Dodaj łącznie 5 amperomierzy,
3 z nich są podłączone do każdego opornika, natomiast 1 przed
rozdzieleniem węzła oraz 1 po połączeniu węzłów.
Sprawdź schemat, aby zobaczyć połączenie elementów.







)END";

	Level* level = new Level("KIR1", L"I Prawo Kirchhoffa", desc, false, shortDesc);
	level->setPrevLevelsIds({ "BOHM" });
	level->setBoardDimension({ 10, 15, 1 });
	level->setPathToSave("save.asc");
	level->setSchematicPath(LEVEL_KIRCHHOFF_SCHEMATIC_PATH);

	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("voltmeter", VOLTMETER_GRAPHIC_PATH);
		graphics.addGraphicToLoad("amperemeter", AMPEREMETER_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor1k", RESISTOR_1k_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor120", RESISTOR_120_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor220", RESISTOR_220_GRAPHIC_PATH);
		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
		});

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 5;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 1;
		tmp[1].x = 1;
		tmp[1].y = 1;
		components[0] = new Voltmeter(L"Woltomierz", L"", Vector2i(2, 2), 2, tmp, GraphicManager::GetInstance().getComponentTexture("voltmeter"), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("1000000000R");
		components[1] = new Amperemeter(L"Amperomierz", L"", Vector2i(2, 2), 2, tmp, GraphicManager::GetInstance().getComponentTexture("amperemeter"), Component::ComponentTypePackage::SMD, true, "id1");
		components[1]->setSimValue("1R");

		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[2] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor220"), Component::ComponentTypePackage::SMD, true, "id2");
		components[2]->setSimValue("220R");
		components[3] = new Resistor(L"Rezystor\n120Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor120"), Component::ComponentTypePackage::SMD, true, "id3");
		components[3]->setSimValue("120R");
		components[4] = new Resistor(L"Rezystor\n1kΩ", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor1k"), Component::ComponentTypePackage::SMD, true, "id4");
		components[4]->setSimValue("1000R");

		delete[] tmp;

		return components;
		}));

	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;

		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(VOLTMETER_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(1, ComponentDesc(AMPEREMETER_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(2, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(3, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(4, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));

		return componentsDesc;
		});

	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();

		if (componentsCount.find("id1")->second != 5)
			return false;

		if (componentsCount.find("id2")->second != 1)
			return false;
		if (componentsCount.find("id3")->second != 1)
			return false;
		if (componentsCount.find("id4")->second != 1)
			return false;

		return true;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		
			for (int i = 0; i < 10; i++)
				if (abs(sim->getComponentValue(string("res") + to_string(i) + "id2")) > 0.052)
					goto secondTest;
			return false;
		secondTest:

			for (int i = 0; i < 10; i++)
				if (abs(sim->getComponentValue(string("res") + to_string(i) + "id3")) > 0.096)
					goto second1Test;
			return false;
		second1Test:

			for (int i = 0; i < 10; i++)
				if (abs(sim->getComponentValue(string("res") + to_string(i) + "id4")) > 0.011)
					goto second2Test;
			return false;
		second2Test:
		
		return true;
		});

	return level;
}

Level* loadLevelKirchhoff2()
{
	Level* level = new Level("KIR2", L"II prawo Kirchhoffa", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "KIR1"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}



Level* loadLevelLED()
{
	const wchar_t* desc = LR"END(
Poziom, w którym poznasz diodę świecącą LED






































)END";

	const wchar_t* shortDesc = LR"END(
Poziom, w którym poznasz diodę świecącą LED

Warunki zaliczenia zadania:
Połącz odpowiedni opornik z diodą LED, tak aby przez diodę LED 
przepływał odpowiedni prąd.
















)END";

	Level* level = new Level("ALED", L"Dioda LED", desc, false, shortDesc);
	level->setPrevLevelsIds({ "ASTART"});
	level->setBoardDimension({ 5, 5, 1 });
	level->setPathToSave("save.asc");
	level->setSchematicPath("Resources\\Images\\levels\\test_level_schematic.png");

	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		graphics.addGraphicToLoad("diode", DIODE_GRAPHIC_PATH);
		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
		});

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 5;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n1kΩ", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("1000R");
		components[1] = new Resistor(L"Rezystor\n47kΩ", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id1");
		components[1]->setSimValue("47000R");
		components[2] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[2]->setSimValue("220R");
		components[3] = new Resistor(L"Rezystor\n1Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id3");
		components[3]->setSimValue("1R");
		components[4] = new LightEmittingDiode(L"Dioda LED\nczerwona", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("diode"), Component::ComponentTypePackage::SMD, true, "id4");
		components[4]->setSimValue("QTLP690C");

		delete[] tmp;

		return components;
		}));


	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();

		/*for (map<std::string, int>::iterator it  = componentsCount.begin(); it != componentsCount.end(); it++)
		{
			cout << it->first << ": " << it->second << endl;
		}*/

		if (componentsCount.find("id4")->second != 1)
			return false;

		/*if (componentsCount.find("id1")->second != 1)
			return false;*/

		return true;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		float val = sim->getComponentValue("res0id0");
		if (abs(sim->getComponentValue("res0id0")) > 0.01)
			return true;
		return false;
		});

	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;

		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(1, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(2, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(3, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(4, ComponentDesc(LED_DESCRIPTION_PATH)));

		return componentsDesc;
		});

	return level;
}

Level* loadLevel0()
{
	const wchar_t* desc = LR"END(
Początkowy poziom, 
który służy do zapoznania się z aplikacją.
)END"; 

	Level* level = new Level("P0", L"Początki", desc, false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor", L"Zamienia część energii elektrycznje w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LightEmittingDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;
		return components;
		}));



	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;

		if (componentsCount[Component::CompoenetType::led] != 1)
			throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);

		return true;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {


		return true;
		});

	return level;
}

Level* loadLevelEmpty2()
{
	const wchar_t* desc = LR"END(







































)END";

	const wchar_t* shortDesc = LR"END(





















)END";

	Level* level = new Level("ALED", L"Dioda LED", desc, false, shortDesc);
	level->setPrevLevelsIds({ "ASTART" });
	level->setBoardDimension({ 5, 5, 1 });
	level->setPathToSave("save.asc");
	level->setSchematicPath("Resources\\Images\\levels\\test_level_schematic.png");

	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n1kΩ", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("1000R");


		delete[] tmp;

		return components;
		}));


	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();


		if (componentsCount.find("id4")->second != 1)
			return false;

		return true;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		float val = sim->getComponentValue("res0id0");
		if (abs(sim->getComponentValue("res0id0")) > 0.01)
			return true;
		return false;
		});

	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;

		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));

		return componentsDesc;
		});

	return level;
}

Level* loadLevelSTART()
{
	const wchar_t* desc = LR"END(
Prądem elektrycznym nazywamy uporządkowany ruch ładunków
elektrycznych, czyli taki, w którym ładunek elektryczny
przemieszcza się w jednym kierunku. Przyjęte zostało, 
że prąd płynie od + do -. Ilość przepływającego prądu
to natężenie oznaczane literą 'i' lub 'I', jednostą 
jest Amper (A).

Napięcie elektryczne to różnica potencjałów, analogią
jest różnica wysokości. Napięcie jest oznaczane literą
'u', jednostą jest Volt (V).

Do dodania elementu elektronicznego na płytkę kliknij
na odpowiedni przycisk z nazwą, następnie umieść go
na płytce

Do dodania połączenia naciśnij przycisk "Połącz"
i trzymając lewy przycisk myszy na płytce przesuń
kursor, aby usunąć zamiast trzymać lewy przycisk 
myszy trzymal prawy przycisk myszy.

Po dodaniu i połączeniu kliknij przycisk "Sprawdź"
w celu sprawdzenia poprawnośći połączenia elementów 
elektronicznych.

Warunki zaliczenia zadania:
Połącz rezystor z źródłem zasilania.
Czerwony kolor odpowiada zasilaniu (+), natomiast 
czarny to masa (tutaj -).











)END";

	const wchar_t* shortDesc = LR"END(
Początkowy poziom, który służy do zapoznania się z aplikacją
oraz z podstawowymi pojęciami. 

Prądem elektrycznym nazywamy uporządkowany ruch ładunków
elektrycznych, czyli taki, w którym ładunek elektryczny
przemieszcza się w jednym kierunku. Przyjęte zostało, 
że prąd płynie od + do -. Ilość przepływającego prądu
to natężenie oznaczane literą 'i' lub 'I', jednostą 
jest Amper (A).

Napięcie elektryczne to różnica potencjałów, analogią
jest różnica wysokości. Napięcie jest oznaczane literą
'u', jednostą jest Volt (V).

Warunki zaliczenia zadania:
Połącz rezystor z źródłem zasilania.






)END";

	Level* level = new Level("ASTART", L"Początki", desc, false, shortDesc);
	level->setPrevLevelsIds({ });
	level->setBoardDimension({ 5, 5, 1 });
	level->setPathToSave("save.asc");
	level->setSchematicPath(LEVEL_START_SCHEMATIC_PATH);

	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		//graphics.addGraphicToLoad("diode", DIODE_GRAPHIC_PATH);
		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
		});

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n1kΩ", L"Zamienia część energii elektrycznej w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("1000R");
	/*	components[1] = new LightEmittingDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("diode"), Component::ComponentTypePackage::SMD, true, "id1");
		components[1]->setSimValue("QTLP690C");*/

		delete[] tmp;

		return components;
		}));


	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();

		/*for (map<std::string, int>::iterator it  = componentsCount.begin(); it != componentsCount.end(); it++)
		{
			cout << it->first << ": " << it->second << endl;
		}*/

		if (componentsCount.find("id0")->second != 1)
			return false;

		/*if (componentsCount.find("id1")->second != 1)
			return false;*/

		return true;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.01)
			return true;
		if (abs(sim->getComponentValue("res1id0")) > 0.01)
			return true;
		if (abs(sim->getComponentValue("res2id0")) > 0.01)
			return true;
		if (abs(sim->getComponentValue("res3id0")) > 0.01)
			return true;
		if (abs(sim->getComponentValue("res4id0")) > 0.01)
			return true;
		if (abs(sim->getComponentValue("res5id0")) > 0.01)
			return true;
		return false;
		});

	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));

		return componentsDesc;
		});

	return level;
}

Level* loadLevelRES0()
{
	const wchar_t* desc = LR"END(
Poziom, w którym poznasz prawo Ohma 
oraz łączenie przyrządów pomiarowych!


Prawo Ohma jest jednym z podstawowych praw fizyki stosowanych
w elektryczności. Opisuje związek między napięciem (U),
natężeniem (I) oraz oporem (R) w obwodzie elektrycznym.
Wzór opisujący to prawo wygląda następująco: R = U / I [Ω]
Jednostką oporu jest Ohm oznaczany grecką literą omega: Ω.


Warunki zaliczenia poziomu:
Podłącz pod zasilanie taki opornik aby płynący przez niego
prąd wyniósł 12mA. Wymagany w tym poziomie jest amperomierz,
który pokaże płynący prąd przez podłączony opornik.

Wykorzystaj woltomierz, by sprawdzić napięcie na źródle
zasilania.

Po dodaniu przyrządu pomiarowego wykonaj symulację, a następnie 
najedź kursorem myszy na przyrząd pomiarowy, by odczytać
zmierzone wartości.

Pamiętaj!
Do pomiaru napięcia wykorzystywany jest woltomierz, który 
podłączany jest równolegle w układ elektroniczny!
Natomiast do pomiaru natężenia płynącego prądu wykorzystywany
jest amperomierz podłączany jest szeregowo (w układ).











)END";

	const wchar_t* shortDesc = LR"END(
Poziom, w którym poznasz prawo Ohma 
oraz łączenie przyrządów pomiarowych!


Prawo Ohma jest jednym z podstawowych praw fizyki stosowanych
w elektryczności. Opisuje związek między napięciem (U),
natężeniem (I) oraz oporem (R) w obwodzie elektrycznym.
Wzór opisujący to prawo wygląda następująco: R = U / I [Ω]
Jednostką oporu jest Ohm oznaczany grecką literą omega: Ω.


Warunki zaliczenia poziomu:
Podłącz pod zasilanie taki opornik aby płynący przez niego
prąd wyniósł 12mA. Wymagany w tym poziomie jest amperomierz,
który pokaże płynący prąd przez podłączony opornik.

Wykorzystaj woltomierz, by sprawdzić napięcie na źródle
zasilania.



)END";

	Level* level = new Level("BOHM", L"Prawo Ohma", desc, false, shortDesc);
	level->setPrevLevelsIds({ "ASTART" });
	level->setBoardDimension({ 10, 10, 1 });
	level->setPathToSave("save.asc");
	level->setSchematicPath(LEVEL_RES0_SCHEMATIC_PATH);

	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("voltmeter", VOLTMETER_GRAPHIC_PATH);
		graphics.addGraphicToLoad("amperemeter", AMPEREMETER_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor1k", RESISTOR_1k_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor220", RESISTOR_220_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor120", RESISTOR_120_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor47", RESISTOR_47_GRAPHIC_PATH);
		graphics.addGraphicToLoad("diode", DIODE_GRAPHIC_PATH);
		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
		});

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 6;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 1;
		tmp[1].x = 1;
		tmp[1].y = 1;
		components[0] = new Voltmeter(L"Woltomierz", L"", Vector2i(2, 2), 2, tmp, GraphicManager::GetInstance().getComponentTexture("voltmeter"), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("1000000000R");
		components[1] = new Amperemeter(L"Amperomierz", L"", Vector2i(2, 2), 2, tmp, GraphicManager::GetInstance().getComponentTexture("amperemeter"), Component::ComponentTypePackage::SMD, true, "id1");
		components[1]->setSimValue("0.000000001R");

		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[2] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor220"), Component::ComponentTypePackage::SMD, true, "id2");
		components[2]->setSimValue("220R");
		components[3] = new Resistor(L"Rezystor\n120Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor120"), Component::ComponentTypePackage::SMD, true, "id3");
		components[3]->setSimValue("120R");
		components[4] = new Resistor(L"Rezystor\n1kΩ", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor1k"), Component::ComponentTypePackage::SMD, true, "id4");
		components[4]->setSimValue("1000R");
		components[5] = new Resistor(L"Rezystor\n47Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor47"), Component::ComponentTypePackage::SMD, true, "id5");
		components[5]->setSimValue("47R");

		delete[] tmp;

		return components;
		}));


	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();



		if (componentsCount.find("id1")->second != 1)
			return false;

		return true;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		try
		{
			if (abs(sim->getComponentValue("res0id4")) > 0.011)
				return true;
			if (abs(sim->getComponentValue("res1id4")) > 0.011)
				return true;
			if (abs(sim->getComponentValue("res2id4")) > 0.011)
				return true;
			if (abs(sim->getComponentValue("res3id4")) > 0.011)
				return true;
			if (abs(sim->getComponentValue("res4id4")) > 0.011)
				return true;
			if (abs(sim->getComponentValue("res5id4")) > 0.011)
				return true;
		}
		catch (const std::string&)
		{

		}
		return false;
		});

	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;

		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(VOLTMETER_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(1, ComponentDesc(AMPEREMETER_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(2, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(3, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(4, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(5, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));

		return componentsDesc;
		});

	return level;
}

Level* loadLevelRES1()
{
	Level* level = new Level("RES1", L"Szeregowe łączenie\noporników", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "KIR1" });
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
	//Level* level = new Level("RES1", L"Opór, połączenie\nrównoległe", L"Poznaj opór w przypadku łączenia oporników równolegle!");
	//level->setPrevLevelsIds({ "RES0" });

	//level->setPathToSave("save.asc");

	//level->setGenerateComponents(([](int* componentsCount)->Component** {
	//	*componentsCount = 2;
	//	Component** components = new Component * [*componentsCount];
	//	Vector2i* tmp = new Vector2i[2];
	//	tmp[0].x = 0;
	//	tmp[0].y = 0;
	//	tmp[1].x = 1;
	//	tmp[1].y = 0;
	//	components[0] = new Resistor(L"Rezystor", L"Zamienia część energii elektrycznje w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id0");
	//	components[1] = new LightEmittingDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id1");

	//	delete[] tmp;

	//	return components;
	//	}));

	////Set components conditions
	//level->setCheckBoard([](Board* board)->bool {
	//	map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

	//	if(componentsCount[Component::CompoenetType::resistor] != 1)
	//		return false;

	//		/*if (componentsCount[Component::CompoenetType::led] != 1)
	//			throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

	//	return true;
	//	});

	////level->setCheckSimulation();

	//return level;
}

Level* loadLevelRES2()
{
	Level* level = new Level("RES2", L"Równoległe łączenie\noporników", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "KIR1"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelRES3()
{
	Level* level = new Level("RES3", L"Mieszane łączenie\noporników", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "RES1", "RES2"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}


Level* loadLevelCAP0()
{
	Level* level = new Level("CAP0", L"Kondensator", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "BOHM"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelCAP1()
{
	Level* level = new Level("CAP1", L"Szeregowe łączenie\nkondensatorów", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "KIR1", "CAP0" });
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelCAP2()
{
	Level* level = new Level("CAP2", L"Równoległe łączenie\nkondensatorów", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "KIR1", "CAP0"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelCAP3()
{
	Level* level = new Level("CAP3", L"Mieszane łączenie\nkondensatorów", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({"CAP1", "CAP2"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelIND0()
{
	Level* level = new Level("IND0", L"Cewka", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "BOHM"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelVOLREG()
{
	Level* level = new Level("VOLTREG", L"Stabilizator\nliniowy", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "KIR2", "CAP0"});
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
		graphics.addGraphicToLoad("voltage_regulator", SOT223_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");


		delete[] tmp;
		tmp = new Vector2i[4];
		tmp[0].x = 1; tmp[0].y = 0;
		tmp[1].x = 0; tmp[1].y = 2;
		tmp[2].x = 1; tmp[2].y = 2;
		tmp[3].x = 2; tmp[3].y = 2;
		components[1] = new Microcontroller(L"Regulator napięcia",L"", Vector2i(3,3), 4, tmp, GraphicManager::GetInstance().getComponentTexture("voltage_regulator"), Component::ComponentTypePackage::SMD, true, "id3");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelBITRAN0()
{
	Level* level = new Level("BITRAN0", L"Tranzystor bipolarny", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "KIR2" });
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelMOSFET0()
{
	Level* level = new Level("MOSFET0", L"Szeregowe łączenie\noporników", L"Opis", false, L"SkróconyOpis");

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	level->setPrevLevelsIds({ "BITRAN0" });
	level->setBoardDimension({ 10, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };
		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		board->placeComponentForce(vcc, pinPos);
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 1;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[0]->setSimValue("220R");
		delete[] tmp;
		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id0")->second != 5)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelUC()
{
	const wchar_t* desc = LR"(
W tym poziomie nauczysz się jak zasilać mikrokontroler!

Mikrokontroler jest scalonym układem mikroprocesorowym, 
czyli zawierącym procesor, pamięć RAM, wejścia/wyjścia
i często pamięć FLASH. Mikrokontroler wykonuje wcześniej 
skompilowany i wgrany program. Z reguły jedna z nóżek
jest nóżką resetującą, co oznacza, że gdy zostanie podany
stan niski to układ się zresetuje i będzie w stanie resetu.

Warunki zaliczenia zadania:
Podłącz mikorkontroler do zasilania przez stabilizator 
napięcia. Nóżkę reset do zasilania mikrokontrolera przez 
opornik 1kΩ tak aby nie spalić układu. Nóżkę numer 2 podłącz 
do niebieskiego goldpina. Natomiast do nóżek 3, 5, 6, 7
diody świecące. Uważaj aby nie spalić układu ani diod!

Wykorzystaj narzędzia pomiarowe, by zmierzyć natężenie 
płynącego prądu oraz spadki napięć na elementach 
elektronicznych.

Pamiętaj!
Po najechaniu na nazwę elementu elektronicznego 
zobaczysz jego opis.














)";

	Level* level = new Level("UC", L"Zasilanie \nmikrokontrolera", desc, false, desc);

	//Ustawienie poprzednich poziomów, które muszą być zrealizowane
	//level->setPrevLevelsIds({"VOLTREG"});
	level->setBoardDimension({ 15, 15, 1 });
	level->setSchematicPath("path_to_level_schematic");

	//Dodanie podstawowych elementów na płtytkę
	level->setInitBoardFun([](Board* board)->bool {
		Vector2i* tmp = new Vector2i[1];
		tmp[0].x = 0;
		tmp[0].y = 0;
		Vector2i pinPos = { 0, 0 };

		Component* vcc = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* gnd = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		Component* reg = new Goldpin(L"Złącze goldpin", L"", { 1,1 }, 1, tmp, GraphicManager::GetInstance().getComponentTexture("goldpin"), Component::ComponentTypePackage::THT, false);
		gnd->rotate();
		delete[] tmp;
		reg->rotate();
		reg->rotate();

		//Vcc
		board->placeComponentForce(vcc, pinPos);

		//GND
		pinPos.x++;
		board->placeComponentForce(gnd, pinPos);

		pinPos = { 10,10 };
		board->placeComponentForce(reg, pinPos);
		return true;
		});

	//Ustawienie grafik komponentów, które są wykorzystywane
	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("voltmeter", VOLTMETER_GRAPHIC_PATH);
		graphics.addGraphicToLoad("amperemeter", AMPEREMETER_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor", RESISTOR_1k_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor2", RESISTOR_200_GRAPHIC_PATH);
		graphics.addGraphicToLoad("diode", DIODE_GRAPHIC_PATH);
		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
		graphics.addGraphicToLoad("capacitor", CAPACITOR_GRAPHIC_PATH);
		graphics.addGraphicToLoad("uc", MICROCONTROLLER_DPAK8_GRAPHIC_PATH);
		graphics.addGraphicToLoad("voltage_regulator", SOT223_GRAPHIC_PATH);
		});

	//Dodanie komponentów, które mogą być ułożone na płytkę
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 8;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp= nullptr;
		
		tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 1;
		tmp[1].x = 1;
		tmp[1].y = 1;
		components[0] = new Voltmeter(L"Woltomierz", L"", Vector2i(2, 2), 2, tmp, GraphicManager::GetInstance().getComponentTexture("voltmeter"), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("1000000000R");
		components[1] = new Amperemeter(L"Amperomierz", L"", Vector2i(2, 2), 2, tmp, GraphicManager::GetInstance().getComponentTexture("amperemeter"), Component::ComponentTypePackage::SMD, true, "id1");
		components[1]->setSimValue("0.000000001R");
		delete[] tmp;

		tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[2] = new Resistor(L"Rezystor\n1kΩ", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id0");
		components[2]->setSimValue("1000R");
		components[3] = new Resistor(L"Rezystor\n220Ω", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor2"), Component::ComponentTypePackage::SMD, true, "id7");
		components[3]->setSimValue("220R");
		components[4] = new LightEmittingDiode(L"Żółta dioda\nLED", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("diode"), Component::ComponentTypePackage::SMD, true, "id1");
		components[4]->setSimValue("QTLP690C");
		components[5] = new Capacitor(L"Kondensator\n0.1µF", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("capacitor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[5]->setSimValue("10C");
		components[6] = new Capacitor(L"Kondensator\n1µF", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("capacitor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[6]->setSimValue("10C");

		delete[] tmp;
		tmp = new Vector2i[8];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		tmp[2].x = 2; tmp[2].y = 0;
		tmp[3].x = 3; tmp[3].y = 0;

		tmp[4].x = 0; tmp[4].y = 3;
		tmp[5].x = 1; tmp[5].y = 3;
		tmp[6].x = 2; tmp[6].y = 3;
		tmp[7].x = 3; tmp[7].y = 3;

		components[7] = new Microcontroller(L"Mikrokontroler", L"awd", Vector2i(4, 4), 8, tmp, GraphicManager::GetInstance().getComponentTexture("uc"), Component::ComponentTypePackage::SMD, true, "id3");
		delete[] tmp;

		tmp = new Vector2i[4];
		tmp[0].x = 1; tmp[0].y = 0;
		tmp[1].x = 0; tmp[1].y = 2;
		tmp[2].x = 1; tmp[2].y = 2;
		tmp[3].x = 2; tmp[3].y = 2;
		components[8] = new Microcontroller(L"Stabilizator\nliniowy", L"", Vector2i(3, 3), 4, tmp, GraphicManager::GetInstance().getComponentTexture("voltage_regulator"), Component::ComponentTypePackage::SMD, true, "id14");
		delete[] tmp;

		return components;
		}));

	//Ustawienie grafik opsisów komponentów, które są wykorzystywane
	level->setInitComponentsDesc([](Component** components, const int& componentsCount)->std::map<int, ComponentDesc> {
		std::map<int, ComponentDesc> componentsDesc;
		componentsDesc.insert(std::pair<int, ComponentDesc>(0, ComponentDesc(VOLTMETER_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(1, ComponentDesc(AMPEREMETER_DESCRIPTION_PATH)));
		componentsDesc.insert(std::pair<int, ComponentDesc>(3, ComponentDesc(RESISTOR_DESCRIPTION_PATH)));
		return componentsDesc;
		});

	//Dodanie warunków sprawdzających płytkę
	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();
		if (componentsCount.find("id14")->second != 1)
			return false;
		return true;
		});

	//Dodanie warunków sprawdzających wynik symulacji
	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		if (abs(sim->getComponentValue("res0id0")) > 0.052)
			return true;
		return false;
		});

	return level;
}


Level* loadLevelLED0()
{
	const wchar_t* shortDesc = LR"END(
Poziom, w którym poznasz diodę świecącą LED!


Diody LED są typem diod, które pod wpływem przechodzącego
prądu świecą. Diody przewodzą prąd tylko w jedną stronę
(są typy diod, które łamią tą zasadę). Posiadają dwa 
wyprowadzenia: anodę i katodę, kierunek przepływu prądu
następeuje od anody(+) do katody(-).

Pamiętaj, że dioda wymaga odpowiedniego rezystora, 
który ograniczy płycący prąd. Jego brak może spowodować
uszkodzenie diody!



Warunki zaliczenia poziomu:
Połącz diodę z odpowiednim opornikiem, który zapewni
odpowiedni pobór prądu przez diodę LED.

Wykorzystaj woltomierz, by sprawdzić spadek napięcia
na diodzie, oporniku oraz zasilaniu.


)END";

	Level* level = new Level("LED0", L"Dioda świecąca!", L"Poznaj diodę LED!", false, shortDesc);
	level->setPrevLevelsIds({ "BOHM" });

	level->setPathToSave("save.asc");

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor", L"Zamienia część energii elektrycznje w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LightEmittingDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture(""), Component::ComponentTypePackage::SMD, true, "id1");

		delete[] tmp;

		return components;
		}));

	//Set components conditions
	level->setCheckBoard([](Board* board)->bool {
		map<Component::CompoenetType, int> componentsCount = board->getComponentsCount();

		/*if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;*/

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}
