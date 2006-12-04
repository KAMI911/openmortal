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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pgmessageobject.h,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#ifndef PG_MESSAGEOBJECT_H
#define PG_MESSAGEOBJECT_H

#include "SDL.h"
#include "pgsignals.h"
#include <vector>

class PG_Widget;

/**
	@author Alexander Pipelka
	
	@short Base class of all objects (provides message handling).
 
	Provides a message pump and global handlers for all other PG_MessageObject instances.
*/

class PG_MessageObject : public virtual SigC::Object {

public:

	/**
	Signal type declaration
	**/
	template<class datatype = PG_Pointer> class SignalActive : public PG_Signal2<PG_MessageObject*, const SDL_ActiveEvent*, datatype> {};
	template<class datatype = PG_Pointer> class SignalKeyDown : public PG_Signal2<PG_MessageObject*, const SDL_KeyboardEvent*, datatype> {};
	template<class datatype = PG_Pointer> class SignalKeyUp : public PG_Signal2<PG_MessageObject*, const SDL_KeyboardEvent*, datatype> {};
	template<class datatype = PG_Pointer> class SignalMouseMotion : public PG_Signal2<PG_MessageObject*, const SDL_MouseMotionEvent*, datatype> {};
	template<class datatype = PG_Pointer> class SignalMouseButtonDown : public PG_Signal2<PG_MessageObject*, const SDL_MouseButtonEvent*, datatype> {};
	template<class datatype = PG_Pointer> class SignalMouseButtonUp : public PG_Signal2<PG_MessageObject*, const SDL_MouseButtonEvent*, datatype> {};
	template<class datatype = PG_Pointer> class SignalQuit : public PG_Signal1<PG_MessageObject*, datatype> {};
	template<class datatype = PG_Pointer> class SignalSysWM : public PG_Signal2<PG_MessageObject*, const SDL_SysWMEvent*, datatype> {};
	template<class datatype = PG_Pointer> class SignalVideoResize : public PG_Signal2<PG_MessageObject*, const SDL_ResizeEvent*, datatype> {};
	
	/**
	Creates a PG_MessageObject
	*/
	PG_MessageObject();

	/**
	Destroys a PG_MessageObject and removes it from the global object list.
	*/
	virtual ~PG_MessageObject();

	static bool HandleEvent(SDL_Event *pEvent);

	/**
	This function enables or disables receiving of SDL_Event messages.

	@param enable	Enable or disable the ability of receiving messages. When set to false
			no event handlers will be called for this object

	*/
	void EnableReceiver(bool enable);

	/**
	Set a message capture for this object. This object will receive all SDL_Event messages regardless
	if it is able to process them or not.
	*/
	PG_MessageObject* SetCapture();

	/**
	Releases a previous capture.
	*/
	void ReleaseCapture();

	/**
	Return the current capture object.
	*/
	PG_MessageObject* GetCapture();

	/**
	Set an inputfocus for this object
	@return	pointer to the object that had the inputfocus till now
	If the inputfocus is set all key events will be sent to this object
	*/
	PG_MessageObject* SetInputFocus();

	/**
	Release the inputfocus
	*/
	void ReleaseInputFocus();

	/**
	Check if the object can receive messages
	@return	true/false
	returns true when the object can receive messages or false if not.
	*/
	bool IsEnabled();

	/** */
	//static SDL_Event WaitEvent(Uint32 delay=0);

	/**
	Sends an event directly to an object.

	@param event SDL_Event message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool ProcessEvent(const SDL_Event* event);

	SignalActive<> sigActive;
	SignalKeyDown<> sigKeyDown;
	SignalKeyUp<> sigKeyUp;
	SignalMouseMotion<> sigMouseMotion;
	SignalMouseButtonDown<> sigMouseButtonDown;
	SignalMouseButtonUp<> sigMouseButtonUp;
	SignalSysWM<> sigSysWM;
	SignalVideoResize<> sigVideoResize;
	SignalQuit<> sigQuit;

protected:

	/**
	Overridable Eventhandler for the SDL_ActiveEvent message.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param active SDL_ActiveEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventActive(const SDL_ActiveEvent* active);

	/**
	Overridable Eventhandler for a SDL_KeyboardEvent message.
	This handler is called when a key changed it's state from unpressed to pressed.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param key SDL_KeyboardEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventKeyDown(const SDL_KeyboardEvent* key);

	/**
	Overridable Eventhandler for a SDL_KeyboardEvent message.
	This handler is called when a key changed it's state from pressed to unpressed.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param key SDL_KeyboardEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventKeyUp(const SDL_KeyboardEvent* key);

	/**
	Overridable Eventhandler for a SDL_MouseMotionEvent message.
	This handler is called when mouse movement is detected.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param motion SDL_MouseMotionEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventMouseMotion(const SDL_MouseMotionEvent* motion);

	/**
	Overridable Eventhandler for a SDL_MouseButtonEvent message.
	This handler is called when a mouse button is pressed.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param button SDL_MouseButtonEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventMouseButtonDown(const SDL_MouseButtonEvent* button);

	/**
	Overridable Eventhandler for a SDL_MouseButtonEvent message.
	This handler is called when a mouse button is released.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param button SDL_MouseButtonEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventMouseButtonUp(const SDL_MouseButtonEvent* button);

	/**
	Overridable Eventhandler for a SDL_QuitEvent message.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param id		id of the sending widget
	@param widget	pointer to the widget
	@param data	event specific data

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventQuit(int id, PG_MessageObject* widget, unsigned long data);

	/**
	Overridable Eventhandler for a MSG_QUITMODAL message.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param id		id of the sending widget
	@param widget	pointer to the widget
	@param data	event specific data

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventQuitModal(int id, PG_MessageObject* widget, unsigned long data);

	/**
	Overridable Eventhandler for a SDL_SysWMEvent message.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param syswm SDL_SysWMEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventSysWM(const SDL_SysWMEvent* syswm);

	/**
	Overridable Eventhandler for a SDL_ResizeEvent message.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param event SDL_ResizeEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool eventResize(const SDL_ResizeEvent* event);

	/**
	Overridable Eventhandler for a SDL_SysUserEvent message.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param event SDL_SysUserEvent message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	//virtual bool eventMessage(MSG_MESSAGE* msg);

	/** */
	virtual void eventInputFocusLost(PG_MessageObject* newfocus);

	/**
	Overridable message filter function. Derivated classes can filter special events.
	The default implementation returns 'false' which indicates that this message is not processed by this object.

	@param event SDL_Event message

	@return Notifies the message pump if this message is processed by this object or it should be routed to the next message receiver.
	*/
	virtual bool AcceptEvent(const SDL_Event* event);

	//static vector<PG_MessageObject*> objectList;

	static PG_MessageObject* captureObject;

private:

	PG_MessageObject(const PG_MessageObject&);
	PG_MessageObject& operator=(const PG_MessageObject&);

	//bool RemoveObject(PG_MessageObject* obj);

	static PG_MessageObject* inputFocusObject;
	static PG_Widget* lastwidget;

	PG_MessageObject* my_oldCapture;
	PG_MessageObject* my_oldFocus;

	bool my_canReceiveMessages;
};

#endif // PG_MESSAGEOBJECT_H
