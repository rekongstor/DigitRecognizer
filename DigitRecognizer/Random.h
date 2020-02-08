#pragma once

class Random
{
	static Random s_random;
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	Random();
	Random(const Random&) = delete;
public:
	static float Float();
	static int32_t Int(int32_t start, int32_t end);
};

