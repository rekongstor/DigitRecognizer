#include "SingleLayerDR.h"
#include "Dataset.h"

Matrix2d SingleLayerDR::Calculate(const Matrix2d& input)
{
	Matrix2d res(10, 1);
	res = w1 * input;
	res = res + b1;
	return res;
}
void SingleLayerDR::NumGrad(Matrix2d& vals, Matrix2d& grad)
{
	float_t loss = Loss();

	for (int i = 0; i < vals.mx.size(); ++i)
	{
		vals.mx[i] += DELTA;
		grad.mx[i] = grad_step * (loss - Loss());
		vals.mx[i] -= DELTA;
	}
}

float_t SingleLayerDR::Loss()
{
	float_t L = 0.f;

	for (int i = 0; i < BATCH_SIZE; ++i)
	{
		Matrix2d output = Calculate(dataset.train_images[i + offset]);

		float s = 0.f;
		for (auto& v : output.mx)
		{
			v = exp(v);
			s += v;
		}
		for (auto& v : output.mx)
			v /= s;
		L -= log(output.mx[dataset.train_labels[i + offset]]);
	}
	for (auto& v : w1.mx)
		L += v * v;
	for (auto& v : b1.mx)
		L += v * v;
	return L;
}

uint32_t SingleLayerDR::True()
{
	uint32_t res = 0;
	for (int i = 0; i < BATCH_SIZE; ++i)
	{
		Matrix2d output = Calculate(dataset.train_images[i + offset]);
		float s = 0.f;
		for (auto& v : output.mx)
		{
			v = exp(v);
			s += v;
		}
		for (auto& v : output.mx)
			v /= s;

		int max = 0;
		float x = output.mx[0];
		for (int j = 1; j < 10; ++j)
			if (output.mx[j] > x)
			{
				max = j;
				x = output.mx[j];
			}

		if (dataset.train_labels[i + offset] == max)
			++res;
	}
	return res;
}

SingleLayerDR::SingleLayerDR(const Dataset& _dataset) : dataset(_dataset), gens(0)
{
}

void SingleLayerDR::InitNN()
{
	w1.Init(10, 1024);
	w1_grad.Init(10, 1024);
	b1.Init(10, 1);
	b1_grad.Init(10, 1);
	// randomize weigths and biases
	for (auto& v : w1.mx)
		v = 0.f;
		//v = 1.f - Random::Float() * 2.f;

	for (auto& v : b1.mx)
		v = 0.f;
		//v = 1.f - Random::Float() * 2.f;
}

void SingleLayerDR::TrainNN()
{	
	offset = 0;
	grad_step = 32.f;
	while (offset + BATCH_SIZE < TRAIN_SIZE)
	{
		float loss = Loss();
		NumGrad(w1, w1_grad);
		NumGrad(b1, b1_grad);
		w1 = w1 + w1_grad;
		b1 = b1 + b1_grad;
		if ((Loss() > loss) && grad_step > 1.f)
			grad_step *= 0.5f;
		else
			if (grad_step < 32.f)
				grad_step *= 2.f;
		++gens;
		offset += BATCH_SIZE;
	}
	return;
}

void SingleLayerDR::TestNN()
{

}
