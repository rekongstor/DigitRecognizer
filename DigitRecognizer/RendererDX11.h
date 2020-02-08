#pragma once
#include "RendererBase.h"

class RendererDX11 : public RendererBase
{
	void OnInit(HWND__*);
	void OnUpdate();
	void OnStop();
public:
	virtual ~RendererDX11() {};
};