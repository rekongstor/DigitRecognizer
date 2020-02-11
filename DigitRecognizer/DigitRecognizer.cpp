#include "DigitRecognizer.h"

template<class T>
inline void ConvertEndian(T& value)
{
	char* data = reinterpret_cast<char*>(&value);

	T tmp;
	char* tmp_data = reinterpret_cast<char*>(&tmp);

	for (int i = 0; i < sizeof(value); ++i)
		tmp_data[i] = data[sizeof(value) - i - 1];
	value = tmp;
}

template<class T>
inline void Read(std::ifstream& in, T& value, bool Convert)
{
	in.read(reinterpret_cast<char*>(&value), sizeof(value));
	if (Convert)
		ConvertEndian<T>(value);
}

Dataset::Dataset()
{
}


void Dataset::SetData(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images)
{
	auto labels_set = [](sample& sample_ref, std::ifstream& in, bool Convert)
	{
		Read<>(in, sample_ref.label, Convert);
	};
	auto images_set = [](sample& sample_ref, std::ifstream& in, bool Convert)
	{
		for (int i = 0; i < 28; ++i)
			for (int j = 0; j < 28; ++j)
			{
				uint8_t tmp_pixel;
				Read<>(in, tmp_pixel, Convert);
				sample_ref.image[i][j] = static_cast<float_t>(tmp_pixel) / 255.f;
			}
	};
	auto load_data = [&](const char* file, std::vector<sample>& set, void (*func)(sample&, std::ifstream&, bool), int32_t msb)
	{
		try
		{
			int32_t MSB_first;
			int32_t number_of_items;
			std::ifstream input_data(file, std::ios_base::binary);
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
			if (MSB_first != msb || set.size() != number_of_items)
				throw (CorruptedFileEX());
			if (MSB_first == 0x00000803) // handle file as an image set, need to read two ints
			{
				int32_t num_rows, num_columns;
				Read<>(input_data, num_rows, needEndianConvert);
				Read<>(input_data, num_columns, needEndianConvert);
				if ((num_columns != IMG_SIZE) || (num_rows != IMG_SIZE))
					throw (CorruptedFileEX());
			}

			for (int i = 0; i < number_of_items; ++i)
			{
				func(set[i], input_data, needEndianConvert);
			}

		}
		catch (...)
		{
			// TODO: Exception handling
		}
	};
	train_data.resize(TRAIN_SIZE);
	test_data.resize(TEST_SIZE);
	load_data(train_file_labels, train_data, labels_set, 0x00000801);
	load_data(train_file_images, train_data, images_set, 0x00000803);
	load_data(test_file_labels, test_data, labels_set, 0x00000801);
	load_data(test_file_images, test_data, images_set, 0x00000803);
}
