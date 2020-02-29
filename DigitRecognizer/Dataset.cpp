#include "Dataset.h"

template<class T>
inline void Dataset::ConvertEndian(T& value)
{
	char* data = reinterpret_cast<char*>(&value);

	T tmp;
	char* tmp_data = reinterpret_cast<char*>(&tmp);

	for (int i = 0; i < sizeof(value); ++i)
		tmp_data[i] = data[sizeof(value) - i - 1];
	value = tmp;
}

template<class T>
inline void Dataset::Read(std::ifstream& in, T& value, bool Convert)
{
	in.read(reinterpret_cast<char*>(&value), sizeof(value));
	if (Convert)
		ConvertEndian<T>(value);
}

inline std::pair<int32_t, bool> Dataset::LoadHeader(std::ifstream& input_data, int32_t msb)
{
	int32_t MSB_first;
	int32_t number_of_items;
	Read<>(input_data, MSB_first, false);

	bool needEndianConvert = false;
	ConvertEndian<>(msb);
	if (MSB_first == msb)
	{
		ConvertEndian<>(msb);
		ConvertEndian<>(MSB_first);
		needEndianConvert = true;
	}
	Read<>(input_data, number_of_items, needEndianConvert);
	if (MSB_first != msb)
		throw (CorruptedFileEX());

	if (MSB_first == 0x00000803) // handle file as an image set, need to read two ints
	{
		int32_t num_rows, num_columns;
		Read<>(input_data, num_rows, needEndianConvert);
		Read<>(input_data, num_columns, needEndianConvert);
		if ((num_columns != IMG_SIZE) || (num_rows != IMG_SIZE))
			throw (CorruptedFileEX());
	}

	return std::make_pair<>(number_of_items, needEndianConvert);
}

inline void Dataset::LoadData(const char* file, uint8_t* set, int32_t msb)
{
	std::ifstream input_data(file, std::ios_base::binary);
	auto [number_of_items, needsEndianConver] = LoadHeader(input_data, msb);
	//set.resize(number_of_items);
	for (int i = 0; i < number_of_items; ++i)
	{
		Read<>(input_data, set[i], needsEndianConver);
	}
}

inline void Dataset::LoadData(const char* file, Matrix2d* set, int32_t msb)
{
	std::ifstream input_data(file, std::ios_base::binary);
	auto [number_of_items, needsEndianConver] = LoadHeader(input_data, msb);
	//set.resize(number_of_items);
	//for (int i = 0; i < number_of_items; ++i) 
	for (int i = 0; i < 512; ++i) // TODO: change back
	{
		for (int u = 0; u < 28; ++u)
			for (int v = 0; v < 28; ++v)
			{
				uint8_t tmp_pixel;
				Read<>(input_data, tmp_pixel, needsEndianConver);
				set[i].mx[u * TEXTURE_SIZE + v] = static_cast<float_t>(tmp_pixel) / 255.f;
			}
		set[i].mx[TEXTURE_SIZE * TEXTURE_SIZE - 1] = 1.f; // избавляемся от bias
	}
}

Dataset::Dataset()
{
}

Dataset::Dataset(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images)
{
	SetData(train_file_labels, train_file_images, test_file_labels, test_file_images);
}


void Dataset::SetData(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images)
{
	for (auto& m : train_images)
		m.Init(1024, 1);
	for (auto& m : test_images)
		m.Init(1024, 1);

	LoadData(train_file_images, train_images, 0x00000803);
	LoadData(train_file_labels, train_labels, 0x00000801);

	LoadData(test_file_images, test_images, 0x00000803);
	LoadData(test_file_labels, test_labels, 0x00000801);
}
