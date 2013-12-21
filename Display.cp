/*
 *  Library.cp
 *  Library
 *
 *  Created by Filipe Silvestrim on 9/22/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

/*************** NORMAL INCLUDES ****************/
#include <locale>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;


/*************** MUGE INCLUDE ****************/
#include <MuGE/Display.h>
#include <MuGE/StringUtil.h>
#include <MuGE/Log.h>
#include <Muge/Kernel.h>
using namespace muge::core::kernel;
using namespace muge::utils::debug;
using namespace muge::utils::str;
using namespace muge::plugins::display;
using namespace muge::plugins::event;


/*************** IRRLICHT INCLUDES ****************/
#include <Irrlicht/Irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


/*************** INTERNAL INCLUDES ****************/
#include "IrrGlobalEventReceiver.h"


/*************** VARIABLES ****************/
int lastFPS			= -1;

IrrlichtDevice			*device;
IVideoDriver			*driver;
ISceneManager			*smgr;
IGUIEnvironment			*guienv;
IrrGlobalEventReceiver	receiver;


//listas
ICameraSceneNode	*camera		= 0;	
ITexture			*texture	= 0;
IGUIFont			*guiFont;


/*************** INTERNAL METHODS ****************/
void addCamera()
{
	//Camera
	//fpsCamera = smgr->addCameraSceneNodeFPS();
	
	// let the cube rotate and set some light settings
	//scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(vector3df(0.3f, 0.3f,0));
	
	// add fixed camera
	camera = smgr->addCameraSceneNode(0, vector3df(0,0,400), vector3df(0,0,0));	
}

void setFont()
{	
	//Carrega a fonte através do bitmap font
	IGUIFont *font = guienv->getFont("/Users/filipesilvestrim/Irrlicht1.5.1/media/fontlucida.png");
	
	//Pega a skin usada no IGUIEnvironment
	IGUISkin* skin = guienv->getSkin(); 
	//Se carregou a font
	if(font)
	{
		//Define a fonte na skin
		skin->setFont(font);
	}
	
	guiFont = device->getGUIEnvironment()->getBuiltInFont();
}


float getPosX(const char *id)
{
	ISceneNode * target;
	vector3df v = target->getPosition();
	return v.X;
}

float getPosY(const char *id)
{
	ISceneNode * target;
	vector3df v = target->getPosition();
	return v.X;
}

float getPosZ(const char *id)
{
	ISceneNode * target;
	vector3df v = target->getPosition();
	return v.X;
}

void setPosition(float x = NULL, float y = NULL, float z = NULL)
{
	ISceneNode * target;
	vector3df v = target->getPosition();
	
	if(x)		v.X = x;
	else if(y)	v.Y = y;
	else if(z)	v.Z = z;
	
	target->setPosition(v);
}

void drawPrimitive2D(int type, float x, float y)
{
	driver->draw2DRectangle(SColor(255,255,255,255), rect<s32>(x - 50, y - 50, x + 50, y + 50));
	//driver->draw2DPolygon(position2d<s32>(x, y), f32 radius, SColor(255,255,255,255), s32 vertices);
	//driver->draw2DLine(position2d<s32>(x, y), position2d<s32>(x, y), SColor(255,255,255,255));
}

void drawText(const char *text)
{
	stringw t = stringw(text);
	guiFont->draw(t.c_str(),
				  rect<s32>(130,10,300,10),
				  SColor(255,255,255,255));
}

void showFPS()
{
	int fps = driver->getFPS();
	if (lastFPS != fps)
	{
		stringw str = L"MuGEPluginIrrlicht";
		str += " FPS:";
		str += fps;
		
		device->setWindowCaption(str.c_str());
		lastFPS = fps;
	}
}


/*************** PLUGIN METHODS ****************/
void Display::init()
{
	Log::print("INIT");
}

void Display::setup(int screenW, int screenH, const char *title)
{	
	device = createDevice( video::EDT_OPENGL, dimension2d<s32>(screenW, screenH), 16, false, false, false, &receiver);
	device->setWindowCaption(StringUtil::widen(title).c_str());
	
	receiver.init();
	Kernel::getInstance()->receiver = (IrrGlobalEventReceiver*) &receiver;
}

void Display::config()
{
	driver		=	device->getVideoDriver();
	smgr		=	device->getSceneManager();
	guienv		=	device->getGUIEnvironment();
	
	//Luzes
	smgr->addLightSceneNode(0, vector3df(-15,5,-105), video::SColorf(1.0f, 1.0f, 1.0f));
	smgr->setAmbientLight(video::SColor(0,60,60,60));
	
}

void Display::render()
{
	device->run();
	
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, 0);
		
		////////
		if (texture)
		{
			driver->setRenderTarget(texture, true, true, video::SColor(0,0,0,255));
			driver->setRenderTarget(0, true, true, 0);
		}
		//drawPrimitives2D();
		
		
		smgr->drawAll();	
		guienv->drawAll();
		driver->endScene();
	}
	
	showFPS();
}

void Display::shutDown()
{
	device->drop();
};


/*************** PLUGIN METHODS CUSTOM ***************/
void *Display::addDisplay(const char *id)
{
	//(DIGEOVisual *)DIGEOManager::getInstance()->getElement(id)
	
	ISceneNode *visualNode;
	
	//create node
	visualNode = smgr->addCubeSceneNode(20);	
	visualNode->setPosition(vector3df(-200,0,10));
	visualNode->setMaterialFlag(video::EMF_LIGHTING, true);
	
	//texture
	texture = driver->addRenderTargetTexture(dimension2d<s32>(256,256), "RTT1");
	visualNode->setMaterialTexture(0, texture);
	
	return visualNode;
}
