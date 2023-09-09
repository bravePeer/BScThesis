#pragma once
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "utilities.h"
#include "graphics.h"

using namespace std;
using namespace sf;


//Przycisk xd
class Button
{
public:
	Button()
	{
		font = nullptr;
		flag = false;
	}
	Button(Vector2f _size, Vector2f _pos, Font* _font = nullptr, String _text = L"", Color _idle = Color(255, 0, 0, 255), Color _hover = Color(249, 0, 110, 255), Color _active = Color(150, 0, 0, 255), Texture* _texture = nullptr)
		:font(_font), idleColor(_idle), hoverColor(_hover), activeColor(_active)
	{
		shape.setSize(_size);
		shape.setPosition(_pos);
		shape.setFillColor(idleColor);
		shape.setTexture(_texture);

		text.setFont(*font);
		text.setCharacterSize(20);
		text.setString(_text);
		text.setFillColor(Color::White);
		text.setPosition(
			shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);

		offset = { 0,0 };

		drawingShape = shape;

		flag = false;
	}
	~Button()
	{}

	void SetPosition(Vector2f _pos)
	{
		drawingShape.setPosition(_pos);
		text.setPosition(
			drawingShape.getPosition().x + (drawingShape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			drawingShape.getPosition().y + (drawingShape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
	void Move(Vector2f _offset)
	{
		drawingShape.move(_offset);
		text.setPosition(
			drawingShape.getPosition().x + (drawingShape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			drawingShape.getPosition().y + (drawingShape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
	void Move(Vector2f _offset, Vector2f _offsetDrawing)
	{
		drawingShape.move(_offset);
		shape.move(_offsetDrawing);
		text.setPosition(
			drawingShape.getPosition().x + (drawingShape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			drawingShape.getPosition().y + (drawingShape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
	void SetString(String _text)
	{
		text.setString(_text);
		text.setPosition(
			drawingShape.getPosition().x + (drawingShape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			drawingShape.getPosition().y + (drawingShape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
	Vector2f GetPosition()
	{
		return shape.getPosition();
	}
	unsigned short GetButtonState()
	{
		return buttonState;
	}
	unsigned short GetLastButtonState()
	{
		return lastButtonState;
	}

	void SetButtonState(unsigned short _state)
	{
		buttonState = _state;
	}
	void Resize(Vector2f _size)
	{
		if (_size.x == -1)
			_size.x = shape.getSize().x;
		if (_size.y == -1)
			_size.y = shape.getSize().y;

		shape.setSize(_size);
		drawingShape.setSize(_size);
	}

	void Update(const Vector2f mousePos)
	{
		buttonState = IDLE;
		if (shape.getGlobalBounds().contains(mousePos))
			buttonState = HOVER;

		if (Mouse::isButtonPressed(Mouse::Left) && buttonState == HOVER)
			flag = true;

		if (!Mouse::isButtonPressed(Mouse::Left) && flag)
		{
			if (buttonState == HOVER)
				buttonState = PRESSED;
			flag = false;
		}

		switch (buttonState)
		{
		case IDLE:
			drawingShape.setFillColor(idleColor);
			break;
		case HOVER:
			drawingShape.setFillColor(hoverColor);
			break;
		case PRESSED:
			drawingShape.setFillColor(activeColor);
			break;
		}
	}
	void Render(RenderTarget* target)
	{
		target->draw(drawingShape);
		target->draw(text);
	}
private:
	RectangleShape shape;
	RectangleShape drawingShape;
	Font* font;
	Text text;

	Color idleColor, hoverColor, activeColor;

	Vector2f offset;//przesuni�cie

	bool flag; // flaga by nie wcisn�� dwoch przyciskow na raz
	unsigned short buttonState = IDLE;
	unsigned short lastButtonState = IDLE;

	void SetTextPosition()
	{
		text.setPosition(
			shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
};
//Pole do wprowadzania tekstu
class InputBox
{
public:
	InputBox()
	{
		font = nullptr;
	}
	InputBox(Vector2f _size, Vector2f _pos, Font* _font, String _text, Color _idle, Color _hover, Color _active, Texture* _texture = nullptr)
		:font(_font), idleColor(_idle), hoverColor(_hover), activeColor(_active)
	{
		shape.setSize(_size);
		shape.setPosition(_pos);
		shape.setFillColor(idleColor);
		shape.setTexture(_texture);

		text.setFont(*font);
		text.setCharacterSize(20);
		text.setString(_text);
		text.setFillColor(Color::White);
		text.setPosition(
			shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
	~InputBox()
	{}

	String GetTypedString()
	{
		return typed;
	}
	short GetInputBoxState()
	{
		return inputBoxState;
	}
	virtual void AddLetter(wchar_t s)
	{
		if (inputBoxState == PRESSED)
			if (s == 8)
			{
				if (typed.getSize())
				{
					typed.erase(typed.getSize() - 1);
				}
			}
			else
			{
				typed += s;
			}
		text.setString(typed);
		text.setPosition(shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f, text.getPosition().y);

	}
	void Update(const Vector2f mousePos)
	{

		if (shape.getGlobalBounds().contains(mousePos))
		{
			if (inputBoxState == IDLE)
				inputBoxState = HOVER;

			if (Mouse::isButtonPressed(Mouse::Left))
				inputBoxState = PRESSED;
		}
		else
		{
			if (Mouse::isButtonPressed(Mouse::Left) || inputBoxState == HOVER)
				inputBoxState = IDLE;
		}

		switch (inputBoxState)
		{
		case IDLE:
			shape.setFillColor(idleColor);
			break;
		case HOVER:
			shape.setFillColor(hoverColor);
			break;
		case PRESSED:
			shape.setFillColor(activeColor);
			break;
		}

	}
	void Render(RenderTarget* target)
	{
		target->draw(shape);
		target->draw(text);
	}
protected:
	RectangleShape shape;
	
	Font* font;
	Text text;
	String typed;

	Color idleColor, hoverColor, activeColor;

	unsigned short inputBoxState = IDLE;
};

class InputBoxPassword :public InputBox
{
public:
	InputBoxPassword()
	{
		font = nullptr;
		letter = 0;
	}
	InputBoxPassword(Vector2f _size, Vector2f _pos, Font* _font, String _text, Color _idle, Color _hover, Color _active, Texture* _texture = nullptr, wchar_t _letter = '*')
		:letter(_letter)
	{
		font = _font;
		idleColor = _idle;
		hoverColor = _hover;
		activeColor = _active;

		shape.setSize(_size);
		shape.setPosition(_pos);
		shape.setFillColor(idleColor);
		shape.setTexture(_texture);

		text.setFont(*font);
		text.setCharacterSize(20);
		text.setString(_text);
		text.setFillColor(Color::White);
		text.setPosition(
			shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
	~InputBoxPassword()
	{}

	String GetTypedString()
	{
		return typed;
	}
	short GetInputBoxState()
	{
		return inputBoxState;
	}
	void AddLetter(wchar_t s)
	{
		if (inputBoxState == PRESSED)
			if (s == 8)
			{
				if (typed.getSize())
				{
					typed.erase(typed.getSize() - 1);
				}
			}
			else
			{
				typed += s;
			}
		String str = "";
		for (unsigned short i = 0; i < typed.getSize(); i++)
		{
			str += letter;
		}

		text.setString(str);
		text.setPosition(shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f, text.getPosition().y);

	}
	void Update(const Vector2f mousePos)
	{

		if (shape.getGlobalBounds().contains(mousePos))
		{
			if (inputBoxState == IDLE)
				inputBoxState = HOVER;

			if (Mouse::isButtonPressed(Mouse::Left))
				inputBoxState = PRESSED;
		}
		else
		{
			if (Mouse::isButtonPressed(Mouse::Left) || inputBoxState == HOVER)
				inputBoxState = IDLE;
		}

		switch (inputBoxState)
		{
		case IDLE:
			shape.setFillColor(idleColor);
			break;
		case HOVER:
			shape.setFillColor(hoverColor);
			break;
		case PRESSED:
			shape.setFillColor(activeColor);
			break;
		}

	}
	void Render(RenderTarget* target)
	{
		target->draw(shape);
		target->draw(text);
	}

private:
	wchar_t letter;

};
//Pole do wypisywania tekstu na ekranie
class TextBox
{
public:
	TextBox()
	{
		font = nullptr;
	}
	TextBox(Vector2f _size, Vector2f _pos, Font* _font, String _text, Color _idle = Color(255, 0, 0, 255), Color _hover = Color(249, 0, 110, 255), Color _active = Color(150, 0, 0, 255), unsigned int _characterSize = 20, Texture* _texture = nullptr)
		:font(_font), idleColor(_idle), hoverColor(_hover), activeColor(_active), texture(_texture)
	{
		shape.setSize(_size);
		shape.setPosition(_pos);
		shape.setFillColor(idleColor);
		shape.setTexture(_texture);

		text.setFont(*font);
		text.setCharacterSize(_characterSize);
		text.setString(_text);
		text.setFillColor(Color::White);
		SetTextPosition();
	}
	~TextBox()
	{}

	Vector2f GetPosition()
	{
		return shape.getPosition();
	}
	short GetInputBoxState()
	{
		return inputBoxState;
	}
	void SetString(String _string)
	{
		string = _string;
		text.setString(string);
		SetTextPosition();

	}
	void Move(Vector2f _offset)
	{
		shape.move(_offset);
		SetTextPosition();
	}
	void SetPostition(Vector2f drawPos)
	{
		shape.setPosition(drawPos);
		text.setPosition(
			shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
	void Update()
	{


	}
	void Render(RenderTarget* target)
	{
		target->draw(shape);
		target->draw(text);
	}
private:
	RectangleShape shape;
	Texture* texture;

	Font* font;
	Text text;
	String string;

	Color idleColor, hoverColor, activeColor;

	unsigned short inputBoxState = IDLE;

	void SetTextPosition()
	{
		text.setPosition(
			shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - text.getGlobalBounds().height / 2.f);
	}
};

class SelectBox
{
public:
	SelectBox()
	{

	}
	SelectBox(Vector2f _size, Vector2f _pos, Font* _font, String _text, Color _idle, Color _hover, Color _active, Button** _buttons = nullptr, unsigned short _amountButtons = 0, unsigned int _characterSize = 20, Texture* _texture = nullptr)
		:font(_font), idleColor(_idle), hoverColor(_hover), activeColor(_active), buttons(_buttons), amountButtons(_amountButtons)
	{
		shape.setSize(_size);
		shape.setPosition(_pos);
		shape.setFillColor(idleColor);
		shape.setTexture(_texture);

		text.setFont(*font);
		text.setCharacterSize(_characterSize);
		text.setString(_text);
		text.setFillColor(Color::White);
		SetTextPosition();

		//Ustawienie pozycji przyciskow wzgl�dem selectBoxa
		for (short i = 0; i < amountButtons; i++)
		{
			buttons[i]->Resize({ _size.x, -1 });
			buttons[i]->Move(shape.getPosition(), shape.getPosition());
		}

		selected = -1;
	}
	~SelectBox() { }

	short GetSelected()
	{
		return selected;
	}

	void Update(RenderWindow* window)
	{
		for (unsigned short i = 0; i < amountButtons; i++)
		{
			if (i == selected)
			{
				continue;
			}

			buttons[i]->Update(static_cast<Vector2f>(Mouse::getPosition(*window)));



			if (buttons[i]->GetButtonState() == PRESSED)
			{
				selected = i;
			}
		}
	}

	void Render(RenderTarget* target)
	{
		target->draw(shape);

		for (short i = 0; i < amountButtons; i++)
		{
			buttons[i]->Render(target);
		}
	}

private:
	RectangleShape shape;
	Texture* texture;

	Font* font;
	Text text;
	String string;

	Color idleColor, hoverColor, activeColor;

	Button** buttons;
	unsigned short amountButtons;
	short selected;

	void SetTextPosition()
	{
		text.setPosition(
			shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - text.getGlobalBounds().width / 2.f,
			shape.getPosition().y + text.getCharacterSize());
	}
};

//Bazowy stan gry w gui.h p�ki co
class State
{
public:
	State() {}
	virtual ~State() {}
	virtual void AddLetter(wchar_t s) {}
	virtual State* IsStateChanged()
	{
		return nullptr;
	}
	virtual void Update(RenderWindow* window, Time* elapsed)
	{}
	virtual void Render(RenderTarget* target)
	{}
private:

};

