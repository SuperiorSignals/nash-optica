#include "matrix.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include "customlens.h"

#define PI 3.14159265359

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

void default()
{
	std::vector<double> lensAngle;
	std::vector<double> angle;
	std::vector<double> sourceIntensity;
	std::vector<double> specifiedIntensity;
	Specification specification;
	CustomLens output;
	double temporary;
	
	for (int i = 0; i < SAMPLE_NUMBER / 2; i++) {
		temporary = PI * static_cast<double>(i) / SAMPLE_NUMBER;
		angle.push_back(temporary);
		sourceIntensity.push_back(gaussian(temporary, 0, 0.5));
		specifiedIntensity.push_back(gaussian(temporary, 0, 0.7));
	}
	specification.setAngles(angle);
	specification.setGivenIntensity(sourceIntensity);
	specification.setDesiredIntensity(specifiedIntensity);
	lensAngle = histogramSpecification(specification);
	angle = specification.getAngles();
	saveSpecification(specification);
	for (int i = 0; i < lensAngle.size(); i++) {
		lensAngle[i] *= PI / 2;
		lensAngle[i] = angle[i] - lensAngle[i];
	}
	output.setAngles(angle);
	output.setLensAngles(lensAngle);
	saveProfile(output);
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

std::vector<double> histogramSpecification(Specification input)
{
	int i;
	std::vector<double> sourceProbability;
	std::vector<double> specifiedProbability;
	std::vector<double> sourceTransform;
	std::vector<double> specifiedTransform;
	std::vector<int> transform;
	std::vector<double> given;
	std::vector<double> desired;
	std::vector<double> result;


	given = input.getGivenIntensity();
	desired = input.getDesiredIntensity();
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

	return result;
}

CustomLens loadProfile(const char *fileName)
{
	CustomLens output;
	std::fstream inputFile;
	std::string temporary;
	double temp;
	std::string::size_type sz;
	std::vector<double> lensAngle;

	inputFile.open(fileName, std::ios::in);
	if (!inputFile.is_open()) {
		std::cout << "ERROR [CustomLens loadProfile(const char *fileName)]: ";
		std::cout << "Unable to open file " << fileName;
		std::cout << std::endl;
	} else {
		while (std::getline(inputFile, temporary)) {
			temp = std::stod(temporary, &sz);
			lensAngle.push_back(temp);
		}
	}
	output.setLensAngles(lensAngle);

	return output;
}

Specification loadSpecification(const char *fileName)
{
	Specification output;
	std::fstream inputFile;
	std::string temporary;
	double temp;
	std::string::size_type sz;
	std::vector<double> angles;
	std::vector<double> givenIntensity;
	std::vector<double> desiredIntensity;
	int i;
	
	inputFile.open(fileName, std::ios::in);
	if (!inputFile.is_open()) {
		std::cout << "ERROR [Specification loadSpecification(const char *fileName)]: ";
		std::cout << "Unable to open file " << fileName;
		std::cout << std::endl;
	} else {
		i = 1;
		while (std::getline(inputFile, temporary)) {
			temp = std::stod(temporary, &sz);
			if (i % 3 == 0) {
				desiredIntensity.push_back(temp);
			} else if (i % 2 == 0) {
				givenIntensity.push_back(temp);
			} else {
				angles.push_back(temp);
			}
			i++;
		}
	}

	return output;
}

CustomLens matrix(Distribution type, double firstParameter, double secondParameter)
{
	int i;
	int menuSelection;
	double temporary;
	double indexOfRefraction;
	double lensThickness;
	std::vector<double> angle;
	std::vector<double> sourceIntensity;
	std::vector<double> lensAngle;
	std::vector<double> specifiedIntensity;
	std::ofstream outputFile;
	std::ifstream inputFile;
	CustomLens output;
	/*
	inputFile.open("input.lns");

	if (!inputFile.is_open()) {
	std::cout << "ERROR: Unable to open file input.lns" << std::endl;
	return 1;
	}
	*/
	switch (type) {
	case GAUSSIAN:
		for (i = 0; i < SAMPLE_NUMBER / 2; i++) {
			temporary = PI * static_cast<double>(i) / (SAMPLE_NUMBER);
			angle.push_back(temporary);
			sourceIntensity.push_back(gaussian(temporary, 0, firstParameter));
			specifiedIntensity.push_back(gaussian(temporary, 0, secondParameter));
			//specifiedIntensity.push_back(maxwell(temporary, 0.5));
		}
		break;
	case MAXWELL:
		for (i = 0; i < SAMPLE_NUMBER / 2; i++) {
			temporary = PI * static_cast<double>(i) / (SAMPLE_NUMBER);
			angle.push_back(temporary);
			sourceIntensity.push_back(gaussian(temporary, 0, firstParameter));
			specifiedIntensity.push_back(maxwell(temporary, secondParameter));
		}
		break;
	case CUSTOM:
	default:
		for (i = 0; i < SAMPLE_NUMBER / 2; i++) {
			temporary = PI * static_cast<double>(i) / (SAMPLE_NUMBER);
			angle.push_back(temporary);
			sourceIntensity.push_back(gaussian(temporary, 0, 0.5));
			specifiedIntensity.push_back(gaussian(temporary, 0, 0.7));
		}
		break;
	}

	//generateIntermediateAngle(angle, sourceIntensity, lensAngle, specifiedIntensity);
	histogramSpecification(lensAngle, sourceIntensity, specifiedIntensity);
	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Angle" << "\t\t" << "Src. Intsy" << "\t" << "Spec. Intsy" << "\t" << "Lens Angle" << std::endl;
	for (i = 0; i < SAMPLE_NUMBER / 2; i++) {
		lensAngle[i] *= PI / 2;
		if (!(i % DISPLAY_NUMBER)) {
			std::cout << angle[i] << "\t\t" << sourceIntensity[i] << "\t\t" << specifiedIntensity[i] << "\t\t" << lensAngle[i] << std::endl;
		}
	}
	outputFile.open("optics_output_1.txt", std::ios::out | std::ios::trunc);
	if (outputFile.is_open()) {
		outputFile << std::fixed << std::setprecision(5);
		for (i = 0; i < angle.size(); i++) {
			outputFile << angle[i] << "\t\t" << sourceIntensity[i] << "\t\t" << specifiedIntensity[i] << "\t\t" << lensAngle[i] << std::endl;
		}
	} else {
		std::cout << "ERROR: Unable to open file optics_output_1.txt" << std::endl;
	}
	outputFile.close();
	for (i = 0; i < angle.size(); i++) {
		lensAngle[i] = angle[i] - lensAngle[i];
	}
	std::cout << "Angle" << "\t\t" << "Src. Intsy" << "\t" << "Spec. Intsy" << "\t" << "Lens Angle" << std::endl;
	for (i = 0; i < SAMPLE_NUMBER / 2; i++) {
		lensAngle[i] *= PI / 2;
		if (!(i % SAMPLE_NUMBER)) {
			std::cout << angle[i] << "\t\t" << sourceIntensity[i] << "\t\t" << specifiedIntensity[i] << "\t\t" << lensAngle[i] << std::endl;
		}
	}
	output.setAngles(angle);
	output.setLensAngles(lensAngle);

	return output;
}

int matrixMenu(int key) {
	int menuSelection;

	menuSelection = 1;
	do {
		switch (menuSelection) {
		case 0:
			key = 0;
			break;
		case 1:
			std::cout << "Menu" << std::endl;
			std::cout << "----------" << std::endl;
			std::cout << "0. Exit" << std::endl;
			std::cout << "1. Display Menu" << std::endl;
			std::cout << "2. Display Lens Matrix" << std::endl;
			std::cout << "3. Choose Source" << std::endl;
			std::cout << "4. Choose Beam" << std::endl;
			std::cout << "@Nash-Optica> ";
			break;
		case 2:
			matrix(CUSTOM, 0.5, 0.75);
			menuSelection = 1;
			break;
		case 3:
			std::cout << "feature not yet available" << std::endl;
			menuSelection = 1;
			break;
		case 4:
			std::cout << "feature not yet available" << std::endl;
			menuSelection = 1;
			break;
		case 64:
			std::cout << "Selection" << std::endl;
			std::cout << "------------" << std::endl;
			std::cout << "0. Exit" << std::endl;
			std::cout << "1. Gaussian" << std::endl;
			std::cout << "2. Maxwell Distribution" << std::endl;
			std::cout << "@Nash-Optica> ";
			break;
		case 65:
			std::cout << "Value: ";
			break;
		case 66:
			std::cout << "Mean: ";
			break;
		case 67:
			std::cout << "Standard Deviation: ";
			break;
		default:
			break;

		}
		std::cin >> menuSelection;
	} while (key == 3);

	return key;
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

void saveProfile(CustomLens input)
{
	std::fstream outputFile;
	std::vector<double> angles;

	angles = input.getLensAngles();
	outputFile.open("lens_profile.txt", std::ios::out | std::ios::trunc);
	if (outputFile.is_open()) {
		for (int i = 0; i < angles.size(); i++) {
			outputFile << angles[i] << std::endl;
		}
	} else {
		std::cout << "Error [void saveProfile(CustomLens)]: Unable to open file ";
		std::cout << "lens_profile.txt" << std::endl;
	}

	outputFile.close();
}

void saveSpecification(Specification input)
{
	std::fstream outputFile;
	std::vector<double> angles;
	std::vector<double> givenIntensity;
	std::vector<double> desiredIntensity;

	angles = input.getAngles();
	givenIntensity = input.getGivenIntensity();
	desiredIntensity = input.getDesiredIntensity();
	outputFile.open("intensity_profile.txt", std::ios::out | std::ios::trunc);
	if (outputFile.is_open()) {
		for (int i = 0; i < angles.size(); i++) {
			outputFile << angles[i] << std::endl;
			outputFile << givenIntensity[i] << std::endl;
			outputFile << desiredIntensity[i] << std::endl;
		}
	} else {
		std::cout << "Error [void saveSpecification(Specification)]: Unable to open file ";
		std::cout << "intensity_profile.txt" << std::endl;
	}

	outputFile.close();
}

