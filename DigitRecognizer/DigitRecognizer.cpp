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
inline void ReadAndConvert(std::ifstream& in, T& value)
{
	in.read(reinterpret_cast<char*>(&value), sizeof(value));
	ConvertEndian<T>(value);
}

DigitRecognizer::DigitRecognizer()
{
}


void DigitRecognizer::SetData(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images)
{
	auto labels_set = [](sample& sample_ref, std::ifstream& in)
	{
		ReadAndConvert<>(in, sample_ref.label);
	};
	auto images_set = [](sample& sample_ref, std::ifstream& in)
	{
		for (int i = 0; i < IMG_SIZE; ++i)
			for (int j = 0; j < IMG_SIZE; ++j)
				ReadAndConvert<>(in, sample_ref.image[i][j]);
	};
	auto load_data = [&](const char* file, std::vector<sample>& set, void (*func)(sample&, std::ifstream&), int32_t msb)
	{
		try
		{
			int32_t MSB_first;
			int32_t number_of_items;
			std::ifstream input_data(file, std::ios_base::binary);
			ReadAndConvert<>(input_data, MSB_first);
			ReadAndConvert<>(input_data, number_of_items);
			if (MSB_first != msb || set.size() != number_of_items)
				throw (CorruptedFileEX());
			if (MSB_first == 0x00000803) // handle file as an image set, need to read two ints
			{
				int32_t num_rows, num_columns;
				ReadAndConvert<>(input_data, num_rows);
				ReadAndConvert<>(input_data, num_columns);
				if ((num_columns != IMG_SIZE) || (num_rows != IMG_SIZE))
					throw (CorruptedFileEX());
			}

			for (int i = 0; i < number_of_items; ++i)
			{
				func(set[i], input_data);
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

void DigitRecognizer::PrintRandom()
{
	sample& s = train_data[rnd.Int(0, static_cast<int32_t>(train_data.size())-1)];
	for (int i = 0; i < IMG_SIZE; ++i)
	{
		for (int j = 0; j < IMG_SIZE; ++j)
			std::cout << (s.image[i][j] > 127 ? '1' : ' ');
		std::cout << std::endl;
	}
	std::cout << "Number: " << static_cast<int32_t>(s.label) << std::endl;
	return;
}
