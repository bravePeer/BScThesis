#include "stateCloseMenu.h"

void CloseMenu::Render(RenderTarget* target)
{
	dynamic_cast<RenderWindow*>(target)->close();
}