#include "SimpleNN.h"
#include "Dataset.h"


SimpleNN::SimpleNN(Dataset& _dataset) : dataset(_dataset)
{
	InitNN();
}

void SimpleNN::InitNN()
{

	layers[0] = (Layer(dataset));
	layers[1] = (Layer(1024, 10));
	layers[2] = (Layer(&(layers[0]), &(layers[1]), &Layer::MMul, BATCH_SIZE, 10));
	for (auto& L : layers)
		L.F();
}

void SimpleNN::TrainNN()
{	

}

void SimpleNN::TestNN()
{

}
