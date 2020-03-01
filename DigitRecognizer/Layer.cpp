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

Layer::Layer(Dataset& input): 
	L(input.train_images[0]), 
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

Layer::Layer(Layer* x, Layer* y, std::pair<Func_XY, Func_XY>& func, uint32_t rows, uint32_t cols):
	L(L_self),
	dL(dL_self),
	func_1(nullptr),
	dfunc_1(nullptr),
	func_2(func.first), 
	dfunc_2(func.second),
	op_1(x), 
	op_2(y)
{
	L_self.Init(rows, cols);
	dL_self.Init(rows, cols);
}

Layer::Layer(Layer* x, std::pair<Func_X, Func_X>& func):
	L(L_self), 
	dL(dL_self), 
	func_1(func.first), 
	dfunc_1(func.second), 
	func_2(nullptr), 
	dfunc_2(nullptr),
	op_1(x), 
	op_2(nullptr)
{
	L_self.Init(x->L.a(), x->L.b()); // размерность сохраняется
	dL_self.Init(x->L.a(), x->L.b());
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

void Layer::FSMul(Layer* l, Layer* r)
{
}

void Layer::dFSMul(Layer* l, Layer* r)
{
}

void Layer::FExp(Layer* l)
{
}

void Layer::dFExp(Layer* l)
{
}

void Layer::FLog(Layer* l)
{
}

void Layer::dFLog(Layer* l)
{
}

std::pair<Layer::Func_XY, Layer::Func_XY> Layer::MMul = std::make_pair<>(&Layer::FMMul, &Layer::dFMMul);
std::pair<Layer::Func_XY, Layer::Func_XY> Layer::SMul = std::make_pair<>(&Layer::FSMul, &Layer::dFSMul);
std::pair<Layer::Func_X, Layer::Func_X> Layer::Exp = std::make_pair<>(&Layer::FExp, &Layer::dFExp);
std::pair<Layer::Func_X, Layer::Func_X> Layer::Log = std::make_pair<>(&Layer::FLog, &Layer::dFLog);