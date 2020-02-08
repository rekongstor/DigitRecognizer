#pragma once

#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include <random>

#include "Exceptions.h"
#include "Random.h"

#define IMG_SIZE 28
#define TRAIN_SIZE 60000
#define TEST_SIZE 10000

extern Random rnd;