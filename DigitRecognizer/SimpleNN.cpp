#include "SimpleNN.h"
#include "Dataset.h"


SimpleNN::SimpleNN(Dataset& _dataset) : dataset(_dataset)
{
	InitNN();
}

void SimpleNN::InitNN()
{
	float batch_size_mas[] = { static_cast<float>(BATCH_SIZE) };
	Matrix2d batch_size(1,1, batch_size_mas);

	float regularization_scale_mas[] = { 1.f };
	Matrix2d regularization_scale(1,1, regularization_scale_mas);
	
	layers[0] = Layer(batch_size); // [1x1] batch_size value
	layers[1] = Layer(dataset.train_labels[0]); // [BSx10] labels
	layers[2] = Layer(dataset.train_images[0]); // [BSx1024] images
	layers[3] = Layer(1024, 10); // [1024x10] weights
	layers[4] = Layer(&layers[2], &layers[3], Layer::MMul);
	layers[5] = Layer(&layers[4], Layer::Exp);
	layers[6] = Layer(&layers[5], Layer::SumCol, BATCH_SIZE, 1);
	layers[7] = Layer(&layers[5], &layers[6], Layer::RDiv, BATCH_SIZE, 10);
	layers[8] = Layer(&layers[7], Layer::Log);
	layers[9] = Layer(&layers[1], &layers[8], Layer::SMMul);
	layers[10] = Layer(&layers[9], Layer::Sum, 1, 1);
	layers[11] = Layer(&layers[10], Layer::Neg);
	layers[12] = Layer(&layers[11], &layers[0], Layer::SDiv); // loss function
	layers[13] = Layer(&layers[3], Layer::Pow2);
	layers[14] = Layer(&layers[13], Layer::Sum, 1, 1); // регул€ризаци€
	layers[15] = Layer(regularization_scale);
	layers[16] = Layer(&layers[14], &layers[15], Layer::SMMul);
	layers[17] = Layer(&layers[12], &layers[16], Layer::MAdd); // задача оптимизации - уменьшать

	for (auto& L : layers)
		L.F();


}

void SimpleNN::TrainNN()
{	

}

void SimpleNN::TestNN()
{

}
