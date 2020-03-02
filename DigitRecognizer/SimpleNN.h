#pragma once
#include "DigitRecognizer.h"
#include "Layer.h"


class SimpleNN : public IDigitRecognizer
{
	std::vector<Layer> layers;
	Dataset& dataset;

	// Inherited via IDigitRecognizer
	virtual void InitNN() override;
	virtual void TrainNN() override;
	virtual void TestNN() override;

public:
	SimpleNN(Dataset& _dataset);
};