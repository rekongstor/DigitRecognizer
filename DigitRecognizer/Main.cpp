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
SimpleNN simple_dr(data);	
DR = &simple_dr;


	return Window::OnInit(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
}

