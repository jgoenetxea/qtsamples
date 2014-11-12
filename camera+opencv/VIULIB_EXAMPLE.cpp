#include "omf.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "setup_strings.h"
#include "bpModel.h"

//#define DEBUG

#define APPROACH_SELECTION 2	// CLM:1, SDM:2

using namespace std;
using namespace cv;
using namespace viulib::omf;

string inputVideoName =	"";
						//viulib::SetupStrings::ExtrasPath + "data/videos/IMG_0116.MOV";
int inputType = CV_CAP_DSHOW; // CV_CAP_OPENNI;CV_CAP_DSHOW

bool flipImageVert = false;
bool flipImageHori = true;
bool paused = false, detect = false, fullReset = true, track = false;
Mat frame, prevFrame, greyFrame, greyDownscaledFrame, matchMap, dispCap, dispMap;
int normWidth = 320;

// Multi-component face object
MCODFace *mcFace = OMFFactory::instance()->createMCODFace();
string faceDetectorName;// = viulib::SetupStrings::ExtrasPath + "data/detection/haarcascade_frontalface_alt2.xml";
string lEyeDetectorName;// = viulib::SetupStrings::ExtrasPath + "data/detection/haarcascade_mcs_lefteye.xml";
string rEyeDetectorName;// = viulib::SetupStrings::ExtrasPath + "data/detection/haarcascade_mcs_righteye.xml";
string noseDetectorName;// = viulib::SetupStrings::ExtrasPath + "data/detection/haarcascade_mcs_nose.xml";
string mouthDetectorName;// = viulib::SetupStrings::ExtrasPath + "data/detection/haarcascade_mcs_mouth.xml";
vector< vector<Rect> > mcFaceRect;

string trainFile;// = viulib::SetupStrings::ExtrasPath + "data/clm/MUCT_full_default.clm";

bool video = false;
VideoWriter vw;

// Facial point detectors
FPDetector *fpDetector = OMFFactory::instance()->createFPDetector();
#if APPROACH_SELECTION == 1
CLMFitter *fitter = OMFFactory::instance()->createCLMFitter();
#else
SDMFitter *fitter = OMFFactory::instance()->createSDMFitter();
#endif

// Online Appearance Model fitter
OAMFitter *oamFitter = OMFFactory::instance()->createOAMFitter();
ImgEnhancer *imgEnhancer = OMFFactory::instance()->createImgEnhancer();

// Animation restrictions for the Candide3mod
vector<float> maxAUValues, minAUValues;

// Main function
int main( int argc, const char **argv )
{
	faceDetectorName = viulib::SetupStrings::ExtrasPath + "data/detection/haarcascade_frontalface_alt2.xml";

	namedWindow( "Tracking" );
	mcFace->loadMCFaceDetector( false, faceDetectorName, lEyeDetectorName, rEyeDetectorName, noseDetectorName, 
		mouthDetectorName );

#if APPROACH_SELECTION == 1
	trainFile = viulib::SetupStrings::ExtrasPath + "data/clm/MUCT_full_default.clm";

	vector<int> clmSWSizesInit( 3 );
	clmSWSizesInit[0] = 15;
	clmSWSizesInit[1] = 13;
	clmSWSizesInit[2] = 11;

	int modes;
	fitter->load( trainFile, modes );

	vector<Point2f> targetModelPointLocations( 3 );
	vector<int> targetModelPointIndices( 3 );
	targetModelPointIndices[0] = 31;
	targetModelPointIndices[1] = 36;
	targetModelPointIndices[2] = 66;

#else
	trainFile = viulib::SetupStrings::ExtrasPath + "data/sdm/MUCT_1000_80_200.sdm";

	fitter->load( trainFile );
#endif

	// Load Candide3mod2
	vector<int> ignoredVertices;

	//// FOREHEAD
	/*ignoredVertices.push_back( 0 );
	ignoredVertices.push_back( 1 );
	ignoredVertices.push_back( 10 );
	ignoredVertices.push_back( 11 );
	//ignoredVertices.push_back( 12 );
	//ignoredVertices.push_back( 13 );
	ignoredVertices.push_back( 33 );
	ignoredVertices.push_back( 35 );
	ignoredVertices.push_back( 36 );
	//ignoredVertices.push_back( 37 );
	//ignoredVertices.push_back( 38 );
	*/

	// MOUTH
	//ignoredVertices.push_back( 6 );
	//ignoredVertices.push_back( 7 );
	//ignoredVertices.push_back( 8 );
	//ignoredVertices.push_back( 9 );
	//ignoredVertices.push_back( 27 );
	//ignoredVertices.push_back( 29 );
	//ignoredVertices.push_back( 30 );
	//ignoredVertices.push_back( 31 );
	//ignoredVertices.push_back( 32 );
	//ignoredVertices.push_back( 34 );
	//ignoredVertices.push_back( 52 );
	//ignoredVertices.push_back( 54 );
	//ignoredVertices.push_back( 55 );
	//ignoredVertices.push_back( 56 );
	//ignoredVertices.push_back( 57 );
	//ignoredVertices.push_back( 61 );
	//ignoredVertices.push_back( 62 );
	//ignoredVertices.push_back( 63 );
	//ignoredVertices.push_back( 64 );
	//ignoredVertices.push_back( 65 );
	//ignoredVertices.push_back( 66 );
	//ignoredVertices.push_back( 67 );
	//ignoredVertices.push_back( 68 );
	//ignoredVertices.push_back( 69 );
	//ignoredVertices.push_back( 70 );
	//ignoredVertices.push_back( 71 );
	//ignoredVertices.push_back( 72 );
	//ignoredVertices.push_back( 73 );
	//ignoredVertices.push_back( 74 );

	// Used shape units
	vector<string> bpInitSUs;
	//bpInitSUs.push_back( "Head Height" );
	bpInitSUs.push_back( "Eyes Vertical Position" );
	bpInitSUs.push_back( "Eyes Width" );
	bpInitSUs.push_back( "Eyes Height" );
	bpInitSUs.push_back( "Eyes Separation Distance" );
	bpInitSUs.push_back( "Eyebrows Vertical Position" );
	//bpInitSUs.push_back( "Nose Z Extension" );
	bpInitSUs.push_back( "Nose Vertical Position" );
	//bpInitSUs.push_back( "Nose Pointing Up" );
	bpInitSUs.push_back( "Mouth Vertical Position" );
	bpInitSUs.push_back( "Mouth Width" );
	//bpInitSUs.push_back( "Jaw Width" );
	//bpInitSUs.push_back( "Face Roundness" );
	bpInitSUs.push_back( "Eyebrow Width" );
	bpInitSUs.push_back( "Eyebrow Separation" );
	bpInitSUs.push_back( "Nose Width" );
	bpInitSUs.push_back( "Lip Thickness" );

	// Used animation units
	vector<string> trackAUs;
	trackAUs.push_back( "Open Mouth" );
	trackAUs.push_back( "Stretch Lips" );
	trackAUs.push_back( "Lower Eyebrows" );
	trackAUs.push_back( "Smile" );
	trackAUs.push_back( "Raise Outer Left Eyebrow" );
	trackAUs.push_back( "Raise Outer Right Eyebrow" );
	trackAUs.push_back( "Close Eyes" );

	vector<string> bpInitAUs( 2 );
	bpInitAUs[0] = "Raise Outer Left Eyebrow";
	bpInitAUs[1] = "Raise Outer Right Eyebrow";

	// Weight list for initialization
	vector<float> bpInitVertWeights;
	// Relation between loaded object model and detected points
	vector<int> bpInitModelVertIds;
	BPModel::getModelConfiguration( bpInitModelVertIds, bpInitVertWeights );
		
	// Eye data
	EyeData lEyeData, rEyeData;

	// Define left eye data
	lEyeData.eyeCenterIndex = 96;
	lEyeData.lCornerIndex = 19;
	lEyeData.rCornerIndex = 22;
	lEyeData.eyeGlobeCenterIndex = 98;

	// Define right eye data
	rEyeData.eyeCenterIndex = 97;
	rEyeData.lCornerIndex = 47;
	rEyeData.rCornerIndex = 44;
	rEyeData.eyeGlobeCenterIndex = 99;

	int sigma = 2;
	int smoothing = 19;
	float centerThresh = 200;
	float closingThresh = 50;
	float closingRatio = 1.f;

	// Set the restriction values as vector
	maxAUValues.push_back( 1.f );	// Open Mouth
	maxAUValues.push_back( 1.f );	// Strech Lips
	maxAUValues.push_back( 1.f );	// Lower Eyebrows
	maxAUValues.push_back( 1.f );	// Smile
	maxAUValues.push_back( 1.f );	// Raise Outer Left Eyebrow
	maxAUValues.push_back( 1.f );	// Raise Outer Right Eyebrow
	maxAUValues.push_back( 1.f );	// Close Eyes

	minAUValues.push_back( 0.f );	// Open Mouth
	minAUValues.push_back( -1.f );	// Strech Lips
	minAUValues.push_back( -1.f );	// Lower Eyebrows
	minAUValues.push_back( -1.f );	// Smile
	minAUValues.push_back( -1.f );	// Raise Outer Left Eyebrow
	minAUValues.push_back( -1.f );	// Raise Outer Right Eyebrow
	minAUValues.push_back( -1.f );	// Close Eyes

	vector<float> configuration;
	vector<Point3f> modelPoints;
	vector<Point2f> bpInitVertProj;
	vector<Point3f> initVertPos;
	Point2f eyeGazeCameraCoords;
	
	int patchWidth = 70;
	int patchHeight = 50;
	oamFitter->load( viulib::SetupStrings::ExtrasPath + "data/oam/candide3fit_extended.wfm", ignoredVertices, 40, 6, 96, 97, patchWidth, patchHeight);

	VideoCapture capture;
	if( inputVideoName == "" ) capture.open( inputType );
	else capture.open( inputVideoName );

	// Main loop
	if( capture.isOpened() )
	{
		int64 t1, t0 = cvGetTickCount();
		int fnum = 0;
		float fps = 0, au57 = 0;

		while( true )
		{
			// Grab image
			if( !paused )
			{
				capture.grab();
				
				if( inputType == CV_CAP_OPENNI || inputType == CV_CAP_OPENNI_ASUS )
				{
					capture.retrieve( frame, CV_CAP_OPENNI_BGR_IMAGE );
					capture.retrieve( dispCap, CV_CAP_OPENNI_DISPARITY_MAP_32F );

					if( dispMap.rows != dispCap.rows || dispMap.cols != dispCap.cols )
						dispMap = Mat( dispCap.rows, dispCap.cols, CV_8U );

					if( dispCap.data != 0 )
					{
						double minVal, maxVal;
						minMaxLoc( dispCap, &minVal, &maxVal );
						double scale = 255.0/(maxVal-minVal);
						double shift = -minVal*255./(maxVal-minVal);
						dispCap.convertTo( dispMap, dispMap.depth(), scale, shift );
						imshow( "Disparity Map", dispMap );
					}
				}

				else capture.retrieve( frame );
				if( flipImageVert ) flip( frame, frame, 0 );
				if( flipImageHori ) flip( frame, frame, 1 );
				frame.copyTo( prevFrame );
			}

			else prevFrame.copyTo( frame );

			int downscale = frame.cols/normWidth;

			int posYText = 0;
			stringstream ss;
			// Detect facial parts
			if( !frame.empty() )
			{
				cvtColor( frame, greyFrame, CV_BGR2GRAY );
				resize( greyFrame, greyDownscaledFrame, Size(greyFrame.cols/downscale,greyFrame.rows/downscale) );
				imgEnhancer->applyCLAHE( greyDownscaledFrame );
				imgEnhancer->normRange( greyDownscaledFrame );

				// Draw framerate on display image
				ss << "FPS: " << (int)floor(fps);
				putText( frame, ss.str(), Point(10,greyFrame.rows-20), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,255) );
			
				ss.str( std::string() );
				ss.clear();
				ss << "Push R to (re)start tracking.";
				putText( frame, ss.str(), Point(10,greyFrame.rows-40), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,255) );

				// Draw animation unit values
				ss.str( std::string() );
				ss.clear();
				ss << "Animation Units:";
				putText( frame, ss.str(), Point(10,15), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,255) );
			
				for( int i=0, n=trackAUs.size(); i<n; ++i )
				{
					posYText = 30+i*15;
				
					ss.str( std::string() );
					ss.clear();
					ss << trackAUs[i];
					putText( frame, ss.str(), Point(55,posYText), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,0) );
				}

				// Draw position values
				ss.str( std::string() );
				ss.clear();
				ss << "Position:";
				putText( frame, ss.str(), Point(10,posYText+20), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,255) );
						
				// Draw orientation values
				ss.str( std::string() );
				ss.clear();
				ss << "Orientation:";
				putText( frame, ss.str(), Point(10,posYText+55), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,255,255) );
			

				if( detect )
				{
					mcFaceRect = mcFace->detectMCFace( false, greyDownscaledFrame );

					if( int n=mcFaceRect.size() == 1 )
					{						
						Rect faceRect = mcFaceRect[0][0];

						/*if( dispCap.data != 0 )
						{
							initVertPos = fpDetector->detect3D( greyFrame, dispMap, 
								faceRect, lEyeRect, rEyeRect, noseRect, mouthRect );
						}

						else
						{
							bpInitVertProj = fpDetector->detect( greyFrame, faceRect, lEyeRect, 
								rEyeRect, noseRect, mouthRect );
						}*/

						vector<Point2f> bpInitVertProjCLM;
						fitter->reset();
#if APPROACH_SELECTION == 1
						vector<Point2f> points = fpDetector->detect( greyDownscaledFrame, faceRect );
							
						targetModelPointLocations[0] = points[4];	// Left eye
						targetModelPointLocations[1] = points[12];	// Left eye
						targetModelPointLocations[2] = 0.5f * ( points[17] + points[18] );

						fitter->fit( greyDownscaledFrame, targetModelPointIndices, targetModelPointLocations, clmSWSizesInit );
#else
						fitter->fit( greyDownscaledFrame,faceRect );
#endif
						bpInitVertProjCLM = fitter->getFittedShapePoints();
						BPModel::generateFaceBPModel( bpInitVertProjCLM, bpInitVertProj, 
							greyDownscaledFrame, faceRect );

#ifdef DEBUG
						Mat img;
						greyDownscaledFrame.copyTo(img);
						cvtColor( img, img, CV_GRAY2BGR );
						BPModel::showModelInImage( img, bpInitVertProj, 1 );
						imshow("Fitted points", img);

						greyDownscaledFrame.copyTo(img);
						int resizeImage = 1;
						resize(img, img, Size(img.cols*resizeImage, img.rows*resizeImage));
						cvtColor( img, img, CV_GRAY2BGR );
						clmFitter->drawFitted( img, resizeImage, true );
						imshow("CLM points", img);
#endif
						
						configuration.clear();
						track = oamFitter->init( greyDownscaledFrame, configuration, modelPoints, 
								bpInitVertProj, bpInitModelVertIds, bpInitVertWeights, 
								bpInitSUs, bpInitAUs, vector<string>(), vector<string>(), 
								fullReset, trackAUs, minAUValues, maxAUValues );

						track = oamFitter->fit( greyDownscaledFrame, configuration, modelPoints );
							
						//// Show the initial detection and the first fit in a sigle image
						//Mat frame3 = frame.clone();
						//clmFitter->drawFitted( frame3, downscale );
						//oamFitter->draw( frame3 );
						//mcFace->drawMCRectangle( frame3, downscale );
						//imshow( "Detection and Fit", frame3 );
							
						if( track )
						{
							detect = false;
							fullReset = false;
						}

						else detect = true;
					}
				}

				if( track )
				{
					track = oamFitter->fit( greyDownscaledFrame, configuration, modelPoints );

					if( !track )
					{
						detect = true;
						
						// Draw the tracking data values
						ss.str( std::string() );
						ss.clear();
						ss << "  -     -     -";
						putText( frame, ss.str(), Point(10,posYText+35), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );

						ss.str( std::string() );
						ss.clear();
						ss << "  -     -     -";
						putText( frame, ss.str(), Point(10,posYText+70), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );
							
						for( int i=0, n=trackAUs.size(); i<n; ++i )
						{
							ss.str( std::string() );
							ss.clear();
							ss << "  -";
							putText( frame, ss.str(), Point(10,30+i*15), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );
						}
					}
						
					else
					{
						// Get facial projection ROI
						vector<Point2f> pointList;
						oamFitter->getProjectedPoints( greyFrame.cols, greyFrame.rows, pointList, ignoredVertices );

						float minX = (float)greyFrame.cols, minY = (float)greyFrame.rows;
						float maxX = 0, maxY = 0;
						for( int j=0, m=pointList.size(); j<m; ++j )
						{
							float x = pointList[j].x;
							float y = pointList[j].y;
							if( minX > x ) minX = x; if( maxX < x ) maxX = x;
							if( minY > y ) minY = y; if( maxY < y ) maxY = y;
						}

						Rect faceRect( (int)( minX + 0.5 ), (int)( minY + 0.5 ), 
							(int)( maxX - minX + 0.5 ), (int)( maxY - minY + 0.5 ) );

						//if( dispCap.data != 0 )
						//	initVertPos = fpDetector->detect3D( greyFrame, dispMap, faceRect );

						oamFitter->eyeGazeEstimation( greyFrame, sigma, smoothing, centerThresh, closingThresh, 
							closingRatio, lEyeData, rEyeData, eyeGazeCameraCoords, false, 20 );
						
						oamFitter->draw( frame, 1, false, -1, false, false, true );
						//circle( frame, eyeGazeCameraCoords, 5, CV_RGB(0,0,255), 3 );

						// Draw the tracking data values
						vector<float> position, orientation, shapeValues, animationValues;
						vector<string> shapeNames, animationNames;
						oamFitter->getObjectConfiguration( position, orientation, shapeValues, 
							shapeNames, animationValues, animationNames );

						ss.str( std::string() );
						ss.clear();
						ss << std::fixed << std::setprecision( 2 ) << position[0] << " " << position[1] << " " << position[2];
						putText( frame, ss.str(), Point(10,posYText+35), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );

						ss.str( std::string() );
						ss.clear();
						ss << std::fixed << std::setprecision( 2 ) << 180.f/CV_PI*orientation[0] 
							<< " " << 180.f/CV_PI*orientation[1] << " " << 180.f/CV_PI*orientation[2];
						putText( frame, ss.str(), Point(10,posYText+70), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );
						
						for( int i=0, n=trackAUs.size(); i<n; ++i )
						{
							ss.str( std::string() );
							ss.clear();
							ss << std::fixed << std::setprecision( 2 ) << animationValues[i];
							putText( frame, ss.str(), Point(10,30+i*15), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );
						}
					}
				}

				else
				{
					// Draw the tracking data values
					ss.str( std::string() );
					ss.clear();
					ss << "  -     -     -";
					putText( frame, ss.str(), Point(10,posYText+35), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );

					ss.str( std::string() );
					ss.clear();
					ss << "  -     -     -";
					putText( frame, ss.str(), Point(10,posYText+70), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );
						
					for( int i=0, n=trackAUs.size(); i<n; ++i )
					{
						ss.str( std::string() );
						ss.clear();
						ss << "  -";
						putText( frame, ss.str(), Point(10,30+i*15), CV_FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255,255,255) );
					}
				}
			}

			else
			{
				cout << " --(!) No captured frame -- Break!" << endl;
				break;
			}

			// Calculate framerate
			if( fnum >= 9 )
			{      
				t1 = cvGetTickCount();
				fps = 10.f / ( (float)( (t1-t0)/cvGetTickFrequency() ) / (float)1e+6 ); 
				t0 = t1;

				fnum = 0;
			}
			
			else fnum += 1;
									
			imshow( "Tracking", frame );
			if( video ) vw << frame;

			int c = waitKey( 10 );
			if( (char)c == 27 )
			{
				vw.release();
				if( imgEnhancer != 0 ) OMFFactory::instance()->destroyImgEnhancer( &imgEnhancer );
				if( mcFace != 0 ) OMFFactory::instance()->destroyMCODFace( &mcFace );
				if( fpDetector != 0 ) OMFFactory::instance()->destroyFPDetector( &fpDetector );
				if( oamFitter != 0 ) OMFFactory::instance()->destroyOAMFitter( &oamFitter );
#if APPROACH_SELECTION == 1
				if( fitter != 0 ) OMFFactory::instance()->destroyCLMFitter( &fitter );
#else
				if( fitter != 0 ) OMFFactory::instance()->destroySDMFitter( &fitter );
#endif
				destroyAllWindows();

				break;
			}

			switch( c )
			{
			case 'p':
			case 'P':
				paused = !paused;
				break;

			case 'r':
			case 'R':
				configuration.clear();
				fullReset = true;
				detect = true;
				track = false;
				break;

			case 'v':
			case 'V':
				video = !video;
				
				if( video )
				{
					vw = VideoWriter( "output.avi", CV_FOURCC('D','I','V','X'), 30, frame.size() );
					vw.open( "output.avi", CV_FOURCC('D','I','V','X'), 30, frame.size() );
				}
				
				else vw.release();

				break;

			default:
				;
			}
		}
	}

	vw.release();
	if( imgEnhancer != 0 ) OMFFactory::instance()->destroyImgEnhancer( &imgEnhancer );
	if( mcFace != 0 ) OMFFactory::instance()->destroyMCODFace( &mcFace );
	if( fpDetector != 0 ) OMFFactory::instance()->destroyFPDetector( &fpDetector );
	if( oamFitter != 0 ) OMFFactory::instance()->destroyOAMFitter( &oamFitter );
#if APPROACH_SELECTION == 1
	if( fitter != 0 ) OMFFactory::instance()->destroyCLMFitter( &fitter );
#else
	if( fitter != 0 ) OMFFactory::instance()->destroySDMFitter( &fitter );
#endif
	destroyAllWindows();

	return 0;
}