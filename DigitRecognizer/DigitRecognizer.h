#pragma once
#include "Dataset.h"

class IDigitRecognizer
{
public:
	virtual void TrainNN() = 0;
	virtual void TestNN() = 0;
};