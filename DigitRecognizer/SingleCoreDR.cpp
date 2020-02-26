#include "SingleCoreDR.h"
#include "Dataset.h"


inline void SingleCoreDR::CalculateNeuron(float_t& neuron, const float_t & bias, const float_t * const data, const float_t * const weight, int32_t size)
{
	neuron = 0.f;
	for (int i = 0; i < size; ++i)
		neuron += data[i] * weight[i];
	neuron -= bias;
	// calculate sigmoid
	neuron = (tanh(neuron) + 1.f) * .5f;
	//neuron = (neuron) / (1 + abs(neuron));
}

void SingleCoreDR::InitNN()
{
	// randomize weigths and biases
	for (auto& n : layer_1_biases)
		n = 1.f - Random::Float() * 2.f;
	for (auto& n : layer_2_biases)
		n = 1.f - Random::Float() * 2.f;
	for (auto& n : res_layer_biases)
		n = 1.f - Random::Float() * 2.f;

	for (auto& n : layer_1_weights)
		for (auto& w : n)
			w = 1.f - Random::Float() * 2.f;
	for (auto& n : layer_2_weights)
		for (auto& w : n)
			w = 1.f - Random::Float() * 2.f;
	for (auto& n : res_layer_weights)
		for (auto& w : n)
			w = 1.f - Random::Float() * 2.f;
}

void SingleCoreDR::TrainNN(const Dataset& dataset)
{

	std::array<float_t, LAYER_1_NEURONS> layer_1_biases_dt;
	std::array<std::array<float_t, IMG_SIZE * IMG_SIZE>, LAYER_1_NEURONS> layer_1_weights_dt;

	std::array<float_t, LAYER_2_NEURONS> layer_2_biases_dt;
	std::array< std::array<float_t, LAYER_1_NEURONS>, LAYER_2_NEURONS> layer_2_weights_dt;

	std::array<float_t, RESULT_NEURONS> res_layer_biases_dt;
	std::array< std::array<float_t, LAYER_2_NEURONS>, RESULT_NEURONS> res_layer_weights_dt;

	int offset = 0;
	int batch_scale = 1;
	float_t grad_step = 1.f;
	volatile float_t disp;
	volatile float_t new_disp = GetDisp(dataset, offset, batch_scale);

	while (true)
	{
		float_t layer_1_biases_len = 0.f;
		float_t layer_1_weights_len = 0.f;
		float_t layer_2_biases_len = 0.f;
		float_t layer_2_weights_len = 0.f;
		float_t res_layer_biases_len = 0.f;
		float_t res_layer_weights_len = 0.f;
		float_t grad_len = 0.f;
		disp = new_disp;
		// calculate differentials
		for (int i = 0; i < LAYER_1_NEURONS; ++i)
		{
			for (int j = 0; j < IMG_SIZE * IMG_SIZE; ++j)
			{
				layer_1_weights[i][j] += GRAD_DELTA;
				layer_1_weights_dt[i][j] = GetDisp(dataset, offset, batch_scale) - disp;
				layer_1_weights_len += layer_1_weights_dt[i][j] * layer_1_weights_dt[i][j];
				grad_len += layer_1_weights_dt[i][j] * layer_1_weights_dt[i][j];
				layer_1_weights[i][j] -= GRAD_DELTA;
			}
			layer_1_biases[i] += GRAD_DELTA;
			layer_1_biases_dt[i] = GetDisp(dataset, offset, batch_scale) - disp;
			layer_1_biases_len += layer_1_biases_dt[i] * layer_1_biases_dt[i];
			grad_len += layer_1_biases_dt[i] * layer_1_biases_dt[i];
			layer_1_biases[i] -= GRAD_DELTA;
		}
		layer_1_weights_len = sqrt(layer_1_weights_len);
		layer_1_biases_len = sqrt(layer_1_biases_len);

		for (int i = 0; i < LAYER_2_NEURONS; ++i)
		{
			for (int j = 0; j < LAYER_1_NEURONS; ++j)
			{
				layer_2_weights[i][j] += GRAD_DELTA;
				layer_2_weights_dt[i][j] = GetDisp(dataset, offset, batch_scale) - disp;
				layer_2_weights_len += layer_2_weights_dt[i][j] * layer_2_weights_dt[i][j];
				grad_len += layer_2_weights_dt[i][j] * layer_2_weights_dt[i][j];
				layer_2_weights[i][j] -= GRAD_DELTA;
			}
			layer_2_biases[i] += GRAD_DELTA;
			layer_2_biases_dt[i] = GetDisp(dataset, offset, batch_scale) - disp;
			layer_2_biases_len += layer_2_biases_dt[i] * layer_2_biases_dt[i];
			grad_len += layer_2_biases_dt[i] * layer_2_biases_dt[i];
			layer_2_biases[i] -= GRAD_DELTA;
		}
		layer_2_weights_len = sqrt(layer_2_weights_len);
		layer_2_biases_len = sqrt(layer_2_biases_len);

		for (int i = 0; i < RESULT_NEURONS; ++i)
		{
			for (int j = 0; j < LAYER_2_NEURONS; ++j)
			{
				res_layer_weights[i][j] += GRAD_DELTA;
				res_layer_weights_dt[i][j] = GetDisp(dataset, offset, batch_scale) - disp;
				res_layer_weights_len += res_layer_weights_dt[i][j] * res_layer_weights_dt[i][j];
				grad_len += res_layer_weights_dt[i][j] * res_layer_weights_dt[i][j];
				res_layer_weights[i][j] -= GRAD_DELTA;
			}
			res_layer_biases[i] += GRAD_DELTA;
			res_layer_biases_dt[i] = GetDisp(dataset, offset, batch_scale) - disp;
			res_layer_biases_len += res_layer_biases_dt[i] * res_layer_biases_dt[i];
			grad_len += res_layer_biases_dt[i] * res_layer_biases_dt[i];
			res_layer_biases[i] -= GRAD_DELTA;
		}
		res_layer_weights_len = sqrt(res_layer_weights_len);
		res_layer_biases_len = sqrt(res_layer_biases_len);


		grad_len = sqrt(grad_len);

		if (grad_len > 0.000001f)
		{
			// subtract gradient
			for (int i = 0; i < LAYER_1_NEURONS; ++i)
			{
				for (int j = 0; j < IMG_SIZE * IMG_SIZE; ++j)
				{
					layer_1_weights[i][j] -= ((layer_1_weights_dt[i][j] / grad_len) * grad_step);
				}
				layer_1_biases[i] -= ((layer_1_biases_dt[i] / grad_len) * grad_step);
			}

			for (int i = 0; i < LAYER_2_NEURONS; ++i)
			{
				for (int j = 0; j < LAYER_1_NEURONS; ++j)
				{
					layer_2_weights[i][j] -= ((layer_2_weights_dt[i][j] / grad_len) * grad_step);
				}
				layer_2_biases[i] -= ((layer_2_biases_dt[i] / grad_len) * grad_step);
			}

			for (int i = 0; i < RESULT_NEURONS; ++i)
			{
				for (int j = 0; j < LAYER_2_NEURONS; ++j)
				{
					res_layer_weights[i][j] -= ((res_layer_weights_dt[i][j] / grad_len) * grad_step);
				}
				res_layer_biases[i] -= ((res_layer_biases_dt[i] / grad_len) * grad_step);
			}

			// calculate new original dispersion
			new_disp = GetDisp(dataset, offset, batch_scale);
			//	batch_scale *= 2;
			if (new_disp > disp)
			{
				grad_step = 1.f;
			}
			else
				if (grad_step > 1.f / 64.f)
					grad_step *= 2.f;
		}
		else
		{
			// move train offset
			offset += TRAIN_SAMPLE_SIZE * batch_scale;
			if (offset + TRAIN_SAMPLE_SIZE * batch_scale >= TRAIN_SIZE)
				break;
			new_disp = GetDisp(dataset, offset, batch_scale);
		}
	};

	return;
}

inline float_t SingleCoreDR::GetDisp(const Dataset& dataset, uint32_t offset, uint32_t batch_scale)
{
	//auto& train_labels = dataset.train_data_label;
	//auto& train_images = dataset.train_data_image;
	//std::array<float_t, LAYER_1_NEURONS> layer_1;
	//std::array<float_t, LAYER_2_NEURONS> layer_2;
	//std::array<float_t, RESULT_NEURONS> res_layer;
	//float_t disp_result = 0.f;
	//for (int t = offset; t < offset + TRAIN_SAMPLE_SIZE * batch_scale; ++t)
	//{
	//	for (int i = 0; i < LAYER_1_NEURONS; ++i)
	//		CalculateNeuron(layer_1[i], layer_1_biases[i], &train_images[t][0], &layer_1_weights[i][0], IMG_SIZE * IMG_SIZE);

	//	for (int i = 0; i < LAYER_2_NEURONS; ++i)
	//		CalculateNeuron(layer_2[i], layer_2_biases[i], &layer_1[0], &layer_2_weights[i][0], LAYER_1_NEURONS);

	//	for (int i = 0; i < RESULT_NEURONS; ++i)
	//		CalculateNeuron(res_layer[i], res_layer_biases[i], &layer_2[0], &res_layer_weights[i][0], LAYER_2_NEURONS);

	//	NormalizeResLayer(res_layer);


	//	disp_result += (1.f - res_layer[train_labels[t]]) * (1.f - res_layer[train_labels[t]]);
	//}
	//return sqrt(disp_result) / (float)(TRAIN_SAMPLE_SIZE * batch_scale);
	return 1.f;
}

inline void SingleCoreDR::NormalizeResLayer(std::array<float_t, RESULT_NEURONS>& res_layer)
{
	float_t len = 0.f;
	for (int i = 0; i < RESULT_NEURONS; ++i)
		len += res_layer[i] * res_layer[i];

	len = sqrt(len);

	for (int i = 0; i < RESULT_NEURONS; ++i)
		res_layer[i] /= len;
}

void SingleCoreDR::TestNN(const Dataset& dataset)
{
	//auto& test_labels = dataset.test_data_label;
	//auto& test_images = dataset.test_data_image;
	//std::array<float_t, LAYER_1_NEURONS> layer_1;
	//std::array<float_t, LAYER_2_NEURONS> layer_2;
	//std::array<float_t, RESULT_NEURONS> res_layer;
	//volatile int32_t true_result = 0;
	//for (int t = 0; t < test_labels.size(); ++t)
	//{
	//	for (int i = 0; i < LAYER_1_NEURONS; ++i)
	//		CalculateNeuron(layer_1[i], layer_1_biases[i], &test_images[t][0], &layer_1_weights[i][0], IMG_SIZE * IMG_SIZE);

	//	for (int i = 0; i < LAYER_2_NEURONS; ++i)
	//		CalculateNeuron(layer_2[i], layer_2_biases[i], &layer_1[0], &layer_2_weights[i][0], LAYER_1_NEURONS);

	//	for (int i = 0; i < RESULT_NEURONS; ++i)
	//		CalculateNeuron(res_layer[i], res_layer_biases[i], &layer_2[0], &res_layer_weights[i][0], LAYER_2_NEURONS);

	//	uint8_t res_label = 0;
	//	float_t max_weight = res_layer[0];
	//	for (int i = 1; i < RESULT_NEURONS; ++i)
	//	{
	//		if (res_layer[i] > max_weight)
	//		{
	//			res_label = i;
	//			max_weight = res_layer[i];
	//		}
	//	}
	//	if (test_labels[t] == res_label + 1)
	//		++true_result;
	//}
	//return;
}
