#pragma once
#include <SFML/Graphics.hpp>
#include "gui.h"
#include "resources.h"

#include "component.h"
#include "simulationEngine.h"
#include "state.h"
#include "level.h"
#include "stateLevelSelect.h"
#include  "boardSave.h"
#include "componentDesc.h"

/*

Aplikacja desktopowa pomagaj¹ca w nauce elektroniki polega na uk³adaniu komponentów elektronicznych wed³ug podanego zadania.
U¿ytkownik dostaje zadanie wraz z ograniczon¹ pul¹ elementów, z których musi u³o¿yæ dzia³aj¹cy uk³ad. 
Wraz z postêpem u¿ytkownika trudnoœæ zadañ roœnie. 
Zadaniem u¿ytkownika jest u³o¿enie elementów na p³ytce w taki sposób aby uk³ad dzia³a³ w sposób okreœlony przez zadanie.
Aplikacja wymaga logowania, w celu zapisania postêpu gracza.

*/


//Ka¿de zadanie zawiera opis elementów oraz 

//Main game state
class SolveLevel: public State
{
public:
	SolveLevel() = delete;
	SolveLevel(Resources* res, Level* level = nullptr, bool loadExistingLevel = false);
	~SolveLevel();
	
	void AddLetter(wchar_t s) { }
	void Update(RenderWindow* window, Time* elapsed);
	
	void Render(RenderTarget* target);
private:
	Logger* logger;
	Resources* res;

	View* view;
	Vector2f origin;
	Vector2f oldOrigin;

	// Menu section
	Button* menuButton;
	Button* helpButton;
	Button* checkButton;
	PopupBox* popupBox;

	//-----------------------------------------
	// Task section
	//-----------------------------------------
	inline void initTaskSection(Resources* res);
	
	inline void destroyTaskSection();

	void updateTaskSection();
	void renderTaskSection(RenderTarget* target);
	

	TextBox* taskName;
	TextBox* taskDescription;
	
	bool showSimulationCheckResponse;
	TextBox* simulationCheckResponse;

	//-----------------------------------------
	// Route section - Bottom 
	//-----------------------------------------
	inline void initRouteSection(Resources* res);
	inline void destroyRouteSection();

	inline void updateRouteSection(RenderWindow* window, Time* elapsed);
	
	inline void renderRouteSection(RenderTarget* target);
	ComponentDesc* componentDesc;

	Button* removeComponentButton;
	Button* moveComponentButotn;
	Button* addRouteButton;
	Button* hideComponentsButton;
	unsigned short addRouteButtonState;

	//-----------------------------------------
	// Component section - Bottom
	//-----------------------------------------
	inline void initComponentSection(Resources* res);
	
	inline void destroyComponentSection();
	inline void updateComponentSection(RenderWindow* window, Time* elapsed);
	inline void renderComponentSection(RenderTarget* target)
	{
		//selectionComponent->Render(target);
	}

	//TextBox* selectionComponent;
	TextBox* selectedComponent;

	//Button* addLedDiode;
	SelectBox* selectComponent;
	Button** addingComponents;

	//Futeure load from outside
	Component** components;
	int componentsCount;

	enum MouseMode {
		Idle,
		Route,
		Place,
		Remove
	};
	MouseMode mouseMode = MouseMode::Idle;

	Vector2i lastTileHover;
	bool rotated = false;

	//-----------------------------------------
	// Board section
	//-----------------------------------------
	inline void initBoardSection();

	inline void destroyBoardSection();

	void (SolveLevel::* updateBoardSection)(RenderWindow* window, Time* elapsed);
	void updateBoardSectionIdle(RenderWindow* window, Time* elapsed)
 	{ 
		//Select component

		//Delete component
	}
	void updateBoardSectionRoute(RenderWindow* window, Time* elapsed);
	
	void updateBoardSectionPlaceComponent(RenderWindow* window, Time* elapsed);
	
	void updateBoardSectionRemoveComponent(RenderWindow* window, Time* elapsed);
	
	
	inline void renderBoardSection(RenderTarget* target);

	Board* board;
	Component* addComponent = nullptr;

	//-----------------------------------------
	// INFO near mouse
	//-----------------------------------------
	inline void initInfoNearMouse(Resources* res);

	inline void destroyInfoNearMouse();
	inline void updateInfoNearMouse(RenderWindow* window, Time* elapsed);
	
	inline void renderInfoNearMouse(RenderTarget* target);


	TextBox* mouseInfoBox;

	SimulationEngine* simulationEngine;
	Level* currentLevel;
};