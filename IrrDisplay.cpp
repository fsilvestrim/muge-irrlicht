/*
 *  Display.cpp
 *  MuGEPluginIrrlicht
 *
 *  Created by Filipe Silvestrim on 11/25/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <locale>
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
using namespace std;


#include <MuGE/StringUtil.h>
#include <MuGE/Log.h>
#include <Muge/Kernel.h>
#include <MuGE/Display.h>
#include <MuGE/DIGEOVisual.h>
#include <MuGE/DIGEOResource.h>
#include <MuGE/DIGEOCamera.h>
#include <MuGE/DIGEOGui.h>
using namespace muge::utils::debug;
using namespace muge::plugins::display;
using namespace muge::elements;


#include "IrrGlobalEventReceiver.h"
using namespace irr::core;
using namespace irr::scene;
using namespace irr::video;
using namespace irr::io;
using namespace irr::gui;


/*************** INTERNAL VARIABLES ****************/
IrrlichtDevice			*device;
IVideoDriver			*driver;
ISceneManager			*smgr;
IGUIEnvironment			*guienv;
IrrGlobalEventReceiver	receiver;


/*************** PLUGIN METHODS ****************/
void Display::init() {}

void Display::setup(int screenW, int screenH, const char *title)
{
	device = createDevice( video::EDT_OPENGL, dimension2d<s32>(screenW, screenH), 32, false, true, true, &receiver);
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
	smgr->setAmbientLight(SColorf(0.6,0.6,0.6,1));
	smgr->addLightSceneNode( 0, vector3df(0,400,-200), SColorf(1.0f,1.0f,1.0f), 1.0f, 1 );
	
	//fonte
	IGUISkin* skin = guienv->getSkin();
	IGUIFont* font = guienv->getFont("../../res/graphics/bigfont.png");
	if (font) {
		skin->setFont(font);
	}
	skin->setFont(guienv->getBuiltInFont(), EGDF_TOOLTIP);
}

void Display::render()
{
	device->run();
	
	if (device->isWindowActive())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));
		
		smgr->drawAll();	
		guienv->drawAll();
		driver->endScene();
	}
}

void Display::shutDown()
{
	device->drop();
};


/*************** PLUGIN METHODS CUSTOM ***************/

/*
 * Camera
 */
void *Display::addCamera(DIGEOElement *obj)
{
	//DIGEOCamera			*elem		= (DIGEOCamera*)obj;
	ICameraSceneNode	*cameraNode = NULL;
	
	// add fixed camera
	cameraNode = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0));	
	
	return cameraNode;
}

void Display::updateCamera(DIGEOElement *obj)
{
	ICameraSceneNode *cameraNode	= ((DIGEOCamera*)obj)->getDisplayData<ICameraSceneNode>();

	vector3df nodePosition	= cameraNode->getPosition();
	vector3df nodeTarget	= cameraNode->getTarget();
	
	if (nodePosition.X != obj->getProperty(DIGEOElement::POSITION, DIGEOElement::X)
		|| nodePosition.Y != obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Y)
		|| nodePosition.Z != obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Z))
	{
		if (((DIGEOCamera*)obj)->camType == DIGEOCamera::FIXED) {
			nodePosition.X			= obj->getProperty(DIGEOElement::POSITION, DIGEOElement::X);
			nodePosition.Y			= obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Y);
			nodePosition.Z			= obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Z);
			
			cameraNode->setPosition(nodePosition);
		}
		else if (((DIGEOCamera*)obj)->camType == DIGEOCamera::TRACKED) {
			if (obj->getTarget() != NULL) {

				nodePosition.X			= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::X) + ((DIGEOCamera*)obj)->distance[0];
				nodePosition.Y			= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::Y) + ((DIGEOCamera*)obj)->distance[1];
				nodePosition.Z			= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::Z) + ((DIGEOCamera*)obj)->distance[2];
				
				cameraNode->setPosition(nodePosition);
			}
		}
	}
	
	if (obj->getTarget() != NULL) {
		nodeTarget.X			= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::X);
		nodeTarget.Y			= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::Y);
		nodeTarget.Z			= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::Z);
		
		cameraNode->setTarget(nodeTarget);
	}
}


/*
 * GUI
 */
void *Display::addGUI(DIGEOElement *obj)
{
	DIGEOGui		*elem		= (DIGEOGui*)obj;
	IGUIStaticText 	*visualNode = NULL;
	
	float			left		= elem->getProperty(DIGEOElement::POSITION, DIGEOElement::X);
	float			top			= elem->getProperty(DIGEOElement::POSITION, DIGEOElement::Y);
	float			width		= elem->getProperty(DIGEOElement::SIZE, DIGEOElement::X);
	float			height		= elem->getProperty(DIGEOElement::SIZE, DIGEOElement::X);
	
	visualNode = guienv->addStaticText(StringUtil::widen(elem->getPropertySingle(DIGEOElement::VALUE)).c_str(),rect<s32>(left,top,left+width,top+height), false);  
	
	return visualNode;
}
 
void Display::updateGUI(DIGEOElement *obj)
{
	IGUIStaticText *visualNode	= ((DIGEOGui*)obj)->getDisplayData<IGUIStaticText>();
 
	if (visualNode) {
		visualNode->setText(StringUtil::widen(((DIGEOGui*)obj)->getPropertySingle(DIGEOElement::VALUE)).c_str());
	}
}


/*
 * Display
 */
void *Display::addDisplay(DIGEOElement *obj)
{
	DIGEOVisual		*elem		= (DIGEOVisual*)obj;
	ISceneNode		*visualNode = NULL;
	
	//create node
	if (elem->getDisplay()->displayType == DIGEOResource::BOX)
		visualNode = smgr->addCubeSceneNode(1);	
	else if (elem->getDisplay()->displayType == DIGEOResource::SPHERE)
		visualNode = smgr->addSphereSceneNode(1);
	else if (elem->getDisplay()->displayType == DIGEOResource::MESH)
	{
		IAnimatedMesh* mesh = smgr->getMesh(elem->getDisplay()->getSrc());
		visualNode = smgr->addAnimatedMeshSceneNode( mesh );
	}
	
	if (visualNode) {
		visualNode->setScale(vector3df(elem->getDisplay()->getProperty(DIGEOElement::SCALE, DIGEOElement::X), elem->getDisplay()->getProperty(DIGEOElement::SCALE, DIGEOElement::Y), elem->getDisplay()->getProperty(DIGEOElement::SCALE, DIGEOElement::Z)));
		visualNode->setPosition(vector3df(elem->getProperty(DIGEOElement::POSITION, DIGEOElement::X),elem->getProperty(DIGEOElement::POSITION, DIGEOElement::Y),elem->getProperty(DIGEOElement::POSITION, DIGEOElement::Z)));
		
		//Texture
		if (elem->getDisplay()->getTexture()) {
			visualNode->setMaterialFlag(EMF_LIGHTING, false);
			visualNode->setMaterialTexture( 0, driver->getTexture(elem->getDisplay()->getTexture()) );
		}
		else {
			visualNode->setMaterialFlag(EMF_LIGHTING, true);
		}

	}
	
	return visualNode;
}

void Display::updateDisplay(DIGEOElement *obj)
{
	ISceneNode *visualNode	= ((DIGEOVisual*)obj)->getDisplayData<ISceneNode>();
	
	if (visualNode) {
		vector3df nodePosition	= visualNode->getPosition();
		if (nodePosition.X != obj->getProperty(DIGEOElement::POSITION, DIGEOElement::X)
			|| nodePosition.Y != obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Y)
			|| nodePosition.Z != obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Z))
		{
			nodePosition.X			= obj->getProperty(DIGEOElement::POSITION, DIGEOElement::X);
			nodePosition.Y			= obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Y);
			nodePosition.Z			= obj->getProperty(DIGEOElement::POSITION, DIGEOElement::Z);
			visualNode->setPosition(nodePosition);
		}
		
		vector3df nodeRotation	= visualNode->getRotation();
		if (nodeRotation.X != obj->getProperty(DIGEOElement::ROTATION, DIGEOElement::X)
			|| nodeRotation.Y != obj->getProperty(DIGEOElement::ROTATION, DIGEOElement::Y)
			|| nodeRotation.Z != obj->getProperty(DIGEOElement::ROTATION, DIGEOElement::Z))
		{
			nodeRotation.X			= obj->getProperty(DIGEOElement::ROTATION, DIGEOElement::X);
			nodeRotation.Y			= obj->getProperty(DIGEOElement::ROTATION, DIGEOElement::Y);
			nodeRotation.Z			= obj->getProperty(DIGEOElement::ROTATION, DIGEOElement::Z);
			visualNode->setRotation(nodeRotation);
		}
	}
}


/*************** EXTRAS ***************/

/*
 //spherical
 
 float x		= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::X);
 float y		= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::Y);
 float z		= obj->getTarget()->getProperty(DIGEOElement::POSITION, DIGEOElement::Z);
 
 float theta = acos(z/sqrt(x*x + y*y + z*z));
 float phi	= atan2(y, x);
 
 nodePosition.X			= ((DIGEOCamera*)obj)->distance * sin(phi) * cos(theta);
 nodePosition.Y			= ((DIGEOCamera*)obj)->distance * sin(phi) * sin(theta);
 nodePosition.Z			= ((DIGEOCamera*)obj)->distance * cos(phi);
 */
