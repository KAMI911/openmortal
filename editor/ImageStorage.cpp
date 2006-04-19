/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#include <qvaluevector.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qregexp.h>
#include <qvaluelist.h>

#include "ImageStorage.h"
#include "OutlineDetector.h"
#include "RlePack.h"

ImageStorage ImageStorage::singleton;

struct ImageStorage_P
{
    QValueVector<ImageData> dataVector;
    int m_iHeadW;
    int m_iHeadH;
    int m_iBodyW;
    double m_dBodyP;
};

ImageStorage& ImageStorage::instance()
{
    return singleton;
}

ImageStorage::ImageStorage()
{
    d = new ImageStorage_P;
    d->m_iHeadW = 35;
    d->m_iHeadH = 45;
    d->m_iBodyW = 55;
    d->m_dBodyP = 0.5;
}

void ImageStorage::load( const QString& filename )
{
    QFile f( filename );
    if ( !f.open(  IO_ReadOnly | IO_Translate ) )
    {
	return;
    }
    
    QTextStream s( &f );
    QString line;
    QStringList sl;
    QRegExp separator( "[^0-9]+" );
    QValueList<ImageData> images;
    
    while (!f.atEnd() )
    {
	line = s.readLine();
	sl = sl.split( separator, line, false );
	if ( sl.count() < 4 )
	    continue;
	
	ImageData data;
	data.isExtrapolated = true;
	
	data.imageOffset.setX( sl[0].toInt() );
	data.imageOffset.setY( sl[1].toInt() );
	data.imageSize.setWidth( sl[2].toInt() );
	data.imageSize.setHeight( sl[3].toInt() );
	
	debug( "Found data: %d %d %d %d", data.imageSize.width(), data.imageSize.height(),
	       data.imageOffset.x(), data.imageOffset.y() );
	
	if ( sl.count() < 31 )
	{
	    images.push_back( data );
	    continue;
	}
	
	int j = 4;
	int i;
	for ( i=0; i<4; ++i )
	{
	    data.head[i].setX( sl[j++].toInt() );
	    data.head[i].setY( sl[j++].toInt() );
	}
	for ( i=0; i<4; ++i )
	{
	    data.body[i].setX( sl[j++].toInt() );
	    data.body[i].setY( sl[j++].toInt() );
	}
	for ( i=0; i<6; ++i )
	{
	    data.legs[i].setX( sl[j++].toInt() );
	    data.legs[i].setY( sl[j++].toInt() );
	}
	data.isExtrapolated = sl[j++].toInt();
	
	if ( sl.count() > 36 )
	{
	    for ( i=0; i<4; ++i )
	    {
		data.hit[i].setX( sl[j++].toInt() );
		data.hit[i].setY( sl[j++].toInt() );
	    }
	}
	
	images.push_back( data );
    }
    
    d->dataVector.resize( images.count() );
    for ( uint i=0; i<images.count(); ++i )
    {
	d->dataVector[i] = images[i];
    }
}


QString array2String( const QPointArray& a )
{
    QString s = "[";
    QString s1;
    for ( uint i=0; i<a.count(); ++i )
    {
	s1.sprintf( "%3d,%3d,", a.at(i).x(), a.at(i).y() );
	s += s1;
    }
    s += "]";
    return s;
}

void ImageStorage::save( const QString& filename, RlePack* images )
{
    QFile f( filename );
    if (! f.open ( IO_WriteOnly ) )
    {
	return;
    }
    QTextStream out( &f );
    
    QString s, s1;
    
    for ( uint i=0; i<count(); ++i )
    {
	ImageData d = getImageData(i, images);
	s.sprintf( "'x'=>%3d, 'y'=>%3d, 'w'=>%3d, 'h'=>%3d, 'head'=>%s, 'body'=>%s, 'legs'=>%s, 'ex'=>%d",
		   d.imageOffset.x(), d.imageOffset.y(),
		   d.imageSize.width(), d.imageSize.height(),
		   array2String( d.head ).latin1(),
		   array2String( d.body ).latin1(),
		   array2String( d.legs ).latin1(),
		   d.isExtrapolated );
	if ( d.hit[0].x() != 10 )
	{
	    s += ", 'hit'=>" + array2String( d.hit );
	}
	out << "{ "<< s << " },\n";
    }
    f.close();
}

uint ImageStorage::count()
{
    return d->dataVector.size();
}

ImageData ImageStorage::getImageData( int index, RlePack* images )
{
    ImageData data = d->dataVector[index];
//    debug( "ImageStorage::getImageData( %d, extrapolated %d )", index, data.isExtrapolated );
    
    if ( data.isExtrapolated )
    {
	QImage image( images->getSize( index ), 8 );
	image.fill(0);
	images->draw( image, index, 0, 0 );
	OutlineDetector::doDetection( d->m_iHeadW, d->m_iHeadH, d->m_iBodyW, d->m_dBodyP,
	    image, data );
    }
    /*
    if ( data.isExtrapolated )
    {
	debug( "Extrapolating image %d", index );
	// Extrapolate data
	if ( index == 0 )
	    return data;
	
	// Find the non-extrapolated image before index
	
	int before, after;
	for ( before=index-1; before>=0; --before )
	    if ( ! d->dataVector[before].isExtrapolated )
		break;
	if ( before < 0 )
	    return data;
	
	// Find the non-extrapolated image after index
	
	for ( after = index+1; after<count(); ++after )
	    if ( ! d->dataVector[after].isExtrapolated )
		break;
	if ( after >= count() )
	    return data;
	
	// Found before and after, now all we have to do is
	// extrapolate...
	debug( "Before = %d, after = %d", before, after );
	
	double wbefore = ((double)after-index) / ((double)after-before);
	double wafter = ((double)index-before) / ((double)after-before);
	ImageData dbefore = d->dataVector[before];
	ImageData dafter = d->dataVector[after];
	int i;
	
	for ( i=0; i<4; ++i )
	{
	    data.head[i].setX( int ( dbefore.head[i].x()*wbefore + dafter.head[i].x()*wafter ) );
	    data.head[i].setY( int ( dbefore.head[i].y()*wbefore + dafter.head[i].y()*wafter ) );
	    data.body[i].setX( int ( dbefore.body[i].x()*wbefore + dafter.body[i].x()*wafter ) );
	    data.body[i].setY( int ( dbefore.body[i].y()*wbefore + dafter.body[i].y()*wafter ) );
	}
	for ( i=0; i<6; ++i )
	{
	    data.legs[i].setX( int ( dbefore.legs[i].x()*wbefore + dafter.legs[i].x()*wafter ) );
	    data.legs[i].setY( int ( dbefore.legs[i].y()*wbefore + dafter.legs[i].y()*wafter ) );
	}
    }
    */
    return data;
}

void ImageStorage::setImageData( int index, const ImageData& data )
{
//    debug( "ImageStorage::setImageData( %d, extrapolated %d )", index, data.isExtrapolated );
    d->dataVector[index] = data;
}

void ImageStorage::setDetectionParams( int a_iHeadW, int a_iHeadH, int a_iBodyW, double a_dBodyP )
{
    d->m_iHeadW = a_iHeadW;
    d->m_iHeadH = a_iHeadH;
    d->m_iBodyW = a_iBodyW;
    d->m_dBodyP = a_dBodyP;
}
