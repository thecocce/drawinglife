/*=======================================================
 Copyright (c) avp::ptr, 2010
=======================================================*/

#include "DrawingLifeApp.h"
#include "GpsData.h"
#include <vector>
//--------------------------------------------------------------
DrawingLifeApp::DrawingLifeApp(const ofxXmlSettings& settings) :
    m_settings(settings),
    m_dbReader(NULL),
    m_gpsData(NULL),
    m_isFullscreen(false),
    m_isDebugMode(false),
    m_isAnimation(true),
    m_zoomX(0.0),
    m_zoomY(0.0),
    m_zoomZ(0.0),
    m_startScreenMode(false)
{
    m_viewXOffset = 0;
    m_viewYOffset = 0;
    m_viewMinDimension = 0;
    m_viewPadding = 15;
}
DrawingLifeApp::~DrawingLifeApp()
{
    SAFE_DELETE(m_gpsData);
}
void DrawingLifeApp::setup()
{
    m_fontTitle.loadFont("mono.ttf", 50);
    m_fontAuthor.loadFont("mono.ttf",24);
    m_fontText.loadFont("mono.ttf",18);

    ofBackground((BACKGROUND >> 16) & 0xFF, (BACKGROUND >> 8) & 0xFF, (BACKGROUND) & 0xFF);
    this->setViewAspectRatio();

    ofSetFrameRate(60);
    ofEnableAlphaBlending();

    // reading settings from xml file
    //m_settings.loadFile("AppSettings.xml");
    //	ofSetLogLevel(m_settings.getAttribute("settings:log", "level", 0));
    ofSetLogLevel(OF_LOG_VERBOSE);
    // db path must be absolute path for DBReader (true as second parameter)
    m_dbPath = ofToDataPath(m_settings.getValue("settings:database", "test.sqlite"), true);

    DBG_VAL(m_dbPath);
    m_gpsData = new GpsData();
    if (m_settings.getValue("settings:data:loadonstart",1) == 1)
    {
        string city = m_settings.getValue("settings:data:defaultcity", "London");
        loadGpsDataCity(city);
        if(m_gpsData->getTotalGpsPoints() == 0)
        {
            m_startScreenMode = true;
        }
    }
    else
    {
        m_startScreenMode = true;
    }
}

//--------------------------------------------------------------
void DrawingLifeApp::update()
{
    m_gpsData->update();
}

//--------------------------------------------------------------
void DrawingLifeApp::draw()
{
    if (m_startScreenMode)
    {
        drawStartScreen();
    }
    else
    {
        if (m_isAnimation)
        {
            // -----------------------------------------------------------------------------
            // Draw rectangle with text.
            // -----------------------------------------------------------------------------
            fillViewAreaUTM( VIEWBOX);
            //---------------------------------------------------------------------------
            ofFill();
            ofSetColor(0xE5A93F);
            ofRect(10,10,300,120);
            ofSetColor(0x000000);
            ofDrawBitmapString(m_gpsData->getCurrentGpsInfo(),30,30);
            // -----------------------------------------------------------------------------
            // Draw Gps data
            // -----------------------------------------------------------------------------
            ofSetColor(FOREGROUND);
            ofNoFill();
            glTranslated(m_zoomX, m_zoomY, m_zoomZ);
            m_gpsData->draw();
        }
        else
        {
            fillViewAreaUTM( VIEWBOX);
            // -----------------------------------------------------------------------------
            // Draw Gps data
            // -----------------------------------------------------------------------------
            ofSetColor(FOREGROUND);
            ofNoFill();
            glTranslated(m_zoomX, m_zoomY, m_zoomZ);
            m_gpsData->draw(false);
        }
    }
}
// -----------------------------------------------------------------------------
// Start screen
// -----------------------------------------------------------------------------
void DrawingLifeApp::drawStartScreen()
{
    ofSetColor(255,255,255);

    string title = APP_NAME_STR;
    title += " ";
    title += APP_VERSION_STR;
    m_fontTitle.drawString(title, ofGetWidth()/2 - 365, ofGetHeight()/2 - 100);

    m_fontAuthor.drawString(APP_AUTHOR_STR, ofGetWidth()/2 - 91, ofGetHeight()/2);

    m_fontText.drawString("Press key 0 - 9 to choose a life map.", ofGetWidth()/2 - 300, ofGetHeight()/2 + 250);
}
// -----------------------------------------------------------------------------
// Retrieving new GpsData
// -----------------------------------------------------------------------------
void DrawingLifeApp::loadGpsDataCity(string city)
{
    m_startScreenMode = false;
    // get GpsData from database
//    m_gpsData->clear();
    SAFE_DELETE(m_gpsData);
    m_gpsData = new GpsData();
    m_gpsData->setViewBounds(ofGetWidth(), ofGetHeight(), m_viewXOffset, m_viewYOffset, m_viewMinDimension, m_viewPadding);
    m_gpsData->reset();

    m_dbReader = new DBReader(m_dbPath);
    if (m_dbReader->setupDbConnection())
    {
        // -----------------------------------------------------------------------------
        // DB query
		// TODO: Query needs to match with original database query used in the setup function.
//        if(m_dbReader->getGpsDataDayRange(*m_gpsData, "Dan", 2010, 2, m_currentSelectedDayStart, m_currentSelectedDayEnd))
        if(m_dbReader->getGpsDataCity(*m_gpsData, "Dan", city))
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
        m_dbReader->closeDbConnection();
    }
    // -----------------------------------------------------------------------------
    SAFE_DELETE(m_dbReader);
    // test print
    maxPoints = 0;
    for (unsigned int i = 0; i < m_gpsData->getSegments().size(); ++i)
    {
        for (unsigned int j = 0; j < m_gpsData->getSegments()[i].getPoints().size(); ++j)
        {
//            stringstream message;
//            //message << "Value i " << i << ", j " << j << ", k " << k <<": ";
//            message << "GpsPoint nr " << maxPoints << ": ";
//            message << m_gpsData->getSegments()[i].getPoints()[j].getUtmY();
//            message << ", ";
//            message << m_gpsData->getSegments()[i].getPoints()[j].getUtmX();
//            message << ", ";
//            message << m_gpsData->getSegments()[i].getPoints()[j].getElevation();
//            message << ", ";
//            message << m_gpsData->getSegments()[i].getPoints()[j].getTimestamp();
//            message << ", ";
//            message << m_gpsData->getSegments()[i].getSegmentNum();
//            ofLog(OF_LOG_NOTICE, message.str() );
            ++maxPoints;
        }
    }

    DBG_VAL(city);
    ofLog(OF_LOG_VERBOSE, "minLon: %lf, maxLon: %lf, minLat: %lf, maxLat: %lf",
      m_gpsData->getMinUtmX(),
      m_gpsData->getMaxUtmX(),
      m_gpsData->getMinUtmY(),
      m_gpsData->getMaxUtmY());
    ofLog(OF_LOG_VERBOSE, "Central Meridian: %lf", m_gpsData->getProjectionCentralMeridian());
}
// -----------------------------------------------------------------------------

void DrawingLifeApp::setViewAspectRatio()
{
    double width = ofGetWidth();
    double height = ofGetHeight();

    // Reset for view padding.
    m_viewXOffset = 0;
    m_viewYOffset = 0;

    // Set square view area and center.
    if (height < width)
    {
        m_viewMinDimension = height;
        m_viewXOffset = (width - height) / 2.0;
    }
    else if (width < height)
    {
        m_viewMinDimension = width;
        m_viewYOffset = (height - width) / 2.0;
    }
    else
    {
        m_viewMinDimension = width;
    }

    // Left and top indentation.
    m_viewXOffset += m_viewPadding;
    m_viewYOffset += m_viewPadding;
}
void DrawingLifeApp::fillViewAreaUTM( int backgroundColor)
{
	// Normalized value range from 0 to 1.
    double x = m_gpsData->getScaledUtmX(0);
    double y = m_gpsData->getScaledUtmY(0);
    double w = m_gpsData->getScaledUtmX(1) - x;
    double h = m_gpsData->getScaledUtmY(1) - y;
    ofFill();
    ofSetColor( backgroundColor);
    ofRect(x, y, w, h);
}

//--------------------------------------------------------------
void DrawingLifeApp::keyPressed  (int key)
{
    switch (key)
    {
    case 'a':
        m_isAnimation = !m_isAnimation;
        break;
    case 'f':
        m_isFullscreen = !m_isFullscreen;
        ofSetFullscreen(m_isFullscreen);
        break;
    case 49:
        loadGpsDataCity("Berlin");
        break;
    case 50:
        loadGpsDataCity("London");
        break;
    case 51:
        loadGpsDataCity("Barcelona");
        break;
    case 52:
        loadGpsDataCity("Hamburg");
        break;
    case 53:
        loadGpsDataCity("Vienna");
        break;
    case 54:
        loadGpsDataCity("New York");
        break;
    case 55:
        loadGpsDataCity("Tokyo");
        break;
    case 56:
        loadGpsDataCity("San Francisco");
        break;
    case 57:
        loadGpsDataCity("Bristol");
        break;
    case 48:
        loadGpsDataCity("Banff");
        break;
    case 'w':
        if(m_zoomZ > 590 && m_zoomZ < 598)
        {
            m_zoomZ += 1;
        }
        else if (m_zoomZ >= 598)
        {
            m_zoomZ += 0.05;
        }
        else if (m_zoomZ < 599)
        {
            m_zoomZ += 10;
        }
        DBG_VAL(m_zoomZ);
        break;
    case 's':
        if(m_zoomZ > 590)
        {
            m_zoomZ -= 1;
        }
        else
        {
            m_zoomZ -= 10;
        }

        DBG_VAL(m_zoomZ);
        break;
    case OF_KEY_UP:
        m_zoomY += 50;
        break;
    case OF_KEY_DOWN:
        m_zoomY -= 50;
        break;
    case OF_KEY_RIGHT:
        m_zoomX -= 50;
        break;
    case OF_KEY_LEFT:
        m_zoomX += 50;
        break;
    default:
        break;
    }
}

//--------------------------------------------------------------
void DrawingLifeApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void DrawingLifeApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void DrawingLifeApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void DrawingLifeApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void DrawingLifeApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void DrawingLifeApp::windowResized(int w, int h)
{
    this->setViewAspectRatio();
    m_gpsData->setViewBounds(ofGetWidth(), ofGetHeight(), m_viewXOffset, m_viewYOffset, m_viewMinDimension, m_viewPadding);
}

