#pragma once
#include "Dataset.h"

class IDigitRecognizer
{
public:
	virtual void InitNN() = 0;
	virtual void TrainNN() = 0;
	virtual void TestNN() = 0;
};