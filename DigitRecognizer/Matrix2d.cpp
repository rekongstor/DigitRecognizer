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
	for (int i = 0; i < _i; ++i)
		for (int j = 0; j < _j; ++j)
			mx[i * _j + j] = data[i * _j + j];
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
	for (int i = 0; i < _i; ++i)
		for (int j = 0; j < _j; ++j)
			mx[i * _j + j] = data[i * _j + j];
}

Matrix2d Matrix2d::operator*(const Matrix2d& r)
{
	// [a x b] * [b x c] = [a x c]
	if (this->b_ != r.a_)
		throw InvalidMatrixMul();

	Matrix2d res(this->a_, r.b_);
	for (int i = 0; i < this->a_; ++i)
	{
		float* l1 = &(this->mx[i * this->b_]);
		for (int j = 0; j < r.b_; ++j)
		{
			for (int v = 0; v < r.a_; ++v)
			{
				res.mx[j  + i * r.b_] += l1[v] * r.mx[j + r.b_ * v];
			}
		}
	}
	return res;
}
