/***************************************************************************
This file is part of MortalEditor, the OpenMortal character editor.
This is an internal tool, no warranty or support. Use at your own risk.

Copyright 2004 UPi upi@sourceforge
 ***************************************************************************/

#ifndef OUTLINEDETECTOR_H
#define OUTLINEDETECTOR_H

class OutlineDetector
{
public:
    static ImageData doDetection( int _headW, int _headH, int _bodyW, double _bodyPercent, 
				  QImage& _image, ImageData& _data );
    
protected:
    
    OutlineDetector( int _headW, int _headH, int _bodyW, double _bodyPercent, QImage& _image );
    
    void findScanline( int _y, int& _from, int& width );
    void findMinMax( int y1, int y2, int& _min, int& _max );
    void findAverage( int y1, int y2, int& from, int& width );
    
protected:
    int headW;
    int headH;
    int bodyW;
    double bodyPercent;
    
    QImage image;
    int imageW;
    int imageH;
    ImageData data;
};

#endif
