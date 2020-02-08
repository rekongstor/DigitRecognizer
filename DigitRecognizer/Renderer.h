#pragma once
class RendererBase;
struct HWND__;

class Renderer
{
	static Renderer renderer;
	static RendererBase* renderer_instance;

	Renderer();
	Renderer(const Renderer&) = delete;
public:
	static void InitRenderer(RendererBase* renderer_inst);
	static void OnInit(HWND__*);
	static void OnUpdate();
	static void OnStop();
};
