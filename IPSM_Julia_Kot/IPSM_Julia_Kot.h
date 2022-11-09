// Include Libraries
#include<opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include<iostream>
#include <cmath>

class Speedometr
{
    cv::VideoCapture VidCapture;    // Initialize a video capture object

    int realRadius = 40;           // Value of rollerblade's wheel in mm

    std::vector< int > listOfCircleRadii;  // List of radii in pixels found on video

    int count;                     // Amount of frames between start and end of measurmeent

    int firstFrameToMeasureFrom;   // Frame on which measurement starts

    int lastFrameToMeasureFrom;    // Frame on which measurement ends

    int firstPositionOfCircle;     // x coordinate of center of circle at the beginnig of measurement

    int lastPositionOfCircle;      // x coordinate of center of circle at the end of measurement

    int fps;                       // Frames per second

    std::string text; 

    // Function to check if video is open.
    bool checkIfVideoIsOpen(cv::VideoCapture vidName);

    // Function to display video.
	void playVideo(cv::VideoCapture vidName);

    // Take each frame and finds circles.
	void detectCircles(cv::Mat frame, cv::Mat frame_gray);

    // Draw circles detected by the function detectCircles on each frame.
	void drawCircles(std::vector<cv::Vec3f> circles, cv::Mat frame);

    // Calculate speed of an object and return it as a string,
	std::string getSpeedOfAnObject();

public:

    // Take video path and set it as an object to capture and make measurement on.
	void setVideo(cv::VideoCapture vidName);
};
