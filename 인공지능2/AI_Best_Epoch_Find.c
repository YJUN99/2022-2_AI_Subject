#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Learning_Rate 0.5
#define INPUT_SZ 3
#define OUTPUT_SZ 3
#define HIDDEN1_SZ 5
#define HIDDEN2_SZ 3

double E_Ch_Value_I_H1[INPUT_SZ][HIDDEN1_SZ] = { 0 };
double E_Ch_Value_H1_H2[HIDDEN1_SZ][HIDDEN2_SZ] = { 0 };
double E_Ch_Value_H2_O[HIDDEN2_SZ][OUTPUT_SZ] = { 0 };

double Hidden2_tmp[HIDDEN2_SZ][OUTPUT_SZ];
double Hidden1_tmp[HIDDEN1_SZ][HIDDEN2_SZ];

// Test 변수
double testData[20][6];
double testInput[3];
double test_target[3];

double Test_Error_Red[3];
double Test_Error_Green[3];
double Test_Error_Blue[3];

double TargetRed[3] = { 1,0,0 };
double TargetGreen[3] = { 0,1,0 };
double TargetBlue[3] = { 0,0,1 };
double Test_Total_Error[3];
int CorrectCnt = 0;
//
double find_min_value(double a, double b, double c) {
	if (a < b && a < c) return a;
	else if (b < a && b < c) return b;
	else return c;
}

double user_abs(double a) {
	return a < 0 ? -1 * a : a;
}

double sigmoid(double _X) {
	return 1 / (1 + exp(-1 * _X));
}

void Forward_Propagation_1(double(*_Input), double(*_weight_I_H1)[HIDDEN1_SZ], double(*_X), double(*_Output)) {
	for (int i = 0; i < HIDDEN1_SZ; i++) {
		_X[i] = 0;
		for (int j = 0; j < INPUT_SZ; j++) {
			_X[i] += _weight_I_H1[j][i] * _Input[j];
		}
		_Output[i] = sigmoid(_X[i]);
	}
}

void Forward_Propagation_2(double(*_Input), double(*_weight_H1_H2)[HIDDEN2_SZ], double(*_X), double(*_Output)) {
	for (int i = 0; i < HIDDEN2_SZ; i++) {
		_X[i] = 0;
		for (int j = 0; j < HIDDEN1_SZ; j++) {
			_X[i] += _weight_H1_H2[j][i] * _Input[j];
		}
		_Output[i] = sigmoid(_X[i]);
	}
}

void Forward_Propagation_3(double(*_Input), double(*_weight_H2_O)[OUTPUT_SZ], double(*_X), double(*_Output)) {
	for (int i = 0; i < OUTPUT_SZ; i++) {
		_X[i] = 0;
		for (int j = 0; j < HIDDEN2_SZ; j++) {
			_X[i] += _weight_H2_O[j][i] * _Input[j];
		}
		_Output[i] = sigmoid(_X[i]);
	}
}

void Func_Backward_3(int _start, int _des, double(*Layer_H2_O), double(*Layer_Output_O), double(*target)) {
	double tmp = (Layer_Output_O[_des] - target[_des]) * (Layer_Output_O[_des]) * (1 - Layer_Output_O[_des]);
	Hidden2_tmp[_start][_des] = tmp;
	E_Ch_Value_H2_O[_start][_des] = tmp * Layer_H2_O[_start];
}

void Func_Backward_2(int _start, int _des, double(*_weight_H2_O)[OUTPUT_SZ], double(*Layer_H1_O), double(*Layer_H2_O)) {

	double tmp1 = Hidden2_tmp[_des][0] * _weight_H2_O[_des][0] * (Layer_H2_O[_des]) * (1 - Layer_H2_O[_des]);
	double tmp2 = Hidden2_tmp[_des][1] * _weight_H2_O[_des][1] * (Layer_H2_O[_des]) * (1 - Layer_H2_O[_des]);
	double tmp3 = Hidden2_tmp[_des][2] * _weight_H2_O[_des][2] * (Layer_H2_O[_des]) * (1 - Layer_H2_O[_des]);
	Hidden1_tmp[_start][_des] = tmp1 + tmp2 + tmp3;
	E_Ch_Value_H1_H2[_start][_des] = (tmp1 + tmp2 + tmp3) * Layer_H1_O[_start];

}

void Func_Backward_1(int _start, int _des, double(*_weight_H1_H2)[HIDDEN2_SZ], double(*Input), double(*Layer_H1_O)) {

	double tmp1 = Hidden1_tmp[_des][0] * _weight_H1_H2[_des][0] * (Layer_H1_O[_des]) * (1 - Layer_H1_O[_des]);
	double tmp2 = Hidden1_tmp[_des][1] * _weight_H1_H2[_des][1] * (Layer_H1_O[_des]) * (1 - Layer_H1_O[_des]);
	double tmp3 = Hidden1_tmp[_des][2] * _weight_H1_H2[_des][2] * (Layer_H1_O[_des]) * (1 - Layer_H1_O[_des]);
	E_Ch_Value_I_H1[_start][_des] = (tmp1 + tmp2 + tmp3) * Input[_start];

}

void Func_Update_Weight(double(*_weight_H2_O)[OUTPUT_SZ], double(*_weight_H1_H2)[HIDDEN2_SZ], double(*_weight_I_H1)[HIDDEN1_SZ]) {
	for (int i = 0; i < HIDDEN2_SZ; i++) {
		for (int j = 0; j < OUTPUT_SZ; j++) {
			_weight_H2_O[i][j] = _weight_H2_O[i][j] - Learning_Rate * E_Ch_Value_H2_O[i][j];
		}
	}
	for (int i = 0; i < HIDDEN1_SZ; i++) {
		for (int j = 0; j < HIDDEN2_SZ; j++) {
			_weight_H1_H2[i][j] = _weight_H1_H2[i][j] - Learning_Rate * E_Ch_Value_H1_H2[i][j];
		}
	}
	for (int i = 0; i < INPUT_SZ; i++) {
		for (int j = 0; j < HIDDEN1_SZ; j++) {
			_weight_I_H1[i][j] = _weight_I_H1[i][j] - Learning_Rate * E_Ch_Value_I_H1[i][j];
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

void File_Write_Weight(FILE* _fp_3, FILE* _fp_2, FILE* _fp_1, double(*_weight3)[OUTPUT_SZ], double(*_weight2)[HIDDEN2_SZ], double(*_weight1)[HIDDEN1_SZ], int _epoch, int _p) {
	fprintf(_fp_3, "%d,", _p + _epoch * 200 + 1);
	fprintf(_fp_2, "%d,", _p + _epoch * 200 + 1);
	fprintf(_fp_1, "%d,", _p + _epoch * 200 + 1);
	for (int i = 0; i < HIDDEN2_SZ; i++) {
		for (int j = 0; j < OUTPUT_SZ; j++) {
			fprintf(_fp_3, "%lf", _weight3[i][j]);
			if (i == HIDDEN2_SZ - 1 && j == OUTPUT_SZ - 1) break;
			fprintf(_fp_3, ",");
		}
		//fprintf(_fp_3, "\n");
	}
	fprintf(_fp_3, "\n");

	for (int i = 0; i < HIDDEN1_SZ; i++) {
		for (int j = 0; j < HIDDEN2_SZ; j++) {
			fprintf(_fp_2, "%lf", _weight2[i][j]);
			if (i == HIDDEN1_SZ - 1 && j == HIDDEN2_SZ - 1) break;
			fprintf(_fp_2, ",");
		}
		//fprintf(_fp_2, "\n");
	}
	fprintf(_fp_2, "\n");
	for (int i = 0; i < INPUT_SZ; i++) {
		for (int j = 0; j < HIDDEN1_SZ; j++) {
			fprintf(_fp_1, "%lf", _weight1[i][j]);
			if (i == INPUT_SZ - 1 && j == HIDDEN1_SZ - 1) break;
			fprintf(_fp_1, ",");
		}
		//fprintf(_fp_1, "\n");
	}
	fprintf(_fp_1, "\n");
}

void File_Write_Output(FILE* _fp, double(*_Output), double(*target)) {
	fprintf(_fp, "%lf,%lf,%lf,%lf,%lf,%lf,\n", _Output[0], _Output[1], _Output[2], target[0], target[1], target[2]);
}

int Neural_Network_Test(double(*weight_H2_O)[OUTPUT_SZ], double(*weight_H1_H2)[HIDDEN2_SZ], double(*weight_I_H1)[HIDDEN1_SZ],
	double(*Layer_H1_Z), double(*Layer_H1_O), double(*Layer_H2_Z), double(*Layer_H2_O), double(*Layer_Output_Z), double(*Layer_Output_O), FILE* Test_case) {

	for (int testcase = 0; testcase < 20; testcase++) {
		printf("------------------------\n");
		testInput[0] = testData[testcase][0];
		testInput[1] = testData[testcase][1];
		testInput[2] = testData[testcase][2];
		test_target[0] = testData[testcase][3];
		test_target[1] = testData[testcase][4];
		test_target[2] = testData[testcase][5];
		printf("Your Input [%lf  %lf  %lf]\n", testInput[0], testInput[1], testInput[2]);
		Forward_Propagation_1(testInput, weight_I_H1, Layer_H1_Z, Layer_H1_O);
		Forward_Propagation_2(Layer_H1_O, weight_H1_H2, Layer_H2_Z, Layer_H2_O);
		Forward_Propagation_3(Layer_H2_O, weight_H2_O, Layer_Output_Z, Layer_Output_O);
		Test_Error_Red[0] = TargetRed[0] - Layer_Output_O[0];
		Test_Error_Red[1] = TargetRed[1] - Layer_Output_O[1];
		Test_Error_Red[2] = TargetRed[2] - Layer_Output_O[2];
		Test_Error_Red[0] *= Test_Error_Red[0] * 0.5;
		Test_Error_Red[1] *= Test_Error_Red[1] * 0.5;
		Test_Error_Red[2] *= Test_Error_Red[2] * 0.5;
		Test_Error_Green[0] = TargetGreen[0] - Layer_Output_O[0];
		Test_Error_Green[1] = TargetGreen[1] - Layer_Output_O[1];
		Test_Error_Green[2] = TargetGreen[2] - Layer_Output_O[2];
		Test_Error_Green[0] *= Test_Error_Green[0] * 0.5;
		Test_Error_Green[1] *= Test_Error_Green[1] * 0.5;
		Test_Error_Green[2] *= Test_Error_Green[2] * 0.5;
		Test_Error_Blue[0] = TargetBlue[0] - Layer_Output_O[0];
		Test_Error_Blue[1] = TargetBlue[1] - Layer_Output_O[1];
		Test_Error_Blue[2] = TargetBlue[2] - Layer_Output_O[2];
		Test_Error_Blue[0] *= Test_Error_Blue[0] * 0.5;
		Test_Error_Blue[1] *= Test_Error_Blue[1] * 0.5;
		Test_Error_Blue[2] *= Test_Error_Blue[2] * 0.5;

		Test_Total_Error[0] = Test_Error_Red[0] + Test_Error_Red[1] + Test_Error_Red[2];
		Test_Total_Error[1] = Test_Error_Green[0] + Test_Error_Green[1] + Test_Error_Green[2];
		Test_Total_Error[2] = Test_Error_Blue[0] + Test_Error_Blue[1] + Test_Error_Blue[2];

		double min_value = find_min_value(Test_Total_Error[0], Test_Total_Error[1], Test_Total_Error[2]);

		printf("#%d RED_T-O::   %6lf %6lf %6lf Error:Red  : %6lf\n", testcase, Test_Error_Red[0], Test_Error_Red[1], Test_Error_Red[2], Test_Error_Red[0] + Test_Error_Red[1] + Test_Error_Red[2]);
		printf("#%d Green_T-O:: %6lf %6lf %6lf Error:Green: %6lf\n", testcase, Test_Error_Green[0], Test_Error_Green[1], Test_Error_Green[2], Test_Error_Green[0] + Test_Error_Green[1] + Test_Error_Green[2]);
		printf("#%d Blue_T-O::  %6lf %6lf %6lf Error:Blue : %6lf\n", testcase, Test_Error_Blue[0], Test_Error_Blue[1], Test_Error_Blue[2], Test_Error_Blue[0] + Test_Error_Blue[1] + Test_Error_Blue[2]);

		printf("Output : %10lf %10lf %10lf\n", Layer_Output_O[0], Layer_Output_O[1], Layer_Output_O[2]);

		if (min_value == Test_Total_Error[0]) {
			printf("#%d It's Red\n\n", testcase);
			if (test_target[0] == 1) {
				CorrectCnt++;
				printf("-> Correct!\n\n");
			}
			else printf("-> Wrong Answer\n\n");
			fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", testInput[0], testInput[1], testInput[2], Layer_Output_O[0], Layer_Output_O[1], Layer_Output_O[2], TargetRed[0], TargetRed[1], TargetRed[2]);
		}
		else if (min_value == Test_Total_Error[1]) {
			printf("#%d It's Green\n\n", testcase);
			if (test_target[1] == 1) {
				CorrectCnt++;
				printf("-> Correct!\n\n");
			}
			else printf("-> Wrong Answer\n\n");
			fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", testInput[0], testInput[1], testInput[2], Layer_Output_O[0], Layer_Output_O[1], Layer_Output_O[2], TargetGreen[0], TargetGreen[1], TargetGreen[2]);
		}
		else if (min_value == Test_Total_Error[2]) {
			printf("#%d It's Blue\n\n", testcase);
			if (test_target[2] == 1) {
				CorrectCnt++;
				printf("-> Correct!\n\n");
			}
			else printf("-> Wrong Answer\n\n");

			fprintf(Test_case, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", testInput[0], testInput[1], testInput[2], Layer_Output_O[0], Layer_Output_O[1], Layer_Output_O[2], TargetBlue[0], TargetBlue[1], TargetBlue[2]);
		}
	}
	return CorrectCnt == 20 ? 1 : 0;
}

void Generation_New_Test_Data_Set() {
	srand((unsigned)time(NULL));

	int i;
	double max = 32767;
	double target_point[3][3] = { {0, 0, 0},	// R
								  {1, 0.5, 1},	// G
								  {0.5, 1, 0.5} };	// B
	double point_x, point_y, point_z;
	double x, y, z;
	int j;
	double dis[3];

	for (i = 0; i < 20; i++) {
		point_x = rand() / max;
		point_y = rand() / max;
		point_z = rand() / max;

		dis[0] = (target_point[0][0] - point_x) * (target_point[0][0] - point_x) +
			(target_point[0][1] - point_y) * (target_point[0][1] - point_y) +
			(target_point[0][2] - point_z) * (target_point[0][2] - point_z);
		dis[0] = sqrtf(dis[0]);

		dis[1] = (target_point[1][0] - point_x) * (target_point[1][0] - point_x) +
			(target_point[1][1] - point_y) * (target_point[1][1] - point_y) +
			(target_point[1][2] - point_z) * (target_point[1][2] - point_z);
		dis[1] = sqrtf(dis[1]);

		dis[2] = (target_point[2][0] - point_x) * (target_point[2][0] - point_x) +
			(target_point[2][1] - point_y) * (target_point[2][1] - point_y) +
			(target_point[2][2] - point_z) * (target_point[2][2] - point_z);
		dis[2] = sqrtf(dis[2]);

		double tmp = find_min_value(dis[0], dis[1], dis[2]);

		if (tmp > 0.2) {
			i--;
			continue;
		}
		for (j = 0; j < 3; j++) {
			if (dis[j] == tmp) {
				break;
			}
		}
		testData[i][0] = point_x;
		testData[i][1] = point_y;
		testData[i][2] = point_z;
		if (j == 0 && i % 3 == 0) {
			testData[i][3] = 1;
			testData[i][4] = 0;
			testData[i][5] = 0;
		}
		else if (j == 1 && i % 3 == 1) {
			testData[i][3] = 0;
			testData[i][4] = 1;
			testData[i][5] = 0;
		}
		else if (j == 2 && i % 3 == 2) {
			testData[i][3] = 0;
			testData[i][4] = 0;
			testData[i][5] = 1;
		}
		else {
			i--;
			continue;
		}
	}
}
int main() {
	double weight_I_H1[INPUT_SZ][HIDDEN1_SZ] = { {0.3 , 0.25, 0.5 , 0.1 , 0.05},				// 
											 {0.7 , 0.1, 0.2, 0.35, 0.4},					// 
											 {0.55, 0.4, 0.5, 0.78, 0.33} };				//
	double weight_H1_H2[HIDDEN1_SZ][HIDDEN2_SZ] = { {0.41, 0.4, 0.15},							// -> ������ �� 0.96
											   {0.05, 0.5, 0.9},							// -> ������ �� 1
											   {0.2, 0.3, 0.5},								// -> ������ �� 1
											   {0.1 , 0.2, 0.4},							// -> ������ �� 0.7
											   {0.03, 0.25, 0.01} };						// -> ������ �� 0.29
	double weight_H2_O[HIDDEN2_SZ][OUTPUT_SZ] = { {0.25, 0.9, 0.5},							     // -> ������ �� 1.65
											  {0.3 , 0.6, 0.2},								 // -> ������ �� 1.1
											  {0.05, 0.25, 0.05} };							 // -> ������ �� 0.35
	double Layer_H1_Z[HIDDEN1_SZ] = { 0 };
	double Layer_H1_O[HIDDEN1_SZ] = { 0 };
	double Layer_H2_Z[HIDDEN2_SZ] = { 0 };
	double Layer_H2_O[HIDDEN2_SZ] = { 0 };
	double Layer_Output_Z[OUTPUT_SZ] = { 0 };
	double Layer_Output_O[OUTPUT_SZ] = { 0 };
	double Error_Node[OUTPUT_SZ] = { 0 };
	double LearningData[1000][6];
	double Input[INPUT_SZ] = { 0.1, 0.1, 0.1 };
	double target[OUTPUT_SZ] = { 1,0,0 };

	int All_Correct_Flag;
	int Test_Correct_CNT = 0;
	int tmp = 0;
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
	fprintf(fp_Weight1, "x,w11,w12,w13,w14,w15,w21,w22,w23,w24,w25,w31,w32,w33,w34,w35\n");
	fprintf(fp_Weight2, "x,w11,w12,w13,w21,w22,w23,w31,w32,w33,w41,w42,w43,w51,w52,w53\n");
	fprintf(fp_Weight3, "x,w11,w12,w13,w21,w22,w23,w31,w32,w33\n");

	//---------------------Test--------------------------

	fprintf(Test_case, "Input1,Input2,Input3,Output1,Output2,Output3,Target1,Target2,Target3\n");
	//---------------------Test--------------------------


	printf("Enter LearningData\n");
	for (int i = 0; i < 200; i++) {
		scanf("%lf %lf %lf %lf %lf %lf",
			&LearningData[i][0], &LearningData[i][1], &LearningData[i][2],
			&LearningData[i][3], &LearningData[i][4], &LearningData[i][5]);

		fprintf(Three_D_Axis, "%.4lf,%.4lf,%.4lf,%.4lf,%.4lf,%.4lf\n",
			LearningData[i][0], LearningData[i][1], LearningData[i][2],
			LearningData[i][3], LearningData[i][4], LearningData[i][5]);
	}
	printf("Enter TestInput\n");
	for (int i = 0; i < 20; i++) {
		scanf("%lf %lf %lf %lf %lf %lf", &testData[i][0], &testData[i][1], &testData[i][2], &testData[i][3], &testData[i][4], &testData[i][5]);
	}
	int epoch = 1;
	for (int k = 0; k < epoch; k++) {
		for (int p = 0; p < 200; p++) {
			Input[0] = LearningData[p][0];
			Input[1] = LearningData[p][1];
			Input[2] = LearningData[p][2];
			target[0] = LearningData[p][3];
			target[1] = LearningData[p][4];
			target[2] = LearningData[p][5];

			// 순전파 진행
			Forward_Propagation_1(Input, weight_I_H1, Layer_H1_Z, Layer_H1_O);
			Forward_Propagation_2(Layer_H1_O, weight_H1_H2, Layer_H2_Z, Layer_H2_O);
			Forward_Propagation_3(Layer_H2_O, weight_H2_O, Layer_Output_Z, Layer_Output_O);

			Error_Node[0] = (target[0] - Layer_Output_O[0]) * (target[0] - Layer_Output_O[0]) * 0.5;
			Error_Node[1] = (target[1] - Layer_Output_O[1]) * (target[1] - Layer_Output_O[1]) * 0.5;
			Error_Node[2] = (target[2] - Layer_Output_O[2]) * (target[2] - Layer_Output_O[2]) * 0.5;

			// 최종 입력에 대한 정보 입력
			File_Write_Output(Final_Output, Layer_Output_O, target);
			// 최종 오차 입력
			fprintf(fp_Error_Total, "%lf\n", (Error_Node[0] + Error_Node[1] + Error_Node[2]));

			for (int Prev = 0; Prev < HIDDEN2_SZ; Prev++) {
				for (int Next = 0; Next < OUTPUT_SZ; Next++) {
					Func_Backward_3(Prev, Next, Layer_H2_O, Layer_Output_O, target);
				}
			}

			for (int Prev = 0; Prev < HIDDEN1_SZ; Prev++) {
				for (int Next = 0; Next < HIDDEN2_SZ; Next++) {
					Func_Backward_2(Prev, Next, weight_H2_O, Layer_H1_O, Layer_H2_O);
				}
			}

			for (int Prev = 0; Prev < INPUT_SZ; Prev++) {
				for (int Next = 0; Next < HIDDEN1_SZ; Next++) {
					Func_Backward_1(Prev, Next, weight_H1_H2, Input, Layer_H1_O, Layer_H2_O);
				}
			}
			File_Write_Weight(fp_Weight3, fp_Weight2, fp_Weight1, weight_H2_O, weight_H1_H2, weight_I_H1, k, p);
			Func_Update_Weight(weight_H2_O, weight_H1_H2, weight_I_H1);
		}
		// 학습 1에폭 완료 -> 테스트 진행 후 에폭 증가 여부 확인
		while (1) {
			printf("Testcnt : %d\n", Test_Correct_CNT);
			if (Test_Correct_CNT != 500) {
				All_Correct_Flag = Neural_Network_Test(weight_H2_O, weight_H1_H2, weight_I_H1, Layer_H1_Z, Layer_H1_O, Layer_H2_Z, Layer_H2_O, Layer_Output_Z, Layer_Output_O, Test_case);
				if (All_Correct_Flag == 1) {
					Test_Correct_CNT++;
					tmp = CorrectCnt;
					CorrectCnt = 0;
					Generation_New_Test_Data_Set();	// Test Input data가 변경됨.
				}
				else {
					CorrectCnt = 0;
					epoch++;
					break;
				}
			}
			else break;
		}
	}
	printf("Final Epoch : %d \n", epoch);
	printf("Hit Ratio : %d / %d %.2lf %%\n", tmp, 20, (double)tmp / 20 * 100);
}
