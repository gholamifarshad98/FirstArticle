#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <memory>
#include <chrono> 
#include <string> 
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
	int kernelSize = 35;
	auto rightImage = make_shared<Mat>();
	auto leftImage = make_shared<Mat>();
	auto result = make_shared<Mat>();

	auto startImread = chrono::high_resolution_clock::now();
	ReadBothImages(leftImage, rightImage);
	auto endImread = chrono::high_resolution_clock::now();


	numOfRows = leftImage->rows;
	numOfColumns = leftImage->cols;


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
	

	auto durationImread = duration_cast<milliseconds>(endImread - startImread);
	auto durationStero= duration_cast<milliseconds>(endImread - startImread);
	auto durationTotall = durationStero + durationImread;
	auto valueImread = durationImread.count();
	string durationImread_s = to_string(valueImread);
	auto valueStero = durationStero.count();
	string durationStero_s = to_string(valueStero);
	auto valueTotall = durationTotall.count();
	string durationTotall_s = to_string(valueTotall);


	imwrite("result.png", *result);
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
	for (int j = 0; j < (kernelSize / 2); j++) {
		for (int i = 0; i < numOfColumns; i++) {
			result->at<uchar>(j, i) = uchar(1);
		}
	}


	return result;
}


////////////////////////////////////////////////////////////////////
/// In this part we can make the result.
////////////////////////////////////////////////////////////////////
void makeResult() {


}

//
//
//for (int u = int(kernelSize / 2); u < numOfColumns - int(kernelSize / 2) - maxDisparity; u++) {
//	for (int v = int(kernelSize / 2); v < numOfRows - int(kernelSize / 2); v++) {
//		tempCost = 9000000000;
//		tempDisparity = 0;
//		endCost = 0;
//		for (int i = 0; i < maxDisparity; i++) {
//			endCost = CalcCost(leftImage, rightImage, v, u, kernelSize, i);
//			//	cout << cost << endl;
//			if (endCost < tempCost) {
//				tempCost = endCost;
//				tempDisparity = i;
//				cout << tempDisparity << endl;
//			}
//			else {
//				//cout << "no" << endl;
//			}
//		}
//		int a = 2;
//		result->at<uchar>(v, u) = uchar(tempDisparity * 255 / 10);
//		cout << tempDisparity << endl;
//	}
//}