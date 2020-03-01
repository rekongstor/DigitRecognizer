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
	void FMMul(Layer* l, Layer* r);
	void dFMMul(Layer* l, Layer* r);
	void FMAdd(Layer* l, Layer* r);
	void dFMAdd(Layer* l, Layer* r);
	void FSMul(Layer* l, Layer* r); 
	void dFSMul(Layer* l, Layer* r);
	void FSDiv(Layer* l, Layer* r);
	void dFSDiv(Layer* l, Layer* r);
	void FSMMul(Layer* l, Layer* r);
	void dFSMMul(Layer* l, Layer* r); 
	void FRMul(Layer* l, Layer* r);
	void dFRMul(Layer* l, Layer* r); 
	void FRDiv(Layer* l, Layer* r); 
	void dFRDiv(Layer* l, Layer* r);
	void FExp(Layer* l);
	void dFExp(Layer* l);
	void FLog(Layer* l); 
	void dFLog(Layer* l);
	void FPow2(Layer* l);
	void dFPow2(Layer* l);
	void FNeg(Layer* l);
	void dFNeg(Layer* l);
	void FSumCol(Layer* l);
	void dFSumCol(Layer* l); 
	void FSumRow(Layer* l); 
	void dFSumRow(Layer* l);
	void FSum(Layer* l);
	void dFSum(Layer* l);
public:
	void F();
	
	Layer();
	Layer(Layer& l);
	Layer& operator=(const Layer& l);
	Layer(Matrix2d& input); //input data layer
	Layer(uint32_t rows, uint32_t cols); // input model layer
	Layer(Layer* x, const Pair_X& func, uint32_t rows = 0, uint32_t cols = 0); // L = f(x)
	Layer(Layer* x, Layer* y, const Pair_XY& func, uint32_t rows = 0, uint32_t cols = 0); // L = f(x,y)

	static const Pair_XY MMul; // matrix multiply by matrix
	static const Pair_XY MAdd; // matrix multiply by matrix
	static const Pair_XY SMul; // matrix multiply by scalar
	static const Pair_XY SDiv; // matrix multiply by scalar
	static const Pair_XY SMMul; // matrix multiply by matrix values
	static const Pair_XY RMul; // matrix multiply by row
	static const Pair_XY RDiv; // matrix divide by row
	static const Pair_X Exp; // matrix values to exp
	static const Pair_X Log; // matrix values to log
	static const Pair_X Pow2; // matrix values to log
	static const Pair_X Neg; // matrix values to log
	static const Pair_X SumCol; // sum matrix by columns
	static const Pair_X SumRow; // sum matrix by columns
	static const Pair_X Sum; // sum matrix values
};