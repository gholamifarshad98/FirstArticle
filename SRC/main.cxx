#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <memory>
#include <chrono> 
#include <string> 
#include<fstream>
using namespace cv;
using namespace std;
using namespace std::chrono;
shared_ptr<Mat> stereo(shared_ptr<Mat> , shared_ptr<Mat> , int , int );
int numOfColumns;
int numOfRows;

void ReadBothImages(shared_ptr<Mat>, shared_ptr<Mat>);


int main()
{
	int maxDisparity = 30;
	int kernelSize = 32;
	auto rightImage = make_shared<Mat>();
	auto leftImage = make_shared<Mat>();
	auto result = make_shared<Mat>();

////////////////////////////////////////////////////////////////////
/// In this part we call readingImages function.
////////////////////////////////////////////////////////////////////
	auto startImread = chrono::high_resolution_clock::now();
	ReadBothImages(leftImage, rightImage);
	auto endImread = chrono::high_resolution_clock::now();
	numOfRows = leftImage->rows;
	numOfColumns = leftImage->cols;

////////////////////////////////////////////////////////////////////
/// In this part we call stereo function.
////////////////////////////////////////////////////////////////////
	auto startStereo = chrono::high_resolution_clock::now();
	try
	{
		result =stereo(leftImage, rightImage, kernelSize, maxDisparity);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	auto endStereo = chrono::high_resolution_clock::now();
	
////////////////////////////////////////////////////////////////////
/// In this part we report the results.
////////////////////////////////////////////////////////////////////
	std::chrono::duration<int, std::milli> durationImread = duration_cast<milliseconds>(endImread - startImread);
	std::chrono::duration<int, std::milli> durationStero= duration_cast<milliseconds>(endStereo - startStereo);
	std::chrono::duration<int, std::milli> durationTotall = durationStero + durationImread;
	auto valueImread = durationImread.count();
	string durationImread_s = to_string(valueImread);
	auto valueStereo = durationStero.count();
	string durationStero_s = to_string(valueStereo);
	auto valueTotall = durationTotall.count();
	string durationTotall_s = to_string(valueTotall);
	//Object for repoting results in a text file.
	ofstream repotringResult;
	repotringResult.open("resultsSimpleStereo.txt");
	repotringResult << "Image numOfRows = " << numOfRows << endl;
	repotringResult << "Image numOfColumns = " << numOfColumns << endl;
	repotringResult << "kernel size is = " << kernelSize << endl;
	repotringResult << "maxDisparity is = " << maxDisparity << endl;
	repotringResult << "duration of Reading Images = " << durationImread_s << "(ms)" << endl;
	repotringResult << "duration of Stereo = " << durationStero_s <<"(ms)"<< endl;
	repotringResult << "duration of Totall = " << durationTotall_s << "(ms)" << endl;
	repotringResult.close();

	imwrite("resultSimpleStereo.png", *result);
	return 0;
}

////////////////////////////////////////////////////////////////////
/// In this part we can load two Images.
////////////////////////////////////////////////////////////////////
void ReadBothImages(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage) {
	try {
		*rightImage = imread("../data/1.png", IMREAD_GRAYSCALE);   // Read the right image
		*rightImage = *rightImage;
		*leftImage = imread("../data/2.png", IMREAD_GRAYSCALE);   // Read the left image
		*leftImage = *leftImage;
		if (!rightImage->data)                             // Check for invalid input
		{
			throw "right";
		}
		if (!leftImage->data)                             // Check for invalid input
		{
			throw "left";
		}
	}
	catch (char* error) {
		cout << "can not load the " << error << " iamge" << endl;
	}
}


////////////////////////////////////////////////////////////////////
/// In this part we clac cost of each pixel for sepecfic disparity.
////////////////////////////////////////////////////////////////////
int CalcCost(shared_ptr<Mat> _leftImage, shared_ptr<Mat> _rightImage, int row, int column, int kernelSize, int _disparity) {
	int temp = 0;
	for (int u = -(kernelSize / 2); u < (kernelSize / 2); u++) {
		for (int v = -(kernelSize / 2); v < (kernelSize / 2); v++) {
			temp = temp + abs(_rightImage->at<uchar>(v+row, u+column+_disparity) - _leftImage->at<uchar>(v+row, u+column));
		}
	}
	return temp;
}

////////////////////////////////////////////////////////////////////
/// In this part we clac disparity of each pixel.
////////////////////////////////////////////////////////////////////
shared_ptr<Mat> stereo(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage, int kernelSize, int maxDisparity) {
	auto result = make_shared<Mat>(numOfRows, numOfColumns, CV_8U);
	//Mat result;// = *rightImage;
	int cost;
	int tempCost;
	int selectDisparity;
	for (int j = (kernelSize / 2); j < numOfRows - (kernelSize / 2); j++) {
		for (int i = (kernelSize / 2); i < numOfColumns - maxDisparity - (kernelSize / 2); i++) {
			tempCost = CalcCost(leftImage, rightImage, j, i, kernelSize, 0);
			selectDisparity = 0;
			for (int k = 1; k < maxDisparity; k++) {
				cost = CalcCost(leftImage, rightImage, j, i, kernelSize, k);
				if (cost < tempCost) {
					tempCost = cost;
					selectDisparity = k;
				}
			}
			result->at<uchar>(j, i) = uchar(255* selectDisparity/maxDisparity);
		}
	}
	// ITs added for removing top of image that is not solved in stereo maching, becase of kernelsize.
	for (int j = 0; j < (kernelSize / 2); j++) {
		for (int i = 0; i < numOfColumns; i++) {
			result->at<uchar>(j, i) = uchar(1);
		}
	}


	return result;
}