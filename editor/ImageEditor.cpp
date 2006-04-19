/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#include <qpen.h>
#include <qpainter.h>
#include "ImageEditor.h"

#define SIZERW 7
#define SIZERW2 3
ImageData::ImageData()
{ 
    head.resize(4); body.resize(4); legs.resize(6); hit.resize(4); isExtrapolated = true; 
    
    head.setPoint( 0, 100, 100 );
    head.setPoint( 1, 150, 100 );
    head.setPoint( 2, 150, 150 );
    head.setPoint( 3, 100, 150 );
    
    body.setPoint( 0, 100, 160 );
    body.setPoint( 1, 100, 250 );
    body.setPoint( 2, 150, 250 );
    body.setPoint( 3, 150, 160 );
    
    legs.setPoint( 0, 150, 260 );
    legs.setPoint( 1, 180, 300 );
    legs.setPoint( 2, 190, 400 );
    legs.setPoint( 3,  60, 400 );
    legs.setPoint( 4,  70, 300 );
    legs.setPoint( 5, 100, 260 );
    
    hit.setPoint( 0, 10, 10 );
    hit.setPoint( 1, 20, 10 );
    hit.setPoint( 2, 20, 20 );
    hit.setPoint( 3, 10, 20 );
}

ImageData& ImageData::operator=( const ImageData& d )
{
    int i;
    for ( i=0; i<4; ++i )
    {
	head[i] = d.head[i];
	body[i] = d.body[i];
	hit[i] = d.hit[i];
    }
    
    for ( i=0; i<6; ++i )
    {
	legs[i] = d.legs[i];
    }
    imageOffset = d.imageOffset;
    imageSize = d.imageSize;
    isExtrapolated = d.isExtrapolated;
    
    return *this;
}

enum 
{
    CANVASSIZER_RTTI = 759238,
    DRAGGABLEPOLYGON_RTTI = 25039,
    DRAGGABLESPRITE_RTTI = 592783,
};

#define POSITION(A) (A->rtti() == CANVASSIZER_RTTI) ? ((CanvasSizer*)A)->position() : \
(A->rtti() == DRAGGABLEPOLYGON_RTTI)  ? ((DraggablePolygon*)A)->position() : \
((DraggableSprite*)A)->position()

#define MOVETO(A,B) (A->rtti() == CANVASSIZER_RTTI) ? ((CanvasSizer*)A)->moveTo(B) : \
(A->rtti() == DRAGGABLEPOLYGON_RTTI)  ? ((DraggablePolygon*)A)->moveTo(B) : \
((DraggableSprite*)A)->moveTo(B);

/*
class CanvasDraggable
{
public:
    virtual QPoint position() { return QPoint(0,0); }
    virtual void reread() {}
    virtual void moveTo( const QPoint& pos ) { debug( "CanvasDraggable::moveTo(%d,%d)", pos.x(), pos.y() ); }
};
*/

class DraggableSprite: public QCanvasSprite
{
public:
    DraggableSprite( QCanvasPixmapArray * a, QCanvas* canvas, QPoint* _point )
	    : QCanvasSprite( a, canvas )
    {
	point = _point;
	reread();
    }
    
    virtual QPoint position() { return *point ; }
    virtual void reread() { setX( point->x() ); setY( point->y() ); }
    virtual void moveTo( const QPoint& pos ) { *point = pos; }
    virtual int rtti() const
    {
	return DRAGGABLESPRITE_RTTI;
    }
protected:
    QPoint* point;
};

class DraggablePolygon: public QCanvasPolygon
{
public:
    DraggablePolygon( QCanvas* canvas, QPointArray* _array ) :
	    QCanvasPolygon( canvas )
    {
	array = _array;
	setPoints( *array );
    }
    
    virtual QPoint position() { return (*array)[0]; }
    virtual void reread() { setPoints( *array ); }
    virtual void moveTo( const QPoint& pos )
    {
	QPoint _delta = pos - position();
	if ( _delta.manhattanLength() == 0 )
	{
	    // Not moved.
	    return;
	}
	for ( uint i=0; i<array->count(); ++i )
	{
	    array->at(i) += _delta;
	    //(*array)[i] += _delta;
	    //(*array)[i] = (*array)[i] + _delta;
	}
    }
    virtual int rtti() const
    {
	return DRAGGABLEPOLYGON_RTTI;
    }
protected:
    QPointArray *array;
};

class CanvasSizer : public QCanvasRectangle
{
public:
    
    CanvasSizer( QCanvas* canvas, 
	QCanvasPolygon* _polygon, QPointArray* _array, int _polyIndex, int _index )
	    : QCanvasRectangle( canvas )
    {
	array = _array;
	index = _index;
	polygon = _polygon;
	setSize( SIZERW, SIZERW );
	reread();
	setPen( black );
	
	switch ( _polyIndex )
	{
	case 0: setBrush( red); break;
	case 1: setBrush( green ); break;
	case 2: setBrush( blue ); break;
	case 3: setBrush( yellow); break;
	}
	setZ( 20 );
	show();
    }
    
    virtual QPoint position()
    {
	return (*array)[index];
    }
    
    virtual void reread()
    {
	setX( (*array)[index].x() - SIZERW2 );
	setY( (*array)[index].y() - SIZERW2 );
	polygon->setPoints( *array );
    }   
    
    virtual void moveTo( const QPoint& p )
    {
	(*array)[index] = p;
	reread();
    }
    
    virtual int rtti() const
    {
	return CANVASSIZER_RTTI;
    }
    
protected:
    QPointArray* array;
    int index;
    QCanvasPolygon* polygon;
};

struct ImageEditor_P
{
    ImageEditor*		This;
    
    ImageData		data;
    QImage		image;
    QCanvasPixmapArray	pixmapArray;
    DraggablePolygon	*head, *body, *legs, *hit;
    DraggableSprite	*background;
    
    CanvasSizer*		headSizer[4];
    CanvasSizer*		bodySizer[4];
    CanvasSizer*		legsSizer[6];
    CanvasSizer*		hitSizer[4];
    
    bool			dragging;
    QPoint		dragOrigin;
    QPoint 		dragOffset;
    QCanvasItem*	draggable;
    
    ImageEditor_P()
    {
	head = body = legs = hit = NULL;
	background = NULL;
	
	dragging = false;
	draggable = NULL;
    }
    
    void data2Canvas()
    {
	int i;
	This->canvas()->setBackgroundColor( QColor( 64,64,64 ) );
	
	if ( !head )
	{
	    head = new DraggablePolygon( This->canvas(), & data.head );
	    head->setBrush( QBrush( Qt::red, Qt::DiagCrossPattern ) );
	    head->setZ( 10 );
	    body = new DraggablePolygon( This->canvas(), & data.body );
	    body->setBrush( QBrush( Qt::green, Qt::DiagCrossPattern ) );
	    body->setZ( 8 );
	    legs = new DraggablePolygon( This->canvas(), & data.legs );
	    legs->setBrush( QBrush( Qt::blue, Qt::DiagCrossPattern ) );
	    legs->setZ( 6 );
	    hit = new DraggablePolygon( This->canvas(), & data.hit );
	    hit->setBrush( QBrush( Qt::yellow, Qt::DiagCrossPattern ) );
	    hit->setZ( 6 );
	    
	    for ( i=0; i<4; ++i )
		headSizer[i] = new CanvasSizer( This->canvas(),
			head, & data.head, 0, i );
	    for ( i=0; i<4; ++i )
		bodySizer[i] = new CanvasSizer( This->canvas(),
			body, & data.body, 1, i );
	    for ( i=0; i<6; ++i )
		legsSizer[i] = new CanvasSizer( This->canvas(),
			legs, & data.legs, 2, i );
	    for ( i=0; i<4; ++i )
		hitSizer[i] = new CanvasSizer( This->canvas(),
			hit, & data.hit, 3, i );
	}
	
	head->setPoints( data.head );
	head->show();
	body->setPoints( data.body );
	body->show();
	legs->setPoints( data.legs );
	legs->show();
	hit->setPoints( data.hit );
	hit->show();
	
	for ( i=0; i<4; ++i ) headSizer[i]->reread();
	for ( i=0; i<4; ++i ) bodySizer[i]->reread();
	for ( i=0; i<6; ++i ) legsSizer[i]->reread();
	for ( i=0; i<4; ++i ) hitSizer[i]->reread();
	
	rereadImage();
	This->canvas()->setAllChanged();
	This->canvas()->update();
    }
    
    void setImage( const QImage& newimage )
    {
	if ( image == newimage )
	    return;
	
	image = newimage;
	pixmapArray.setImage( 0, new QCanvasPixmap( image ) );
	
	if ( ! background )
	{
	    background = new DraggableSprite( &pixmapArray, This->canvas(), & data.imageOffset );
	    background->show();
	}
	else
	{
	    background->setSequence( &pixmapArray );
	}
	rereadImage();
	//
	//This->canvas()->update();
    }
    
    void rereadImage()
    {
	background->setX( data.imageOffset.x() );
	background->setY( data.imageOffset.y() );
    }
};

ImageEditor::ImageEditor( QWidget * parent, const char * name, WFlags f )
	: QCanvasView( parent, name, f )
{
    p = new ImageEditor_P;
    p->This = this;
}

ImageEditor::~ImageEditor()
{
    delete( p );
    p = NULL;
}
    
void ImageEditor::setImage( const QImage& image )
{
    QImage img = image;    
    p->setImage( image );
} 

ImageData ImageEditor::getData()
{
    return p->data;
}

void ImageEditor::setData( ImageData data )
{
    p->data = data;
    p->data2Canvas();
    emit extrapolated( data.isExtrapolated );
    
    QPoint focalPoint = data.imageOffset;
    focalPoint += QPoint( p->image.width() / 2, p->image.height() / 2 );
    focalPoint = worldMatrix() * focalPoint;
    center( focalPoint.x(), focalPoint.y(), 0.5, 0.5 );
}

void ImageEditor::contentsMousePressEvent ( QMouseEvent * e )
{
    if ( p->dragging )
    {
	MOVETO( p->draggable, p->dragOrigin );
	p->dragging = false;
	p->data2Canvas();
	return;
    }
    
    QPoint src = inverseWorldMatrix().map(e->pos());
    
    // Find which canvas widget is there...
    
    QCanvasItemList items = canvas()->collisions ( src );
    if ( items.count() == 0 )
	return;	// Nothing is there
    
    QCanvasItem* item = items.first();
    
    p->draggable = item;
    p->dragging = true;
    p->dragOrigin = POSITION(item);
    p->dragOffset = src - p->dragOrigin;   
    
}

void ImageEditor::contentsMouseMoveEvent ( QMouseEvent * e )
{
    if ( !p->dragging )
	return;
    QPoint src = inverseWorldMatrix().map(e->pos());
    src -= p->dragOffset;
    
    MOVETO(p->draggable, src);
    p->data2Canvas();
}

void ImageEditor::contentsMouseReleaseEvent( QMouseEvent * e )
{
    contentsMouseMoveEvent( e );
    if (!p->dragging)
	return;
    p->dragging = false;
    p->draggable = false;
    p->data.isExtrapolated = false;
    emit extrapolated(false);
}

