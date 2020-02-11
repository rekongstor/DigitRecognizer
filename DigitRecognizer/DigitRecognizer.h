#pragma once
#include "Dataset.h"

class IDigitRecognizer
{
public:
	virtual void InitNN() = 0;
	virtual void TrainNN(const Dataset& dataset) = 0;
	virtual void TestNN(const Dataset& dataset) = 0;
};