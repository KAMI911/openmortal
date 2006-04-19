/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#ifndef QIMAGE_H
#include <qimage.h>
#endif
#ifndef QWMATRIX_H
#include <qwmatrix.h> 
#endif
#include <qfiledialog.h>

#include "RlePack.h"
#include "ImageEditor.h"
#include "ImageStorage.h"

// #define CHARNAME "CUMI"


bool bLoaded = false;
#define CHECKLOADED if ( !bLoaded ) return;


void MortalEditor::init()
{
    // Load images and image4 data
    images = NULL;
    m_sFileName = "";
    
    // Set up widgets
    lImage->hide();
    QCanvas *c = new QCanvas( this );
    c->resize( 1000, 700 );
    imageEditor = new ImageEditor( tab );
    imageEditor->setCanvas( c );
    imageEditor->setSizePolicy( QSizePolicy(
	    (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)5, 0, false ) );
    imageEditor->setFrameShape( QLabel::Panel );
    imageEditor->setFrameShadow( QLabel::Plain );
    imageEditor->setLineWidth( 1 );
    tabLayout->addWidget( imageEditor );
    
    connect( imageEditor, SIGNAL(extrapolated(bool)),
	    this, SLOT(imageEditor_extrapolated(bool)) );
    
}

void MortalEditor::load(  )
{
    delete images;
    images = new RlePack( m_sFileName );
    ImageStorage::instance().load( m_sFileName + ".txt" );
    bLoaded = true;
    currentImage = 0;
    sbImage->setMaxValue( images->count()-1 );
    sbImage->setValue(0);
    transferImage();
}

void MortalEditor::fileNew()
{

}

void MortalEditor::fileOpen()
{
    QString sOpen = QFileDialog::getOpenFileName ( QString::null, QString::null, this );
    if ( sOpen.isEmpty() )
    {
	return;
    }
    m_sFileName = sOpen;
    load();
}

void MortalEditor::fileSave()
{
    CHECKLOADED;
    ImageStorage::instance().save( m_sFileName + ".txt", images );
}

void MortalEditor::fileSaveAs()
{

}

void MortalEditor::filePrint()
{

}

void MortalEditor::fileExit()
{

}

void MortalEditor::editUndo()
{

}

void MortalEditor::editRedo()
{

}

void MortalEditor::editCut()
{

}

void MortalEditor::editCopy()
{

}

void MortalEditor::editPaste()
{

}

void MortalEditor::editFind()
{

}

void MortalEditor::helpIndex()
{

}

void MortalEditor::helpContents()
{

}

void MortalEditor::helpAbout()
{

}   

void MortalEditor::transferImage()
{
    CHECKLOADED;

    QSize size = images->getSize( currentImage );
    QImage image( size, 8, 256 );
    images->transferPalette( image );    
    image.fill( 0 );
    images->draw( image, currentImage, 0, 0 );
    image.setAlphaBuffer( true );
    image.setColor( 0, qRgba( 0,0,0,0 ) );
    
    ImageData data = ImageStorage::instance().getImageData( currentImage, images );
    
    imageEditor->setImage( image );
    imageEditor->setData( data );
}

void MortalEditor::sbImage_valueChanged( int i )
{
    CHECKLOADED;

    ImageData data = imageEditor->getData();
    debug( "Data %d extrapolated=%d", currentImage, data.isExtrapolated );
    ImageStorage::instance().setImageData( currentImage, data );
    
    currentImage = i;
    transferImage();
    updatelFrameName();
}

void MortalEditor::cbExtrapolated_toggled( bool ex )
{
    CHECKLOADED;

    if ( ex )
    {
	ImageData data;
	ImageStorage::instance().setImageData( currentImage, data );
	transferImage();
    }
    else
    {
	ImageData data = imageEditor->getData();
	data.isExtrapolated = false;
    }
}

void MortalEditor::imageEditor_extrapolated( bool ex )
{
    CHECKLOADED;
    cbExtrapolated->blockSignals( true );
    cbExtrapolated->setChecked( ex );
    cbExtrapolated->blockSignals( false );
}


void MortalEditor::tbZoomIn_clicked()
{
    CHECKLOADED;
    QWMatrix wm = imageEditor->worldMatrix();
    double _zoom = wm.m11();
    if ( _zoom >= 1.0 )
    {
	_zoom += 1.0;
	wm.setMatrix( _zoom, 0, 0, _zoom, 0, 0 );
    }
    else
    {
	wm.scale( 2.0, 2.0 );
    }
	
    imageEditor->setWorldMatrix( wm );
}



void MortalEditor::tbZoomOut_clicked()
{
    CHECKLOADED;
    QWMatrix wm = imageEditor->worldMatrix();
    double _zoom = wm.m11();
    if ( _zoom > 1.0 )
    {
	_zoom -= 1.0;
	wm.setMatrix( _zoom, 0, 0, _zoom, 0, 0 );
    }
    else
    {
	wm.scale( 0.5, 0.5 );
    }
	
    imageEditor->setWorldMatrix( wm );
}


void MortalEditor::applyFrames_clicked()
{
    CHECKLOADED;
    frameNames.clear();
    QStringList sl = QStringList::split( ',', teFrames->text() );
    QString s;
    
    for ( int i=0; i<sl.count()-1; i+=2 )
    {
	QString name = sl[i].stripWhiteSpace();
	
	int n = sl[i+1].toInt();
	
	for ( int j=0; j<n; ++j )
	{
	    s = name + QString::number(j+1);
	    frameNames.push_back(s);
	}
    }
    
    updatelFrameName();
}

void MortalEditor::updatelFrameName()
{
    CHECKLOADED;
    int i = sbImage->value();
    
    if ( i>= frameNames.count() )
    {
	lFrameName->setText( "???" );
    }
    else
    {
	lFrameName->setText( frameNames[i] );
    }
}

void MortalEditor::detectionParamChanged()
{
    CHECKLOADED;
    ImageStorage::instance().setDetectionParams(
	    sbHeadW->value(),
	    sbHeadH->value(),
	    sbBodyW->value(),
	    double(sbBodyP->value()) / 100.0 );
    transferImage();
    updatelFrameName();
}
