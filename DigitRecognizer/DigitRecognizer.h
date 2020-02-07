#pragma once


struct sample
{
	uint8_t image[IMG_SIZE][IMG_SIZE];
	uint8_t label;
};

class DigitRecognizer
{
protected:
	std::vector<sample> train_data;
	std::vector<sample> test_data;
public:
	DigitRecognizer();
	void SetData(const char* train_file_labels, const char* train_file_images, const char* test_file_labels, const char* test_file_images);
	void PrintRandom();
	virtual ~DigitRecognizer() = 0 {};
};

typedef std::shared_ptr<DigitRecognizer> DigitRecognizer_sp;
typedef std::unique_ptr<DigitRecognizer> DigitRecognizer_up;

