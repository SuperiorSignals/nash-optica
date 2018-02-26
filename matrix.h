#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <stdlib.h>

#define SAMPLE_NUMBER 128
#define PI 3.14159265359

int matrix();
double gaussian(double value, double mean, double stddev);
double maxwell(double value, double alpha);
void histogramSpecification(std::vector<double> &result, std::vector<double> &given, std::vector<double> &desired);
void normalizeProbability(std::vector<double> &destination, std::vector<double> &source);
void cdfTransform(std::vector<double> &destination, std::vector<double> &source);
void minimumDistance(std::vector<int> &result, std::vector<double> &primaryTo, std::vector<double> &secondaryFrom);
void generateIntermediateAngle(std::vector<double> &sourceAngle, std::vector<double> &sourceIntensity, std::vector<double> &lensAngle, std::vector<double> &specifiedIntensity);