#pragma once
#include <SFML/Graphics.hpp>
#include "component.h"
#include <fstream>
#include <map>
#include "tile.h"
//#include "boardSave.h"

//#define TILE_LENGTH 64
//#define TILE_WIDTH 32


class Board
{
	friend class BoardSave;
	Logger* logger;

	int length;
	int width;
	int layers;
	Vector2f origin;

	Tile* boardTiles;
	vector<Component*> components; //TODO Trzeba b�dzie sortowa� po dodaniu nowego komponentu
	//bool* isComponentOnBoard;
	Component** isComponentOnBoard;
	Component* ghostComponent;

	//Vector2f viewOrigin = { 0.f, 0.f };
	Vector2f viewOrigin = { 500.f, 15.f };

	bool hideComponents = false;
public:
	Board();
	Board(int length, int width, int layers);
	
	~Board();

	void Update(RenderWindow* window, Time* elapsed);

	void Render(RenderTarget* target);

	Vector2i getHoverTilePos(Vector2i& mouse);

	Tile& getTile(Vector2i pos);

	const Vector2i getBoardDimension();

	Vector2f& getViewOrigin();
	void moveViewOrigin(Vector2f offset);

	char tileNighbourDirection(Vector2i pos0, Vector2i pos1);
	void addRoute(Vector2i pos0, Vector2i pos1);
	//Add route on one tile, pos is miltiplied by 16
	void setRoute(Vector2i pos0, Vector2i pos1);
	void removeRoute(Vector2i pos0, Vector2i pos1);

	bool canPlaceComponent(Component* component, Vector2i& pos);
	void placeComponent(Component* component, Vector2i& pos);
	void placeComponentForce(Component* component, Vector2i& pos);
	void removeComponent(Vector2i pos);
	Component* getComponentOnBoard(Vector2i pos);


	map<Component::CompoenetType, int> getComponentsCount();


	void setHideComponent(bool hide);

	bool isHideComponent();

	//Saving board as asc file
	void saveBoard();
	void loadBoard(const std::string& path);


	//---------Testing---------
	void printComponentsMap();
	void printRoutMap();
};

