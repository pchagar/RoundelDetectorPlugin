#include "pch.h"
#include "BMWRoundelDetectorPlugin.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"
#include "stdlib.h"

#define DLL_API __declspec(dllexport) 
#define CASCADE_NOT_LOADED -1
#define FOCAL_LENGTH 543.85
#define ROUNDEL_PHYSICAL_SIZE 2.4
#define LOGO_NOT_FOUND -1
#define X 0
#define Y 1
#define Z 2

using namespace cv;
using namespace std;

static int displayLogoDelay;
static CascadeClassifier logo_cascade;

extern "C" {

	DLL_API int detectLogo(unsigned char* inPixels, int width, int height, int* result) {
		int size = width * height * 4;

		int logoDepth = 0;

		Mat frame = Mat(height, width, CV_8UC4);
		Mat gray = Mat(height, width, CV_8UC3);

		Point3i foundLogoPoint = Point3i(0, 0, 0);
		vector<Rect> logos;
		vector<Mat> rgbaSeparated;

		//Must explicitly copy data from unsigned char* inPixels to Mat frame
		memcpy(frame.data, inPixels, size);

		//Image comes in flipped on vertical axis, correcting below
		flip(frame, frame, 1);

		//Convert to 1 channel grayscale Mat of size height, width 
		cvtColor(frame, gray, COLOR_BGRA2GRAY);
		equalizeHist(gray, gray);

		// Try to detect logo, detected objects put in logos Rect
		if (logo_cascade.empty()) 
		{
			return CASCADE_NOT_LOADED;
		}
		else
		{
			//Detect any logos in the given grayscale frame
			logo_cascade.detectMultiScale(gray, logos, 1.13, 3,
				CASCADE_DO_CANNY_PRUNING);
		}

		for (auto i = 0; i < logos.size(); i++)
		{
			//Depth of the object approximated with focal length and 
			//actual physical size of roundel
			logoDepth = (FOCAL_LENGTH + ROUNDEL_PHYSICAL_SIZE) / logos[i].width;
			foundLogoPoint = Point3i(logos[i].x + logos[i].width / 2, logos[i].y + logos[i].height / 2, logoDepth);
		}
		
		if (foundLogoPoint.x != 0 && foundLogoPoint.y != 0)
		{
			result[X] = foundLogoPoint.x;
			// TODO: Reapproximate y value as opencv coordinate starts in top left
			//		 instead of bottom left
			result[Y] = foundLogoPoint.y;
			result[Z] = foundLogoPoint.z;
			return 1;
		}
		else
		{
			result[X] = LOGO_NOT_FOUND;
			result[Y] = LOGO_NOT_FOUND;
			result[Z] = LOGO_NOT_FOUND;
			return 0;
		}
	}

	DLL_API int loadCascade(const char* str)
	{
		string path = str;
		bool loaded = logo_cascade.load(path);
		if (!loaded) {
			return CASCADE_NOT_LOADED;
		}
		return 1;
	}

	DLL_API int getExePath (unsigned char* outBytes)
	{
		wchar_t buffer[MAX_PATH];
		char charBuf[MAX_PATH];

		//Get the file path on device
		GetModuleFileName(NULL, buffer, MAX_PATH);
		size_t length = wcstombs(charBuf, buffer, MAX_PATH);

		memcpy(outBytes, charBuf, MAX_PATH);
		return 1;
	}
}