#include "Renderer.h"
#include "RendererBase.h"


Renderer::Renderer() 
{
}

void Renderer::InitRenderer(RendererBase* renderer_inst)
{
	if (renderer_instance)
		OnStop(); // в случае существования рендерера происходит его остановка
	Renderer::renderer_instance = renderer_inst;
}

void Renderer::OnInit(HWND__* hWnd)
{
	renderer_instance->OnInit(hWnd);
}

void Renderer::OnUpdate()
{
	renderer_instance->OnUpdate();
}

void Renderer::OnStop()
{
	renderer_instance->OnStop();
}


Renderer Renderer::renderer;
RendererBase* Renderer::renderer_instance = nullptr;

//void Renderer::OnInit(HWND__*) {}
//void Renderer::OnUpdate() {}
//void Renderer::OnStop() {}