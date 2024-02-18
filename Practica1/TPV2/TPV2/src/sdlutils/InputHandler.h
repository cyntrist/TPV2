// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

#include <iostream>
#include <SDL.h>
#include <array>

#include "../utils/Singleton.h"

// Instead of a Singleton class, we could make it part of
// SDLUtils as well.

class InputHandler : public Singleton<InputHandler>
{
	friend Singleton<InputHandler>;

public:
	enum MOUSEBUTTON : uint8_t
	{
		LEFT = 0,
		MIDDLE,
		RIGHT,
		_LAST_MOUSEBUTTON_VALUE
	};

	~InputHandler() override
	{
	}

	// clear the state
	void clearState()
	{
		isCloseWindoEvent_ = false;
		isKeyDownEvent_ = false;
		isKeyUpEvent_ = false;

		isMouseButtonUpEvent_ = false;
		isMouseButtonDownEvent_ = false;

		isMouseMotionEvent_ = false;
	}

	// update the state with a new event
	void update(const SDL_Event& event)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			onKeyDown(event);
			break;
		case SDL_KEYUP:
			onKeyUp(event);
			break;
		case SDL_MOUSEMOTION:
			onMouseMotion(event);
			break;
		case SDL_MOUSEBUTTONDOWN:
			onMouseButtonDown(event);
			break;
		case SDL_MOUSEBUTTONUP:
			onMouseButtonUp(event);
			break;
		case SDL_WINDOWEVENT:
			handleWindowEvent(event);
			break;
		default:
			break;
		}
	}

	// refresh
	void refresh()
	{
		SDL_Event event;

		clearState();
		while (SDL_PollEvent(&event))
			update(event);
	}

	// close window event
	bool closeWindowEvent()
	{
		return isCloseWindoEvent_;
	}

	// keyboard
	bool keyDownEvent()
	{
		return isKeyDownEvent_;
	}

	bool keyUpEvent()
	{
		return isKeyUpEvent_;
	}

	bool isKeyDown(SDL_Scancode key)
	{
		return kbState_[key] == 1;
	}

	bool isKeyDown(SDL_Keycode key)
	{
		return isKeyDown(SDL_GetScancodeFromKey(key));
	}

	bool isKeyUp(SDL_Scancode key)
	{
		return kbState_[key] == 0;
	}

	bool isKeyUp(SDL_Keycode key)
	{
		return isKeyUp(SDL_GetScancodeFromKey(key));
	}

	// mouse
	bool mouseMotionEvent()
	{
		return isMouseMotionEvent_;
	}

	bool mouseButtonEvent()
	{
		return isMouseButtonUpEvent_ || isMouseButtonDownEvent_;
	}

	bool mouseButtonUpEvent()
	{
		return isMouseButtonUpEvent_;
	}

	bool mouseButtonDownEvent()
	{
		return isMouseButtonDownEvent_;
	}

	const std::pair<Sint32, Sint32>& getMousePos()
	{
		return mousePos_;
	}

	int getMouseButtonState(uint8_t b)
	{
		assert(b < _LAST_MOUSEBUTTON_VALUE);
		return mbState_[b];
	}

	// TODO add support for Joystick, see Chapter 4 of
	// the book 'SDL Game Development'

private:
	InputHandler()
	{
		kbState_ = SDL_GetKeyboardState(nullptr);
		clearState();
	}

	void onKeyDown(const SDL_Event&)
	{
		isKeyDownEvent_ = true;
	}

	void onKeyUp(const SDL_Event&)
	{
		isKeyUpEvent_ = true;
	}

	void onMouseMotion(const SDL_Event& event)
	{
		isMouseMotionEvent_ = true;
		mousePos_.first = event.motion.x;
		mousePos_.second = event.motion.y;
	}

	void onMouseButtonDown(const SDL_Event& event)
	{
		isMouseButtonDownEvent_ = true;
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = true;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = true;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = true;
			break;
		default:
			break;
		}
	}

	void onMouseButtonUp(const SDL_Event& event)
	{
		isMouseButtonUpEvent_ = true;
		switch (event.button.button)
		{
		case SDL_BUTTON_LEFT:
			mbState_[LEFT] = false;
			break;
		case SDL_BUTTON_MIDDLE:
			mbState_[MIDDLE] = false;
			break;
		case SDL_BUTTON_RIGHT:
			mbState_[RIGHT] = false;
			break;
		default:
			break;
		}
	}

	void handleWindowEvent(const SDL_Event& event)
	{
		switch (event.window.event)
		{
		case SDL_WINDOWEVENT_CLOSE:
			isCloseWindoEvent_ = true;
			break;
		default:
			break;
		}
	}

	bool isCloseWindoEvent_;
	bool isKeyUpEvent_;
	bool isKeyDownEvent_;
	bool isMouseMotionEvent_;
	bool isMouseButtonUpEvent_;
	bool isMouseButtonDownEvent_;
	std::pair<Sint32, Sint32> mousePos_;
	std::array<bool, 3> mbState_;
	const Uint8* kbState_;
};

// This macro defines a compact way for using the singleton InputHandler, instead of
// writing InputHandler::instance()->method() we write ih().method()
//
inline InputHandler& ih()
{
	return *InputHandler::instance();
}
