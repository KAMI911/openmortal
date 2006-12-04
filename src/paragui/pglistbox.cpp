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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pglistbox.cpp,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#include "pglistbox.h"
#include "pglistboxbaseitem.h"
#include "pgscrollarea.h"
#include "common.h"

PG_ListBox::PG_ListBox(PG_Widget* parent, const PG_Rect& r, const char* style) :
	PG_WidgetList(parent, r, style),
	m_poCurrentItem(NULL), my_alignment(PG_Label::LEFT) 
{
	my_multiselect = false;
	my_indent = 0;
}

PG_ListBox::~PG_ListBox()
{
}

void PG_ListBox::AddChild(PG_Widget* item) 
{
	if(item == NULL) 
	{
        return;
	}

	item->SizeWidget(Width(), item->Height());	

	PG_WidgetList::AddChild(item);
}

void PG_ListBox::SetMultiSelect(bool multi) 
{
	my_multiselect = multi;
}

bool PG_ListBox::GetMultiSelect() 
{
	return my_multiselect;
}

// =======================================================================
// Interface for PG_ListBoxBaseItems
// =======================================================================

void PG_ListBox::OnItemSelected(PG_ListBoxBaseItem* item, bool select) 
{
	if(item == NULL) 
	{
		return;
	}

	if(!my_multiselect) 
	{
		if((m_poCurrentItem != NULL) && (m_poCurrentItem != item))
		{
			m_poCurrentItem->Select(false);
			m_poCurrentItem->Update();
		}

		m_poCurrentItem = item;
		m_poCurrentItem->Update();
	}

	if (select)
	{
		sigSelectItem(item);
		eventSelectItem(item);
		EnsureVisible(item);
	}
}

void PG_ListBox::OnItemDeleted(PG_ListBoxBaseItem *a_poItem)
{
	if (m_poCurrentItem == a_poItem)
	{
		m_poCurrentItem = NULL;
	}
}

void PG_ListBox::EnsureVisible(PG_ListBoxBaseItem *a_poItem)
{
	my_scrollarea->EnsureVisible(a_poItem);
	CheckScrollBars();
}

bool PG_ListBox::eventSelectItem(PG_ListBoxBaseItem* item)
{
	return false;
}

bool PG_ListBox::eventKeyDown(const SDL_KeyboardEvent* key) 
{
	switch(key->keysym.sym) 
	{
	case SDLK_UP:
		SelectPrevItem();
		return true;
	case SDLK_DOWN:
		SelectNextItem();
		return true;
	case SDLK_HOME:
		SelectFirstItem();
		return true;
	}

	return false;
}

bool PG_ListBox::eventMouseButtonUp(const SDL_MouseButtonEvent* button) 
{
	SetInputFocus();
	return true;
}

bool PG_ListBox::eventMouseButtonDown(const SDL_MouseButtonEvent* button) 
{
	return true;
}

bool PG_ListBox::eventMouseMotion(const SDL_MouseMotionEvent* motion) 
{
	return true;
}

void PG_ListBox::RemoveAll()
{
	m_poCurrentItem = NULL;
	my_scrollarea->RemoveAll();
}

void PG_ListBox::DeleteAll()
{
	m_poCurrentItem = NULL;
	my_scrollarea->DeleteAll();
	my_scrollarea->ScrollTo(0,0);
	Update();
}

PG_ListBoxBaseItem* PG_ListBox::GetCurrentItem()
{
	return m_poCurrentItem;
}

void PG_ListBox::SetIndent(Uint16 indent) {
	my_indent = indent;
	PG_RectList* list = my_scrollarea->GetChildList();
	if(list == NULL) {
		return;
	}

	for(PG_Widget* w = list->first(); w != NULL; w = w->next()) {
		PG_ListBoxBaseItem* item = static_cast<PG_ListBoxBaseItem*>(w);
		item->SetIndent(my_indent);
	}
	Update();
}

void PG_ListBox::SelectFirstItem() 
{
	PG_ListBoxBaseItem* item = (PG_ListBoxBaseItem*)FindWidget(0);
	
	if(item == NULL) 
	{
		return;
	}

	item->Select();
}

void PG_ListBox::SelectNextItem() 
{
	if (m_poCurrentItem)
	{
		int index = FindIndex(m_poCurrentItem);
		index = omMAX(0, index+1);
		PG_ListBoxBaseItem *item = (PG_ListBoxBaseItem *)FindWidget(index);
		if (item)
		{
			item->Select();
		}
	}
}

void PG_ListBox::SelectPrevItem() 
{
	if (m_poCurrentItem)
	{
		int index = FindIndex(m_poCurrentItem);
		if (index > 0)
		{
			PG_ListBoxBaseItem *item = (PG_ListBoxBaseItem *)FindWidget(index - 1);
			if (item)
			{
				item->Select();
			}
		}
	}
}

void PG_ListBox::SelectLastItem()
{
	PG_ListBoxBaseItem* item = (PG_ListBoxBaseItem*)my_scrollarea->GetChildList()->last();
	if(item == NULL) 
	{
		return;
	}

	item->Select();
}

int PG_ListBox::GetSelectedIndex() 
{
	if (m_poCurrentItem)
	{
		return FindIndex(m_poCurrentItem);
	}
	return -1;
}

void PG_ListBox::GetSelectedItems(std::vector<PG_ListBoxBaseItem*>& items) {
	PG_RectList* list = my_scrollarea->GetChildList();
	if(list == NULL) {
		return;
	}

	for(PG_Widget* i = list->first(); i != NULL; i = i->next()) {
		PG_ListBoxBaseItem* item = static_cast<PG_ListBoxBaseItem*>(i);
		if (item->IsSelected()) {
			items.push_back(item);	
		}
	}
}

Uint16 PG_ListBox::GetIndent() {
	return my_indent;
}

void PG_ListBox::SetAlignment(PG_Label::TextAlign style) {
	my_alignment = style;
	
	PG_RectList* list = my_scrollarea->GetChildList();
	if(list == NULL) {
		return;
	}
	
	for(PG_Widget* i = list->first(); i != NULL; i = i->next()) {
		static_cast<PG_ListBoxBaseItem*>(i)->SetAlignment(style);
	}
	Update();
}

PG_Label::TextAlign PG_ListBox::GetAlignment() {
	return my_alignment;
}
