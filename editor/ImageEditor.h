/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#ifndef QIMAGE_H
#include <qimage.h>
#endif
#ifndef QCANVASVIEW_H
#include <qcanvas.h> 
#endif
#ifndef QPOINTARRAY_H
#include <qpointarray.h>
#endif

struct ImageData
{
    QPointArray head;
    QPointArray body;
    QPointArray legs;
    QPointArray hit;
    QPoint imageOffset;
    QSize imageSize;
    bool isExtrapolated;
    
    ImageData();
    ImageData& operator=( const ImageData& );
};

struct ImageEditor_P;
class QImage;

class ImageEditor: public QCanvasView
{
    Q_OBJECT
public:
    ImageEditor( QWidget * parent = 0, const char * name = 0, WFlags f = 0 );
    ~ImageEditor();
    
    virtual void		setImage( const QImage& image );
    virtual ImageData	getData();
    virtual void		setData( ImageData data );
    
signals:
    void			extrapolated( bool );
    
protected:
    virtual void 		contentsMousePressEvent ( QMouseEvent * e );
    virtual void 		contentsMouseMoveEvent ( QMouseEvent * e );
    virtual void 		contentsMouseReleaseEvent( QMouseEvent * e );
    
private:
    ImageEditor_P	*	p;
};

#endif
