void Board::placeComponent(Component* component, Vector2i& pos) {
	if (!canPlaceComponent(component, pos))
		throw sf::String(L"Can't place component! pos: ") + to_wstring(pos.x) + " " + to_wstring(pos.y);

	for (int j = 0; j < component->getTileSize().y; j++) {
		for (int i = 0; i < component->getTileSize().x; i++) {
			isComponentOnBoard[(j + pos.y) * length + (i + pos.x)] = component;
		}
	}

	Vector2i* padsPos = component->getPadsPos();
	for (int i = 0; i < component->getPadsCount(); i++) {
		Vector2i buf = component->getBoardPosition() + padsPos[i];
		if (component->getComponentTypePackage() 
		== Component::ComponentTypePackage::SMD)
			boardTiles[buf.x + buf.y * length].setState(Tile::TileState::SMD_PAD);
		else if (component->getComponentTypePackage() 
		== Component::ComponentTypePackage::THT)
			boardTiles[buf.x + buf.y * length].setState(Tile::TileState::THT_PAD);
	}
	component->setBoardPosition(pos);
	components.push_back(component);
	logger->Info("Added " + component->getId() + " pos:" 
	+ to_string(pos.x) + " " + to_string(pos.y));

	sort(components.begin(), components.end(), [](Component* i, Component* j) {
		return (i->getGlobalPos().y > j->getGlobalPos().y);
		});
}