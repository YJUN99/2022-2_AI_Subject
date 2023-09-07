#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>

double min(double a, double b, double c) {
	if (a < b && a < c) return a;
	else if (b < a && b < c) return b;
	else return c;
}

double user_abs(double a) {
	return a < 0 ? -1 * a : a;
}
int main(void)
{
	int i;
	double max = 32767;
	double target_point[3][3] = { {0, 0, 0},	// R
								  {1, 0.5, 1},	// Y
								  {0.5, 1, 0.5} };	// G
	double point_x, point_y, point_z;
	double x, y, z;
	int j;
	double dis[3];
	FILE* fp = fopen("3D_Area.csv", "w+");
	fprintf(fp, "x,y,z,R,G,B\n");
	double Input[6][200];
	for (i = 0; i < 200; i++) {
		point_x = rand() / max ;
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

		double tmp = min(dis[0], dis[1], dis[2]);

		if (tmp > 0.2) {
			i--;
			continue;
		}
		for (j = 0; j < 3; j++) {
			if (dis[j] == tmp) {
				break;
			}
		}
		Input[0][i] = point_x;
		Input[1][i] = point_y;
		Input[2][i] = point_z;
		if (j == 0 && i % 3 == 0) {
			Input[3][i] = 1;
			Input[4][i] = 0;
			Input[5][i] = 0;
		}
		else if (j == 1 && i % 3 == 1) {
			Input[3][i] = 0;
			Input[4][i] = 1;
			Input[5][i] = 0;
		}
		else if (j == 2 && i % 3 == 2) {
			Input[3][i] = 0;
			Input[4][i] = 0;
			Input[5][i] = 1;
		}
		else {
			i--;
			continue;
		}
	}
	for (int i = 0; i < 200; i++) {
		printf("%10.4f %10.4f %10.4f %10.4f %10.4f %10.4f\n", Input[0][i], Input[1][i], Input[2][i], Input[3][i], Input[4][i], Input[5][i]);
		fprintf(fp, "%10.4lf,%10.4lf,%10.4lf,%10.4lf,%10.4lf,%10.4lf\n", Input[0][i], Input[1][i], Input[2][i], Input[3][i], Input[4][i], Input[5][i]);
	}
	return 0;
}
