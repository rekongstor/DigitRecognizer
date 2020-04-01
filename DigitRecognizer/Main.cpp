/*
main с загрузкой датасета в память
*/

#include "Dataset.h"
#include "Window.h"
#include "SimpleNN.h"
const uint32_t thread_count = std::thread::hardware_concurrency();
IDigitRecognizer* DR = nullptr;


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int iCmdShow)
{

	Dataset data("train-labels.idx1-ubyte", "train-images.idx3-ubyte", "t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");
	{
		std::ofstream out("MNIST", std::ios::binary);
		// u64 endian check = 4221
		// u32 inputs
		// u32 outputs
		// u64 dataSize
		// f32[inputs] * dataSize
		// f32[outputs] * dataSize
		uint64_t endian_check = 4221;
		uint32_t inputs = TEXTURE_SIZE * TEXTURE_SIZE;
		uint32_t outputs = RESULT_NEURONS;
		uint64_t dataSize = TRAIN_SIZE + TEST_SIZE;

		out.write(reinterpret_cast<char*>(&endian_check), sizeof(endian_check));
		out.write(reinterpret_cast<char*>(&inputs), sizeof(inputs));
		out.write(reinterpret_cast<char*>(&outputs), sizeof(outputs));
		out.write(reinterpret_cast<char*>(&dataSize), sizeof(dataSize));
		for (auto& m : data.train_images)
			out.write(reinterpret_cast<char*>(m.mx.data()), sizeof(float_t)* m.mx.size());
		for (auto& m : data.test_images)
			out.write(reinterpret_cast<char*>(m.mx.data()), sizeof(float_t)* m.mx.size());
		for (auto& m : data.train_labels)
			out.write(reinterpret_cast<char*>(m.mx.data()), sizeof(float_t)* m.mx.size());
		for (auto& m : data.test_labels)
			out.write(reinterpret_cast<char*>(m.mx.data()), sizeof(float_t)* m.mx.size());
	}
	SimpleNN simple_dr(data);
	DR = &simple_dr;


	return Window::OnInit(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
}

