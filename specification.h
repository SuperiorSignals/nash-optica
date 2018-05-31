#pragma once
#ifndef SPECIFICATION_H_
#define SPECIFICATION_H_
#include <vector>

class Specification {
public:
	std::vector<double> getAngles();
	std::vector<double> getGivenIntensity();
	std::vector<double> getDesiredIntensity();
	void setAngles(std::vector<double> input);
	void setGivenIntensity(std::vector<double> input);
	void setDesiredIntensity(std::vector<double> input);

private:
	std::vector<double> angles;
	std::vector<double> givenIntensity;
	std::vector<double> desiredIntensity;
};

#endif // !SPECIFICATION_H_
