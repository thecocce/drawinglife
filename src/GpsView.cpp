/*=======================================================
 Copyright (c) avp::ptr, 2010
=======================================================*/

#include "GpsView.h"
#include <exception>


GpsView::GpsView( unsigned int screenWidth, unsigned int screenHeight) : 
	m_gpsData( 0),
	m_walk( 0),
	m_width( 400), 
	m_height( 200),
	m_margin( 10),
	m_screenWidth( screenWidth),
	m_screenHeight( screenHeight),
	m_position( NORTHEAST),
	m_backgroundColor( MakeHexARGB( 127, 127, 127, 127)),
	m_borderColor( MakeHexARGB( 255, 255, 255, 255))
{
	setPosition( m_position);
}


GpsView::~GpsView(void)
{
	if(m_walk)
		m_walk = 0;
	if(m_gpsData)
		m_gpsData = 0;
}


void GpsView::setGpsData(GpsData* gpsData)
{
	m_gpsData = 0;
	m_gpsData = gpsData;
}


void GpsView::setWalk(Walk* walk)
{
	m_walk = walk;
}


void GpsView::setPosition( const Point2D positionCoordinates)
{
	m_positionCoordinates = positionCoordinates;
}


void GpsView::setPosition( const Position position)
{
	m_position = position;

	switch( position)
	{
	case NORTHWEST:
		m_positionCoordinates = MakePoint2D( m_margin, m_margin);
		break;
	case NORTHEAST:
		m_positionCoordinates = MakePoint2D( m_screenWidth - m_width - m_margin, m_margin);
		break;
	case SOUTHWEST:
		m_positionCoordinates = MakePoint2D( m_margin, m_screenHeight - m_height - m_margin);
		break;
	case SOUTHEAST:
		m_positionCoordinates = MakePoint2D( m_screenWidth - m_width - m_margin, m_screenHeight - m_height - m_margin);
		break;
	case NORTH:
		m_positionCoordinates = MakePoint2D( m_screenWidth * 0.5f - m_width * 0.5f, m_margin);
		break;
	case EAST:
		m_positionCoordinates = MakePoint2D( m_margin, m_screenHeight * 0.5f - m_height * 0.5f);
		break;
	case WEST:
		m_positionCoordinates = MakePoint2D( m_screenWidth - m_width - m_margin, m_screenHeight * 0.5f - m_height * 0.5f);
		break;
	case SOUTH:
		m_positionCoordinates = MakePoint2D( m_screenWidth * 0.5f - m_width * 0.5f, m_screenHeight - m_height - m_margin);
		break;
	case CENTER:
		m_positionCoordinates = MakePoint2D( m_screenWidth * 0.5f - m_width * 0.5f, m_screenHeight * 0.5f - m_height * 0.5f);
		break;
	default:
		throw std::runtime_error("Undefined position");
		break;
	}
}


void GpsView::setMargin( const float margin)
{
	m_margin = margin;
}


void GpsView::setDimensions( const float width, const float height)
{
	m_width = width;
	m_height = height;
}


void GpsView::setBackgroundColor( const ColorRGBA backgroundColor)
{
	m_backgroundColor = MakeHexARGB( backgroundColor);
}


void GpsView::setBorderColor( const ColorRGBA borderColor)
{
	m_borderColor = MakeHexARGB( borderColor);
}


void GpsView::setScreenDimensions( unsigned int screenWidth, unsigned int screenHeight)
{
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	setPosition( m_position);
}