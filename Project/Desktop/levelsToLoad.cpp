#include "levelsToLoad.h"

Level* loadLevelExampleOfImplemetation()
{

	Level* level = new Level(L"ID", L"NAME", L"DESC");
	
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
	Level* level = new Level(L"Test0", L"Test", L"Poziom testujący", false);
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

Level* loadMicrocontrollerLevel()
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

Pamiętaj!
Po najechaniu na nazwę elementu elektronicznego 
zobaczysz jego opis.

)";

	Level* level = new Level(L"ZEL1", L"Zasilanie\nmikrokontrolera", desc, true);
	level->setPrevLevelsIds({ });

	level->setPathToSave("save.asc");
	level->setBoardDimension({ 15, 15, 1});
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

		pinPos = {10,10};
		board->placeComponentForce(reg, pinPos);
		return true;
		});

	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_10k_GRAPHIC_PATH);
		graphics.addGraphicToLoad("resistor2", RESISTOR_200_GRAPHIC_PATH);
		graphics.addGraphicToLoad("diode", DIODE_GRAPHIC_PATH);
		graphics.addGraphicToLoad("goldpin", GOLDPIN_GRAPHIC_PATH);
		graphics.addGraphicToLoad("capacitor", CAPACITOR_GRAPHIC_PATH);
		graphics.addGraphicToLoad("uc", MICROCONTROLLER_DPAK8_GRAPHIC_PATH);
		});

	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 6;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0;
		tmp[0].y = 0;
		tmp[1].x = 1;
		tmp[1].y = 0;
		components[0] = new Resistor(L"Rezystor\n1kΩ", L"Zamienia część energii elektrycznej w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor"), Component::ComponentTypePackage::SMD, true, "id0");
		components[0]->setSimValue("1000R");
		components[4] = new Resistor(L"Rezystor\n200Ω", L"Zamienia część energii elektrycznej w ciepło", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("resistor2"), Component::ComponentTypePackage::SMD, true, "id7");
		components[4]->setSimValue("200R");
		components[1] = new LightEmittingDiode(L"LED", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("diode"), Component::ComponentTypePackage::SMD, true, "id1");
		components[1]->setSimValue("QTLP690C");
		components[5] = new Capacitor(L"Capacitor", L"", Vector2i(2, 1), 2, tmp, GraphicManager::GetInstance().getComponentTexture("capacitor"), Component::ComponentTypePackage::SMD, true, "id2");
		components[5]->setSimValue("10C");


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

		components[3] = new Microcontroller(L"Mikrokontroler", L"awd", Vector2i(4, 4), 8, tmp, GraphicManager::GetInstance().getComponentTexture("uc"), Component::ComponentTypePackage::SMD, true, "id3");
		components[2] = new Microcontroller(L"Stabilizator\nnapięcia", L"awd", Vector2i(4, 4), 8, tmp, GraphicManager::GetInstance().getComponentTexture("uc"), Component::ComponentTypePackage::SMD, true, "id3");
		delete[] tmp;

		return components;
		}));


	level->setCheckBoard([](Board* board)->bool {
		map<std::string, int> componentsCount = board->getComponentsCountById();

		/*for (map<std::string, int>::iterator it  = componentsCount.begin(); it != componentsCount.end(); it++)
		{
			cout << it->first << ": " << it->second << endl;
		}*/

	/*	if (componentsCount.find("id0")->second != 1)
			return false;*/

		/*if (componentsCount.find("id1")->second != 1)
			return false;*/

		return false;
		});

	level->setCheckSimulation([](Board* board, SimulationEngine* sim)->bool {
		float val = sim->getComponentValue("res0id0");
		if (abs(sim->getComponentValue("res0id0")) > 0.1)
			return true;
		return false;
		});

	return level;
}

Level* loadLevel0()
{
	const wchar_t* desc = LR"END(
Początkowy poziom, 
który służy do zapoznania się z aplikacją.
)END"; 

	Level* level = new Level(L"P0", L"Początki", desc, false);
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

Level* loadLevelSTART()
{
	const wchar_t* desc = LR"END(
Początkowy poziom, który służy do zapoznania się z aplikacją
oraz z podstawowymi pojęciami. 

Prądem elektrycznym nazywamy uporządkowany ruch ładunków
elektrycznych, czyli taki, w którym ładunek elektryczny
przemieszcza się w jednym kierunku. Przyjęte zostało, 
że prąd płynie od + do -. Ilość przepływającego prądu
to natężenie oznaczane literką 'i' lub 'I', jednostą 
jest Amper (A).

Napięcie elektryczne to różnica potencjałów, analogią
jest różnica wysokości. Napięcie jest oznaczane literką
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
	Level* level = new Level(L"ASTART", L"Początki", desc, true);
	level->setPrevLevelsIds({ });
	level->setBoardDimension({ 5, 5, 1 });
	level->setPathToSave("save.asc");

	level->setComponentsGraphicsToLoad([]()->void {
		GraphicManager& graphics = GraphicManager::GetInstance();
		graphics.addGraphicToLoad("resistor", RESISTOR_10k_GRAPHIC_PATH);
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
		float val = sim->getComponentValue("res0id0");
		if (abs(sim->getComponentValue("res0id0")) > 0.01)
			return true;
		return false;
		});

	return level;
}

Level* loadLevelRES0()
{
	Level* level = new Level(L"RES0", L"Opór", L"Poznaj opór!");
	level->setPrevLevelsIds({ "ASTART" });

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

Level* loadLevelRES1()
{
	Level* level = new Level(L"RES1", L"Opór, połączenie\nrównoległe", L"Poznaj opór w przypadku łączenia oporników równolegle!");
	level->setPrevLevelsIds({ "RES0" });

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

		if(componentsCount[Component::CompoenetType::resistor] != 1)
			return false;

			/*if (componentsCount[Component::CompoenetType::led] != 1)
				throw "Oczekiwano: 1 dostano: " + to_string(componentsCount[Component::CompoenetType::led]);*/

		return true;
		});

	//level->setCheckSimulation();

	return level;
}

Level* loadLevelCAP0()
{
	Level* level = new Level(L"CAP0", L"Pojemność", L"Poznaj pojemność!");
	level->setPrevLevelsIds({ "RES0" });

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

Level* loadLevelLED0()
{
	Level* level = new Level(L"LED0", L"Dioda świecąca!", L"Poznaj diodę LED!");
	level->setPrevLevelsIds({ "RES0" });

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

Level* emptyLevel(sf::String id, sf::String name, sf::String desc)
{
	return new Level(id, name, desc);
}
