/*
main с загрузкой датасета в память
*/

#include "Dataset.h"
#include "Window.h"
#include "SimpleNN.h"

Dataset data("train-labels.idx1-ubyte", "train-images.idx3-ubyte", "t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");	

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int iCmdShow)
{

	std::vector<IDigitRecognizer*> drs;

	SimpleNN simple_dr(data);
	drs.push_back(&simple_dr);

	for (auto& it : drs)
	{
		it->TrainNN();
		it->TestNN();
	}
	


	return Window::OnInit(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
}

