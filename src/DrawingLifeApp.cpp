/*=======================================================
 Copyright (c) avp::ptr, 2010
=======================================================*/

#include "DrawingLifeApp.h"
#include "Data.h"
#include <vector>
//--------------------------------------------------------------
DrawingLifeApp::~DrawingLifeApp()
{
	if (m_gpsData != NULL) 
	{
		delete m_gpsData;
	}
}
void DrawingLifeApp::setup(){
	
	ofSetFrameRate(60);

	// reading settings from xml file
	m_settings.loadFile("AppSettings.xml");
//	ofSetLogLevel(m_settings.getAttribute("settings:log", "level", 0));
	ofSetLogLevel(OF_LOG_VERBOSE);
	// db path must be absolute path for DBReader (true as second parameter)
	string dbPath = ofToDataPath(m_settings.getValue("settings:database", "test.db"), true);
	
	DBG_VAL(dbPath);
	
	m_gpsData = new GpsData();
	
	// get GpsData from database
	m_dbReader = new DBReader(dbPath);
	m_dbReader->setupDbConnection();
	// -----------------------------------------------------------------------------
	// DB query
	if(m_dbReader->getGpsDataDay(*m_gpsData, "Dan", 2010, 2, 9))
	{
		ofLog(OF_LOG_SILENT, "--> GpsData load ok!");
		ofLog(OF_LOG_SILENT, "--> Total data: %d GpsSegments, %d GpsPoints!", 
			  m_gpsData->getSegments().size(),
			  m_gpsData->getTotalGpsPoints());
	}
	else
	{
		ofLog(OF_LOG_SILENT, "--> No GpsData loaded!");
	}
	// -----------------------------------------------------------------------------
	m_dbReader->closeDbConnection();
	delete m_dbReader;
	
	// test print
	maxPoints = 0;
//	for (unsigned int i = 0; i < m_gpsData->getSegments().size(); ++i) {
//		for (unsigned int j = 0; j < m_gpsData->getSegments()[i].getPoints().size(); ++j) {
//			stringstream message;
//			//message << "Value i " << i << ", j " << j << ", k " << k <<": ";
//			message << "GpsPoint nr " << maxPoints << ": ";
//			message << m_gpsData->getSegments()[i].getPoints()[j].getLatitude();
//			message << ", ";
//			message << m_gpsData->getSegments()[i].getPoints()[j].getLongitude();
//			message << ", ";
//			message << m_gpsData->getSegments()[i].getPoints()[j].getElevation();
//			message << ", ";
//			message << m_gpsData->getSegments()[i].getPoints()[j].getTimestamp();
//			message << ", ";
//			message << m_gpsData->getSegments()[i].getSegmentNum();
//			ofLog(OF_LOG_NOTICE, message.str() );
//			++maxPoints;
//		}
//	}
	// printing min/max values
	ofLog(OF_LOG_VERBOSE, "minLon: %lf, maxLon: %lf, minLat: %lf, maxLat: %lf", 
		  m_gpsData->getMinLon(), 
		  m_gpsData->getMaxLon(),
		  m_gpsData->getMinLat(),
		  m_gpsData->getMaxLat());
	
	m_minLon = m_gpsData->getMinLon();
	m_maxLon = m_gpsData->getMaxLon();
	m_minLat = m_gpsData->getMinLat();
	m_maxLat = m_gpsData->getMaxLat();
	
	// Because the above test print is so slow, here you can prove 
	// that the data have been read by showing las gpsData
//	ofLog(OF_LOG_NOTICE, m_gpsData->getSegments().back().getPoints().back().getTimestamp());
	

	ofBackground(0, 0, 0);
	
	m_currentGpsPoint = 0;
	m_currentGpsSegment = 0;
	m_currentPoint = -1;
	m_firstPoint = true;
}

//--------------------------------------------------------------
void DrawingLifeApp::update(){

	// Counting through GpsSegments and GpsPoints
	if (m_gpsData->getSegments().size() > 0) 
	{
		if ((unsigned int)m_currentGpsSegment < m_gpsData->getSegments().size()-1) 
		{
			if ((unsigned int)m_currentGpsPoint < m_gpsData->getSegments()[m_currentGpsSegment].getPoints().size() - 1) 
			{
				if (!m_firstPoint) 
					++m_currentGpsPoint;
				else
					m_firstPoint = false;
			}
			else
			{
				++m_currentGpsSegment;
				m_currentGpsPoint = 0;
			}
		}
		else
		{
			if ((unsigned int)m_currentGpsPoint < m_gpsData->getSegments()[m_currentGpsSegment].getPoints().size() - 1) 
			{
				++m_currentGpsPoint;
			}
			else
			{
				m_currentGpsPoint = 0;
				m_currentGpsSegment = 0;
				m_currentPoint = -1;				
			}
		}
		++m_currentPoint;
	}	
}


//--------------------------------------------------------------
void DrawingLifeApp::draw(){
	
	
	if (m_gpsData->getSegments().size() > 0 && m_gpsData->getSegments()[m_currentGpsSegment].getPoints().size() > 0)
	{
		// -----------------------------------------------------------------------------
		// Draw rectangle with text.
		// -----------------------------------------------------------------------------
		double lat = m_gpsData->getSegments()[m_currentGpsSegment].getPoints()[m_currentGpsPoint].getLatitude();
		double lon = m_gpsData->getSegments()[m_currentGpsSegment].getPoints()[m_currentGpsPoint].getLongitude();
		double ele = m_gpsData->getSegments()[m_currentGpsSegment].getPoints()[m_currentGpsPoint].getElevation();
		string timest = m_gpsData->getSegments()[m_currentGpsSegment].getPoints()[m_currentGpsPoint].getTimestamp();
		string info =	"Longitude  : " + ofToString(lon) + "\n" +
						"Latitude   : " + ofToString(lat) + "\n" +
						"Elevation  : " + ofToString(ele) + "\n" +
						"Time       : " + timest + "\n" +
						"Cur. point : " + ofToString(m_currentPoint) + "\n" +
						"Segment nr : " + ofToString(m_gpsData->getSegments()[m_currentGpsSegment].getSegmentNum());
		
//		ofLog(OF_LOG_VERBOSE, ofToString(m_currentPoint) + " " + ofToString(m_gpsData->getSegments()[m_currentGpsSegment].getSegmentNum()));
		ofFill();
		ofSetColor(0xE5A93F);
		ofRect(10,10,300,100);
		ofSetColor(0x000000);
		ofDrawBitmapString(info,30,30);
		
		// -----------------------------------------------------------------------------
		// Draw Gps data
		// -----------------------------------------------------------------------------
		ofSetColor(0xffffff);
		ofNoFill();
		for (unsigned int i = 0; i <= m_currentGpsSegment; ++i) 
		{
			ofBeginShape();
			int pointEnd;
			if (i == m_currentGpsSegment)
				pointEnd = m_currentGpsPoint;
			else
				pointEnd = (int)m_gpsData->getSegments()[i].getPoints().size()-1;
			for (unsigned int j = 0; j <= pointEnd; ++j) 
			{
				//				ofVertex(getNormalizedLongitude(m_gpsData->getSegments()[i].getPoints()[j].getLongitude()), 
				//						   getNormalizedLatitude(m_gpsData->getSegments()[i].getPoints()[j].getLatitude()));
				ofVertex(getNormalizedLongitude(m_gpsData->getLongitude(i,j)), 
						 getNormalizedLatitude(m_gpsData->getLatitude(i,j)));
				//				ofLog(OF_LOG_VERBOSE, "currentSeg: %d, m_currentPoint: %d, lon: %lf, lat: %lf",
				//					  m_currentGpsSegment, 
				//					  m_currentGpsPoint,
				//					  tmpPt.x,
				//					  tmpPt.y);			
			}
			ofEndShape();
		}
	}
	
	
	
}
// -----------------------------------------------------------------------------
double DrawingLifeApp::getNormalizedLatitude(double lat)
{
	return ( (lat - m_minLat) / (m_maxLat - m_minLat) * (ofGetHeight()-30) );
}

double DrawingLifeApp::getNormalizedLongitude(double lon)
{
	return ( (lon - m_minLon) / (m_maxLon - m_minLon) * (ofGetWidth()-30) ); 
}

//--------------------------------------------------------------
void DrawingLifeApp::keyPressed  (int key){

}

//--------------------------------------------------------------
void DrawingLifeApp::keyReleased(int key){

}

//--------------------------------------------------------------
void DrawingLifeApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void DrawingLifeApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void DrawingLifeApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void DrawingLifeApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void DrawingLifeApp::resized(int w, int h){

}

