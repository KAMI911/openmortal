/***************************************************************************
                          Background.h  -  description
                             -------------------
    begin                : Sun Jan 11 2004
    copyright            : (C) 2004 by upi
    email                : upi@feel
 ***************************************************************************/

#ifndef __BACKGROUND_H
#define __BACKGROUND_H


#include <vector>
struct SDL_Surface;

struct BackgroundLayer
{
	SDL_Surface*	m_poSurface;
	int				m_iXOffset;
	int				m_iYOffset;
	double			m_dDistance;
};
typedef std::vector<BackgroundLayer> LayerVector;
typedef LayerVector::iterator LayerIterator;



class Background
{
public:
	Background();
	~Background();

	void		Clear();
	void		Load( int a_iBackgroundNumber );
	bool		IsOK();
	void		Draw( int a_iXPosition, int a_iYPosition );

protected:
	int			m_iNumber;
	bool		m_bOK;
	LayerVector	m_aLayers;
};

#endif // __BACKGROUND_H
