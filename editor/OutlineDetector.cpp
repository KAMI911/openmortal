/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#include <qimage.h>
#include <qwmatrix.h> 
#include "ImageEditor.h"
#include "OutlineDetector.h"



ImageData OutlineDetector::doDetection( int _headW, int _headH, int _bodyW, 
					double _bodyPercent, QImage& _image, ImageData& _data )
{
    OutlineDetector o( _headW, _headH, _bodyW, _bodyPercent, _image );
    
    QPoint _imageOffset = _data.imageOffset;
    o.data.imageOffset = _imageOffset;
    o.data.imageSize = _data.imageSize;
    o.data.head.translate( _imageOffset.x(), _imageOffset.y() );
    o.data.body.translate( _imageOffset.x(), _imageOffset.y() );
    o.data.legs.translate( _imageOffset.x(), _imageOffset.y() );
    
    _image = o.image;
    _data = o.data;
    return o.data;
}



void OutlineDetector::findScanline( int _y, int& _outFrom, int& _outWidth )
{
    int _maxfrom = -1, _maxwidth = 0;
    int _from = 0, _width = 0;
    bool _inside = false;
    int _pixel;
    
    for ( int x=0; x<imageW; ++x )
    {
	_pixel = image.pixelIndex( x, _y );
	if ( _pixel == 0 )
	{
	    if ( _inside )
	    {
		// We have reached the end of this round.
		if ( _width > _maxwidth )
		{
		    _maxfrom = _from; _maxwidth = _width;
		}
		//debug( "Reached the end of the run at %d, %d", x, _y );
	    }
	    _inside = false;
	    continue;
	}
	
	if ( !_inside )
	{
	    //debug( "Reached the start of the run at %d, %d", x, _y );
	    _from = x;
	    _width = 0;
	    _inside = true;	    
	}
	++_width;
    }
    
    if ( _inside && _width > _maxwidth )
    {
	_maxwidth = _width; _maxfrom = _from;
    }
    
    _outFrom = _maxfrom;
    _outWidth = _maxwidth;
}


void OutlineDetector::findMinMax( int y1, int y2, int& _min, int& _max )
{
    _min = _max = -1;
    int _pixel;
    
    for ( int y=y1; y<=y2; ++y )
    {
	for ( int x=0; x<imageW; ++x )
	{
	    _pixel = image.pixelIndex( x, y );
	    if ( _pixel == 0 )
	    {
		continue;
	    }
	    
	    if ( (_min < 0) || (_min > x) ) 
		_min = x;
	    if ( (_max < x ) )
		_max = x;
	}
    }
    
}


void OutlineDetector::findAverage( int y1, int y2, int& from, int& width )
{
    if ( y1>y2 )
    {
	from = width = -1;
	return;
    }
    
    int totX = 0, totW = 0;
    
    for ( int y=y1; y<y2; ++y )
    {
	int _from, _width;
	findScanline( y, _from, _width );
	totX += _from;
	totW += _width;
	//image.setPixel( scanX[0], y, 5 );
    }
    
    from = totX / (y2-y1+1);
    width = totW / (y2-y1 + 1 );
}



OutlineDetector::OutlineDetector( int __headW, int __headH, int __bodyW, double __bodyPercent, QImage& __image )
{
    headW = __headW;
    headH = __headH;
    bodyW = __bodyW;
    bodyPercent = __bodyPercent;
    image = __image;
    
    imageW = image.width();
    imageH = image.height();
    
    if ( imageH < headH *1.25 )
    {
	// Image is too small for detection
	data.head.setPoint( 0, 0, 0 );
	data.head.setPoint( 1, imageW, 0 );
	data.head.setPoint( 2, imageW, imageH );
	data.head.setPoint( 3, 0, imageH );
	
	data.body.setPoint( 0, 0, 0 );
	data.body.setPoint( 1, imageW, 0 );
	data.body.setPoint( 2, imageW, imageH );
	data.body.setPoint( 3, 0, imageH );
	
	data.legs.setPoint( 0, 0, 0 );
	data.legs.setPoint( 1, imageW/2, 0 );
	data.legs.setPoint( 2, imageW, 0 );
	data.legs.setPoint( 3, imageW, imageH  );
	data.legs.setPoint( 4, imageW/2, imageH );
	data.legs.setPoint( 5, 0, imageH );
	return;
    }
    
    bool bRotated = false;
    
    if ( imageH < 150 && imageW > 200 )
    {
	// laying position. Change strategy.
	QWMatrix oMatrix;
	oMatrix.rotate( -90.0 );
	image = image.xForm( oMatrix );
	
	imageW = image.width();
	imageH = image.height();
	bRotated = true;
    }
    
    int _headX, _headW, _bodyX, _bodyW;
    int _bodyY = int( (imageH - headH) * bodyPercent + headH );
    int _legX1, _legX2;
    
    findAverage( 0, headH, _headX, _headW );
    findAverage( headH, _bodyY, _bodyX, _bodyW );
    
    int _legH = imageH - _bodyY + 1;
    findMinMax( _bodyY + _legH/2, imageH-1, _legX1, _legX2 );
       
//    debug( "Head X: %d, head W: %d", _headX, _headW );
//    debug( "Body X: %d, body W: %d", _bodyX, _bodyW );
    
    if ( bRotated )
    {
	data.head.setPoint( 0, 0, imageW-_headX-_headW );
	data.head.setPoint( 1, headH, imageW-_headX-_headW );
	data.head.setPoint( 2, headH, imageW-_headX );
	data.head.setPoint( 3, 0, imageW-_headX );
	
	data.body.setPoint( 0, headH, imageW-_bodyX-bodyW );
	data.body.setPoint( 1, _bodyY+5, imageW-_bodyX-bodyW );
	data.body.setPoint( 2, _bodyY+5, imageW-_bodyX );
	data.body.setPoint( 3, headH, imageW-_bodyX );
	
	data.legs.setPoint( 0, _bodyY-5, imageW-_bodyX-bodyW );
	data.legs.setPoint( 1, _bodyY-5, imageW-_bodyX-bodyW-5 );
	data.legs.setPoint( 2, imageH, imageW-_legX2 );
	data.legs.setPoint( 3, imageH, imageW-_legX1 );
	data.legs.setPoint( 4, _bodyY-5, imageW );
	data.legs.setPoint( 5, _bodyY-5, imageW-5 );
	return;
    }
    
    data.head.setPoint( 0, _headX, 0 );
    data.head.setPoint( 1, _headX + _headW, 0 );
    data.head.setPoint( 2, _headX + _headW, headH+5 );
    data.head.setPoint( 3, _headX, headH+5 );
    
    data.body.setPoint( 0, _bodyX, headH );
    data.body.setPoint( 1, _bodyX + bodyW, headH );
    data.body.setPoint( 2, _bodyX + bodyW, _bodyY+5 );
    data.body.setPoint( 3, _bodyX, _bodyY+5 );
    
    data.legs.setPoint( 0, _bodyX, _bodyY-5 );
    data.legs.setPoint( 1, _bodyX + 5, _bodyY-5 );
    data.legs.setPoint( 2, _bodyX + bodyW - 5, _bodyY-5 );
    data.legs.setPoint( 3, _bodyX + bodyW, _bodyY-5 );
    data.legs.setPoint( 4, _legX2, imageH );
    data.legs.setPoint( 5, _legX1, imageH );
}


