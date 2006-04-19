/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#ifndef _IMAGEDATA_H
#define _IMAGEDATA_H

#include "ImageEditor.h"

struct ImageStorage_P;
class RlePack;

class ImageStorage
{
public:
    static ImageStorage& instance();
    
    void		load( const QString& filename );
    void		save( const QString& filename, RlePack* images );
    
    uint 		count();
    ImageData	getImageData( int index, RlePack* images );
    void		setImageData( int index, const ImageData& data );
    void		setDetectionParams( int a_iHeadW, int a_iHeadH, int a_iBodyW, double a_dBodyP );
    
protected:
    ImageStorage();
    
protected:
    static ImageStorage singleton;
    ImageStorage_P* d;
};

#endif
