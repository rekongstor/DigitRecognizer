/*
main � ��������� �������� � ������
*/

#include "Dataset.h"
#include "Window.h"
#include "SimpleNN.h"
const uint32_t thread_count = std::thread::hardware_concurrency();
Dataset data("train-labels.idx1-ubyte", "train-images.idx3-ubyte", "t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");
SimpleNN simple_dr(data);	
IDigitRecognizer* DR = &simple_dr;


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int iCmdShow)
{
	


	return Window::OnInit(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
}

