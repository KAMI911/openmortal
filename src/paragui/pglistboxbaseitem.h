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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pglistboxbaseitem.h,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#ifndef PG_LISTBOXBASEITEM_H
#define PG_LISTBOXBASEITEM_H

#include "pglabel.h"

/**
	@author Alexander Pipelka
	
	@short Base class for all items that can be inserted into a PG_ListBox
*/

class PG_ListBox;

class PG_ListBoxBaseItem : public PG_Label {
public:

	/** */
	PG_ListBoxBaseItem(PG_Widget* parent, Uint16 height, void* userdata = NULL);

	/** */
	~PG_ListBoxBaseItem();

	/** */
	void SetUserData(void* userdata);

	/** */
	void* GetUserData();

	/** */
	bool IsSelected();

	/** */
	void Select(bool select = true);

	/** */
	PG_ListBox* GetParent();

protected:

	void eventSizeWidget(Uint16 w, Uint16 h);

	/** */
	void eventMouseEnter();

	/** */
	void eventMouseLeave();

	/** */
	bool eventMouseButtonUp (const SDL_MouseButtonEvent* button);

	/** */
	void eventHide();

	void* my_userdata;
	bool my_selected;
	bool my_hover;
	//Uint16 my_itemheight;
};

#endif	// PG_LISTBOXBASEITEM_H
