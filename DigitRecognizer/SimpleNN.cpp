#include "SimpleNN.h"
#include "Dataset.h"


SimpleNN::SimpleNN(Dataset& _dataset) : dataset(_dataset)
{
	InitNN();
}

void SimpleNN::InitNN()
{

	layers[0] = (Layer(10, 1024));
	layers[1] = (Layer(dataset));
	layers[2] = (Layer(&(layers[0]), &(layers[1]), &Layer::MMul, 10, 1));
	for (auto& L : layers)
		L.F();
}

void SimpleNN::TrainNN()
{	

}

void SimpleNN::TestNN()
{

}
