#include "Matrix2d.h"


uint32_t Matrix2d::a()
{
	return a_;
}

uint32_t Matrix2d::b()
{
	return b_;
}

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

Matrix2d Matrix2d::operator*(const Matrix2d& r) const
{
	// [a x b] * [b x c] = [a x c]
	if (this->b_ != r.a_)
		throw InvalidMatrixMul();

	Matrix2d res(this->a_, r.b_);
	for (uint32_t i = 0; i < this->a_; ++i)
	{
		const float* l1 = &(this->mx[i * this->b_]);
		for (uint32_t j = 0; j < r.b_; ++j)
		{
			for (uint32_t v = 0; v < r.a_; ++v)
			{
				res.mx[j  + i * r.b_] += l1[v] * r.mx[j + r.b_ * v];
			}
		}
	}
	return res;
}

Matrix2d& Matrix2d::operator+(const Matrix2d& r)
{
	if ((this->a_ != r.a_) || (this->b_ != r.b_))
		throw InvalidMatrixAdd();
	for (uint32_t i = 0; i < this->a_; ++i)
	{
		for (uint32_t j = 0; j < r.b_; ++j)
		{
			this->operator()(i, j) = this->operator()(i, j) + r(i, j);
		}
	}
	return *this;
}

float& Matrix2d::operator()(uint32_t _i, uint32_t _j)
{
	return mx[_i * b_ + _j];
}

float Matrix2d::operator()(uint32_t _i, uint32_t _j) const
{
	return mx[_i * b_ + _j];
}
