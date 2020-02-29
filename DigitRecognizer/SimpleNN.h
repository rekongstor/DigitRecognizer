#pragma once
#include "DigitRecognizer.h"
#include "Layer.h"


class SimpleNN : public IDigitRecognizer
{
	Dataset& dataset;
	std::array<Layer,100> layers;

	// Inherited via IDigitRecognizer
	virtual void InitNN() override;
	virtual void TrainNN() override;
	virtual void TestNN() override;

public:
	SimpleNN(Dataset& _dataset);
};