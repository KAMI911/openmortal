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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pglabel.h,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#ifndef PG_LABEL_H
#define PG_LABEL_H

#include "pgwidget.h"

/**
	@author Alexander Pipelka
 
	@short Text label.
 
	A text label (unmodifiable by end user).  Has attributes for font style,
	alignment, etc.
*/

class PG_Label : public PG_Widget {
public:

	// Text alignment
	typedef enum {
		LEFT,
		CENTER,
		RIGHT
	} TextAlign;
	
	/** Only constructor
	@param parent			Parent widget.
	@param r				Rectangle to draw in, relative to parent.
	@param text				Text displayed by label.
	@param style			initial widget style (from xml theme)
	*/
	PG_Label(PG_Widget* parent, const PG_Rect& r = PG_Rect::null, const char* text = NULL, const char* style="Label");

	/** Only destructor */
	~PG_Label();

	/** */
//	void LoadThemeStyle(const char* widgettype);

	/** */
//	void LoadThemeStyle(const char* widgettype, const char* objectname);

	/** Sets text alignment */
	void SetAlignment(TextAlign a);

	/** */
	SDL_Surface* SetIcon(const char* filename);

	/** */
	SDL_Surface* SetIcon(SDL_Surface* icon);

	SDL_Surface* GetIcon();
	
	/**
	Set the text indentation
	@param	indent		number of pixels for text indentation
	*/
	void SetIndent(Uint16 indent);

	/** Returns the text indentation */
	Uint16 GetIndent();

	void SetSizeByText(int Width = 0, int Height = 0, const char *Text = NULL);	

protected:

	/** Draw event handler
	@param surface	Surface to draw to.
	@param rect		Rectangle to draw in, relative to surface.
	*/
	void eventDraw(SDL_Surface* surface, const PG_Rect& rect);

	/** */
	void eventBlit(SDL_Surface* surface, const PG_Rect& src, const PG_Rect& dst);

	SDL_Surface* my_srfIcon;

private:

	PG_Label(const PG_Label&);
	PG_Label& operator=(const PG_Label&);

	TextAlign my_alignment;
	Uint16 my_indent;
	bool my_freeicon;
};

#endif // PG_LABEL_H
