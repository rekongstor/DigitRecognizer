/*
main с загрузкой датасета в память
*/
#include "SingleCoreDG.h"

int main()
{
	char train_images_file[] = "";
	std::vector<DigitRecognizer_up> Recs;
	Recs.push_back(std::make_unique<SingleCoreDG>());

	std::for_each(Recs.begin(), Recs.end(), [](DigitRecognizer_up& R)
	{
		R->SetData("train-labels.idx1-ubyte", "train-images.idx3-ubyte", "t10k-labels.idx1-ubyte", "t10k-images.idx3-ubyte");
		R->PrintRandom();
	});
	return 0;
}