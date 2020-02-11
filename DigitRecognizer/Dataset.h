#pragma once

class Dataset
{
	friend class SingleCoreDR;
	image_vector train_data_image;
	label_vector train_data_label;
	image_vector test_data_image;
	label_vector test_data_label;
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
	inline void LoadData(const char* file, label_vector& set, int32_t msb);
	inline void LoadData(const char* file, image_vector& set, int32_t msb);
};