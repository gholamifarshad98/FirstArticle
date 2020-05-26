#include "mathOp.h"
#include <cmath>
mathOp::mathOp()
{
}

void mathOp::getVectors(vector<double> a, vector<double>b) {
	input_1 = a;
	input_2 = b;
}

double mathOp::calcMu(vector<double> input)
{
	double sum = 0;
	for (size_t i = 0; i < input.size(); i++) {
		sum = sum + input[i];
	}
	if (input.size() == 0) {
		cout << "the size of your input is zero!!!" << endl;
		return 0;
	}
	return sum/input.size();
}

double mathOp::calcCorro(vector<double> _input_1, vector<double> _input_2)
{
	input_1 = _input_1;
	input_2 = _input_2;
	Mu_1 = calcMu(input_1);
	Mu_2 = calcMu(input_2);
	Var_1 = calcVari(input_1, Mu_1);
	Var_2 = calcVari(input_2, Mu_2);
	double num = 0;
	for (size_t i = 0; i < input_1.size(); i++) {
		num = num + (input_1[i]-Mu_1) * (input_2[i]-Mu_2);
	}
	double denum = Var_1 * Var_2;

	if (denum == 0) {
		cout << "somr thing wrong... the denum is zearo..." << endl;
		return 0;
	}
	



	return num/denum;
}

double mathOp::calcVari(vector<double> input)
{
	auto Mu = calcMu(input);

	return calcVari(input , Mu);
}

double mathOp::calcVari(vector<double> input, double mu)
{
	double vari = 0;
	for (size_t i = 0; i < input.size(); i++) {
		vari = vari + pow((input[i] - mu),2);
	}
	return sqrt(vari);
}


