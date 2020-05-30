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
#include "mathOp.hxx"
using namespace cv;
using namespace std;
using namespace std::chrono;
shared_ptr<Mat> stereo(shared_ptr<Mat> , shared_ptr<Mat> , int , int , shared_ptr<vector<shared_ptr<Mat>>>);
shared_ptr<Mat> stereo(shared_ptr<Mat>, shared_ptr<Mat>, shared_ptr<Mat> , shared_ptr<Mat>, shared_ptr<vector<shared_ptr<Mat>>>, shared_ptr<vector<shared_ptr<Mat>>>, int , int );
void setLimits(shared_ptr<Mat>, shared_ptr<Mat>, int, int, int, int*, int*);

int numOfColumns;
int numOfRows;

void ReadImages(shared_ptr<Mat>, shared_ptr<Mat>, shared_ptr<Mat>, shared_ptr<Mat>, shared_ptr<Mat>, shared_ptr<Mat>);


int main()
{
	int maxDisparity = 30;
	int kernelSize = 32;
	auto rightImage = make_shared<Mat>();
	auto leftImage = make_shared<Mat>();
	auto result = make_shared<Mat>();

	auto rightImage1 = make_shared<Mat>();
	auto leftImage1 = make_shared<Mat>();
	auto result1 = make_shared<Mat>();

	auto rightImage2 = make_shared<Mat>();
	auto leftImage2 = make_shared<Mat>();
	auto result2 = make_shared<Mat>();

////////////////////////////////////////////////////////////////////
/// In this part we call readingImages function.
////////////////////////////////////////////////////////////////////
	auto startImread = chrono::high_resolution_clock::now();
	ReadImages(leftImage, rightImage, leftImage1, rightImage1, leftImage2, rightImage2);
	auto endImread = chrono::high_resolution_clock::now();
	numOfRows = leftImage->rows;
	numOfColumns = leftImage->cols;

////////////////////////////////////////////////////////////////////
/// In this part we call stereo function. background
////////////////////////////////////////////////////////////////////
	auto startStereo = chrono::high_resolution_clock::now();
	auto costsLayerVector_1 = make_shared<vector<shared_ptr<Mat>>>();
	auto costsLayerVector_2 = make_shared<vector<shared_ptr<Mat>>>();
	//result1 = stereo(leftImage1, rightImage1, kernelSize, maxDisparity, costsLayerVector_1);
	//result2 = stereo(leftImage2, rightImage2, kernelSize, maxDisparity, costsLayerVector_2);
	//cout << "ccvvcv" << (*costsLayerVector_1).size() << endl;
	try
	{
		result1 = stereo(leftImage1, rightImage1, kernelSize, maxDisparity, costsLayerVector_1);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	try{

		result2 = stereo(leftImage2, rightImage2, kernelSize, maxDisparity, costsLayerVector_2);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	try{
		imshow("resul1", *result1);
		imshow("result2", *result2);

		waitKey(0);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	auto endStereo = chrono::high_resolution_clock::now();
	
////////////////////////////////////////////////////////////////////
/// In this part we call stereo function. obostecle
////////////////////////////////////////////////////////////////////
	//result = stereo(leftImage, rightImage, result1, result2, costsLayerVector_1, costsLayerVector_2, kernelSize, maxDisparity);
	auto startDeprivedStereo = chrono::high_resolution_clock::now();
	try
	{
		result = stereo(leftImage, rightImage, result1, result2, costsLayerVector_1, costsLayerVector_2, kernelSize, maxDisparity);

	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	auto endDeprivedStereo = chrono::high_resolution_clock::now();
	

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
	repotringResult.open("resultsSimpleSccctereo.txt");
	repotringResult << "Image numOfRows = " << numOfRows << endl;
	repotringResult << "Image numOfColumns = " << numOfColumns << endl;
	repotringResult << "kernel size is = " << kernelSize << endl;
	repotringResult << "maxDisparity is = " << maxDisparity << endl;
	repotringResult << "duration of Reading Images = " << durationImread_s << "(ms)" << endl;
	repotringResult << "duration of Stereo = " << durationStero_s <<"(ms)"<< endl;
	repotringResult << "duration of Totall = " << durationTotall_s << "(ms)" << endl;
	repotringResult.close();

	imshow("resultDeprivedStereo png", *result);
	waitKey(0);
	return 0;
}

////////////////////////////////////////////////////////////////////
/// In this part we can load two Images.
////////////////////////////////////////////////////////////////////
void ReadImages(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage, shared_ptr<Mat> leftImage1, shared_ptr<Mat> rightImage1, shared_ptr<Mat> leftImage2, shared_ptr<Mat> rightImage2) {
	try {
		*rightImage = imread("../data/left_6.png", IMREAD_GRAYSCALE);   // Read the right image
		*rightImage = *rightImage;
		*leftImage = imread("../data/right_6.png", IMREAD_GRAYSCALE);   // Read the left image
		*leftImage = *leftImage;

		*rightImage1 = imread("../data/left_5.png", IMREAD_GRAYSCALE);   // Read the right image
		*rightImage1 = *rightImage1;
		*leftImage1 = imread("../data/right_5.png", IMREAD_GRAYSCALE);   // Read the left image
		*leftImage1 = *leftImage1;

		*rightImage2 = imread("../data/left_0.png", IMREAD_GRAYSCALE);   // Read the right image
		*rightImage2 = *rightImage2;
		*leftImage2 = imread("../data/right_0.png", IMREAD_GRAYSCALE);   // Read the left image
		*leftImage2 = *leftImage2;

		if (!rightImage->data | !rightImage1->data| !rightImage2->data)                             // Check for invalid input
		{
			throw "right";
		}
		if (!leftImage->data| !leftImage1->data| !leftImage2->data)                             // Check for invalid input
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
			auto fvrfv = _rightImage->at<uchar>(v + row, u + column + _disparity);
			auto dfbf = _leftImage->at<uchar>(v + row, u + column);
			temp = temp + abs(_rightImage->at<uchar>(v+row, u+column+_disparity) - _leftImage->at<uchar>(v+row, u+column));
		}
	}
	return int(temp);
}

////////////////////////////////////////////////////////////////////
/// In this part we clac disparity of each pixel.
////////////////////////////////////////////////////////////////////
shared_ptr<Mat> stereo(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage, int kernelSize, int maxDisparity, shared_ptr<vector<shared_ptr<Mat>>> output) {
	auto result = make_shared<Mat>(numOfRows, numOfColumns, CV_8U);

	auto costsLayerVector = make_shared<vector<shared_ptr<Mat>>>();
	for (size_t layer = 0; layer < maxDisparity; layer++) {
		costsLayerVector->push_back(make_shared<Mat>(numOfRows, numOfColumns, CV_32SC1));
	}
	cout << "in the streo function the size is " << (*costsLayerVector).size() << endl;
	int cost;
	int tempCost;
	int selectDisparity;
	for (int j = (kernelSize / 2); j < numOfRows - (kernelSize / 2); j++) {
		for (int i = (kernelSize / 2); i < numOfColumns - maxDisparity - (kernelSize / 2); i++) {
			//cout << "(i,j)==> (" << i << "," << j << ")" << endl;
			//cout << (*costsLayerVector)[0]->at<int>(j, i) << endl;
			//cout << "hi fer" << endl;
			tempCost = CalcCost(leftImage, rightImage, j, i, kernelSize, 0);
			(*costsLayerVector)[0]->at<int>(j, i) = tempCost;
			selectDisparity = 0;
			for (int k = 1; k < maxDisparity; k++) {
				cost = CalcCost(leftImage, rightImage, j, i, kernelSize, k);
				(*costsLayerVector)[k]->at<int>(j, i) = cost;
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
			result->at<uchar>(j, i) = uchar(0);
		}
	}
	*output = *costsLayerVector;
	cout << "at the end of the stereo function the size is " << (*output).size() << endl;
	return result;
}


////////////////////////////////////////////////////////////////////
/// In this part we clac deprived disparity of each pixel.
////////////////////////////////////////////////////////////////////
shared_ptr<Mat> stereo(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage , shared_ptr<Mat> result_in1, shared_ptr<Mat> result_in2, shared_ptr<vector<shared_ptr<Mat>>>costVec_1, shared_ptr<vector<shared_ptr<Mat>>> costVec_2, int kernelSize, int maxDisparity) {
	auto result = make_shared<Mat>(numOfRows, numOfColumns, CV_8U);
	auto calculator = make_shared<mathOp>();

	int cost;
	int tempCost;
	int selectDisparity;
	int lowerLimit;
	int upperLimit;
	for (int j = (kernelSize / 2); j < numOfRows - (kernelSize / 2)-5; j++) {
		for (int i = (kernelSize / 2); i < numOfColumns - maxDisparity- - (kernelSize / 2); i++) {
			setLimits(result_in1, result_in2, i, j, maxDisparity, &lowerLimit, &upperLimit);
			auto vecCost_1 = make_shared<vector<int>>();
			auto vecCost_2 = make_shared<vector<int>>();
			auto vecCurCost = make_shared<vector<int>>();
			for (int layer = lowerLimit; layer <= upperLimit; layer++) {
				vecCurCost->push_back(CalcCost(leftImage, rightImage, j, i, kernelSize, layer));
				vecCost_1->push_back((*costVec_1)[layer]->at<int>(j, i));
				vecCost_2->push_back((*costVec_2)[layer]->at<int>(j, i));
			}

			if (abs(calculator->calcCorro(*vecCurCost, *vecCost_1)) < 0.2) {
				
				result->at<uchar>(j, i) = uchar(0);
			}
			else {
				result->at<uchar>(j, i) = uchar(255);
			}
		}
	}
	
	// ITs added for removing top of image that is not solved in stereo maching, becase of kernelsize.
	for (int j = 0; j < (kernelSize / 2); j++) {
		for (int i = 0; i < numOfColumns; i++) {
			result->at<uchar>(j, i) = uchar(0);
		}
	}


	return result;
}

void setLimits(shared_ptr<Mat> resu_1, shared_ptr<Mat> resu_2, int i, int j,int maxDisparity, int* lowLim, int* upLim) {

	auto lim_1 = int(resu_1->at<uchar>(j, i) * maxDisparity / 255);
	auto lim_2 = int(resu_2->at<uchar>(j, i) * maxDisparity / 255);

	if (lim_2 <= lim_1) {
		int a = lim_2;
		lim_2 = lim_1;
		lim_1 = a;
	}
	lim_1 = lim_1 - 1;
	lim_2 = lim_2 + 1;
	if (lim_1 < 0) {
		lim_1 = 0;
	}
	if (lim_2>maxDisparity)
	{
		lim_2 = maxDisparity;
	}
	*lowLim = lim_1;
	*upLim = lim_2;

}

