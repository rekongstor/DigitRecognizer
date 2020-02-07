#pragma once
#include "DigitRecognizer.h"

class SingleCoreDG : public DigitRecognizer
{
public:
	virtual ~SingleCoreDG();
};

typedef std::shared_ptr<SingleCoreDG> SingleCoreDG_sp;
typedef std::unique_ptr<SingleCoreDG> SingleCoreDG_up;