#pragma once
#include <vector>
#include "customlens.h"
#include "specification.h"

#define SAMPLE_NUMBER 1024
#define DISPLAY_NUMBER 32

enum Distribution {GAUSSIAN, MAXWELL, CUSTOM};

void cdfTransform(std::vector<double> &destination, std::vector<double> &source);
void default();
double gaussian(double value, double mean, double stddev);
void generateIntermediateAngle(std::vector<double> &sourceAngle, std::vector<double> &sourceIntensity, std::vector<double> &lensAngle, std::vector<double> &specifiedIntensity);
void histogramSpecification(std::vector<double> &result, std::vector<double> &given, std::vector<double> &desired);
std::vector<double> histogramSpecification(Specification input);
CustomLens loadProfile(const char *fileName);
Specification loadSpecification(const char *fileName);
CustomLens matrix(Distribution type, double firstParameter, double secondParameter);
int matrixMenu(int key);
double maxwell(double value, double alpha);
void minimumDistance(std::vector<int> &result, std::vector<double> &primaryTo, std::vector<double> &secondaryFrom);
void normalizeProbability(std::vector<double> &destination, std::vector<double> &source);
void saveProfile(CustomLens input);
void saveSpecification(Specification input);
