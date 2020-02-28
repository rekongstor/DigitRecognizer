#pragma once

#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <array>

#include "Exceptions.h"
#include "Random.h"

#define IMG_SIZE 28
#define TEXTURE_SIZE 32
#define TRAIN_SIZE 60000
#define TEST_SIZE 10000
#define LAYER_1_NEURONS 16
#define LAYER_2_NEURONS 16
#define RESULT_NEURONS 10 // [0-9]
#define BATCH_SIZE 20
#define DELTA 0.001f

typedef std::vector<uint8_t> label_vector;
typedef std::vector<std::array<float_t,IMG_SIZE*IMG_SIZE>> image_vector;