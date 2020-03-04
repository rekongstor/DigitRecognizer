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
#define REG_SCALE 350.f
#define GRAD_STEP .000005f
#define GRAD_SCALE 64
#define SAMPLE_TIME 2

float batch_size_mas[];
float regularization_scale_mas[];
