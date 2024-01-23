Level* loadLevel0()
{
	Level* level = new Level(L"P0", L"Początki", L"Początkowy poziom, \nktóry służy do zapoznania się z aplikacją.", false);
	level->setPathToSave("save.asc");
	level->setGenerateComponents(([](int* componentsCount)->Component** {
		*componentsCount = 2;
		Component** components = new Component * [*componentsCount];
		Vector2i* tmp = new Vector2i[2];
		tmp[0].x = 0; tmp[0].y = 0;
		tmp[1].x = 1; tmp[1].y = 0;
		components[0] = new Resistor(L"Opornik", L"Zamienia część energii elektrycznje w ciepło", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getResistorTexture(), Component::ComponentTypePackage::SMD, true, "id0");
		components[1] = new LedDiode(L"", L"", Vector2i(2, 1), 2, tmp, GraphicAll::GetInstance().getDiodeTexture(), Component::ComponentTypePackage::SMD, true, "id1");
		delete[] tmp;
		return components;
		}));

	level->setCheckBoard([](Board* board)->bool { ... });

	level->setCheckSimulation([](Board* board)->bool { ... });

	return level;
}