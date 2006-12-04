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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pglistboxitem.cpp,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#include "gfx.h"
#include "common.h"
#include "pglistboxitem.h"
#include "pglistbox.h"

PG_ListBoxItem::PG_ListBoxItem(PG_Widget* parent, int height, const char* text, SDL_Surface* icon, void* userdata, const char* style) : PG_ListBoxBaseItem(parent, height, userdata) {
	
	for(int i=0; i<3; i++) {
		my_background[i] = NULL;
		my_bkmode[i] = BKMODE_TILE;
		my_blend[i] = 0;
		my_gradient[i] = NULL;
	}

	SetFontColor( PG_Color(0xc0, 0xc0, 0xc0) );
	SetText(text);
//	LoadThemeStyle(style, "ListBoxItem");

	my_srfHover = NULL;
	my_srfSelected = NULL;
	
	my_srfIcon = icon;
}

PG_ListBoxItem::~PG_ListBoxItem() {
	SDL_FreeSurface(my_srfHover);
	SDL_FreeSurface(my_srfSelected);
}

void PG_ListBoxItem::eventSizeWidget(Uint16 w, Uint16 h) {
	SDL_FreeSurface(my_srfHover);
	SDL_FreeSurface(my_srfSelected);

	// reset surface pointers, will be regenerated on next blit
	my_srfHover = NULL;
	my_srfSelected = NULL;
}

void PG_ListBoxItem::eventBlit(SDL_Surface* srf, const PG_Rect& src, const PG_Rect& dst) {

	if((dst.my_width == 0) || (dst.my_height == 0)) {
		return;
	}

	if (NULL == my_srfHover)
	{
		my_srfHover= SDL_CreateRGBSurface( SDL_HWSURFACE, my_width, my_height,
			gamescreen->format->BitsPerPixel,
			gamescreen->format->Rmask, gamescreen->format->Gmask, gamescreen->format->Bmask, gamescreen->format->Amask
		);
		SDL_FillRect(my_srfHover, NULL, SDL_MapRGB(my_srfHover->format, 60, 60, 60));
		SDL_SetAlpha(my_srfHover, SDL_SRCALPHA, 128);
	}

	if (NULL == my_srfSelected)
	{
		my_srfSelected= SDL_CreateRGBSurface( SDL_HWSURFACE, my_width, my_height,
			gamescreen->format->BitsPerPixel,
			gamescreen->format->Rmask, gamescreen->format->Gmask, gamescreen->format->Bmask, gamescreen->format->Amask
		);
		SDL_FillRect(my_srfSelected, NULL, C_BLUE);
		SDL_SetAlpha(my_srfSelected, SDL_SRCALPHA, 128);
	}
	/*
	if(my_srfHover == NULL) {
		my_srfHover = PG_ThemeWidget::CreateThemedSurface(
						  PG_Rect(0, 0, my_width, my_height),
						  my_gradient[2],
						  my_background[2],
						  my_bkmode[2],
						  my_blend[2]);
	}
	
	if(my_srfSelected == NULL) {
		my_srfSelected = PG_ThemeWidget::CreateThemedSurface(
							 PG_Rect(0, 0, my_width, my_height),
							 my_gradient[1],
							 my_background[1],
							 my_bkmode[1],
							 my_blend[1]);
	}
	*/

	if(my_selected) {
		RedrawBackground(&dst);
		SetFontColor( PG_Color(0xf0, 0xf0, 0xf0) );
		PG_Widget::eventBlit(my_srfSelected, src, dst);
	}
	else if(my_hover) {
		RedrawBackground(&dst);
		SetFontColor( PG_Color(0xf0, 0xf0, 0x30) );
		PG_Widget::eventBlit(my_srfHover, src, dst);
	}
	else
	{
		SetFontColor( PG_Color(0xc0, 0xc0, 0xc0) );
	}

	PG_Label::eventBlit(NULL, src, dst);
}
/*
void PG_ListBoxItem::LoadThemeStyle(const char* widgettype, const char* objectname) {
	static char prop[80];
	PG_Theme* t = PG_Application::GetTheme();

	for(int i=0; i<3; i++) {
		sprintf(prop, "background%i", i);
		my_background[i] = t->FindSurface(widgettype, objectname, prop);

		sprintf(prop, "blend%i", i);
		t->GetProperty(widgettype, objectname, prop, my_blend[i]);

		sprintf(prop, "backmode%i", i);
		t->GetProperty(widgettype, objectname, prop, my_bkmode[i]);

		sprintf(prop, "gradient%i", i);
		PG_Gradient* g = t->FindGradient(widgettype, objectname, prop);

		if(g) {
			my_gradient[i] = g;
		}

	}
	
	PG_Color fontcolor(0xFFFFFF);
	t->GetColor(widgettype, objectname, "textcolor", fontcolor);
	SetFontColor(fontcolor);
}
*/