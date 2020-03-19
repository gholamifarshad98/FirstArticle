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
int maxDisparity = 10;
int kernelSize = 16;
void ReadBothImages(shared_ptr<Mat>, shared_ptr<Mat>);


int main()
{

	auto rightImage = make_shared<Mat>();
	auto leftImage = make_shared<Mat>();
	ReadBothImages(leftImage, rightImage);
	numOfRows = leftImage->rows;
	numOfColumns = leftImage->cols;
	auto start = chrono::high_resolution_clock::now();
	
	try
	{
		auto result =stereo(leftImage, rightImage, kernelSize, maxDisparity);
		imshow("result", *result);
		waitKey(12);
	}
	catch (cv::Exception & e)
	{
		cerr << e.msg << endl; // output exception message
	}

	chrono::high_resolution_clock::time_point stop = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(stop - start);
	auto value = duration.count();
	string duration_s = to_string(value);



	cout << "hello" << endl;
	int x;


	cin >> x;// Show our image inside it.
			 // Wait for a keystroke in the window
	return 0;
}

////////////////////////////////////////////////////////////////////
/// In this part we can load two Images.
////////////////////////////////////////////////////////////////////
void ReadBothImages(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage) {

	try {
		*rightImage = imread("../data/1.png", IMREAD_GRAYSCALE);   // Read the right image
		//rightImage->convertTo(*rightImage, CV_64F);
		*rightImage = *rightImage;
		*leftImage = imread("../data/2.png", IMREAD_GRAYSCALE);   // Read the left image
		//leftImage->convertTo(*leftImage, CV_64F);
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
	//imshow("test", *rightImage);

	//waitKey(0);
}


////////////////////////////////////////////////////////////////////
/// In this part we clac cost of each pixel for sepecfic disparity.
////////////////////////////////////////////////////////////////////
int CalcCost(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage, int row, int column, int kernelSize, int disparity) {
	int cost=0;
	for (int u = -int(kernelSize / 2); u <= int(kernelSize / 2); u++) {
		for (int v = -int(kernelSize / 2); v <= int(kernelSize / 2); v++) {
			cost = cost + int(pow((leftImage->at<uchar>(row + v, column + u) - (rightImage->at<uchar>(row + v, column + u + disparity))), 2));
		}
	}
	return cost;
}

////////////////////////////////////////////////////////////////////
/// In this part we clac disparity of each pixel.
////////////////////////////////////////////////////////////////////
shared_ptr<Mat> stereo(shared_ptr<Mat> leftImage, shared_ptr<Mat> rightImage, int kernelSize, int maxDisparity) {
	auto result = make_shared<Mat>(numOfRows, numOfColumns, CV_8UC1);
	//imshow("leftImage", *leftImage);
	//imshow("rightImage", *rightImage);
	//waitKey(12);
	int cost;
	int tempCost;
	int tempDisparity;
	

	for (int u = int(kernelSize / 2); u < numOfColumns - int(kernelSize / 2) - maxDisparity; u++) {
		for (int v = int(kernelSize / 2); v < numOfRows- int(kernelSize / 2); v++) {
			tempCost = 10000;
			tempDisparity = 0;
			for (int i = 0; i < maxDisparity; i++) {
				cost = CalcCost(leftImage, rightImage, v, u, kernelSize, i);
				if (cost < tempCost) { tempCost = cost; tempDisparity = i; }
			}
			result->at<uchar>(Point(u, v)) = uchar(tempDisparity / maxDisparity * 255);
		}
	}
	return result;

}


////////////////////////////////////////////////////////////////////
/// In this part we can make the result.
////////////////////////////////////////////////////////////////////
void makeResult() {

}