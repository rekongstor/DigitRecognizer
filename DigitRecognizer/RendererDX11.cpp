#include "RendererDX11.h"

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


struct Vertex
{
	DirectX::XMFLOAT4 Pos;
};


static IDXGISwapChain* s_pSwapchain							= nullptr;
static ID3D11Device*s_pDevice								= nullptr;
static ID3D11DeviceContext* s_pContext						= nullptr;
static ID3D11Texture2D* s_pBackbuffer						= nullptr;
static ID3D11RenderTargetView* s_pRenderTargetView			= nullptr;


void RendererDX11::OnInit(HWND__* hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;                                    // one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
	scd.OutputWindow = hWnd;                                // the window to be used
	scd.SampleDesc.Count = 1;                               // how many multisamples
	scd.SampleDesc.Quality = 0;                             // multisample quality level
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// create a device, device context and swap chain using the information in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&s_pSwapchain,
		&s_pDevice,
		NULL,
		&s_pContext);


	// get the address of the back buffer
	s_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&s_pBackbuffer);
	if (s_pBackbuffer == nullptr)
		throw (RendererDX11EX());

	s_pDevice->CreateRenderTargetView(s_pBackbuffer, NULL, &s_pRenderTargetView);
	// use the back buffer address to create the render target
	s_pBackbuffer->Release();

	// set the render target as the back buffer
	s_pContext->OMSetRenderTargets(1, &s_pRenderTargetView, NULL);


	// Set the viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	s_pContext->RSSetViewports(1, &viewport);
}

void RendererDX11::OnUpdate()
{
	// clear the back buffer to a deep blue
	float color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	s_pContext->ClearRenderTargetView(s_pRenderTargetView, color);

	// do 3D rendering on the back buffer here

	// switch the back buffer and the front buffer
	s_pSwapchain->Present(0, 0);
}

void RendererDX11::OnStop()
{
	s_pSwapchain->Release();
	s_pRenderTargetView->Release();
	s_pDevice->Release();
	s_pContext->Release();
}
