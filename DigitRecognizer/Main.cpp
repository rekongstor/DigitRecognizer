/*
main с загрузкой датасета в память
*/

#include "SingleCoreDG.h"
#include "Window.h"

int digit_recognizer()
{
	std::vector<DigitRecognizer_up> Recs;
	Recs.push_back(std::make_unique<SingleCoreDG>());

	std::for_each(Recs.begin(), Recs.end(), [](DigitRecognizer_up& R)
	{
		R->SetData("train-labels.idx1-ubyte", "train-images.idx3-ubyte", "t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");
		R->PrintRandom();
	});
	return 0;
}



INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int iCmdShow)
{
	digit_recognizer();
	return Window::OnInit(hInstance, hPrevInstance, lpCmdLine, iCmdShow);
}

