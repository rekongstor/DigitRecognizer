#pragma once

#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <array>
#include <functional>
#include <queue>
#include <DirectXMath.h>

#include "Exceptions.h"
#include "Random.h"

#define IMG_SIZE 28
#define TEXTURE_SIZE 32
#define TRAIN_SIZE 60000
#define TEST_SIZE 10000
#define LAYER_1_NEURONS 16
#define LAYER_2_NEURONS 16
#define RESULT_NEURONS 10 // [0-9]
#define BATCH_SIZE 2000
#define DELTA 0.005f
#define REG_SCALE 1500.f
#define GRAD_STEP .00004f
#define GRAD_SCALE 16
#define SAMPLE_TIME 1

float batch_size_mas[];
float regularization_scale_mas[];
class IDigitRecognizer;
extern IDigitRecognizer* DR;