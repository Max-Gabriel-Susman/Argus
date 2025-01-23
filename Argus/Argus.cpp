#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
//#include "detector.h"
//#include "poseEstimation.h"
using namespace cv;
int main()
{
	//VideoCapture cap(0, cv::CAP_MSMF);
	VideoCapture cap("C:/www/town.avi", cv::CAP_MSMF);
	VideoWriter writer;
	/*
	OLD RTSP pipeline
	writer.open("appsrc ! videoconvert ! videoscale ! video/x-raw,width=640,height=480 ! x264enc speed-preset=veryfast tune=zerolatency bitrate=640 ! rtspclientsink location=rtsp://localhost:8554/mystream ",0,10,Size(640, 480),true);
	*/
	//NEW RTMP2SINK pipeline
	writer.open("appsrc ! videoconvert ! videoscale ! video/x-raw,width=640,height=480 ! x264enc ! flvmux ! rtmp2sink location=rtmp://localhost:1935/live/Mystream ", 0, 10, Size(640, 480), true);
	/*
	poseEstimation pose = poseEstimation();
	Detector dec = Detector();
	*/
	Mat img;
	for (;;) {
		if (!cap.isOpened()) {
			std::cout << "Video Capture Fail" << std::endl;
			break;
		}
		cap.read(img);
		/*dec.ProcessFrame(img);
		dec.detect();
		Mat proc = dec.getResult();
		pose.ProcessFrame(proc);
		pose.drawResults();
		*/
		cv::resize(img, img, Size(800, 600));
		cv::imshow("raw", img);
		cv::resize(img, img, Size(640, 480));
		writer << img;
		cv::waitKey(25);
	}
}