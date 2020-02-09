#pragma once
struct HWND__;
class Renderer;

class RendererBase
{
	friend class Renderer;
	virtual void OnInit(HWND__*) = 0;
	virtual void OnUpdate() = 0;
	virtual void OnStop() = 0;
protected:
	virtual ~RendererBase() = 0 {};
};