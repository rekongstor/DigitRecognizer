#pragma once

class Random
{
	std::mt19937 s_RandomEngine;
	std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
public:
	Random();
	float Float();
	int32_t Int(int32_t start, int32_t end);
};

