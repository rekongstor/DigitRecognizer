#pragma once
#include "DigitRecognizer.h"


class SingleCoreDR : public IDigitRecognizer
{
	std::array<float_t, LAYER_1_NEURONS> layer_1_biases;
	std::array<std::array<float_t, IMG_SIZE * IMG_SIZE>, LAYER_1_NEURONS> layer_1_weights;

	// layer 2: 16 neurons
	std::array<float_t, LAYER_2_NEURONS> layer_2_biases;
	std::array< std::array<float_t,LAYER_1_NEURONS>, LAYER_2_NEURONS> layer_2_weights;

	// resulting layer: 10 neurons
	std::array<float_t, RESULT_NEURONS> res_layer_biases;
	std::array< std::array<float_t,LAYER_2_NEURONS>, RESULT_NEURONS> res_layer_weights;
	// layer 1: 16 neurons

	inline void CalculateNeuron(float_t& neuron, const float_t & bias, const float_t * const data, const float_t * const weight, int32_t size);
	inline float_t GetDisp(const Dataset& dataset);

	// Inherited via IDigitRecognizer
	virtual void InitNN() override;
	virtual void TrainNN(const Dataset& dataset) override;
	virtual void TestNN(const Dataset& dataset) override;
};