/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#include <qapplication.h>
#include "mortaleditor.h"

int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    MortalEditor *w = new MortalEditor;
    w->show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
