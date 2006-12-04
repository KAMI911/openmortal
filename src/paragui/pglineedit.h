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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pglineedit.h,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#ifndef PG_LINEEDIT
#define PG_LINEEDIT

#include "pgwidget.h"
#include "pgsignals.h"
// #include "pgstring.h"

/**
	@author Alexander Pipelka
 
	@short An editable text box.
 
	This really should be renamed to PG_TextBox or something similar, because
	PG_LineEdit really makes no sense :).  Also the 'Edit' part implies that
	it will always be modifiable, but having a modifyable attribute would
	probably be better than creating a whole new widget for an unmodifiable
	text box.
*/

class PG_LineEdit : public PG_Widget {
//class PG_LineEdit : public PG_ThemeWidget {
public:

	/**
	Signal type declaration
	**/
	template<class datatype = PG_Pointer> class SignalEditBegin : public PG_Signal1<PG_LineEdit*, datatype> {};
	template<class datatype = PG_Pointer> class SignalEditEnd : public PG_Signal1<PG_LineEdit*, datatype> {};
	template<class datatype = PG_Pointer> class SignalEditReturn : public PG_Signal1<PG_LineEdit*, datatype> {};

	/** */
	PG_LineEdit(PG_Widget* parent, const PG_Rect& r = PG_Rect::null, const char* style="LineEdit", int maximumLength = 1000000);

	/** */
	~PG_LineEdit();

	// void LoadThemeStyle(const char* widgettype);

	/** */
	// void LoadThemeStyle(const char* widgettype, const char* objectname);

	/** start edit */
	void EditBegin();

	/** */
	void EditEnd();

	/** */
	virtual void SetCursorPos(int p);

	/** */
	int GetCursorPos();

	/**
	Set the current text string
	@param	new_text	pointer to text string
	*/
	virtual void SetText(const char* new_text);

	/** */
	bool IsCursorVisible();

	/**
	Send a char into the LineEdit widget

	@param	c		the character to insert
	*/
	void SendChar(PG_Char c);

	/**
	Send a 'del' keystroke into the LineEdit widget
	*/
	void SendDel();

	/**
	Send a 'BKSPC' keystroke into the LineEdit widget
	*/
	void SendBackspace();

	/**
	Define a set of valid keys

	@param	keys		a string which contains all valid chars
	*/
	void SetValidKeys(const char* keys);

	/**
	Set if the widget is editable by the user
	@param	edit		true - widget is editable / false - widget is read only
	*/
	void SetEditable(bool edit);

	/**
	Check if the widget is editable by the user
	@return			true - widget is editable / false - widget is read only
	*/
	bool GetEditable();

	/**
	Set password character (witch will be displayed in place of letter)
	@param	passchar		character (if = 0 do not any replacement)
	*/
	void SetPassHidden(char passchar);

	/**
	Return setted passchar
	@return			Returns character, witch is displayed in place on letters
	*/
	char GetPassHidden();

	bool Action(KeyAction action);

	SignalEditBegin<> sigEditBegin;
	SignalEditEnd<> sigEditEnd;
	SignalEditReturn<> sigEditReturn;

protected:

	/** */
	virtual void InsertChar(const PG_Char* c);
		
	/** */
	virtual void DeleteChar(Uint16 pos);

	/** */
	void CopyText(bool del = false);

	/** */
	void PasteText(Uint16 pos);

	/** */
	void StartMark(Uint16 pos);

	/** */
	void EndMark(Uint16 pos);

	/** */
	virtual bool eventFilterKey(const SDL_KeyboardEvent* key);

	/** */
	virtual void eventEditBegin(int id, PG_Widget* widget, unsigned long data, void *clientdata);

	/** */
	virtual void eventEditEnd(int id, PG_Widget* widget, unsigned long data, void *clientdata);

	/** */
	void eventBlit(SDL_Surface* surface, const PG_Rect& src, const PG_Rect& dst);
	//void eventDraw(SDL_Surface* surface, const PG_Rect& rect);

	/** */
	bool eventKeyDown(const SDL_KeyboardEvent* key);

	/** */
	bool eventMouseButtonUp(const SDL_MouseButtonEvent* button);

	/** */
	void eventInputFocusLost(PG_MessageObject* newfocus);

	void eventHide();

	int my_cursorPosition;

	SDL_Surface* my_srfTextCursor;

private:

	PG_LineEdit(const PG_LineEdit&);
	PG_LineEdit& operator=(PG_LineEdit&);

	void DrawText(const PG_Rect& dst);
	void DrawTextCursor();
	Uint16 GetCursorXPos();
	PG_String GetDrawText();
	int GetCursorPosFromScreen(int x, int y);

	bool IsValidKey(PG_Char c);

	std::string my_buffer;
	int my_startMark;
	int my_endMark;

	bool my_isCursorVisible;
	int my_offsetX;
	PG_String my_validkeys;
	bool my_isEditable;
	int my_maximumLength;

	PG_Char my_passchar;
};

#endif // PG_LINEEDIT
