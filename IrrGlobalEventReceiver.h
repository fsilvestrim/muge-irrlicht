/*
 *  IrrGlobalEventReceiver.h
 *  MuGEPluginIrrlicht
 *
 *  Created by Filipe Silvestrim on 10/13/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __IrrGlobalEventReceiver_h__
#define __IrrGlobalEventReceiver_h__

#include <Irrlicht/Irrlicht.h>
#include <MuGE/GlobalEventReceiver.h>

using namespace muge::plugins::event;
using namespace irr;

class IrrGlobalEventReceiver : public GlobalEventReceiver, public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent & event)
	{
		bool eventprocessed = false;
		
		//////////////////////////////
		// Keyboard Input Event
		//////////////////////////////
		if (event.EventType == EET_KEY_INPUT_EVENT)
		{
			if (processState == STARTED)
			{
				// if key is Pressed Down
				if (event.KeyInput.PressedDown == true)
				{
					// If key was not down before
					if (keyState[event.KeyInput.Key] != DOWN)
					{
						keyState[event.KeyInput.Key] = PRESSED; // Set to Pressed
					}
					else
					{
						// if key was down before
						keyState[event.KeyInput.Key] = DOWN; // Set to Down
					}
				}
				else
				{
					
					// if the key is down
					if (keyState[event.KeyInput.Key] != UP)
					{
						keyState[event.KeyInput.Key] = RELEASED; // Set to Released
					}
				}
			}
			
			
			eventprocessed = true;
		}
		
		//////////////////////////////
		// Mouse Input Event
		//////////////////////////////
		
		if (event.EventType == EET_MOUSE_INPUT_EVENT)
		{
			if (processState == STARTED)
			{
				//Mouse changed position
				if (event.MouseInput.Event == EMIE_MOUSE_MOVED)
				{
					mouse.Y = event.MouseInput.Y;
					mouse.X = event.MouseInput.X;
				}
				
				//Wheel moved.
				if (event.MouseInput.Event == EMIE_MOUSE_WHEEL)
				{
					mouse.wheel += event.MouseInput.Wheel;
				}
				
				//Left Mouse Button Pressed
				if (event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
				{
					//
					if (mouseButtonState[0] == UP || mouseButtonState[0] == RELEASED)
					{
						mouseButtonState[0] = PRESSED;
					}
					else
					{
						mouseButtonState[0] = DOWN;
					}
				}
				
				//Left Mouse Button Rleased
				if (event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
				{
					//
					if (mouseButtonState[0] != UP)
					{
						mouseButtonState[0] = RELEASED;
					}
				}
				
				//Middle Mouse Button Pressed
				if (event.MouseInput.Event == EMIE_MMOUSE_PRESSED_DOWN)
				{
					//
					if (mouseButtonState[1] == UP || mouseButtonState[1] == RELEASED)
					{
						mouseButtonState[1] = PRESSED;
					}
					else
					{
						mouseButtonState[1] = DOWN;
					}
				}
				
				//Middle Mouse Button Rleased
				if (event.MouseInput.Event == EMIE_MMOUSE_LEFT_UP)
				{
					//
					if (mouseButtonState[1] != UP)
					{
						mouseButtonState[1] = RELEASED;
					}
				}
				
				//Right Mouse Button Pressed
				if (event.MouseInput.Event == EMIE_RMOUSE_PRESSED_DOWN)
				{
					//
					if (mouseButtonState[2] == UP || mouseButtonState[2] == RELEASED)
					{
						mouseButtonState[2] = PRESSED;
					}
					else
					{
						mouseButtonState[2] = DOWN;
					}
				}
				
				//Right Mouse Button Rleased
				if (event.MouseInput.Event == EMIE_RMOUSE_LEFT_UP)
				{
					//
					if (mouseButtonState[2] != UP)
					{
						mouseButtonState[2] = RELEASED;
					}
				}
			}
			
			
			eventprocessed = true;
		}
		
		return eventprocessed;
	}
};

#endif

