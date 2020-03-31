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
shared_ptr<Mat> cuttedSSD(shared_ptr<Mat>, shared_ptr<Mat>, int, int, int);
shared_ptr<Mat> cuttedSSDCrossCheck(shared_ptr<Mat>, shared_ptr<Mat>, int, int, int);
shared_ptr<Mat> fillteringResult(shared_ptr<Mat>, int, int, int, int);
int fillterWin(shared_ptr<Mat> , int , int , int , int );
int numOfColumns;
int numOfRows;

void ReadBothImages(shared_ptr<Mat>, shared_ptr<Mat>);


int main()
{
	int maxDisparity = 30;
	int kernelSize = 32;
	auto rightImage = make_shared<Mat>();
	auto leftImage = make_shared<Mat>();
	auto result_cutted = make_shared<Mat>();
	auto result_cutted_crossCheck = make_shared<Mat>();

////////////////////////////////////////////////////////////////////
/// In this part we call readingImages function.
////////////////////////////////////////////////////////////////////
	auto startImread = chrono::high_resolution_clock::now();
	ReadBothImages(leftImage, rightImage);
	auto endImread = chrono::high_resolution_clock::now();
	numOfRows = leftImage->rows;
	numOfColumns = leftImage->cols;

////////////////////////////////////////////////////////////////////
/// In this part we call cuttedSSD function.
////////////////////////////////////////////////////////////////////
	auto startCuttedSSD = chrono::high_resolution_clock::now();
	try
	{
		result_cutted = cuttedSSD(leftImage, rightImage, kernelSize, maxDisparity,12);
		imwrite("cuttedSSD.png", *result_cutted);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	auto endCuttedSSD = chrono::high_resolution_clock::now();
	

////////////////////////////////////////////////////////////////////
/// In this part we call cuttedSSD with crossCheck function.
////////////////////////////////////////////////////////////////////
	auto startCuttedSSDCrossCheck = chrono::high_resolution_clock::now();
	try
	{
		result_cutted_crossCheck = cuttedSSDCrossCheck(leftImage, rightImage, kernelSize, maxDisparity, 12);
		imwrite("cuttedSSDCrossCheck.png", *result_cutted_crossCheck);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}
	auto endCuttedSSDCrossCheck = chrono::high_resolution_clock::now();


////////////////////////////////////////////////////////////////////
/// In this part we will fillter the results.
////////////////////////////////////////////////////////////////////
	auto startFilltering = chrono::high_resolution_clock::now();
	int _fillterSizeU = 8;
	int _fillterSizeV = 8;
	int _threshold = 30;
	auto result_cutted_crossCheck_fillterde = fillteringResult(result_cutted_crossCheck, _fillterSizeU, _fillterSizeV, maxDisparity, _threshold);

	imwrite("cuttedSSDCrossCheckFillterd.png", *result_cutted_crossCheck_fillterde);
	auto endFilltering = chrono::high_resolution_clock::now();
	

////////////////////////////////////////////////////////////////////
/// In this part we report the results.
////////////////////////////////////////////////////////////////////
	std::chrono::duration<int, std::milli> durationImread = duration_cast<milliseconds>(endImread - startImread);
	std::chrono::duration<int, std::milli> durationSSDCutted= duration_cast<milliseconds>(endCuttedSSD - startCuttedSSD);
	std::chrono::duration<int, std::milli> durationSSDCuttedCrossCheck = duration_cast<milliseconds>(endCuttedSSDCrossCheck - startCuttedSSDCrossCheck);
	std::chrono::duration<int, std::milli> durationFilltering = duration_cast<milliseconds>(endFilltering - startFilltering);
	std::chrono::duration<int, std::milli> durationTotall_cutted = durationSSDCutted + durationImread;
	std::chrono::duration<int, std::milli> durationTotall_cutted_crossCheck = durationSSDCuttedCrossCheck + durationImread;
	std::chrono::duration<int, std::milli> durationTotall_cutted_crossCheck_filltering = durationFilltering + durationSSDCuttedCrossCheck + durationImread;

	auto valueImread = durationImread.count();
	string durationImread_s = to_string(valueImread);
	
	auto valueSSDCutted = durationSSDCutted.count();
	string durationSSDCutted_s = to_string(valueSSDCutted);

	auto valueSSDCutted_CrossCheck = durationSSDCuttedCrossCheck.count();
	string durationSSDCuttedCrossCheck_s = to_string(valueSSDCutted_CrossCheck);
	
	auto valueFilltering = durationFilltering.count();
	string duratioFilltering_s = to_string(valueFilltering);

	auto valueTotallSSDCutted = durationTotall_cutted.count();
	string durationTotallSSDCutted_s = to_string(valueTotallSSDCutted);

	auto valueTotallSSDCutted_CrossCheck = durationTotall_cutted_crossCheck.count();
	string durationTotallSSDCuttedCrossCheck_s = to_string(valueTotallSSDCutted_CrossCheck);

	auto valueTotallSSDCutted_CrossCheck_Fillering = durationTotall_cutted_crossCheck_filltering.count();
	string durationTotallSSDCuttedCrossCheckFilltering_s = to_string(valueTotallSSDCutted_CrossCheck_Fillering);

	//Object for repoting results in a text file.
	ofstream repotringResult;
	repotringResult.open("cuttedSSD.txt");
	repotringResult << "Image numOfRows = " << numOfRows << endl;
	repotringResult << "Image numOfColumns = " << numOfColumns << endl;
	repotringResult << "kernel size is = " << kernelSize << endl;
	repotringResult << "maxDisparity is = " << maxDisparity << endl;
	repotringResult << "duration of Reading Images = " << durationImread_s << "(ms)" << endl;
	repotringResult << "duration of cuttedSSD = " <<durationSSDCutted_s <<"(ms)"<< endl;
	repotringResult << "duration of cuttedSSD with cross check = " << durationSSDCuttedCrossCheck_s << "(ms)" << endl;
	repotringResult << "duration of Filltering = " << duratioFilltering_s << "(ms)" << endl;
	repotringResult << "duration of Totall cuttedSSD= " << durationTotallSSDCutted_s << "(ms)" << endl;
	repotringResult << "duration of Totall cuttedSSD with CrossCheck= " << durationTotallSSDCuttedCrossCheck_s << "(ms)" << endl;
	repotringResult << "duration of Totall cuttedSSD with CrossCheck and Filltering= " << durationTotallSSDCuttedCrossCheckFilltering_s << "(ms)" << endl;
	repotringResult.close();

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


////////////////////////////////////////////////////////////////////
/// In this part we will find the pixels which 
/// had selected disparity by cutted stereo.
////////////////////////////////////////////////////////////////////
shared_ptr<Mat> cuttedSSD(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage, int kernelSize, int maxDisparity, int selectedDisparity) {
	auto result = make_shared<Mat>(numOfRows, numOfColumns, CV_8U);
	int cost_01;
	int cost_02;
	int cost_03;


	for (int j = (kernelSize / 2); j < numOfRows - (kernelSize / 2); j++) {
		for (int i = (kernelSize / 2); i < numOfColumns - maxDisparity - (kernelSize / 2); i++) {
			cost_01 = CalcCost(leftImage, rightImage, j, i, kernelSize, selectedDisparity-1);
			cost_02 = CalcCost(leftImage, rightImage, j, i, kernelSize, selectedDisparity);
			cost_03 = CalcCost(leftImage, rightImage, j, i, kernelSize, selectedDisparity + 1);
			if (cost_02 < cost_03 & cost_02 < cost_01) {
				result->at<uchar>(j, i) = uchar(255);
			}
			else {
				result->at<uchar>(j, i) = uchar(0);
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

////////////////////////////////////////////////////////////////////
/// In this part we will find the pixels which had selected disparity
/// by cutted stereo and we will also check them by cross checking. 
////////////////////////////////////////////////////////////////////
shared_ptr<Mat> cuttedSSDCrossCheck(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage, int kernelSize, int maxDisparity, int selectedDisparity) {
	auto result_crossCheck = make_shared<Mat>(numOfRows, numOfColumns, CV_8U);
	int cost_01;
	int cost_02;
	int cost_03;
	int cost_04;
	int cost_05;
	int cost_06;


	for (int j = (kernelSize / 2); j < numOfRows - (kernelSize / 2); j++) {
		for (int i = (kernelSize / 2)+1 ; i < numOfColumns - maxDisparity - (kernelSize / 2); i++) {
			cost_01 = CalcCost(leftImage, rightImage, j, i, kernelSize, selectedDisparity - 1);
			cost_02 = CalcCost(leftImage, rightImage, j, i, kernelSize, selectedDisparity);
			cost_03 = CalcCost(leftImage, rightImage, j, i, kernelSize, selectedDisparity + 1);

			cost_04 = CalcCost(rightImage, leftImage, j, i, kernelSize, -(selectedDisparity - 1));
			cost_05 = CalcCost(rightImage, leftImage, j, i, kernelSize, -(selectedDisparity));
			cost_06 = CalcCost(rightImage, leftImage, j, i, kernelSize, -(selectedDisparity + 1));
			if (cost_02 < cost_03 & cost_02 < cost_01 & cost_05 < cost_06 & cost_05 < cost_04) {
				result_crossCheck->at<uchar>(j, i) = uchar(255);
			}
			else {
				result_crossCheck->at<uchar>(j, i) = uchar(0);
			}

		}
	}
	// ITs added for removing top of image that is not solved in stereo maching, becase of kernelsize.
	for (int j = 0; j < (kernelSize / 2); j++) {
		for (int i = 0; i < numOfColumns; i++) {
			result_crossCheck->at<uchar>(j, i) = uchar(0);
		}
	}
	return result_crossCheck;
}


////////////////////////////////////////////////////////////////////
/// In this part we will fillter the result of cuttedSSDCrossCheck.
////////////////////////////////////////////////////////////////////
shared_ptr<Mat> fillteringResult(shared_ptr<Mat> input, int fillterSizeU, int fillterSizeV, int maxDisparity, int threshold) {
	int _sum = 0;
	auto _result = make_shared<Mat>(numOfRows, numOfColumns, CV_8U);
	for (int j = (fillterSizeV / 2)+10; j < numOfRows - (fillterSizeV / 2)-10; j++) {
		for (int i = (fillterSizeU / 2)+1+10; i < numOfColumns - maxDisparity - (fillterSizeU / 2)-10; i++) {
			_sum = fillterWin(input, i, j, fillterSizeU, fillterSizeV);
			if(_sum>= threshold){
				_result->at<uchar>(j, i) = uchar(255);
			}
			else {
				_result->at<uchar>(j, i) = uchar(0);
			}
		}
	}
	return _result;
}

int fillterWin(shared_ptr<Mat> input,int u, int v, int fillterSizeU, int fillterSizeV) {
	int sum = 0;
	for (int i = -(fillterSizeU /2); i < (fillterSizeU/2); i++) {
		for (int j = -(fillterSizeV/2); j < (fillterSizeV/2); j++) {
			if (input->at<uchar>(v +j , u +i) >= uchar(250)) {
				sum = sum + 1;
			}
		}
	}
	return sum;
}