#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

double sigmoid(float _X) {
	return 1 / (1 + exp(-1 * _X));
}

void Layer_Output_Caculator(double(*_Input)[1], double(*_Weight)[3], double(*_X)[1], double(*_Output)[1]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			_X[i][0] += _Weight[i][j] * _Input[j][0];
		}
		_Output[i][0] = sigmoid(_X[i][0]);
	}
}
int main() {
	double Input[3][1] = { 0 };
	double X_Hidden[3][1] = { 0 };
	double X_Output[3][1] = { 0 };
	double O_Hidden[3][1] = { 0 };
	double O_Output[3][1] = { 0 };
	double Weight_Input_Hidden[3][3];
	double Weight_Hidden_Output[3][3];
	printf("Enter the Input-Layer\n");
	for (int i = 0; i < 3; i++) {
		scanf_s("%lf", &Input[i][0]);
	}
	printf("Enter the initial Weight between I-H Layer\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			scanf_s("%lf", &Weight_Input_Hidden[i][j]);
		}
	}
	printf("Enter the initial Weight between H-O Layer\n");
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			scanf_s("%lf", &Weight_Hidden_Output[i][j]);
		}
	}
	
	// Forward-Propagation execution
	Layer_Output_Caculator(Input, Weight_Input_Hidden, X_Hidden, O_Hidden);
	Layer_Output_Caculator(O_Hidden, Weight_Hidden_Output, X_Output, O_Output);

	printf("-----------------Result-----------------\n\n");
	printf("%-10s%-10s%-10s%-10s%-10s\n\n", "Input", "X_Hidden", "O_Hidden", "X_Output", "O_Output");
	for (int i = 0; i < 3; i++) {
		printf("%-10.3lf%-10.3lf%-10.3lf%-10.3lf%-10.3lf\n\n",
			Input[i][0], X_Hidden[i][0], O_Hidden[i][0], X_Output[i][0], O_Output[i][0]);
	}
}
