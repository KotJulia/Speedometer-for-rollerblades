// Include Libraries
#include "IPSM_Julia_Kot.h"

void Speedometr::setVideo(cv::VideoCapture vidName) {

	VidCapture = vidName;
	fps = VidCapture.get(5);
	playVideo(VidCapture);
}

bool Speedometr::checkIfVideoIsOpen(cv::VideoCapture vidName) {

	// Display error picture if the video path is invalid
	if (!VidCapture.isOpened())
	{
		auto imageread = cv::imread("C:\\Users\\user\\OneDrive\\Pulpit\\ROLKI\\ErrorMsg1.jpg");
		cv::namedWindow("ERROR", cv::WINDOW_AUTOSIZE);
		cv::imshow("ERROR", imageread);
		cv::waitKey(0);
		cv::destroyAllWindows();
		return false;
	}

	else
	{
		return true;
	}
}

void Speedometr::playVideo(cv::VideoCapture vidName) {

	// Read the frames to the last frame
	while (checkIfVideoIsOpen(VidCapture))
	{
		// Initialise frame matrix
		cv::Mat frame;

		// Initialise frame matrix in grayscale
		cv::Mat frame_gray;

		// Initialize a boolean to check if frames are there or not
		auto isSuccess = VidCapture.read(frame);

		// If frames are present, detect circles and show it
		if (isSuccess == true)
		{
			detectCircles(frame, frame_gray);
			imshow("Video", frame);
		}


		// If frames are not there, close it
		if (isSuccess == false)
		{
			std::cout << "Video camera is disconnected" << std::endl;
			break;
		}

		//wait 10 ms between successive frames and break the loop if key q is pressed
		int key = cv::waitKey(10);
		if (key == 'q')
		{
			std::cout << "q key is pressed by the user. Stopping the video" << std::endl;
			break;
		}
	}

	// Release the video capture object
	VidCapture.release();
	cv::destroyAllWindows();

}

void Speedometr::detectCircles(cv::Mat frame, cv::Mat frame_gray) {

	// convert the image to grayscale format
	cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
	medianBlur(frame_gray, frame_gray, 5);
	std::vector<cv::Vec3f> circles;
	HoughCircles(frame_gray, circles, cv::HOUGH_GRADIENT, 1,
		frame_gray.rows / 16,  // change this value to detect circles with different distances to each other
		60, 40, 30, 50 // change the last two parameters
   // (min_radius & max_radius) to detect larger circles
	);

	drawCircles(circles, frame);
}

void Speedometr::drawCircles(std::vector<cv::Vec3f> circles, cv::Mat frame) {

	for (size_t i = 0; i < circles.size(); i++)
	{
		// Choose first object of circles list
		cv::Vec3i c = circles[0];

		// Points with circle center coordinates
		cv::Point center = cv::Point(c[0], c[1]);

		// Draw circle center
		circle(frame, center, 1, cv::Scalar(0, 100, 100), 1, cv::LINE_AA);

		// Draw circle outline
		int radius = c[2];
		circle(frame, center, radius, cv::Scalar(255, 0, 255), 1, cv::LINE_AA);

		// Add circle radius to list of radii
		listOfCircleRadii.push_back(radius);

		//Perform different actions depending on object's position
		if (c[0] == 0 || (c[0] > 0 && c[0] < 30)) {

			firstFrameToMeasureFrom = VidCapture.get(1);
			firstPositionOfCircle = c[0];
		}
		else if (c[0] == 600 || (c[0] > 580 && c[0] < 600)) {
			lastFrameToMeasureFrom = VidCapture.get(1);
			count = lastFrameToMeasureFrom - firstFrameToMeasureFrom;
			lastPositionOfCircle = c[0];
		}
		else if (c[0] >= 600 || circles.size() == 0) {

			cv::putText(frame,
				getSpeedOfAnObject(),
				cv::Point(frame.cols / 4, frame.rows / 4),
				cv::FONT_HERSHEY_COMPLEX,
				1.0,
				CV_RGB(186, 23, 2),
				2);
		}
		else if (c[0] < 600) {

			cv::putText(frame,
				"Measurements in progress...",
				cv::Point(frame.cols / 4, frame.rows / 4),
				cv::FONT_HERSHEY_COMPLEX,
				1.0,
				CV_RGB(0, 0, 0), 
				2);
		}
	}
	
}

std::string Speedometr::getSpeedOfAnObject() {

	// Sort list of circle radii
	sort(listOfCircleRadii.begin(), listOfCircleRadii.end());
	
	// Find median value of circle radii
	int middle_index = listOfCircleRadii.size() / 2;
	int middle_index_plus = middle_index + 1;
	float circleRadiusInPixels = listOfCircleRadii.size() % 2 == 0 ? listOfCircleRadii[middle_index] : listOfCircleRadii[middle_index_plus];

	if (lastPositionOfCircle == 0 || firstPositionOfCircle == 0) {
		text = "Please change parameteres of HoughCircles and try again";
	}

	else {
		// Calculate velocity of an object
		float velocity = (((((lastPositionOfCircle - firstPositionOfCircle) / count) / (circleRadiusInPixels / realRadius)) * fps) / 1000) * 3.6;
		// Create text to display
		text = std::to_string(round(velocity * 10) / 10) + " km/h";
	}

	return text;
}



int main()
{
	// initialize a video capture object
	cv::VideoCapture vidCapture("C:\\Users\\user\\OneDrive\\Pulpit\\ROLKI\\rolki7.mp4");
	Speedometr video;
	video.setVideo(vidCapture);



	return 0;
}
