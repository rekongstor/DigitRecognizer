#pragma once
#include "DigitRecognizer.h"
#include "Layer.h"


class SimpleNN : public IDigitRecognizer
{
	std::vector<Layer> layers;
	Dataset& dataset;
	uint32_t AddLayer(Matrix2d& input);
	uint32_t AddLayer(uint32_t rows, uint32_t cols);
	uint32_t AddLayer(uint32_t x, const Layer::Pair_X& func, uint32_t rows = 0, uint32_t cols = 0); // L = f(x)
	uint32_t AddLayer(uint32_t x, uint32_t y, const Layer::Pair_XY& func, uint32_t rows = 0, uint32_t cols = 0); // L = f(x,y)

	// Inherited via IDigitRecognizer
	virtual void TrainNN() override;
	virtual void TestNN() override;

public:
	SimpleNN(Dataset& _dataset);
};