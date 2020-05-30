#pragma once
#include <iostream>
#include<vector>
using namespace std;
class mathOp
{
public:
	mathOp();
	void getVectors(vector<int> input_1, vector<int> input_2);
	double calcMu(vector<int> input);
	double calcCorro(vector<int> input_1, vector<int> input_2);
	double calcVari(vector<int> input_1);
	double calcVari(vector<int> input_1, double mu);

private:
	double Mu_1;
	double Mu_2;
	double Var_1;
	double Var_2;

	vector<int> input_1;
	vector<int> input_2;

};

