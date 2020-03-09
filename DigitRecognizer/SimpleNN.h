#pragma once
#include "DigitRecognizer.h"
#include "Layer.h"


class SimpleNN : public IDigitRecognizer
{
	Matrix2d batch_size;
	Matrix2d regularization_scale;
	std::vector<Layer> layers;
	Dataset& dataset;
	uint32_t X; // input images
	uint32_t L; // input labels
	uint32_t W; // weights
	uint32_t S; // softmax
	uint32_t F; // function
	float_t grad_step;
	void ForwardProp();
	void BackProp();
	uint32_t AddLayer(Matrix2d& input);
	uint32_t AddLayer(uint32_t rows, uint32_t cols);
	uint32_t AddLayer(uint32_t x, const Layer::Pair_X& func, uint32_t rows = 0, uint32_t cols = 0); // L = f(x)
	uint32_t AddLayer(uint32_t x, uint32_t y, const Layer::Pair_XY& func, uint32_t rows = 0, uint32_t cols = 0); // L = f(x,y)
	// Inherited via IDigitRecognizer
	virtual void TrainNN() override;
	virtual float_t TestNN() override;
	virtual float_t* GetWeights() override;

public:
	SimpleNN(Dataset& _dataset);
};