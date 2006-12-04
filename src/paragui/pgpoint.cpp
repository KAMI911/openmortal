/*
   ParaGUI - crossplatform widgetset
   Copyright (C) 2000-2004  Alexander Pipelka
 
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
   Source File:      $Source: /cvsroot/openmortal/openmortal/src/paragui/pgpoint.cpp,v $
   CVS/RCS Revision: $Revision: 1.1 $
   Status:           $State: Exp $
*/

#include "pgpoint.h"

PG_Point PG_Point::null;

PG_Point::PG_Point() : x(0), y(0) {
}

PG_Point::PG_Point(Sint16 _x, Sint16 _y) : x(_x), y(_y) {
}
