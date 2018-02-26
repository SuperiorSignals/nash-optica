#include "matrix.h"

int matrix()
{
	int i;
	double temporary;
	double indexOfRefraction;
	double lensThickness;
	std::vector<double> angle;
	std::vector<double> sourceIntensity;
	std::vector<double> lensAngle;
	std::vector<double> specifiedIntensity;
	std::ofstream outputFile;
	std::ifstream inputFile;
	/*
	inputFile.open("input.lns");

	if (!inputFile.is_open()) {
		std::cout << "ERROR: Unable to open file input.lns" << std::endl;
		return 1;
	}
	*/
	for (i = 0; i < SAMPLE_NUMBER / 2; i++) {
		temporary = PI * static_cast<double>(i) / (SAMPLE_NUMBER);
		angle.push_back(temporary);
		sourceIntensity.push_back(gaussian(temporary, 0, 0.5));
		specifiedIntensity.push_back(gaussian(temporary, 0, 0.7));
		//specifiedIntensity.push_back(maxwell(temporary, 0.5));
	}
	//generateIntermediateAngle(angle, sourceIntensity, lensAngle, specifiedIntensity);
	histogramSpecification(lensAngle, sourceIntensity, specifiedIntensity);
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Angle" << "\t\t" << "Src. Intsy" << "\t" << "Spec. Intsy" << "\t" << "Lens Angle" << std::endl;
	for (i = 0; i < SAMPLE_NUMBER / 2; i++) {
		lensAngle[i] *= PI / 2;
		std::cout << angle[i] << "\t\t" << sourceIntensity[i] << "\t\t" << specifiedIntensity[i] << "\t\t" << lensAngle[i] << std::endl;
	}
	outputFile.open("optics_output_1.txt", std::ios::out | std::ios::trunc);

	outputFile.close();
	//system("pause");
	return 0;
}

double gaussian(double value, double mean, double stddev)
{
	double output;
	double temporary;

	if (stddev == 0) {
		std::cout << "ERROR: stddev must not be 0" << std::endl;
		return 0;
	}
	temporary = -(value - mean) * (value - mean) / (2.0 * stddev * stddev);
	output = exp(temporary);
	temporary = 1 / (2.0 * PI * stddev * stddev);
	output = temporary * output;

	return output;
}

double maxwell(double value, double alpha)
{
	double output;
	double temporary;

	if (alpha == 0) {
		std::cout << "ERROR: alpha must not be 0" << std::endl;
		return 0;
	}
	temporary = -(value * value) / (alpha * alpha);
	output = exp(temporary);
	temporary = value * value;
	output = output * temporary;
	temporary = 4 / (pow(alpha, 3) * sqrt(PI));
	output = output * temporary;

	return output;
}

void histogramSpecification(std::vector<double> &result, std::vector<double> &given, std::vector<double> &desired)
{
	int i;
	std::vector<double> sourceProbability;
	std::vector<double> specifiedProbability;
	std::vector<double> sourceTransform;
	std::vector<double> specifiedTransform;
	std::vector<int> transform;

	normalizeProbability(sourceProbability, given);
	normalizeProbability(specifiedProbability, desired);
	cdfTransform(sourceTransform, sourceProbability);
	cdfTransform(specifiedTransform, specifiedProbability);
	minimumDistance(transform, specifiedTransform, sourceTransform);
	if (result.size() > 0) {
		result.clear();
	}
	for (i = 0; i < transform.size(); i++) {
		result.push_back(static_cast<double>(transform[i]) / static_cast<double>(transform.size()));
	}
}

void normalizeProbability(std::vector<double> &destination, std::vector<double> &source)
{
	int i;
	double total;

	total = 0;
	if (destination.size() > 0) {
		destination.clear();
	}
	for (i = 0; i < source.size(); i++) {
		total += source[i];
	}
	for (i = 0; i < source.size(); i++) {
		destination.push_back(source[i] / total);
	}
}

void cdfTransform(std::vector<double> &destination, std::vector<double> &source)
{
	int i;
	double total;

	total = 0;
	if (destination.size() > 0) {
		destination.clear();
	}
	for (i = 0; i < source.size(); i++) {
		total += source[i];
		destination.push_back(source.size() * total);
	}
}

void minimumDistance(std::vector<int> &result, std::vector<double> &primaryTo, std::vector<double> &secondaryFrom)
{
	int i, j;
	int size;
	double distance;

	(primaryTo.size() > secondaryFrom.size()) ? size = secondaryFrom.size() : size = primaryTo.size();
	for (i = 0; i < size; i++) {
		distance = abs(primaryTo[0] - secondaryFrom[0]);
		result.push_back(0);
		for (j = 0; j < size; j++) {
			if (distance > abs(primaryTo[j] - secondaryFrom[i])) {
				result[i] = j;
				distance = abs(primaryTo[j] - secondaryFrom[i]);
			}
		}
	}
}

void generateIntermediateAngle(std::vector<double> &sourceAngle, std::vector<double> &sourceIntensity, std::vector<double> &lensAngle, std::vector<double> &specifiedIntensity)
{
	int i, j;
	double deltaAngle;

	j = SAMPLE_NUMBER / 2 - 1;
	deltaAngle = PI / SAMPLE_NUMBER;
	for (i = 0; i < SAMPLE_NUMBER / 2 - 1; i++) {
		lensAngle[j - i] = (specifiedIntensity[j - (i + 1)] - sourceIntensity[j - (i + 1)]);
		lensAngle[j - i] = lensAngle[j - i] / sourceIntensity[j - i];
		lensAngle[j - i] = lensAngle[j - i] * deltaAngle;
	}
}



