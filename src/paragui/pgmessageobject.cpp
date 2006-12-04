/*
    ParaGUI - crossplatform widgetset
    Copyright (C) 2000,2001,2002  Alexander Pipelka
 
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
 
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
 
    You should have received a copy of the GNU Library General Public
    License along with this library; if not, write to the Free
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 
    Alexander Pipelka
    pipelka@teleweb.at
 
    Last Update:      $Author: upi $
    Update Date:      $Date: 2006/12/04 09:34:33 $
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pgmessageobject.cpp,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#include "pgmessageobject.h"
//#include "pgapplication.h"
#include "pgwidget.h"
//#include "pglog.h"

#include <iostream>
#include <algorithm>

// static variables for message processing
//vector<PG_MessageObject*> PG_MessageObject::objectList;
PG_MessageObject* PG_MessageObject::captureObject = NULL;
PG_MessageObject* PG_MessageObject::inputFocusObject = NULL;
PG_Widget* PG_MessageObject::lastwidget = NULL;

/** constructor */

PG_MessageObject::PG_MessageObject() {

	// init vars
	my_canReceiveMessages = true;
	my_oldCapture = NULL;
	my_oldFocus = NULL;

	//objectList.push_back(this);
}


/**  destructor */

PG_MessageObject::~PG_MessageObject() {

	//RemoveObject(this);

	//PG_UnregisterEventObject(this);

	if (inputFocusObject == this) {
		inputFocusObject = NULL;
	}

	if (lastwidget == this) {
		lastwidget = NULL;
	}

	if (captureObject == this) {
		captureObject = NULL;
	}
}


bool PG_MessageObject::HandleEvent(SDL_Event *pEvent) // static
{
	if((pEvent->type != SDL_USEREVENT) && (pEvent->type != SDL_VIDEORESIZE)) {
		if(captureObject) {
			return captureObject->ProcessEvent(pEvent);
		}
	}

	PG_Widget* widget = NULL;
	
	switch(pEvent->type) 
	{

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			if(inputFocusObject) {
				// first send it to the focus object
				if(inputFocusObject->ProcessEvent(pEvent)) {
					return true;
				}
				// if the focus object doesn't respond -> pump it into the queue
			}
			break;

		case SDL_MOUSEMOTION:
			widget = PG_Widget::FindWidgetFromPos(pEvent->motion.x, pEvent->motion.y);

			if(lastwidget && (lastwidget != widget)) {
				lastwidget->eventMouseLeave();
				lastwidget = NULL;
			}

			if(widget) {
				lastwidget = widget;
				widget->ProcessEvent(pEvent);
				return true;
			}
			return true;

		case SDL_MOUSEBUTTONUP:
		case SDL_MOUSEBUTTONDOWN:
			widget = PG_Widget::FindWidgetFromPos(pEvent->button.x, pEvent->button.y);
			if(widget) {
				widget->ProcessEvent(pEvent);
				return true;
			}
			break;
	}

	return false;
}


/** enable / disable if object can receive messages */

void PG_MessageObject::EnableReceiver(bool enable) {
	my_canReceiveMessages = enable;
}


/** message dispatcher */

bool PG_MessageObject::ProcessEvent(const SDL_Event* event) {
	SDL_Event e;

	// check if we are able to process messages
	if(!my_canReceiveMessages) {
		return false;
	}

	if(event->type != SDL_USEREVENT) {
		if(captureObject != this)
			if(!AcceptEvent(event)) {
				return false;
			}
	}

	if((captureObject != NULL) && (captureObject != this)) {
		return false;
	}

	while(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_MOUSEMOTIONMASK) > 0);

	bool rc = false;

	// dispatch message
	switch(event->type) {
		case SDL_ACTIVEEVENT:
			rc = eventActive(&event->active) || sigActive(this, &event->active);
			break;

		case SDL_KEYDOWN:
			rc = eventKeyDown(&event->key) || sigKeyDown(this, &event->key);
			break;

		case SDL_KEYUP:
			rc = eventKeyUp(&event->key) || sigKeyUp(this, &event->key);;
			break;

		case SDL_MOUSEMOTION:
			rc = eventMouseMotion(&event->motion) || sigMouseMotion(this, &event->motion);
			break;

		case SDL_MOUSEBUTTONDOWN:
			rc = eventMouseButtonDown(&event->button) || sigMouseButtonDown(this, &event->button);
			break;

		case SDL_MOUSEBUTTONUP:
			rc = eventMouseButtonUp(&event->button) || sigMouseButtonUp(this, &event->button);
			break;

		case SDL_QUIT:
			rc = eventQuit(0, NULL, (unsigned long)&event->quit) || sigQuit(this);
			break;

		case SDL_SYSWMEVENT:
			rc = eventSysWM(&event->syswm) || sigSysWM(this, &event->syswm);
			break;

		case SDL_VIDEORESIZE:
			rc = eventResize(&event->resize) || sigVideoResize(this, &event->resize);
			break;

		default:
			rc = false;
			break;
	}

	return rc;
}


/** virtual message handlers */

bool PG_MessageObject::eventActive(const SDL_ActiveEvent* active) {
	return false;
}


bool PG_MessageObject::eventKeyDown(const SDL_KeyboardEvent* key) {
	return false;
}


bool PG_MessageObject::eventKeyUp(const SDL_KeyboardEvent* key) {
	return false;
}


bool PG_MessageObject::eventMouseMotion(const SDL_MouseMotionEvent* motion) {
	return false;
}


bool PG_MessageObject::eventMouseButtonDown(const SDL_MouseButtonEvent* button) {
	return false;
}


bool PG_MessageObject::eventMouseButtonUp(const SDL_MouseButtonEvent* button) {
	return false;
}


bool PG_MessageObject::eventQuit(int id, PG_MessageObject* widget, unsigned long data) {
	return false;
}

bool PG_MessageObject::eventQuitModal(int id, PG_MessageObject* widget, unsigned long data) {
	return false;
}


bool PG_MessageObject::eventSysWM(const SDL_SysWMEvent* syswm) {
	return false;
}


bool PG_MessageObject::eventResize(const SDL_ResizeEvent* event) {
	return false;
}

bool PG_MessageObject::AcceptEvent(const SDL_Event* event) {
	return true;				// PG_MessageObject accepts all events
}

/** capture handling (an object can capture all messages) */

PG_MessageObject* PG_MessageObject::SetCapture() {
	if(captureObject == this)
		return my_oldCapture;

	my_oldCapture = captureObject;
	captureObject = this;
	return my_oldCapture;
}


void PG_MessageObject::ReleaseCapture() {
	if(captureObject != this) {
		return;
	}

	captureObject = my_oldCapture;
}

PG_MessageObject* PG_MessageObject::SetInputFocus() {
	if(inputFocusObject == this)
		return my_oldFocus;

	my_oldFocus = inputFocusObject;

	if(my_oldFocus != NULL) {
		my_oldFocus->eventInputFocusLost(inputFocusObject);
	}

	inputFocusObject = this;
	return my_oldFocus;
}


/** */
void PG_MessageObject::eventInputFocusLost(PG_MessageObject* newfocus) {}


/** */
void PG_MessageObject::ReleaseInputFocus() {
	if(inputFocusObject != this) {
		return;
	}

	inputFocusObject = NULL;
}

/*SDL_Event PG_MessageObject::WaitEvent(Uint32 delay) {
	static SDL_Event event;

	while(SDL_PollEvent(&event) == 0) {
		//		eventIdle();
		if(delay > 0) {
			SDL_Delay(delay);
		}
	}

	return event;
}*/


/** Remove an object from the message queue  */

/*bool PG_MessageObject::RemoveObject(PG_MessageObject* obj) {
	vector<PG_MessageObject*>::iterator list = objectList.begin();

	// search the object
	list = find(objectList.begin(), objectList.end(), obj);

	// check if object was found
	if(list == objectList.end()) {
		return false;
	}

	// mark object for removal
	*list = NULL;
	
	return true;
}*/

PG_MessageObject* PG_MessageObject::GetCapture() {
	return captureObject;
}

bool PG_MessageObject::IsEnabled() {
	return my_canReceiveMessages;
}
