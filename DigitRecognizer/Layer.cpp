#include "Layer.h"

void Layer::F()
{
	if (func_1 != nullptr)
		(this->*func_1)(op_1);
	
	if (func_2 != nullptr)
		(this->*func_2)(op_1, op_2);
}

Layer::Layer() : L(L_self), dL(dL_self), func_1(nullptr), func_2(nullptr)
{
}

Layer::Layer(Layer& l): L(l.L), dL(l.dL), prop(l.prop), func_1(l.func_1), func_2(l.func_2), op_1(l.op_1), op_2(l.op_2)
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
	prop = l.prop;
	func_1 = l.func_1;
	func_2 = l.func_2;
	op_1 = l.op_1;
	op_2 = l.op_2;
	return *this;
}

Layer::Layer(Dataset& input): L(input.train_images[0]), dL(dL_self), func_1(nullptr), func_2(nullptr)
{
}

Layer::Layer(uint32_t rows, uint32_t cols): L(L_self), dL(dL_self), func_1(nullptr), func_2(nullptr)
{
	L_self.Init(rows, cols);
	dL_self.Init(rows, cols);
	// TODO: randomize
}

Layer::Layer(Layer* x, Layer* y, void (Layer::*func)(Layer*,Layer*), uint32_t rows, uint32_t cols): op_1(x), op_2(y), func_2(func), L(L_self), dL(dL_self), func_1(nullptr)
{
	L_self.Init(rows, cols);
	dL_self.Init(rows, cols);
}

Layer::Layer(Layer* x, void (Layer::*func)(Layer*)): op_1(x), func_1(func), L(L_self), dL(dL_self), func_2(nullptr)
{
	L_self.Init(x->L.a(), x->L.b()); // размерность сохраняется
	dL_self.Init(x->L.a(), x->L.b());
}

void Layer::MMul(Layer* l, Layer* r)
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

void Layer::SMul(Layer* l, Layer* r)
{
}

void Layer::Exp(Layer& l)
{
}

void Layer::Log(Layer& l)
{
}
