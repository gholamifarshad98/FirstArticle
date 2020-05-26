#pragma once
#include <iostream>
#include<vector>
using namespace std;
class mathOp
{
public:
	mathOp();
	void getVectors(vector<double> input_1, vector<double> input_2);
	double calcMu(vector<double> input);
	double calcCorro(vector<double> input_1, vector<double> input_2);
	double calcVari(vector<double> input_1);
	double calcVari(vector<double> input_1, double mu);

private:
	double Mu_1;
	double Mu_2;
	double Var_1;
	double Var_2;

	vector<double> input_1;
	vector<double> input_2;

};

