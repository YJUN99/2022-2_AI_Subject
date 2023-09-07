#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

#define Learning_Rate 0.5
#define INPUT_SZ 3
#define OUTPUT_SZ 3
#define HIDDEN1_SZ 5
#define HIDDEN2_SZ 3
#define bias 0.1

double Error_Hidden1[INPUT_SZ][HIDDEN1_SZ] = { 0 };
double Error_Hidden2[HIDDEN1_SZ][HIDDEN2_SZ] = { 0 };
double Error_Hidden3[HIDDEN2_SZ][OUTPUT_SZ] = { 0 };

double Layer3_tmp[HIDDEN2_SZ][OUTPUT_SZ];
double Layer2_tmp[HIDDEN1_SZ][HIDDEN2_SZ];

double sigmoid(double _X) {
	return 1 / (1 + exp(-1 * _X));
}

void Forward_Propagation_1(double(*_Input), double(*_Weight1)[HIDDEN1_SZ], double(*_X), double(*_Output)) {
	for (int i = 0; i < HIDDEN1_SZ; i++) {
		_X[i] = 0;
		for (int j = 0; j < INPUT_SZ; j++) {
			_X[i] += _Weight1[j][i] * _Input[j];
		}
		//_X[i] += 0.5;
		_Output[i] = sigmoid(_X[i]);
	}
}

void Forward_Propagation_2(double(*_Input), double(*_Weight2)[HIDDEN2_SZ], double(*_X), double(*_Output)) {
	for (int i = 0; i < HIDDEN2_SZ; i++) {
		_X[i] = 0;
		for (int j = 0; j < HIDDEN1_SZ; j++) {
			_X[i] += _Weight2[j][i] * _Input[j];
		}
		//_X[i] += 0.3;
		_Output[i] = sigmoid(_X[i]);
	}
}

void Forward_Propagation_3(double(*_Input), double(*_Weight3)[OUTPUT_SZ], double(*_X), double(*_Output)) {
	for (int i = 0; i < OUTPUT_SZ; i++) {
		_X[i] = 0;
		for (int j = 0; j < HIDDEN2_SZ; j++) {
			_X[i] += _Weight3[j][i] * _Input[j];
		}
		//_X[i] += 0.3;
		_Output[i] = sigmoid(_X[i]);
	}
}

void Func_Backward_3(int _start, int _des, double(*Layer2_O), double(*Layer3_O), double(*target)) {
	double tmp = (Layer3_O[_des] - target[_des]) * (Layer3_O[_des]) * (1 - Layer3_O[_des]);
	Layer3_tmp[_start][_des] = tmp;
	Error_Hidden3[_start][_des] = tmp * Layer2_O[_start];
}

void Func_Backward_2(int _start, int _des, double(*_weight3)[OUTPUT_SZ], double(*Layer1_O), double(*Layer2_O)) {

	double tmp1 = Layer3_tmp[_des][0] * _weight3[_des][0] * (Layer2_O[_des]) * (1 - Layer2_O[_des]);
	double tmp2 = Layer3_tmp[_des][1] * _weight3[_des][1] * (Layer2_O[_des]) * (1 - Layer2_O[_des]);
	double tmp3 = Layer3_tmp[_des][2] * _weight3[_des][2] * (Layer2_O[_des]) * (1 - Layer2_O[_des]);
	Layer2_tmp[_start][_des] = tmp1 + tmp2 + tmp3;
	Error_Hidden2[_start][_des] = (tmp1 + tmp2 + tmp3) * Layer1_O[_start];

}

void Func_Backward_1(int _start, int _des, double(*_weight2)[HIDDEN2_SZ], double(*Input), double(*Layer1_O)) {

	double tmp1 = Layer2_tmp[_des][0] * _weight2[_des][0] * (Layer1_O[_des]) * (1 - Layer1_O[_des]);
	double tmp2 = Layer2_tmp[_des][1] * _weight2[_des][1] * (Layer1_O[_des]) * (1 - Layer1_O[_des]);
	double tmp3 = Layer2_tmp[_des][2] * _weight2[_des][2] * (Layer1_O[_des]) * (1 - Layer1_O[_des]);
	Error_Hidden1[_start][_des] = (tmp1 + tmp2 + tmp3) * Input[_start];

}

void Func_Update_Weight(double(*weight3)[OUTPUT_SZ], double(*weight2)[HIDDEN2_SZ], double(*weight1)[HIDDEN1_SZ]) {
	for (int i = 0; i < HIDDEN2_SZ; i++) {
		for (int j = 0; j < OUTPUT_SZ; j++) {
			weight3[i][j] = weight3[i][j] - Learning_Rate * Error_Hidden3[i][j];
		}
	}
	for (int i = 0; i < HIDDEN1_SZ; i++) {
		for (int j = 0; j < HIDDEN2_SZ; j++) {
			weight2[i][j] = weight2[i][j] - Learning_Rate * Error_Hidden2[i][j];
		}
	}
	for (int i = 0; i < INPUT_SZ; i++) {
		for (int j = 0; j < HIDDEN1_SZ; j++) {
			weight1[i][j] = weight1[i][j] - Learning_Rate * Error_Hidden1[i][j];
		}
	}
}

int File_Open_Check(FILE* _fp) {
	if (_fp == NULL) {
		printf("FileOPEN ERROR");
		return 1;
	}
	else return 0;
}

void File_Write_Weight(FILE* _fp_3, FILE* _fp_2, FILE* _fp_1, double(*_weight3)[OUTPUT_SZ], double(*_weight2)[HIDDEN2_SZ], double(*_weight1)[HIDDEN1_SZ]) {
	for (int i = 0; i < HIDDEN2_SZ; i++) {
		for (int j = 0; j < OUTPUT_SZ; j++) {
			fprintf(_fp_3, "%lf,", _weight3[i][j]);
		}
		fprintf(_fp_3, "\n");
	}
	fprintf(_fp_3, "\n");

	for (int i = 0; i < HIDDEN1_SZ; i++) {
		for (int j = 0; j < HIDDEN2_SZ; j++) {
			fprintf(_fp_2, "%lf,", _weight2[i][j]);
		}
		fprintf(_fp_2, "\n");
	}
	fprintf(_fp_2, "\n");
	for (int i = 0; i < INPUT_SZ; i++) {
		for (int j = 0; j < HIDDEN1_SZ; j++) {
			fprintf(_fp_1, "%lf,", _weight1[i][j]);
		}
		fprintf(_fp_1, "\n");
	}
	fprintf(_fp_1, "\n");
}

void File_Write_Output(FILE* _fp, double(*_Output),double(*target)) {
	fprintf(_fp, "%lf,%lf,%lf,%lf,%lf,%lf\n", _Output[0], _Output[1], _Output[2], target[0], target[0], target[0]);
}

double min(double a, double b, double c) {
	if (a < b && a < c) return a;
	else if (b < a && b < c) return b;
	else return c;
}

int main() {

	double weight1[INPUT_SZ][HIDDEN1_SZ] = { {0.3 , 0.25, 0.5 , 0.4 , 0.35},
											 {0.4 , 0.32, 0.2, 0.35, 0.4},
											 {0.33, 0.4, 0.2, 0.45, 0.33} };

	double weight2[HIDDEN1_SZ][HIDDEN2_SZ] = { {0.41, 0.4, 0.3},
											   {0.3, 0.5, 0.15},
											   {0.4, 0.32, 0.5},
											   {0.4 , 0.22, 0.4},
											   {0.3, 0.25, 0.33} };

	double weight3[HIDDEN2_SZ][OUTPUT_SZ] = { {0.25, 0.4, 0.5},
											  {0.3 , 0.6, 0.51},
											  {0.4, 0.25, 0.2} };

	double Layer1_Z[HIDDEN1_SZ] = { 0 };
	double Layer1_O[HIDDEN1_SZ] = { 0 };

	double Layer2_Z[HIDDEN2_SZ] = { 0 };
	double Layer2_O[HIDDEN2_SZ] = { 0 };

	double Layer3_Z[OUTPUT_SZ] = { 0 };
	double Layer3_O[OUTPUT_SZ] = { 0 };


	double Error_Layer1[OUTPUT_SZ] = { 0 };

	double LearningData[1000][6];

	double Input[INPUT_SZ] = { 0.1, 0.1, 0.1 };

	double target[OUTPUT_SZ] = { 1,0,0 };

	//printf("-------\n");	
	FILE* fp_Error_Total = fopen("Error_Total.csv", "w+");
	FILE* fp_Weight3 = fopen("Weight3.csv", "w+");
	FILE* fp_Weight2 = fopen("Weight2.csv", "w+");
	FILE* fp_Weight1 = fopen("Weight1.csv", "w+");
	FILE* Final_Output = fopen("Final_Output.csv", "w+");
	FILE* Axis_Color = fopen("Axis_Color.csv", "w+");
	FILE* Three_D_Axis = fopen("3D_Axis.csv", "w+");
	FILE* Test_case = fopen("Test_Case.csv", "w+");

	if (File_Open_Check(fp_Error_Total) == 1) return 0;
	if (File_Open_Check(fp_Weight3) == 1) return 0;
	if (File_Open_Check(fp_Weight2) == 1) return 0;
	if (File_Open_Check(fp_Weight1) == 1) return 0;
	if (File_Open_Check(Final_Output) == 1) return 0;
	if (File_Open_Check(Axis_Color) == 1) return 0;
	if (File_Open_Check(Three_D_Axis) == 1) return 0;
	if (File_Open_Check(Test_case) == 1) return 0;

	fprintf(Axis_Color, "x,y,z,R,G,B\n");
	fprintf(Final_Output, "x,y,z,R,G,B\n");
	fprintf(Three_D_Axis, "x,y,z,R,G,B\n");

	for (int i = 0; i < 200; i++) {
		scanf("%lf %lf %lf %lf %lf %lf", 
			&LearningData[i][0], &LearningData[i][1], &LearningData[i][2], 
			&LearningData[i][3], &LearningData[i][4], &LearningData[i][5]);
		
		fprintf(Three_D_Axis, "%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf\n", 
			LearningData[i][0], LearningData[i][1], LearningData[i][2], LearningData[i][3], LearningData[i][4], LearningData[i][5]);
	}
	for (int k = 0; k < 100; k++) {
		for (int p = 0; p < 200; p++) {
			Input[0] = LearningData[p][0];
			Input[1] = LearningData[p][1];
			Input[2] = LearningData[p][2];
			target[0] = LearningData[p][3];
			target[1] = LearningData[p][4];
			target[2] = LearningData[p][5];

			Forward_Propagation_1(Input, weight1, Layer1_Z, Layer1_O);
			Forward_Propagation_2(Layer1_O, weight2, Layer2_Z, Layer2_O);
			Forward_Propagation_3(Layer2_O, weight3, Layer3_Z, Layer3_O);

			Error_Layer1[0] = (target[0] - Layer3_O[0]) * (target[0] - Layer3_O[0]) * 0.5;
			Error_Layer1[1] = (target[1] - Layer3_O[1]) * (target[1] - Layer3_O[1]) * 0.5;
			Error_Layer1[2] = (target[2] - Layer3_O[2]) * (target[2] - Layer3_O[2]) * 0.5;

			// File_Write_Weight(fp_Weight3, fp_Weight2, fp_Weight1, weight3, weight2, weight1);
			File_Write_Output(Final_Output, Layer3_O,target);	// ������ �ѻ���Ŭ ���������� ��°�
			fprintf(fp_Error_Total, "%lf\n", Error_Layer1[0] + Error_Layer1[1] + Error_Layer1[2]);
			//printf("%lf\n", Error_Layer1[0] + Error_Layer1[1] + Error_Layer1[2]);
			
			//fprintf(Axis_Color, "%lf,%lf,%lf,%lf,%lf,%lf\n");

			// printf("--------- cycle %d ------------- \n", k);
			// printf("��¾� ~ %10f %10f %10f\n", Layer3_O[0], Layer3_O[1], Layer3_O[2]);
			// ������ �Ϸ�
			
			for (int Prev = 0; Prev < HIDDEN2_SZ; Prev++) {
				for (int Next = 0; Next < OUTPUT_SZ; Next++) {
					Func_Backward_3(Prev, Next, Layer2_O, Layer3_O, target);
				}
			}

			for (int Prev = 0; Prev < HIDDEN1_SZ; Prev++) {
				for (int Next = 0; Next < HIDDEN2_SZ; Next++) {
					Func_Backward_2(Prev, Next, weight3, Layer1_O, Layer2_O);
				}
			}

			for (int Prev = 0; Prev < INPUT_SZ; Prev++) {
				for (int Next = 0; Next < HIDDEN1_SZ; Next++) {
					Func_Backward_1(Prev, Next, weight2, Input, Layer1_O, Layer2_O);
				}
			}
			File_Write_Weight(fp_Weight3, fp_Weight2, fp_Weight1, weight3, weight2, weight1);
			printf("--------Layer3-------\n");
			for (int Prev = 0; Prev < HIDDEN2_SZ; Prev++) {
				for (int Next = 0; Next < OUTPUT_SZ; Next++) {
					printf("%6lf ", Error_Hidden3[Prev][Next]);
				}
				printf("\n");
			}
			printf("--------Layer2-------\n");
			for (int Prev = 0; Prev < HIDDEN1_SZ; Prev++) {
				for (int Next = 0; Next < HIDDEN2_SZ; Next++) {
					printf("%6lf ", Error_Hidden2[Prev][Next]);
					
				}
				printf("\n");
			}
			printf("--------Layer1-------\n");
			for (int Prev = 0; Prev < INPUT_SZ; Prev++) {
				for (int Next = 0; Next < HIDDEN1_SZ; Next++) {
					printf("%6lf ", Error_Hidden1[Prev][Next]);

				}
				printf("\n");
			}
			Func_Update_Weight(weight3, weight2, weight1);
		}
	}

	double testInput[3];
	double test_target[3];

	double Test_Error_Red[3];
	double Test_Error_Green[3];
	double Test_Error_Blue[3];

	double TargetRed[3] = { 1,0,0 };
	double TargetGreen[3] = { 0,1,0 };
	double TargetBlue[3] = { 0,0,1 };
	double Test_Total_Error[3];
	fprintf(Test_case, "Input1,Input2,Input3,Output1,Output2,Output3,Target1,Target2,Target3\n");
	for (int testcase = 1; testcase <= 20; testcase++) {
		printf("Enter the TestInput\n\n");
		scanf("%lf %lf %lf", &testInput[0], &testInput[1], &testInput[2]);

		Forward_Propagation_1(testInput, weight1, Layer1_Z, Layer1_O);
		Forward_Propagation_2(Layer1_O, weight2, Layer2_Z, Layer2_O);
		Forward_Propagation_3(Layer2_O, weight3, Layer3_Z, Layer3_O);
		Test_Error_Red[0] = TargetRed[0] - Layer3_O[0];
		Test_Error_Red[1] = TargetRed[1] - Layer3_O[1];
		Test_Error_Red[2] = TargetRed[2] - Layer3_O[2];
		Test_Error_Red[0] *= Test_Error_Red[0] * 0.5;
		Test_Error_Red[1] *= Test_Error_Red[1] * 0.5;
		Test_Error_Red[2] *= Test_Error_Red[2] * 0.5;
		Test_Error_Green[0] = TargetGreen[0] - Layer3_O[0];
		Test_Error_Green[1] = TargetGreen[1] - Layer3_O[1];
		Test_Error_Green[2] = TargetGreen[2] - Layer3_O[2];
		Test_Error_Green[0] *= Test_Error_Green[0] * 0.5;
		Test_Error_Green[1] *= Test_Error_Green[1] * 0.5;
		Test_Error_Green[2] *= Test_Error_Green[2] * 0.5;
		Test_Error_Blue[0] = TargetBlue[0] - Layer3_O[0];
		Test_Error_Blue[1] = TargetBlue[1] - Layer3_O[1];
		Test_Error_Blue[2] = TargetBlue[2] - Layer3_O[2];

		Test_Error_Blue[0] *= Test_Error_Blue[0] * 0.5;
		Test_Error_Blue[1] *= Test_Error_Blue[1] * 0.5;
		Test_Error_Blue[2] *= Test_Error_Blue[2] * 0.5;

		Test_Total_Error[0] = Test_Error_Red[0] + Test_Error_Red[1] + Test_Error_Red[2];
		Test_Total_Error[1] = Test_Error_Green[0] + Test_Error_Green[1] + Test_Error_Green[2];
		Test_Total_Error[2] = Test_Error_Blue[0] + Test_Error_Blue[1] + Test_Error_Blue[2];
		
		double min_value = min(Test_Total_Error[0], Test_Total_Error[1], Test_Total_Error[2]);

		printf("#%d RED_T-O::   %10lf %10lf %10lf\n", testcase, Test_Error_Red[0], Test_Error_Red[1], Test_Error_Red[2]);
		printf("#%d Green_T-O:: %10lf %10lf %10lf\n", testcase, Test_Error_Green[0], Test_Error_Green[1], Test_Error_Green[2]);
		printf("#%d Blue_T-O::  %10lf %10lf %10lf\n", testcase, Test_Error_Blue[0], Test_Error_Blue[1], Test_Error_Blue[2]);

		printf("Output : %10lf %10lf %10lf\n", Layer3_O[0], Layer3_O[1], Layer3_O[2]);
		
		if (min_value == Test_Total_Error[0]) {
			printf("#%d It's Red\n\n",testcase);
			fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", testInput[0], testInput[1], testInput[2], Layer3_O[0], Layer3_O[1], Layer3_O[2], TargetRed[0], TargetRed[1], TargetRed[2]);
		}
		else if (min_value == Test_Total_Error[1]) {
			printf("#%d It's Green\n\n", testcase);
			fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", testInput[0], testInput[1], testInput[2], Layer3_O[0], Layer3_O[1], Layer3_O[2], TargetGreen[0], TargetGreen[1], TargetGreen[2]);
		}
		else if (min_value == Test_Total_Error[2]) {
			printf("#%d It's Blue\n\n", testcase);
			fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", testInput[0], testInput[1], testInput[2], Layer3_O[0], Layer3_O[1], Layer3_O[2], TargetBlue[0], TargetBlue[1], TargetBlue[2]);
		}
		//fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,", testInput[0], testInput[1], testInput[2], Layer3_O[0], Layer3_O[1], Layer3_O[2], Test_Error[0], Test_Error[1], Test_Error[2]);
		//fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", 1 - Layer3_O[0], 0 - Layer3_O[1], 0 - Layer3_O[2], 0 - Layer3_O[0], 1 - Layer3_O[1], 0 - Layer3_O[2], 0 - Layer3_O[0], 0 - Layer3_O[1], 1 - Layer3_O[2]);

	
	}
}
