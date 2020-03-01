#pragma once
#include "Dataset.h"

class Layer
{
	using Func_XY = void(Layer::*)(Layer*, Layer*);
	using Func_X = void(Layer::*)(Layer*);

	using Pair_XY = std::pair<Func_XY, Func_XY>;
	using Pair_X = std::pair<Func_X, Func_X>;

	Matrix2d& L;
	Matrix2d& dL;
	void (Layer::* func_1)(Layer*);
	void (Layer::* dfunc_1)(Layer*);
	void (Layer::* func_2)(Layer*, Layer*);
	void (Layer::* dfunc_2)(Layer*, Layer*);
	Layer* op_1;
	Layer* op_2;
	Matrix2d L_self; // for non-input layers
	Matrix2d dL_self;
	void FMMul(Layer* l, Layer* r); // matrix be matrix multiplication
	void dFMMul(Layer* l, Layer* r); // matrix be matrix multiplication grad
	void FSMul(Layer* l, Layer* r); // matrix by scalar (1x1 matrix)
	void dFSMul(Layer* l, Layer* r); // matrix by scalar (1x1 matrix) grad
	void FExp(Layer* l); // exponential function for each value
	void dFExp(Layer* l); // exponential function for each value grad
	void FLog(Layer* l); // logarithmic function for each value
	void dFLog(Layer* l); // logarithmic function for each value grad
public:
	void F();
	
	Layer();
	Layer(Layer& l);
	Layer& operator=(const Layer& l);
	Layer(Dataset& input); //input data layer
	Layer(uint32_t rows, uint32_t cols); // input model layer
	Layer(Layer* x, const Pair_X& func); // L = f(x)
	Layer(Layer* x, Layer* y, const Pair_XY& func, uint32_t rows, uint32_t cols); // L = f(x,y)

	static const Pair_XY MMul;
	static const Pair_XY SMul;
	static const Pair_X Exp;
	static const Pair_X Log;
};