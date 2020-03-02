#include "Layer.h"

void Layer::F()
{
	Layer* op_1, * op_2;
	if ((it_1 != -1) && (it_2 == -1))
	{
		op_1 = &((*layers)[it_1]);
		(this->*func_1)(op_1);
	}
	if ((it_1 != -1) && (it_2 != -1))
	{
		op_1 = &((*layers)[it_1]);
		op_2 = &((*layers)[it_2]);
		(this->*func_2)(op_1, op_2);
	}
}

void Layer::dF(float& f)
{

	// numeric
	if (dL->mx.size() == 0)
		return;
	float y = f;
	for (uint32_t i = 0; i < (*L).mx.size(); ++i)
	{
		if (depended.size() == 0) // target function
		{
			(*dL).mx[i] = 1.f;
		}
		else
		{
			(*L).mx[i] += DELTA;
			for (auto& d : depended)
				(*layers)[d].FollowProp();
			(*L).mx[i] -= DELTA;
			(*dL).mx[i] = (f - y) / DELTA;
		}
	}
}

void Layer::dF()
{
	if (depended.size() == 0) // конечная функция
		for (auto& v : dL->mx)
			v = 1.f;
	else
	{
		// расчитать свою dF
		// сумма dependencies
		for (uint32_t i = 0; i < dL->a(); ++i)
			for (uint32_t j = 0; j < dL->b(); ++j)
			{
				(*dL)(i, j) = 0.f;
				for (auto&& d : depended)
				{
					if (it_self == (*layers)[d].it_1)
						(*dL)(i, j) += (*(*layers)[d].dL1)(i, j);
					if (it_self == (*layers)[d].it_2)
						(*dL)(i, j) += (*(*layers)[d].dL2)(i, j);
				}
			}
	}

	// расчитать dF1 и dF2 
	if (dfunc_1 != nullptr)
		(this->*func_1)(&(*layers)[it_1]);

	if (dfunc_1 != nullptr)
		(this->*func_2)(&(*layers)[it_1], &(*layers)[it_2]);
}


float& Layer::getVal()
{
	if ((depended.size() != 0) || ((*L).mx.size() != 1))
		throw LayerNotFinalValue();

	return (*L).mx[0];
}

void Layer::FollowProp()
{
	F();
	for (auto& d : depended)
		(*layers)[d].FollowProp();
}



Layer::Layer(const Layer& l):
	layers(l.layers),
	depended(l.depended),
	func_1(l.func_1),
	dfunc_1(l.dfunc_1),
	func_2(l.func_2),
	dfunc_2(l.dfunc_2),
	it_1(l.it_1),
	it_2(l.it_2),
	it_self(l.it_self)
{
	if (l.L_self.mx.size() == 0) // input data
	{
		L = l.L;
		dL = l.dL;
		dL1 = l.dL1;
		dL2 = l.dL2;
	}
	else
	{
		L_self = *l.L;
		dL_self = *l.dL;
		dL_self1 = *l.dL1;
		if (l.dL2)
			dL_self2 = *l.dL2;
		L = &L_self;
		dL = &dL_self;
		dL1 = &dL_self1;
		dL2 = &dL_self2;
	}
}

Layer& Layer::operator=(const Layer& l)
{
	layers = l.layers;
	depended = l.depended;
	func_1 = l.func_1;
	dfunc_1 = l.dfunc_1;
	func_2 = l.func_2;
	dfunc_2 = l.dfunc_2;
	it_1 = l.it_1;
	it_2 = l.it_2;
	it_self = l.it_self;
	if (l.L_self.mx.size() == 0) // input data
	{
		L = l.L;
		dL = l.dL;
		dL1 = l.dL1;
		dL2 = l.dL2;
	}
	else
	{
		L_self = *l.L;
		dL_self = *l.dL;
		dL_self1 = *l.dL1;
		dL_self2 = *l.dL2;
		L = &L_self;
		dL = &dL_self;
		dL1 = &dL_self1;
		dL2 = &dL_self2;
	}
	return *this;
}

Layer::Layer(std::vector<Layer>* l, Matrix2d& input) :
	L(&input),
	dL(&dL_self),
	dL1(&dL_self1),
	dL2(&dL_self2),
	func_1(nullptr),
	dfunc_1(nullptr),
	func_2(nullptr),
	dfunc_2(nullptr),
	it_1(-1),
	it_2(-1),
	layers(l)
{
	it_self = static_cast<uint32_t>(l->size());
}

Layer::Layer(std::vector<Layer>* l, uint32_t rows, uint32_t cols):
	L(&L_self), 
	dL(&dL_self), 
	dL1(&dL_self1), 
	dL2(&dL_self2), 
	func_1(nullptr), 
	dfunc_1(nullptr),
	func_2(nullptr), 
	dfunc_2(nullptr),
	it_1(-1), 
	it_2(-1),
	layers(l)
{
	it_self = static_cast<uint32_t>(l->size());
	L_self.Init(rows, cols);
	dL_self.Init(rows, cols);
	// TODO: randomize
	for (auto& v : L->mx)
		v = 0.2f;
}

Layer::Layer(std::vector<Layer>* l, uint32_t x, uint32_t y, const Pair_XY& func, uint32_t rows, uint32_t cols):
	L(&L_self),
	dL(&dL_self),
	dL1(&dL_self1),
	dL2(&dL_self2),
	func_1(nullptr),
	dfunc_1(nullptr),
	//func_2(func.first), 
	//dfunc_2(func.second),
	it_1(x),
	it_2(y),
	layers(l)
{
	it_self = static_cast<uint32_t>(l->size());
	auto [f2, df2] = func;
	func_2 = f2;
	dfunc_2 = df2;
	if (rows == 0)
		rows = (*layers)[x].L->a();
	if (cols == 0)
		cols = (*layers)[y].L->b();

	L_self.Init(rows, cols);
	dL_self.Init(rows, cols);
	dL_self1.Init((*l)[x].L->a(), (*l)[x].L->b());
	dL_self2.Init((*l)[y].L->a(), (*l)[y].L->b());
	(*layers)[x].depended.push_back(static_cast<uint32_t>((*l).size()));
	(*layers)[y].depended.push_back(static_cast<uint32_t>((*l).size()));
}

Layer::Layer(std::vector<Layer>* l, uint32_t x, const Pair_X& func, uint32_t rows, uint32_t cols):
	L(&L_self), 
	dL(&dL_self), 
	dL1(&dL_self1), 
	dL2(&dL_self2), 
	//func_1(func.first), 
	//dfunc_1(func.second), 
	func_2(nullptr), 
	dfunc_2(nullptr),
	it_1(x),
	it_2(-1),
	layers(l)
{
	it_self = static_cast<uint32_t>(l->size());
	auto [f2, df2] = func;
	func_1 = f2;
	dfunc_1 = df2;
	if (rows == 0)
		rows = (*layers)[x].L->a();
	if (cols == 0)
		cols = (*layers)[x].L->b();

	L_self.Init(rows, cols); // размерность сохраняется
	dL_self.Init(rows, cols);
	dL_self1.Init((*l)[x].L->a(), (*l)[x].L->b());
	dL_self2.Init(0, 0);
	(*layers)[x].depended.push_back(static_cast<uint32_t>((*l).size()));
}

void Layer::FMMul(Layer* l, Layer* r)
{
	// [a x b] * [b x c] = [a x c]
	auto& x = *l->L;
	auto& y = *r->L;
	if (x.b() != y.a())
		throw InvalidMatrixMul();

	for (uint32_t i = 0; i < x.a(); ++i)
	{
		const float* l1 = &(x.mx[i * x.b()]);
		for (uint32_t j = 0; j < y.b(); ++j)
		{
			L->mx[j + i * y.b()] = 0.f;
			for (uint32_t v = 0; v < x.a(); ++v)
			{
				L->mx[j + i * y.b()] += l1[v] * y.mx[j + y.b() * v];
			}
		}
	}
}

void Layer::dFMMul(Layer* l, Layer* r)
{
}

void Layer::FMAdd(Layer* l, Layer* r)
{
	auto& x = *l->L;
	auto& y = *r->L;
	if ((x.a() != y.a()) || ((*L).a() != x.a()) || (x.b() != y.b()) || ((*L).b() != x.b()))
		throw InvalidMatrixAdd();

	for (uint32_t i = 0; i < (*L).mx.size(); ++i)
		L->mx[i] = x.mx[i] + y.mx[0];
}

void Layer::dFMAdd(Layer* l, Layer* r)
{
	for (auto& dx : l->dL->mx)
		dx = 1.f;
	for (auto& dx : r->dL->mx)
		dx = 1.f;
}

void Layer::FSMul(Layer* l, Layer* r)
{
	auto& x = *l->L;
	auto& y = *r->L;

	if (y.mx.size() != 1)
		throw InvalidMatrixScalMul();

	for (uint32_t i = 0; i < L->mx.size(); ++i)
		L->mx[i] = x.mx[i] * y.mx[0];
}

void Layer::dFSMul(Layer* l, Layer* r)
{
	for (auto& dx : l->dL->mx)
		dx = r->L->mx[0];
}

void Layer::FSDiv(Layer* l, Layer* r)
{
	auto& x = *l->L;
	auto& y = *r->L;

	if (y.mx.size() != 1)
		throw InvalidMatrixScalMul();

	for (uint32_t i = 0; i < (*L).mx.size(); ++i)
		L->mx[i] = x.mx[i] / y.mx[0];
}

void Layer::dFSDiv(Layer* l, Layer* r)
{
}

void Layer::FSMMul(Layer* l, Layer* r)
{
	auto& x = *l->L;
	auto& y = *r->L;
	if ((x.a() != y.a()) || (L->a() != x.a()) || (x.b() != y.b()) || (L->b() != x.b()))
		throw InvalidMatrixSMul();

	for (uint32_t i = 0; i < L->a(); ++i)
	{
		for (uint32_t j = 0; j < x.b(); ++j)
			(*L)(i, j) = x(i, j) * y(i, j);
	}
}

void Layer::dFSMMul(Layer* l, Layer* r)
{
}

void Layer::FRMul(Layer* l, Layer* r)
{
	auto& x = *l->L;
	auto& y = *r->L;

	if ((x.a() != y.a()) || (y.b() != 1) || ((*L).a() != x.a()) || ((*L).b() != 1))
		throw InvalidMatrixRowMul();

	for (uint32_t i = 0; i < (*L).a(); ++i)
	{
		(*L)(i, 0) = 0.f;
		for (uint32_t j = 0; j < x.b(); ++j)
			(*L)(i, 0) += x(i, j) * y(i, 0);
	}
}

void Layer::dFRMul(Layer* l, Layer* r)
{
}

void Layer::FRDiv(Layer* l, Layer* r)
{
	auto& x = *l->L;
	auto& y = *r->L;

	if ((x.a() != y.a()) || (y.b() != 1) || ((*L).a() != x.a()))
		throw InvalidMatrixRowMul();

	for (uint32_t i = 0; i < (*L).a(); ++i)
	{
		for (uint32_t j = 0; j < x.b(); ++j)
			(*L)(i, j) = x(i, j) / y(i, 0);
	}
}

void Layer::dFRDiv(Layer* l, Layer* r)
{
}

void Layer::FExp(Layer* l)
{
	auto& x = *l->L;
	for (uint32_t i = 0; i < (*L).mx.size(); ++i)
		(*L).mx[i] = exp(x.mx[i]);
}

void Layer::dFExp(Layer* l)
{
}

void Layer::FLog(Layer* l)
{
	auto& x = *l->L;
	for (uint32_t i = 0; i < (*L).mx.size(); ++i)
		(*L).mx[i] = log(x.mx[i]);
}

void Layer::dFLog(Layer* l)
{
}

void Layer::FPow2(Layer* l)
{
	auto& x = *l->L;
	for (uint32_t i = 0; i < (*L).mx.size(); ++i)
		(*L).mx[i] = x.mx[i] * x.mx[i];
}

void Layer::dFPow2(Layer* l)
{
}

void Layer::FNeg(Layer* l)
{
	auto& x = *l->L;
	for (uint32_t i = 0; i < (*L).mx.size(); ++i)
		(*L).mx[i] = -x.mx[i];
}

void Layer::dFNeg(Layer* l)
{
}

void Layer::FSumCol(Layer* l)
{
	auto& x = *l->L;
	if ((*L).b() != 1)
		throw InvalidMatrixSumCol();

	for (uint32_t i = 0; i < (*L).a(); ++i)
	{
		(*L)(i, 0) = 0.f;
		for (uint32_t j = 0; j < x.b(); ++j)
			(*L)(i, 0) += x(i, j);
	}
}

void Layer::dFSumCol(Layer* l)
{
}

void Layer::FSumRow(Layer* l)
{
	auto& x = *l->L;
	if ((*L).a() != 1)
		throw InvalidMatrixSumRow();

	for (uint32_t j = 0; j < (*L).b(); ++j)
	{
		(*L)(0, j) = 0.f;
		for (uint32_t i = 0; i < x.a(); ++i)
			(*L)(0, j) += x(i, j);
	}
}

void Layer::dFSumRow(Layer* l)
{
}

void Layer::FSum(Layer* l)
{
	auto& x = *l->L;
	if (((*L).a() != 1) || ((*L).b() != 1))
		throw InvalidMatrixSum();

	(*L)(0, 0) = 0.f;
	for (uint32_t i = 0; i < x.a(); ++i)
		for (uint32_t j = 0; j < x.b(); ++j)
			(*L)(0, 0) += x(i, j);
}

void Layer::dFSum(Layer* l)
{
}


const Layer::Pair_XY Layer::MMul = { &Layer::FMMul, &Layer::dFMMul };
const Layer::Pair_XY Layer::MAdd = { &Layer::FMAdd, &Layer::dFMAdd };
const Layer::Pair_XY Layer::SMul = { &Layer::FSMul, &Layer::dFSMul };
const Layer::Pair_XY Layer::SDiv = { &Layer::FSDiv, &Layer::dFSDiv };
const Layer::Pair_XY Layer::SMMul = { &Layer::FSMMul, &Layer::dFSMMul };
const Layer::Pair_XY Layer::RMul = { &Layer::FRMul, &Layer::dFRMul };
const Layer::Pair_XY Layer::RDiv = { &Layer::FRDiv, &Layer::dFRDiv };
const Layer::Pair_X Layer::Exp = { &Layer::FExp, &Layer::dFExp };
const Layer::Pair_X Layer::Log = { &Layer::FLog, &Layer::dFLog };
const Layer::Pair_X Layer::Pow2 = { &Layer::FPow2, &Layer::dFPow2 };
const Layer::Pair_X Layer::Neg = { &Layer::FNeg, &Layer::dFNeg };
const Layer::Pair_X Layer::SumCol = { &Layer::FSumCol, &Layer::dFSumCol };
const Layer::Pair_X Layer::SumRow = { &Layer::FSumRow, &Layer::dFSumRow };
const Layer::Pair_X Layer::Sum = { &Layer::FSum, &Layer::dFSum };