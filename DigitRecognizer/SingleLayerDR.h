#pragma once
#include "DigitRecognizer.h"
#include "Matrix2d.h"


class SingleLayerDR : public IDigitRecognizer
{
	const Dataset& dataset;
	uint32_t gens;
	Matrix2d w1; // weights matrix [10x1024]
	Matrix2d w1_grad; // [10x1024]
	Matrix2d b1; // biases matrix [10x1]
	Matrix2d b1_grad; // [10x1]
	float_t grad_step;
	uint32_t offset;

	Matrix2d Calculate(const Matrix2d& input);
	void NumGrad(Matrix2d& vals, Matrix2d& grad);
	float_t Loss();
	uint32_t True();

	// Inherited via IDigitRecognizer
	virtual void InitNN() override;
	virtual void TrainNN() override;
	virtual void TestNN() override;

public:
	SingleLayerDR(const Dataset& _dataset);
};