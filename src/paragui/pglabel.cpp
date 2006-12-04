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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pglabel.cpp,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#include "pglabel.h"
#include "gfx.h"
#include "common.h"

PG_Label::PG_Label(PG_Widget* parent, const PG_Rect& r, const char* text, const char* style) :
PG_Widget(parent, r),
my_indent(0) {

	my_alignment = LEFT;
	my_srfIcon = NULL;
	my_freeicon = false;

	SetText(text);
//	LoadThemeStyle(style);
}

PG_Label::~PG_Label() {
	if(my_freeicon) {
		SDL_FreeSurface(my_srfIcon); // false
	}
}
/*
void PG_Label::LoadThemeStyle(const char* style) {
	if(strcmp(style, "Label") != 0) {
		PG_Label::LoadThemeStyle("Label", "Label");
	}
	PG_Label::LoadThemeStyle(style, "Label");
}

void PG_Label::LoadThemeStyle(const char* widgettype, const char* object) {
	PG_Theme* t = PG_Application::GetTheme();

	const char* s = t->FindString(widgettype, object, "label");

	if(s != NULL) {
		SetText(s);
	}

	t->GetAlignment(widgettype, object, "alignment", my_alignment);

	PG_Widget::LoadThemeStyle(widgettype, object);
}
*/
void PG_Label::eventBlit(SDL_Surface* srf, const PG_Rect& src, const PG_Rect& dst) {
	PG_Rect my_rectLabel;
	int xshift = my_indent;

	// should we blit an icon ?
	if(my_srfIcon != NULL) {
		xshift = my_srfIcon->w + my_indent;

		PG_Rect icon_rect(
		    my_xpos + my_indent,
		    my_ypos + (my_height - my_srfIcon->h)/2,
		    my_srfIcon->w,
		    my_srfIcon->h);

		PG_Rect icon_src;
		PG_Rect icon_dst;

		GetClipRects(icon_src, icon_dst, icon_rect);

		PG_Widget::eventBlit(my_srfIcon, icon_src, icon_dst);
	}

	Uint16 wl;
	Uint16 hl;

	GetTextSize(wl, hl);

	switch (my_alignment) {
		case LEFT:
			my_rectLabel.my_xpos = xshift;
			my_rectLabel.my_ypos = (my_height - hl) >> 1;
			break;

		case RIGHT:
			my_rectLabel.my_xpos = (my_width - wl);
			my_rectLabel.my_ypos = (my_height - hl) >> 1;
			break;

		case CENTER:
			my_rectLabel.my_xpos = (my_width - wl) >> 1;
			my_rectLabel.my_ypos = (my_height - hl) >> 1;
			break;
	}

	DrawText(my_rectLabel, my_text.c_str());
}

/**  */
void PG_Label::eventDraw(SDL_Surface* surface, const PG_Rect& rect) {}

void PG_Label::SetAlignment(TextAlign a) {
	my_alignment = a;
	Update();
}

SDL_Surface* PG_Label::SetIcon(SDL_Surface* icon) {

	if((icon != my_srfIcon)  && my_freeicon){
		SDL_FreeSurface(my_srfIcon);
	}

	my_srfIcon = icon;
	my_freeicon = false;
	Update();

	return my_srfIcon;
}

SDL_Surface* PG_Label::SetIcon(const char* filename) {
	if(my_freeicon){
		SDL_FreeSurface(my_srfIcon);
	}

	my_srfIcon = LoadImage(filename);
	my_freeicon = true;
	Update();

	return my_srfIcon;
}

SDL_Surface* PG_Label::GetIcon() {
	return my_srfIcon;
}

void PG_Label::SetIndent(Uint16 indent) {
	my_indent = indent;
}

Uint16 PG_Label::GetIndent() {
	return my_indent;
}

void PG_Label::SetSizeByText(int Width, int Height, const char *Text) {
	if (GetIcon() == NULL) {
		PG_Widget::SetSizeByText(Width, Height, Text);
		return;
	}

	Uint16 w,h;
	int baselineY;
	
	if (Text == NULL) {
		Text = my_text.c_str();
	}

	if (!PG_FontEngine::GetTextSize(Text, GetFont(), &w, &h, &baselineY)) {
		return;
	}

	if (GetIcon()->w > w) {
		my_width = GetIcon()->w + my_indent + Width;
	}
	else {
		my_width = w + GetIcon()->w + my_indent + Width;
	}
	my_height = omMAX(GetIcon()->h, h + baselineY) + Height + baselineY;
}
