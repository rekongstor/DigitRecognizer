#include "SingleCoreDR.h"
#include "Dataset.h"


inline void SingleCoreDR::CalculateNeuron(float_t& neuron, const float_t & bias, const float_t * const data, const float_t * const weight, int32_t size)
{
	neuron = 0.f;
	for (int i = 0; i < size; ++i)
		neuron += data[i] * weight[i];
	neuron -= bias;
	// calculate sigmoid
	neuron = neuron / (1 + abs(neuron));
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
}

void SingleCoreDR::TestNN(const Dataset& dataset)
{
	auto& test_labels = dataset.test_data_label;
	auto& test_images = dataset.test_data_image;
	std::array<float_t, LAYER_1_NEURONS> layer_1;
	std::array<float_t, LAYER_2_NEURONS> layer_2;
	std::array<float_t, RESULT_NEURONS> res_layer;
	int32_t true_result = 0;
	for (int t = 0; t < test_labels.size(); ++t)
	{
		auto p = (test_images[t]);
		for (int i = 0; i < LAYER_1_NEURONS; ++i)
			CalculateNeuron(layer_1[i], layer_1_biases[i], &test_images[t][0], &layer_1_weights[i][0], IMG_SIZE * IMG_SIZE);

		for (int i = 0; i < LAYER_2_NEURONS; ++i)
			CalculateNeuron(layer_2[i], layer_2_biases[i], &layer_1[0], &layer_2_weights[i][0], LAYER_1_NEURONS);

		for (int i = 0; i < RESULT_NEURONS; ++i)
			CalculateNeuron(res_layer[i], res_layer_biases[i], &layer_2[0], &res_layer_weights[i][0], LAYER_2_NEURONS);

		uint8_t res_label = 0;
		float_t max_weight = res_layer[0];
		for (int i = 1; i < RESULT_NEURONS; ++i)
		{
			if (res_layer[i] > max_weight)
			{
				res_label = i;
				max_weight = res_layer[i];
			}
		}
		if (test_labels[t] == res_label + 1)
			++true_result;
	}
	return;
}