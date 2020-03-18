#include "RendererDX11.h"
#include "DigitRecognizer.h"


#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment(lib,"D3dcompiler.lib")


struct Vertex
{
	DirectX::XMFLOAT4 Pos;
};


static IDXGISwapChain* s_pSwapchain							= nullptr;
static ID3D11Device*s_pDevice								= nullptr;
static ID3D11DeviceContext* s_pContext						= nullptr;
static ID3D11Texture2D* s_pBackbuffer						= nullptr;
static ID3D11RenderTargetView* s_pRenderTargetView			= nullptr;
static ID3D11VertexShader* s_pVS 							= nullptr;
static ID3D11PixelShader* s_pPS								= nullptr;
static ID3D11Buffer* s_pVBuffer								= nullptr;
static ID3D11InputLayout* s_pLayout							= nullptr;

Vertex OurVertices[SCREEN_WIDTH];
Vertex* ov;

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
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	s_pContext->RSSetViewports(1, &viewport);

	ID3DBlob* VS, * PS;
	D3DCompileFromFile(L"traindata_vs.hlsl", 0, 0, "main", "vs_4_0", 0, 0, &VS, 0);
	D3DCompileFromFile(L"traindata_ps.hlsl", 0, 0, "main", "ps_4_0", 0, 0, &PS, 0);
	s_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &s_pVS);
	s_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &s_pPS);
	s_pContext->VSSetShader(s_pVS, 0, 0);
	s_pContext->PSSetShader(s_pPS, 0, 0);


	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	s_pDevice->CreateInputLayout(ied, 1, VS->GetBufferPointer(), VS->GetBufferSize(), &s_pLayout);
	s_pContext->IASetInputLayout(s_pLayout);

	for (int i = 0; i < SCREEN_WIDTH; ++i)
	{
		OurVertices[i].Pos.x = -1.f + static_cast<float_t>(i) / static_cast<float_t>(SCREEN_WIDTH) * 2.f;
		OurVertices[i].Pos.y = -1.f;
		OurVertices[i].Pos.z = 0.f;
		OurVertices[i].Pos.w = 1.f;
	}


	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(Vertex) * SCREEN_WIDTH;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	//bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
	s_pDevice->CreateBuffer(&bd, NULL, &s_pVBuffer);
	D3D11_MAPPED_SUBRESOURCE ms;
	s_pContext->Map(s_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer

	ov = static_cast<Vertex*>(ms.pData);
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
	s_pContext->Unmap(s_pVBuffer, NULL);                                      // unmap the buffer

}

void RendererDX11::OnUpdate()
{
	static int32_t current_x = 0;
	static float_t max_y = 1.f;
	// clear the back buffer to a deep blue
	float color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	s_pContext->ClearRenderTargetView(s_pRenderTargetView, color);

	D3D11_MAPPED_SUBRESOURCE ms;
	s_pContext->Map(s_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer

	float_t value = DR->TestNN();
	DR->TrainNN();
	OurVertices[current_x].Pos.y = (value * 2.f - 1.f); //sin(static_cast<float_t>(current_x) / static_cast<float_t>(SCREEN_WIDTH) * 6.28f) / 2.f + .5f;

	++current_x;
	if (current_x == SCREEN_WIDTH)
		current_x = 0;
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));                // copy the data
	s_pContext->Unmap(s_pVBuffer, NULL);                                     // unmap the buffer
	// do 3D rendering on the back buffer here
			// select which vertex buffer to display
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	s_pContext->IASetVertexBuffers(0, 1, &s_pVBuffer, &stride, &offset);

	// select which primtive type we are using
	s_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	// draw the vertex buffer to the back buffer
	s_pContext->Draw(SCREEN_WIDTH, 0);
	// switch the back buffer and the front buffer
	s_pSwapchain->Present(0, 0);
}

void RendererDX11::OnStop()
{
	s_pVS->Release();
	s_pPS->Release();
	s_pSwapchain->Release();
	s_pRenderTargetView->Release();
	s_pDevice->Release();
	s_pContext->Release();
}
