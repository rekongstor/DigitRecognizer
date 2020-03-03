#include "SimpleNN.h"
#include "Dataset.h"


SimpleNN::SimpleNN(Dataset& _dataset) : dataset(_dataset)
{
	InitNN();
}

uint32_t SimpleNN::AddLayer(Matrix2d& input)
{
	layers.push_back(Layer(&layers, input)); // 0: [1x1] 
	return layers.size() - 1;
}

uint32_t SimpleNN::AddLayer(uint32_t rows, uint32_t cols)
{
	layers.push_back(Layer(&layers, rows, cols)); // 3: [10x1024] 
	return layers.size() - 1;
}

uint32_t SimpleNN::AddLayer(uint32_t x, const Layer::Pair_X& func, uint32_t rows, uint32_t cols)
{
	layers.push_back(Layer(&layers, x, func, rows, cols)); // 4: [1024x10] 
	return layers.size() - 1;
}

uint32_t SimpleNN::AddLayer(uint32_t x, uint32_t y, const Layer::Pair_XY& func, uint32_t rows, uint32_t cols)
{
	layers.push_back(Layer(&layers, x, y, func, rows, cols)); // 5: [BSx10]
	return layers.size() - 1;
}

void SimpleNN::InitNN()
{
	float batch_size_mas[] = { static_cast<float>(BATCH_SIZE) };
	Matrix2d batch_size(1,1, batch_size_mas);

	float regularization_scale_mas[] = { 1.f };
	Matrix2d regularization_scale(1,1, regularization_scale_mas);

	// 0 B = batch_size value
	// 1 L = labels
	// 2 I = images
	// 3 W = weights
	// 4 WT = transposed weights
	// 5 M = I x WT
	// 6 E = exp(M)
	// 7 Es = sum(E)
	// 8 S = softmax (E/Es)
	// 9 Lg = Log(S)
	// 10 GT = ground-truth (Lg*L)
	// 11 SGT = Sum(GT)
	// 12 NS
	// 13 Loss = -SGT/BS
	// 14 W2 = Pow2(W)
	// 15 L2 = Sum(W2)
	// 16 RS = regularization parameter
	// 17 Reg = RS*L2
	// 18 F = L2 + Reg
	
	auto B = AddLayer(batch_size); // [1x1] 
	auto L = AddLayer(dataset.train_labels[0]); // [BSx10] 
	auto I = AddLayer(dataset.train_images[0]); // [BSx1024] 
	auto W = AddLayer(RESULT_NEURONS, TEXTURE_SIZE * TEXTURE_SIZE); // [10x1024] 
	auto WT = AddLayer(W, Layer::Trans, TEXTURE_SIZE * TEXTURE_SIZE, RESULT_NEURONS); // [1024x10] 
	auto M = AddLayer(I, WT, Layer::MMul); // [BSx10]
	auto E = AddLayer(M, Layer::Exp); // 6: [BSx10]
	auto Es = AddLayer(E, Layer::SumCol, BATCH_SIZE, 1); // [BSx1]
	auto S = AddLayer(E, Es, Layer::RDiv, BATCH_SIZE, 10); // [BSx10]
	auto Lg = AddLayer(S, Layer::Log); // 9: [BSx10]
	auto GT = AddLayer(L, Lg, Layer::SMMul); // 10: [BSx10]
	auto SGT = AddLayer(GT, Layer::Sum, 1, 1); // [1x1]
	auto NS = AddLayer(SGT, Layer::Neg); // [1x1]
	auto Loss = AddLayer(NS, 0, Layer::SDiv); // loss function [1x1]
	auto W2 = AddLayer(WT, Layer::Pow2); // 14: [1024x10]
	auto L2 = AddLayer(W2, Layer::Sum, 1, 1); // 15: [1x1]
	auto RS = AddLayer(regularization_scale); // 16: input RS [1x1]
	auto Reg = AddLayer(L2, RS, Layer::SMMul); // 17: регул€ризаци€ [1x1]
	auto F = AddLayer(Loss, Reg, Layer::MAdd); // 18: задача оптимизации - уменьшать [1x1]

	for (auto& L : layers)
		L.F();

	layers[F].dF();
	//layers[3].dF(layers[17].getVal());

}

void SimpleNN::TrainNN()
{	

}

void SimpleNN::TestNN()
{

}
