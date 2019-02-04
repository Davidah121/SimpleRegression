#include <iostream>
#include "Matrix.h"
#include "BasicIO.h"
#include <vector>
#include <string>
#include <math.h>

struct point
{
	float x;
	float y;
};


std::vector<point> pointsOfEquation = std::vector<point>();

void getValues(char* filename)
{
	BasicIO file(filename);

	std::string tempData = "";
	char byte = '\0';

	bool xSet = false;

	point tempPoint;

	while (!file.isEndOfFile())
	{
		byte = file.readByte();

		if (byte != ',')
		{
			tempData += byte;
		}
		else
		{
			if (xSet == false)
			{
				tempPoint.x = std::stof(tempData);
				tempData = "";
				xSet = true;
			}
			else
			{
				tempPoint.y = std::stof(tempData);
				tempData = "";
				xSet = false;

				pointsOfEquation.push_back(tempPoint);
			}
		}
	}

	file.close();
}

void doLinearRegression()
{
	//float* values = new float[pointsOfEquation.size()];
	//B = Inverse( Transpose(X) * X ) * Transpose(X) * Y;

	Matrix X = Matrix(pointsOfEquation.size(), 2);
	Matrix Y = Matrix(pointsOfEquation.size(), 1);
	Matrix TransposeX = Matrix(2, pointsOfEquation.size());

	for (int i = 0; i < pointsOfEquation.size(); i++)
	{
		X[i][0] = 1;
		X[i][1] = pointsOfEquation[i].x;

		TransposeX[0][i] = 1;
		TransposeX[1][i] = pointsOfEquation[i].x;

		Y[i][0] = pointsOfEquation[i].y;
	}

	Matrix Answer = (TransposeX*X).getInverse() * TransposeX * Y;
	
	std::cout << "XT * X Transpose Matrix" << std::endl;
	for (int i = 0; i < Answer.getRows(); i++)
	{
		for (int i2 = 0; i2 < Answer.getCols(); i2++)
		{
			std::cout << Answer[i][i2] << ", ";
		}
		std::cout << std::endl;
	}

	std::cout << "Y - Intercept: " << Answer[0][0] << std::endl;
	std::cout << "Slope: " << Answer[1][0] << std::endl;
}


void createEquation()
{
	//float* values = new float[pointsOfEquation.size()];

	Matrix A = Matrix(pointsOfEquation.size(), pointsOfEquation.size());

	for (int i = 0; i < A.getRows(); i++)
	{
		for (int i2 = 0; i2 < A.getCols(); i2++)
		{
			if (A.getCols() - i2 - 1 == 0)
			{
				A[i][i2] = 1;
			}
			else if (A.getCols() - i2 - 1 == -1)
			{
				A[i][i2] = pointsOfEquation[i].y;
			}
			else
			{
				A[i][i2] = pow(pointsOfEquation[i].x, A.getCols() - i2 - 1);
			}
		}
	}

	Matrix matrixOfY = Matrix(pointsOfEquation.size(), 1);

	for (int i = 0; i < A.getRows(); i++)
	{
		matrixOfY[i][0] = pointsOfEquation[i].y;
	}

	Matrix inverseMat = A.getInverse();

	Matrix finalAnswers = inverseMat * matrixOfY;

	for (int i = 0; i < finalAnswers.getRows(); i++)
	{
		if (finalAnswers.getRows() - 1 - i == 0)
		{
			std::cout << finalAnswers[i][0] << std::endl;
		}
		else if (finalAnswers.getRows() - 1 - i == 0)
		{
			std::cout << finalAnswers[i][0] << " X" << std::endl;
		}
		else
		{
			std::cout << finalAnswers[i][0] << " X^" << finalAnswers.getRows() - 1 - i << std::endl;
		}
	}
}

void createApproximateEquation(int degree)
{
	Matrix X = Matrix(pointsOfEquation.size(), degree+1);
	Matrix Y = Matrix(pointsOfEquation.size(), 1);
	
	for (int i = 0; i < pointsOfEquation.size(); i++)
	{
		//X[i][0] = 1;
		//X[i][1] = pointsOfEquation[i].x;

		for (int i2 = 0; i2 < X.getCols(); i2++)
		{
			if (X.getCols() - i2 - 1 == 0)
			{
				X[i][i2] = 1;
			}
			else if (X.getCols() - i2 - 1 == -1)
			{
				//Should not be possible
				X[i][i2] = pointsOfEquation[i].y;
			}
			else
			{
				X[i][i2] = pow(pointsOfEquation[i].x, X.getCols() - i2 - 1);
			}
		}

		Y[i][0] = pointsOfEquation[i].y;
	}

	Matrix TransposeX = X.getTranspose();

	Matrix Answer = (TransposeX*X).getInverse() * TransposeX * Y;

	std::cout << "Answer: " << std::endl;
	for (int i = 0; i < Answer.getRows(); i++)
	{
		if (degree - i > 1)
		{
			std::cout << Answer[i][0] << "X^" << (degree - i) << std::endl;
		}
		else if (degree - i == 1)
		{
			std::cout << Answer[i][0] << "X" << std::endl;
		}
		else
		{
			std::cout << Answer[i][0] << std::endl;
		}
	}

}
void main(int sizeOfInput, char** input)
{
	for (int i = 0; i < sizeOfInput; i++)
	{
		std::cout << input[i] << std::endl;
	}

	if (sizeOfInput > 1)
	{
		getValues(input[1]);
		createApproximateEquation( std::stoi(input[2]) );
	}
	else
	{
		std::cout << std::endl;
		std::cout << "Not enough inputs" << std::endl;
		std::cout << "Please input a file with your points, and input the highest degree equation you desire." << std::endl;
		std::cout << "Please note that your desired degree can not be higher than the amount of points given - 1." << std::endl;
		std::cout << "Requires a file with values separated by commas with an ending comma" << std::endl;
		std::cout << std::endl;
	}

	system("pause");
}