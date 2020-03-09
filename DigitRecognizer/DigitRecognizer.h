#pragma once
#include "Dataset.h"

class IDigitRecognizer
{
public:
	virtual void TrainNN() = 0;
	virtual float_t TestNN() = 0;
	virtual float_t* GetWeights() = 0;
};