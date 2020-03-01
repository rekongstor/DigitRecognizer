#include "Layer.h"

void Layer::F()
{
	if (func_1 != nullptr)
		(this->*func_1)(op_1);

	if (func_2 != nullptr)
		(this->*func_2)(op_1, op_2);
}

Layer::Layer() : 
	L(L_self), 
	dL(dL_self), 
	func_1(nullptr), 
	dfunc_1(nullptr),
	func_2(nullptr), 
	dfunc_2(nullptr),
	op_1(nullptr), 
	op_2(nullptr)
{
}

Layer::Layer(Layer& l): 
	L(l.L), 
	dL(l.dL), 
	func_1(l.func_1), 
	func_2(l.func_2), 
	op_1(l.op_1), 
	op_2(l.op_2)
{
	L_self = std::move(l.L_self);
	dL_self = std::move(l.dL_self);
}

Layer& Layer::operator=(const Layer& l)
{
	L_self = std::move(l.L_self);
	dL_self = std::move(l.dL_self);
	L = l.L;
	dL = l.dL;
	func_1 = l.func_1;
	func_2 = l.func_2;
	op_1 = l.op_1;
	op_2 = l.op_2;
	return *this;
}

Layer::Layer(Matrix2d& input):
	L(input), 
	dL(dL_self),
	func_1(nullptr),
	dfunc_1(nullptr),
	func_2(nullptr),
	dfunc_2(nullptr),
	op_1(nullptr), 
	op_2(nullptr)
{
}

Layer::Layer(uint32_t rows, uint32_t cols): 
	L(L_self), 
	dL(dL_self), 
	func_1(nullptr), 
	dfunc_1(nullptr),
	func_2(nullptr), 
	dfunc_2(nullptr),
	op_1(nullptr), 
	op_2(nullptr)
{
	L_self.Init(rows, cols);
	dL_self.Init(rows, cols);
	// TODO: randomize
}

Layer::Layer(Layer* x, Layer* y, const Pair_XY& func, uint32_t rows, uint32_t cols):
	L(L_self),
	dL(dL_self),
	func_1(nullptr),
	dfunc_1(nullptr),
	func_2(func.first), 
	dfunc_2(func.second),
	op_1(x), 
	op_2(y)
{
	if (rows == 0)
		rows = x->L.a();
	if (cols == 0)
		cols = y->L.b();

	L_self.Init(rows, cols);
	dL_self.Init(rows, cols);
}

Layer::Layer(Layer* x, const Pair_X& func, uint32_t rows, uint32_t cols):
	L(L_self), 
	dL(dL_self), 
	func_1(func.first), 
	dfunc_1(func.second), 
	func_2(nullptr), 
	dfunc_2(nullptr),
	op_1(x), 
	op_2(nullptr)
{
	if (rows == 0)
		rows = x->L.a();
	if (cols == 0)
		cols = x->L.b();

	L_self.Init(rows, cols); // размерность сохраняется
	dL_self.Init(rows, cols);
}

void Layer::FMMul(Layer* l, Layer* r)
{
	// [a x b] * [b x c] = [a x c]
	auto& x = l->L;
	auto& y = r->L;
	if (x.b() != y.a())
		throw InvalidMatrixMul();

	for (uint32_t i = 0; i < x.a(); ++i)
	{
		const float* l1 = &(x.mx[i * x.b()]);
		for (uint32_t j = 0; j < y.b(); ++j)
		{
			L.mx[j + i * y.b()] = 0.f;
			for (uint32_t v = 0; v < x.a(); ++v)
			{
				L.mx[j + i * y.b()] += l1[v] * y.mx[j + y.b() * v];
			}
		}
	}
}

void Layer::dFMMul(Layer* l, Layer* r)
{
}

void Layer::FMAdd(Layer* l, Layer* r)
{
	auto& x = l->L;
	auto& y = r->L;
	if ((x.a() != y.a()) || (L.a() != x.a()) || (x.b() != y.b()) || (L.b() != x.b()))
		throw InvalidMatrixAdd();

	for (uint32_t i = 0; i < L.mx.size(); ++i)
		L.mx[i] = x.mx[i] + y.mx[0];
}

void Layer::dFMAdd(Layer* l, Layer* r)
{
}

void Layer::FSMul(Layer* l, Layer* r)
{
	auto& x = l->L;
	auto& y = r->L;

	if (y.mx.size() != 1)
		throw InvalidMatrixScalMul();

	for (uint32_t i = 0; i < L.mx.size(); ++i)
		L.mx[i] = x.mx[i] * y.mx[0];
}

void Layer::dFSMul(Layer* l, Layer* r)
{
}

void Layer::FSDiv(Layer* l, Layer* r)
{
	auto& x = l->L;
	auto& y = r->L;

	if (y.mx.size() != 1)
		throw InvalidMatrixScalMul();

	for (uint32_t i = 0; i < L.mx.size(); ++i)
		L.mx[i] = x.mx[i] / y.mx[0];
}

void Layer::dFSDiv(Layer* l, Layer* r)
{
}

void Layer::FSMMul(Layer* l, Layer* r)
{
	auto& x = l->L;
	auto& y = r->L;
	if ((x.a() != y.a()) || (L.a() != x.a()) || (x.b() != y.b()) || (L.b() != x.b()))
		throw InvalidMatrixSMul();

	for (uint32_t i = 0; i < L.a(); ++i)
	{
		for (uint32_t j = 0; j < x.b(); ++j)
			L(i, j) = x(i, j) * y(i, j);
	}
}

void Layer::dFSMMul(Layer* l, Layer* r)
{
}

void Layer::FRMul(Layer* l, Layer* r)
{
	auto& x = l->L;
	auto& y = r->L;

	if ((x.a() != y.a()) || (y.b() != 1) || (L.a() != x.a()) || (L.b() != 1))
		throw InvalidMatrixRowMul();

	for (uint32_t i = 0; i < L.a(); ++i)
	{
		L(i, 0) = 0.f;
		for (uint32_t j = 0; j < x.b(); ++j)
			L(i, 0) += x(i, j) * y(i, 0);
	}
}

void Layer::dFRMul(Layer* l, Layer* r)
{
}

void Layer::FRDiv(Layer* l, Layer* r)
{
	auto& x = l->L;
	auto& y = r->L;

	if ((x.a() != y.a()) || (y.b() != 1) || (L.a() != x.a()))
		throw InvalidMatrixRowMul();

	for (uint32_t i = 0; i < L.a(); ++i)
	{
		for (uint32_t j = 0; j < x.b(); ++j)
			L(i, j) = x(i, j) / y(i, 0);
	}
}

void Layer::dFRDiv(Layer* l, Layer* r)
{
}

void Layer::FExp(Layer* l)
{
	auto& x = l->L;
	for (uint32_t i = 0; i < L.mx.size(); ++i)
		L.mx[i] = exp(x.mx[i]);
}

void Layer::dFExp(Layer* l)
{
}

void Layer::FLog(Layer* l)
{
	auto& x = l->L;
	for (uint32_t i = 0; i < L.mx.size(); ++i)
		L.mx[i] = log(x.mx[i]);
}

void Layer::dFLog(Layer* l)
{
}

void Layer::FPow2(Layer* l)
{
	auto& x = l->L;
	for (uint32_t i = 0; i < L.mx.size(); ++i)
		L.mx[i] = x.mx[i] * x.mx[i];
}

void Layer::dFPow2(Layer* l)
{
}

void Layer::FNeg(Layer* l)
{
	auto& x = l->L;
	for (uint32_t i = 0; i < L.mx.size(); ++i)
		L.mx[i] = -x.mx[i];
}

void Layer::dFNeg(Layer* l)
{
}

void Layer::FSumCol(Layer* l)
{
	auto& x = l->L;
	if (L.b() != 1)
		throw InvalidMatrixSumCol();

	for (uint32_t i = 0; i < L.a(); ++i)
	{
		L(i, 0) = 0.f;
		for (uint32_t j = 0; j < x.b(); ++j)
			L(i, 0) += x(i, j);
	}
}

void Layer::dFSumCol(Layer* l)
{
}

void Layer::FSumRow(Layer* l)
{
	auto& x = l->L;
	if (L.a() != 1)
		throw InvalidMatrixSumRow();

	for (uint32_t j = 0; j < L.b(); ++j)
	{
		L(0, j) = 0.f;
		for (uint32_t i = 0; i < x.a(); ++i)
			L(0, j) += x(i, j);
	}
}

void Layer::dFSumRow(Layer* l)
{
}

void Layer::FSum(Layer* l)
{
	auto& x = l->L;
	if ((L.a() != 1) || (L.b() != 1))
		throw InvalidMatrixSum();

	L(0, 0) = 0.f;
	for (uint32_t i = 0; i < x.a(); ++i)
		for (uint32_t j = 0; j < x.b(); ++j)
			L(0, 0) += x(i, j);
}

void Layer::dFSum(Layer* l)
{
}

const Layer::Pair_XY Layer::MMul = Layer::Pair_XY(&Layer::FMMul, &Layer::dFMMul);
const Layer::Pair_XY Layer::MAdd = Layer::Pair_XY(&Layer::FMAdd, &Layer::dFMAdd);
const Layer::Pair_XY Layer::SMul = Layer::Pair_XY(&Layer::FSMul, &Layer::dFSMul);
const Layer::Pair_XY Layer::SDiv = Layer::Pair_XY(&Layer::FSDiv, &Layer::dFSDiv);
const Layer::Pair_XY Layer::SMMul = Layer::Pair_XY(&Layer::FSMMul, &Layer::dFSMMul);
const Layer::Pair_XY Layer::RMul = Layer::Pair_XY(&Layer::FRMul, &Layer::dFRMul);
const Layer::Pair_XY Layer::RDiv = Layer::Pair_XY(&Layer::FRDiv, &Layer::dFRDiv);
const Layer::Pair_X Layer::Exp = Layer::Pair_X(&Layer::FExp, &Layer::dFExp);
const Layer::Pair_X Layer::Log = Layer::Pair_X(&Layer::FLog, &Layer::dFLog);
const Layer::Pair_X Layer::Pow2 = Layer::Pair_X(&Layer::FPow2, &Layer::dFPow2);
const Layer::Pair_X Layer::Neg = Layer::Pair_X(&Layer::FNeg, &Layer::dFNeg);
const Layer::Pair_X Layer::SumCol = Layer::Pair_X(&Layer::FSumCol, &Layer::dFSumCol);
const Layer::Pair_X Layer::SumRow = Layer::Pair_X(&Layer::FSumRow, &Layer::dFSumRow);
const Layer::Pair_X Layer::Sum = Layer::Pair_X(&Layer::FSum, &Layer::dFSum);