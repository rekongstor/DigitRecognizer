#pragma once
#include "Dataset.h"

class Layer
{
	Matrix2d& L;
	Matrix2d& dL;
	bool prop; // 0 = forward propagation, 1 = backward propagation
	void (Layer::* func_1)(Layer*);
	void (Layer::* func_2)(Layer*, Layer*);
	Layer* op_1;
	Layer* op_2;
	Matrix2d L_self; // for non-input layers
	Matrix2d dL_self;
public:
	void F();

	Layer();
	Layer(Layer& l);
	Layer& operator=(const Layer& l);
	Layer(Dataset& input); //input data layer
	Layer(uint32_t rows, uint32_t cols); // input model layer
	Layer(Layer* x, Layer* y, void (Layer::*func)(Layer*,Layer*), uint32_t rows, uint32_t cols); // L = f(x,y)
	Layer(Layer* x, void (Layer::*func)(Layer*)); // L = f(x)

	void MMul(Layer* l, Layer* r); // matrix be matrix multiplication
	void SMul(Layer* l, Layer* r); // matrix by scalar (1x1 matrix)
	void Exp(Layer& l); // exponential function for each value
	void Log(Layer& l); // logarithmic function for each value
};