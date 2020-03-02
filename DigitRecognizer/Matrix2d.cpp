#include "Matrix2d.h"


uint32_t Matrix2d::a()
{
	return a_;
}

uint32_t Matrix2d::b()
{
	return b_;
}

//Matrix2d::Matrix2d(const Matrix2d& l) :
//	a_(l.a_),
//	b_(l.b_)
//{
//	mx.resize(l.mx.size());
//	for (int i = 0; i < mx.size(); ++i)
//		mx[i] = l.mx[i];
//}
//
//Matrix2d& Matrix2d::operator=(const Matrix2d& l)
//{
//	a_ = l.a_;
//	b_ = l.b_;
//	mx.resize(l.mx.size());
//	for (int i = 0; i < mx.size(); ++i)
//		mx[i] = l.mx[i];
//	return *this;
//}

Matrix2d::Matrix2d() : a_(0), b_(0)
{
}

Matrix2d::Matrix2d(uint32_t _i, uint32_t _j) : a_(_i), b_(_j)
{
	mx.resize(a_ * b_);
}

Matrix2d::Matrix2d(uint32_t _i, uint32_t _j, float* data) : a_(_i), b_(_j)
{
	mx.resize(a_ * b_);
	for (uint32_t i = 0; i < _i; ++i)
		for (uint32_t j = 0; j < _j; ++j)
			mx[i * _j + j] = data[i * _j + j];
}

void Matrix2d::Resize(uint32_t _i, uint32_t _j)
{
	if (a_ * b_ == _i * _j)
	{
		a_ = _i;
		b_ = _j;
	}
	else
		throw InvalidMatrixResize();
}

void Matrix2d::Init(uint32_t _i, uint32_t _j)
{
	a_ = _i;
	b_ = _j;
	mx.resize(a_ * b_);
}

void Matrix2d::Init(uint32_t _i, uint32_t _j, float* data)
{
	a_ = _i;
	b_ = _j;
	mx.resize(a_ * b_);
	for (uint32_t i = 0; i < _i; ++i)
		for (uint32_t j = 0; j < _j; ++j)
			mx[i * _j + j] = data[i * _j + j];
}


float& Matrix2d::operator()(uint32_t _i, uint32_t _j)
{
	if ((_i > a_) || _j > b_)
		MatrixOutOfRange();
	return mx[_i * b_ + _j];
}

float Matrix2d::operator()(uint32_t _i, uint32_t _j) const
{
	if ((_i > a_) || _j > b_)
		MatrixOutOfRange();
	return mx[_i * b_ + _j];
}
