#include "common.h"
#include "pgmultilineedit.h"

using namespace std;

/*
Selection is between: my_mark and my_cursorPosition.
If my_mark is negative, there is no selection.
*/

PG_MultiLineEdit::PG_MultiLineEdit(PG_Widget* parent, const PG_Rect& r, const char* style, int maximumLength) 
: PG_LineEdit(parent, r, style, maximumLength) 
{ 
	my_vscroll = new PG_ScrollBar(this, PG_Rect(r.w-16,0,16,r.h));
	my_isCursorAtEOL = false; 
	my_allowHiddenCursor = true;
	my_firstLine = 0; 	
	my_vscroll->sigScrollPos.connect(slot(*this, &PG_MultiLineEdit::handleScroll));
	my_vscroll->sigScrollTrack.connect(slot(*this, &PG_MultiLineEdit::handleScroll));
	my_vscroll->Hide();
	my_mark = -1;
}

bool PG_MultiLineEdit::handleScroll(PG_ScrollBar* widget, long data) 
{
	SetVPosition(my_vscroll->GetPosition());
	my_allowHiddenCursor = true;
	return true;
}

void PG_MultiLineEdit::AddText(const char* text, PG_Color color, bool update)
{
	SetFontColor(color);
	AddText(text, update);
}

void PG_MultiLineEdit::AddText(const char* text, bool update)
{
	size_t PreviousLength = my_text.size();
	my_text += "\n";
	my_text += text;
	AppendTextVector(PreviousLength+1);

	SetupVScroll();
	my_isCursorAtEOL = false;
	my_allowHiddenCursor = false;
	if (!SetVPosition(my_vscroll->GetMaxRange()))
	{
		Update();
	}
}

bool PG_MultiLineEdit::SetVPosition(int line) 
{
	line = omMAX(line, 0);
	line = omMIN(line, my_vscroll->GetMaxRange());

	if (line == my_firstLine)
	{
		return false;
	}

	my_firstLine = line;
	
	if (my_vscroll->GetPosition() != my_firstLine) 
	{
		my_vscroll->SetPosition(my_firstLine);
	}
	
	Update();
	return true;
}

void PG_MultiLineEdit::eventBlit(SDL_Surface* surface, const PG_Rect& src, const PG_Rect& dst) 
{
	PG_Widget::eventBlit(surface, src, dst);
	DrawText(dst);
}

void PG_MultiLineEdit::DrawText(const PG_Rect& dst) 
{
	int _x = 3;
	int _y = 3; 
	unsigned int i;

	// should we draw the cursor ? 
	if(IsCursorVisible()) { 
		DrawTextCursor(); 
	}

	// figure out the cursor position that we start at 
	int pos = 0; 
	for (i = 0; i < (unsigned int)my_firstLine; ++i) { 
		pos += my_textdata[i].first.size(); 
	} 

	// draw text 
	int maxLines = my_height/GetFontSize() + 1;
	int endpos, start, end;

	int x1 = 0;
	Uint16 w = 0;
	int offset = 0;

	for (i = my_firstLine; 
		i < (unsigned int)my_firstLine + maxLines && i < my_textdata.size(); 
		++i) 
	{ 
		PG_Color linecolor= my_textdata[i].second;
		endpos = pos + my_textdata[i].first.size();
		start = (my_cursorPosition < my_mark ? my_cursorPosition : my_mark); 
		end  = (my_cursorPosition >= my_mark ? my_cursorPosition : my_mark); 
		
		// check if we are in the highlighted section 
		if (my_mark != -1 
			&& my_mark != my_cursorPosition 
			&& pos <= end 
			&& endpos >= start) 
		{ 
			x1 = _x;
			offset = 0;
			
			// draw the initial unhighlighted part 
			if (pos < start) { 
				string s = my_textdata[i].first.substr(0, start-pos); 
				PG_Widget::DrawText(x1, _y, s.c_str(), linecolor ); 
				PG_FontEngine::GetTextSize(s.c_str(), GetFont(), &w); 
				x1 += w; 
				offset = start-pos; 
			}

			string middlepart = my_textdata[i].first.c_str() + offset;
			// check if the end part is unhighlighted
			if (endpos > end) {
				middlepart = middlepart.substr(0, middlepart.size() - (endpos-end));
				string s = my_textdata[i].first.substr(end - pos, my_textdata[i].first.size() - (end - pos));
				PG_FontEngine::GetTextSize(middlepart.c_str(), GetFont(), &w);
				PG_Widget::DrawText(x1+w, _y, s.c_str(), linecolor);
			}
			
			PG_Color color(linecolor); 
			PG_Color inv_color(255 - color.r, 255 - color.g, 255 - color.b);			
			SetFontColor(inv_color);
			PG_FontEngine::GetTextSize(middlepart.c_str(), GetFont(), &w);
			SDL_Rect rect = {x + x1, y + _y, w, GetFontHeight()};
			SDL_Surface* screen = gamescreen;
			SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, color.r, color.g, color.b));
			PG_Widget::DrawText(x1, _y, middlepart.c_str());
			SetFontColor(color); 
		} 
		else { 
			PG_Widget::DrawText(_x, _y, my_textdata[i].first.c_str(), linecolor);
		} 
		_y += GetFontHeight(); 
		pos += my_textdata[i].first.size();
	} 
}

void PG_MultiLineEdit::DrawTextCursor() 
{
	int x = my_xpos + 1; 
	int y = my_ypos + 1; 
	int xpos, ypos;
	GetCursorPos(xpos, ypos); 

	/*
	// check for a hidden cursor 
	if(!my_allowHiddenCursor) { 
		// scroll up for cursor 
		while (ypos < 0 && my_firstLine > 0) { 
			SetVPosition(--my_firstLine); 
			GetCursorPos(xpos, ypos); 
		} 

		// scroll down for cursor 
		while (ypos + GetFontHeight() > my_height && my_firstLine < my_vscroll->GetMaxRange()) { 
			SetVPosition(++my_firstLine); 
			GetCursorPos(xpos, ypos); 
		} 
	}
	*/

	// draw simple cursor 
	if(my_srfTextCursor == NULL)
	{
		DrawVLine(xpos + 2, ypos + 2, GetFontHeight()-4, PG_Color(255, 255, 255));
	}
	// draw a nice cursor bitmap 
	else 
	{
		PG_Rect src, dst; 
		PG_Rect rect(x + xpos, y + ypos + GetFontHeight()/2 - my_srfTextCursor->h/2, 
		my_srfTextCursor->w, my_srfTextCursor->h); 
		GetClipRects(src, dst, rect); 
		PG_Widget::eventBlit(my_srfTextCursor, src, dst); 
	} 
}

void PG_MultiLineEdit::FindWordRight() { 
	unsigned int currentPos = my_cursorPosition; 
  
	// step off the initial space 
	++currentPos; 
	
	// find the next space 
	while (currentPos-1 <= my_text.size() && my_text[currentPos-1] != ' ' && my_text[currentPos-1] != '\n') {
		++currentPos; 
	}
	
	// go to the end of multiple spaces 
	while (currentPos <= my_text.size() && (my_text[currentPos] == ' ' || my_text[currentPos] == '\n')) {
		++currentPos;
	}

	SetCursorPos(currentPos); 
}

void PG_MultiLineEdit::FindWordLeft() { 
	unsigned int currentPos = my_cursorPosition; 

	// step off the initial space(s) 
	while (currentPos-1 >= 0 && (my_text[currentPos-1] == ' ' || my_text[currentPos-1] == '\n')) {
		--currentPos;
	}
  
	// find the next space 
	while (currentPos-1 >= 0 && my_text[currentPos-1] != ' ' && my_text[currentPos-1] != '\n') {
		--currentPos;
	}

	SetCursorPos(currentPos); 
}

void PG_MultiLineEdit::GetCursorTextPosFromScreen(int x, int y, unsigned int& horzOffset, unsigned int& lineOffset) 
{ 
	// check for an empty text box 
	if (my_textdata.size() == 0) { 
		horzOffset = 0;
		lineOffset = 0;
		return; 
	}

	// get the line number 
	int ypos = (y - my_ypos - 3) / GetFontHeight() + my_firstLine; 

	// stay within limits 
	if (ypos < 0) 
	{
		ypos = 0;
	}
  
	if ((unsigned int)ypos >= my_textdata.size())
	{
		ypos = my_textdata.size()-1;
	}

	unsigned int min = (unsigned int)-1; 
	unsigned int min_xpos = 0; 
	
	// loop through to find the closest x position 
	string temp;
	for (Uint16 i = 0; i <= my_textdata[ypos].first.size(); ++i) 
	{  
		// get the string up to that point 		
		temp = my_textdata[ypos].first.substr(0, i);
	
		// get the distance for that section 
		Uint16 w; 
		PG_FontEngine::GetTextSize(temp.c_str(), GetFont(), &w); 
		unsigned int dist = abs(x - (my_xpos + 3 + w)); 
    
		// update minimum 
		if (dist < min) { 
			min = dist; 
			min_xpos = i; 
		} 
	}

	// set return data 
	horzOffset = min_xpos;
	lineOffset = (unsigned int)ypos; 
}

void PG_MultiLineEdit::GetCursorTextPos(unsigned int& horzOffset, unsigned int& lineOffset) { 
	// check for an empty text box 
	if (my_textdata.size() == 0) { 
		horzOffset = 0;
		lineOffset = 0;
		return; 
	}

	unsigned int currentPos = my_cursorPosition; 
	unsigned int line = 0; 

	// cycle through the lines, finding where our cursor lands 
	for (cTextData::iterator i = my_textdata.begin(); i != my_textdata.end(); ++i) { 
		if(currentPos < i->first.size() || (currentPos <= i->first.size() && my_isCursorAtEOL)) {
			break;
		}
		currentPos -= i->first.size(); 
		line++;
	} 

	// if we're too far, assume we're at the end of the string 
	if (line >= my_textdata.size()) {  
		line = my_textdata.size()-1; 
		currentPos = my_textdata[line].first.size();
	} 
	
	// if we're too far on this line, assum we're at the end of line
	if (currentPos > my_textdata[line].first.size()) {  
		currentPos = my_textdata[line].first.size(); 
	} 

	horzOffset = currentPos;
	lineOffset = line; 
} 

void PG_MultiLineEdit::GetCursorPos(int& x, int& y) { 
	// check for an empty text box 
	if (my_textdata.size() == 0) { 
		x = 0;
		y = 0;
		return; 
	} 

	// get the cursor text position 
	unsigned int currentPos, line; 
	GetCursorTextPos(currentPos, line); 

	// now get the x,y position 
	string temp = my_textdata[line].first.substr(0, currentPos);

	Uint16 w; 
	PG_FontEngine::GetTextSize(temp.c_str(), GetFont(), &w); 
	
	x = w; 
	y = (line - my_firstLine)*GetFontHeight(); 
} 

void PG_MultiLineEdit::CreateTextVector(bool bSetupVScroll) 
{ 
	my_textdata.clear();
	AppendTextVector(0);
	// setup the scrollbar
	if(bSetupVScroll) {
		SetupVScroll(); 
	}
}


void PG_MultiLineEdit::AppendTextVector(unsigned int start)
{
//	int w = my_width - 6 - ((my_vscroll->IsVisible() || !my_vscroll->IsHidden()) ? my_vscroll->w : 0); 
	int w = my_width - 6 - my_vscroll->w; 
	unsigned int end = start, last = start;
	
	do { 
		Uint16 lineWidth = 0; 
		PG_String temp = my_text.substr(start, end-start);
		PG_FontEngine::GetTextSize(temp.c_str(), GetFont(), &lineWidth); 
		
		if (lineWidth > w) { 
			if (last == start) { 
				PG_String s = my_text.substr(start, end-start-1); 
				my_textdata.push_back( cColoredText(s, GetFontColor()) );
				start = --end; 
			} 
			else { 
				PG_String s = my_text.substr(start, last-start); 
				my_textdata.push_back( cColoredText(s, GetFontColor()) );
				start = last; 
				end = last-1; 
			} 
			last = start; 
		} 
		
		else if (my_text[end] == ' ') { 
			last = end+1; 
		} 
		
		else if (my_text[end] == '\n' || my_text[end] == '\0') { 
			PG_String s = my_text.substr(start, end-start+1); 
			my_textdata.push_back( cColoredText(s, GetFontColor()) ); 
			start = end+1; 
			last = start; 
		} 
	} while (end++ < my_text.size()); 
}

void PG_MultiLineEdit::SetupVScroll() 
{
	if (my_textdata.size()*GetFontHeight() < my_height) 
	{ 
		my_vscroll->SetRange(0, 0); 
		my_vscroll->Hide(); 
		SetVPosition(0);
		Update();
	}
	else
	{
	
		my_vscroll->SetRange(0, my_textdata.size() - my_height/GetFontHeight()); 
		if (my_firstLine > my_vscroll->GetMaxRange()) {
			SetVPosition(my_vscroll->GetMaxRange()); 
		}
		
		if (!my_vscroll->IsVisible() || my_vscroll->IsHidden()) { 
			// scrollbar makes the window less wide, so we have to redo the text 
			// (note: don't switch these next two lines, unless you like infinite loops) 
			my_vscroll->Show(); 
		} 
	} 
} 

bool PG_MultiLineEdit::eventKeyDown(const SDL_KeyboardEvent* key) {
	PG_Char c; 

//	if(!IsCursorVisible()) {
//		return false;
//	}

	SDL_KeyboardEvent key_copy = *key; // copy key structure 

	if ((key_copy.keysym.mod & KMOD_SHIFT) && my_mark == -1) {
		my_mark = my_cursorPosition; 
	}

	if(key_copy.keysym.mod & KMOD_CTRL) {
		switch(key_copy.keysym.sym) { 
			case SDLK_HOME: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				SetCursorPos(0); 
				return true; 
			
			case SDLK_END: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				SetCursorPos(my_text.length()); 
				return true; 
      
			case SDLK_RIGHT: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				FindWordRight(); 
				return true; 
		
			case SDLK_LEFT: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				FindWordLeft(); 
				return true; 
			
			case SDLK_UP: 
				my_allowHiddenCursor = true; 
				SetVPosition(--my_firstLine); 
				return true; 
		
			case SDLK_DOWN: 
				my_allowHiddenCursor = true; 
				SetVPosition(++my_firstLine); 
				return true;

			default:
				break;
		} 
	} 
	else if(key_copy.keysym.mod & (KMOD_ALT | KMOD_META)) { 
	} 
	else { 
		unsigned int currentPos, line; 
		int x, y; 
  
		switch(key_copy.keysym.sym) { 
			case SDLK_RIGHT: 
			case SDLK_LEFT: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				// break here, we still want PG_LineEdit to handle these 
				break; 
			
			case SDLK_UP: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
				my_mark = -1; 
				}
				GetCursorPos(x, y); 
				GetCursorTextPosFromScreen(my_xpos + x + 3, my_ypos + y + 3 - GetFontHeight(), currentPos, line);
				SetCursorTextPos(currentPos, line); 
				return true; 
      
			case SDLK_DOWN: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				GetCursorPos(x, y);
				GetCursorTextPosFromScreen(my_xpos + x + 3, my_ypos + y + 3 + GetFontHeight(), currentPos, line);
				SetCursorTextPos(currentPos, line); 
				return true; 
			
			case SDLK_PAGEUP: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}				
				GetCursorPos(x, y); 
				GetCursorTextPosFromScreen(my_xpos + x + 3, my_ypos + y + 3 - (my_height - GetFontHeight()), currentPos, line); 
				SetCursorTextPos(currentPos, line); 
				return true; 
		
			case SDLK_PAGEDOWN: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
        
				GetCursorPos(x, y); 
				GetCursorTextPosFromScreen(my_xpos + x + 3, my_ypos + y + 3 + (my_height - GetFontHeight()), currentPos, line); 
				SetCursorTextPos(currentPos, line); 
				return true; 
			
			case SDLK_HOME: 
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				GetCursorTextPos(currentPos, line); 
				SetCursorTextPos(0, line); 
				return true; 
		
			case SDLK_END: {
				if (!(key_copy.keysym.mod & KMOD_SHIFT)) {
					my_mark = -1; 
				}
				GetCursorTextPos(currentPos, line);
				PG_String &roThisLine= my_textdata[line].first;
				int cursorPos = roThisLine.size() - 
					(roThisLine[roThisLine.size()-1] == '\n' ? 1 : 0); 
				SetCursorTextPos(cursorPos, line);
			}
				return true; 

			case SDLK_RETURN: 
				// c = '\n'; 
				//InsertChar(&c); 
				//SetCursorPos(my_cursorPosition); 
				return true;
				
			default:
				break;
		} 
	} 

	SetCursorPos(my_cursorPosition); 
	
	return PG_LineEdit::eventKeyDown(key); 
}

bool PG_MultiLineEdit::eventMouseButtonDown(const SDL_MouseButtonEvent* button) { 
	// check for mousewheel 
	if ((button->button == 4 || button->button == 5) && my_vscroll->IsVisible()) { 
		if (button->button == 4) {
			SetVPosition(my_firstLine - 1); 
		}
		else {
			SetVPosition(my_firstLine + 1);
		}
		return true; 
	} 

//	if (!GetEditable()) {
//		return false; 
//	}

	if (!IsCursorVisible()) {
		EditBegin(); 
	}

	// if we're clicking the scrollbar.... 
	if (my_vscroll->IsVisible() && button->x > my_xpos + my_width - my_vscroll->w) {
		return false; 
	}

	if (button->button == 1) { 
		Uint8* keys = SDL_GetKeyState(NULL); 
		
		if (!(keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])) {
			my_mark = -1;
		}
		
		unsigned int currentPos, line; 
		GetCursorTextPosFromScreen(button->x, button->y, currentPos, line); 
		SetCursorTextPos(currentPos, line); 
		if (!(keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])) {
			my_mark = my_cursorPosition;
		}
	} 
	
	return true; 
} 

bool PG_MultiLineEdit::eventMouseMotion(const SDL_MouseMotionEvent* motion) {
	if (motion->state & SDL_BUTTON(1)) { 
	    unsigned int currentPos, line; 
		GetCursorTextPosFromScreen(motion->x, motion->y, currentPos, line); 
		SetCursorTextPos(currentPos, line); 
	}
	
	return PG_LineEdit::eventMouseMotion(motion); 
} 

bool PG_MultiLineEdit::eventMouseButtonUp(const SDL_MouseButtonEvent* button) {
	if(!GetEditable()) {
		return false;
	}

	if(!IsCursorVisible()) {
		EditBegin();
	}

	return true;
}

void PG_MultiLineEdit::SetCursorTextPos(unsigned int offset, unsigned int line) { 
	if (line < 0) {
		SetCursorPos(0); 
	}
	else if (line >= my_textdata.size()) { 
		SetCursorPos(my_text.size()); 
		my_isCursorAtEOL = false; 
	} 
	else { 
		PG_LineEdit::SetCursorPos(ConvertCursorPos(offset, line)); 
		my_isCursorAtEOL = (offset == my_textdata[line].first.size() && my_textdata[line].first.size() != 0);
		Update();
	} 
} 

int PG_MultiLineEdit::ConvertCursorPos(unsigned int offset, unsigned int line) {
	unsigned int charCount = 0; 
	for (unsigned int i = 0; i < line; ++i) {
		charCount += my_textdata[i].first.size();
	}
	
	return charCount+offset;
} 

void PG_MultiLineEdit::SetCursorPos(int p) {
	my_isCursorAtEOL = false; 
	PG_LineEdit::SetCursorPos(p);
} 

void PG_MultiLineEdit::InsertChar(const PG_Char* c) { 
	if (my_mark != -1 && my_mark != my_cursorPosition) {
		DeleteSelection(); 
	}
	
	PG_LineEdit::InsertChar(c); 
	my_mark = -1; 
	CreateTextVector();
	Update();
} 

void PG_MultiLineEdit::DeleteChar(Uint16 pos) { 
	if (my_mark != -1 && my_mark != my_cursorPosition) { 
		Uint16 oldpos = my_cursorPosition; 
		DeleteSelection(); 
		// check if backspace was pressed 
		if (pos == oldpos-1) {
			my_cursorPosition++; 
		}
	} 
	else {
		PG_LineEdit::DeleteChar(pos);
	}
  
	my_mark = -1; 
	CreateTextVector();
	Update();
} 

void PG_MultiLineEdit::DeleteSelection() {
	if (my_mark != -1 && my_mark != my_cursorPosition) {
		int start = (my_cursorPosition < my_mark ? my_cursorPosition : my_mark);
		int end  = (my_cursorPosition >= my_mark ? my_cursorPosition : my_mark);
		my_text.erase(start, end-start);
		if (my_mark < my_cursorPosition) {
			SetCursorPos(my_mark);
		}
		my_mark = -1;
	}
}

void PG_MultiLineEdit::SetText(const char* new_text) { 
	PG_LineEdit::SetText(new_text);
	CreateTextVector();
	my_isCursorAtEOL = false;
	my_mark = -1;
	SetVPosition(0);
}
