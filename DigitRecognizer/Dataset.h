#pragma once
#include "Matrix2d.h"

class Dataset
{
	friend class Layer;

public:
	std::vector<Matrix2d> train_images; // BATCH_SIZE x 1024 
	std::vector<Matrix2d> train_labels; // BATCH_SIZE x 10
	std::vector<Matrix2d> test_images; // BATCH_SIZE x 1024 
	std::vector<Matrix2d> test_labels; // BATCH_SIZE x 10
	Dataset();
	Dataset(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images);
	inline void SetData(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images);

private:
	template<class T>
	inline void ConvertEndian(T& value);
	template<class T>
	inline void Read(std::ifstream& in, T& value, bool Convert);

	inline std::pair<int32_t, bool> LoadHeader(std::ifstream& input_data, int32_t msb);
	inline void LoadData(const char* file, std::vector<Matrix2d>& set, int32_t msb);
};