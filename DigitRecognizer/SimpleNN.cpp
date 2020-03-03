#include "SimpleNN.h"
#include "Dataset.h"

float batch_size_mas[] = { static_cast<float>(BATCH_SIZE) };
float regularization_scale_mas[] = { 1.f };

SimpleNN::SimpleNN(Dataset& _dataset) : dataset(_dataset)
{

	batch_size = Matrix2d(1, 1, batch_size_mas);
	regularization_scale = Matrix2d(1, 1, regularization_scale_mas);
	auto B = AddLayer(batch_size);														// [1x1] 									// 0 B = batch_size value
	L = AddLayer(dataset.train_labels[0]);												// [BSx10] 									// 1 L = labels
	X = AddLayer(dataset.train_images[0]);												// [BSx1024] 								// 2 I = images
	W = AddLayer(RESULT_NEURONS, TEXTURE_SIZE * TEXTURE_SIZE);							// [10x1024] 								// 3 W = weights
	auto WT = AddLayer(W, Layer::Trans, TEXTURE_SIZE * TEXTURE_SIZE, RESULT_NEURONS);	// [1024x10] 								// 4 WT = transposed weights
	auto M = AddLayer(X, WT, Layer::MMul);												// [BSx10]									// 5 M = I x WT
	auto E = AddLayer(M, Layer::Exp);													// [BSx10]									// 6 E = exp(M)
	auto Es = AddLayer(E, Layer::SumCol, BATCH_SIZE, 1);								// [BSx1]									// 7 Es = sum(E)
	S = AddLayer(E, Es, Layer::RDiv, BATCH_SIZE, 10);								// [BSx10]									// 8 S = softmax (E/Es)
	auto Lg = AddLayer(S, Layer::Log);													// [BSx10]									// 9 Lg = Log(S)
	auto GT = AddLayer(L, Lg, Layer::SMMul);											// [BSx10]									// 10 GT = ground-truth (Lg*L)
	auto SGT = AddLayer(GT, Layer::Sum, 1, 1);											// [1x1]									// 11 SGT = Sum(GT)
	auto NS = AddLayer(SGT, Layer::Neg);												// [1x1]									// 12 NS
	auto Loss = AddLayer(NS, B, Layer::SDiv);											// loss function [1x1]						// 13 Loss = NS/B
	auto W2 = AddLayer(WT, Layer::Pow2);												// [1024x10]								// 14 W2 = Pow2(W)
	auto L2 = AddLayer(W2, Layer::Sum, 1, 1);											// [1x1]									// 15 L2 = Sum(W2)
	auto RS = AddLayer(regularization_scale);											// input RS [1x1]							// 16 RS = regularization parameter
	auto Reg = AddLayer(L2, RS, Layer::SMMul);											// регул€ризаци€ [1x1]						// 17 Reg = RS*L2
	F = AddLayer(Loss, Reg, Layer::MAdd);												// задача оптимизации - уменьшать [1x1]		// 18 F = L2 + Reg
}

void SimpleNN::ForwardProp()
{
	for (auto& L : layers)
		L.F();
}

void SimpleNN::BackProp()
{
	float& f = layers[F].getVal();
	float y = f + 0.1f;
	grad_step = 1.f / 4096.f;
	while (f < y)
	{
		y = f;
		layers[F].dF();
		layers[W].SubGrad(grad_step);
		ForwardProp();
	}
}

uint32_t SimpleNN::AddLayer(Matrix2d& input)
{
	layers.push_back(Layer(&layers, input)); // 0: [1x1] 
	return static_cast<uint32_t>(layers.size() - 1);
}

uint32_t SimpleNN::AddLayer(uint32_t rows, uint32_t cols)
{
	layers.push_back(Layer(&layers, rows, cols)); // 3: [10x1024] 
	return static_cast<uint32_t>(layers.size() - 1);
}

uint32_t SimpleNN::AddLayer(uint32_t x, const Layer::Pair_X& func, uint32_t rows, uint32_t cols)
{
	layers.push_back(Layer(&layers, x, func, rows, cols)); // 4: [1024x10] 
	return static_cast<uint32_t>(layers.size() - 1);
}

uint32_t SimpleNN::AddLayer(uint32_t x, uint32_t y, const Layer::Pair_XY& func, uint32_t rows, uint32_t cols)
{
	layers.push_back(Layer(&layers, x, y, func, rows, cols)); // 5: [BSx10]
	return static_cast<uint32_t>(layers.size() - 1);
}

void SimpleNN::TrainNN()
{	
	size_t offset = 0;
	do
	{
		layers[L] = Layer(&layers,dataset.train_labels[offset]);
		layers[X] = Layer(&layers,dataset.train_images[offset]);
		ForwardProp();
		BackProp();
		offset += TRAIN_SIZE;
	} while (offset < TRAIN_SIZE);
}

void SimpleNN::TestNN()
{
	size_t offset = 0;
	uint32_t valid = 0;
	do
	{
		layers[L] = Layer(&layers, dataset.test_labels[offset]);
		layers[X] = Layer(&layers, dataset.test_images[offset]);
		ForwardProp();
		valid += layers[S].Test(L);
		offset += TEST_SIZE;
	} while (offset < TEST_SIZE);
	valid;
}
