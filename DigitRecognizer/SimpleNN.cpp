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

	float regularization_scale_mas[] = { 1.3f };
	Matrix2d regularization_scale(1,1, regularization_scale_mas);
	
	// 0 B = batch_size value
	// 1 L = labels
	// 2 I = images
	// 3 W = weights
	// 4 M = I x W
	// 5 E = exp(M)
	// 6 Es = sum(E)
	// 7 S = softmax (E/Es)
	// 8 Lg = Log(S)
	// 9 GT = ground-truth (Lg*L)
	// 10 SGT = Sum(GT)
	// 11 NS
	// 12 Loss = -SGT/BS
	// 13 W2 = Pow2(W)
	// 14 L2 = Sum(W2)
	// 15 RS = regularization parameter
	// 16 Reg = RS*L2
	// 17 F = L2 + Reg
	layers.push_back(Layer(&layers, batch_size)); // 0: [1x1] 
	layers.push_back(Layer(&layers, dataset.train_labels[0])); // 1: [BSx10] 
	layers.push_back(Layer(&layers, dataset.train_images[0])); // 2: [BSx1024] 
	layers.push_back(Layer(&layers, 1024, 10)); // 3: [1024x10] 
	layers.push_back(Layer(&layers, 2, 3, Layer::MMul)); // 4:
	layers.push_back(Layer(&layers, 4, Layer::Exp)); // 5:
	layers.push_back(Layer(&layers, 5, Layer::SumCol, BATCH_SIZE, 1)); // 6:
	layers.push_back(Layer(&layers, 5, 6, Layer::RDiv, BATCH_SIZE, 10)); // 7:
	layers.push_back(Layer(&layers, 7, Layer::Log)); // 8:
	layers.push_back(Layer(&layers, 1, 8, Layer::SMMul)); // 9:
	layers.push_back(Layer(&layers, 9, Layer::Sum, 1, 1)); //10:
	layers.push_back(Layer(&layers, 10, Layer::Neg)); // 11:
	layers.push_back(Layer(&layers, 11, 0, Layer::SDiv)); // 12: loss function
	layers.push_back(Layer(&layers, 3, Layer::Pow2)); // 13:
	layers.push_back(Layer(&layers, 13, Layer::Sum, 1, 1)); // 14:
	layers.push_back(Layer(&layers, regularization_scale)); // 15: input RS
	layers.push_back(Layer(&layers, 14, 15, Layer::SMMul)); // 16: регул€ризаци€
	layers.push_back(Layer(&layers, 12, 16, Layer::MAdd)); // 17: задача оптимизации - уменьшать
	float m = log(exp(1.f));
	for (auto& L : layers)
		L.F();
	layers[17].dF();
	layers[5].dF(layers[17].getVal());
	layers[6].dF(layers[17].getVal());
	layers[7].dF(layers[17].getVal());
}

void SimpleNN::TrainNN()
{	

}

void SimpleNN::TestNN()
{

}
