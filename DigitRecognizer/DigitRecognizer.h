#pragma once


struct sample
{
	float_t image[TEXTURE_SIZE][TEXTURE_SIZE];
	uint8_t label;
};

class Dataset
{
protected:
	std::vector<sample> train_data;
	std::vector<sample> test_data;
public:
	Dataset();
	void SetData(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images);
};
