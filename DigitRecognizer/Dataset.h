#pragma once
#include "Matrix2d.h"

class Dataset
{
	friend class Layer;
	//image_vector train_data_image;
	//label_vector train_data_label;
	//image_vector test_data_image;
	//label_vector test_data_label;

	Matrix2d train_images[TRAIN_SIZE]; //1024x1
	uint8_t train_labels[TRAIN_SIZE];
	Matrix2d test_images[TEST_SIZE]; //1024x1
	uint8_t test_labels[TEST_SIZE];

public:
	Dataset();
	Dataset(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images);
	inline void SetData(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images);

private:
	template<class T>
	inline void ConvertEndian(T& value);
	template<class T>
	inline void Read(std::ifstream& in, T& value, bool Convert);

	inline std::pair<int32_t, bool> LoadHeader(std::ifstream& input_data, int32_t msb);
	inline void LoadData(const char* file, Matrix2d* set, int32_t msb);
	inline void LoadData(const char* file, uint8_t* set, int32_t msb);
};