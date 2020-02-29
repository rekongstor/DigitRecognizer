#pragma once

class Matrix2d
{
	uint32_t a_;
	uint32_t b_;
public:
	std::vector<float> mx;
	uint32_t a();
	uint32_t b();

	Matrix2d();
	Matrix2d(uint32_t _i, uint32_t _j);
	Matrix2d(uint32_t _i, uint32_t _j, float* data);
	void Resize(uint32_t _i, uint32_t _j);
	void Init(uint32_t _i, uint32_t _j);
	void Init(uint32_t _i, uint32_t _j, float* data);;

	float& operator()(uint32_t _i, uint32_t _j);
	float operator()(uint32_t _i, uint32_t _j) const;
};
