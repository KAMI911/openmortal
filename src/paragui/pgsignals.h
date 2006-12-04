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
    Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pgsignals.h,v $
    CVS/RCS Revision: $Revision: 1.1 $
    Status:           $State: Exp $
*/

#ifndef PG_SIGNALS_H
#define PG_SIGNALS_H

#include <sigc++/sigc++.h>
//#include "pgsigconvert.h"

#ifndef DOXYGEN_SKIP
using namespace SigC;
//@ using namespace SigCX;
#endif // DOXYGEN_SKIP

typedef void* PG_Pointer;

template<class datatype = PG_Pointer> class PG_Signal0 : public Signal0<bool> {
public:

	Connection connect(const Slot1<bool, datatype>& s, datatype data) {
		return Signal0<bool>::connect(bind(s, data));
	};

};

template<class P1, class datatype = PG_Pointer> class PG_Signal1 : public Signal1<bool, P1> {

	static bool sig_convert0(Slot0<bool>& s, P1 p1) {
		return s();
	}

public:
	
	Connection connect(const Slot2<bool, P1, datatype>& s, datatype data) {
		return Signal1<bool, P1>::connect(bind(s, data));
	};

	Connection connect(const Slot1<bool, datatype>& s, datatype data) {
		return connect(bind(s, data));
	}

	Connection connect(const Slot1<bool, P1>& s) {
		return Signal1<bool, P1>::connect(s);
	}

	Connection connect(const Slot0<bool>& s) {
		return Signal1<bool, P1>::connect(convert(s, sig_convert0));
	}

	PG_Signal1& operator=(const PG_Signal1&);
};


template<class P1, class P2, class datatype = PG_Pointer> class PG_Signal2 : public Signal2<bool, P1, P2> {

	static bool sig_convert_p2(Slot1<bool, P2>& s, P1 p1, P2 p2) {
		return s(p2);
	}

	static bool sig_convert_p1(Slot1<bool, P1>& s, P1 p1, P2 p2) {
		return s(p1);
	}

	static bool sig_convert0(Slot0<bool>& s, P1 p1, P2 p2) {
		return s();
	}
public:

	Connection connect(const Slot3<bool, P1, P2, datatype>& s, datatype data) {
		return Signal2<bool, P1, P2>::connect(bind(s, data));
	}

	Connection connect(const Slot2<bool, P1, datatype>& s, datatype data) {
		return Signal2<bool, P1, P2>::connect(bind(s, data));
	};

	Connection connect(const Slot2<bool, P1, P2>& s) {
		return Signal2<bool, P1, P2>::connect(s);
	}

	Connection connect(const Slot1<bool, P2>& s) {
		return Signal2<bool, P1, P2>::connect(convert(s, sig_convert_p2));
	}

	Connection connect(const Slot1<bool, P1>& s) {
		return Signal2<bool, P1, P2>::connect(convert(s, sig_convert_p2));
	}

	Connection connect(const Slot0<bool>& s) {
		return Signal2<bool, P1, P2>::connect(convert(s, sig_convert0));
	}

private:
	PG_Signal2& operator=(const PG_Signal2&);

};

/*
typedef PG_Signal1<PG_MessageObject*> PG_SignalAppIdle;

typedef PG_Signal1<PG_Application*> PG_SignalAppQuit;

typedef PG_Signal1<const SDL_ResizeEvent*> PG_SignalVideoResize;

typedef PG_Signal2<PG_TabBar*, PG_Button*> PG_SignalTabSelect;

typedef Slot1<bool, PG_Button*> PG_TabSelectSlot;

typedef PG_Signal2<PG_NoteBook*, PG_Widget*> PG_PageSelect;

typedef Slot1<bool, PG_Widget*> PG_PageSelectSlot;
*/

#endif // PG_SIGNALS_H
