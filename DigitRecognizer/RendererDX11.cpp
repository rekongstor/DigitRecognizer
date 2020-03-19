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

struct TexVertex
{
	DirectX::XMFLOAT4 Pos;
	DirectX::XMFLOAT2 Tex;
};


static IDXGISwapChain* s_pSwapchain = nullptr;
static ID3D11Device* s_pDevice = nullptr;
static ID3D11DeviceContext* s_pContext = nullptr;
static ID3D11Texture2D* s_pBackbuffer = nullptr;
static ID3D11RenderTargetView* s_pRenderTargetView = nullptr;
static ID3D11VertexShader* s_pVS = nullptr;
static ID3D11PixelShader* s_pPS = nullptr;
static ID3D11Buffer* s_pVBuffer = nullptr;
static ID3D11InputLayout* s_pLayout = nullptr;


static ID3D11VertexShader* s_pVSTex = nullptr;
static ID3D11PixelShader* s_pPSTex = nullptr;
static ID3D11Buffer* s_pVBufferTex = nullptr;
static ID3D11InputLayout* s_pLayoutTex = nullptr;
static ID3D11SamplerState* s_pSampler = nullptr;
static ID3D11ShaderResourceView* s_pTextureRV = nullptr;
static ID3D11Texture2D* s_pTexResource = nullptr;
static ID3D11Texture2D* s_pTexResourceStaging = nullptr;

static Vertex OurVertices[SCREEN_WIDTH];
static HWND hW;

void RendererDX11::OnInit(HWND__* hWnd)
{
	// create a struct to hold information about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	hW = hWnd;
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


	ID3DBlob* VSTex, * PSTex;
	D3DCompileFromFile(L"traindata_lin_vs.hlsl", 0, 0, "main", "vs_4_0", 0, 0, &VSTex, 0);
	D3DCompileFromFile(L"traindata_lin_ps.hlsl", 0, 0, "main", "ps_4_0", 0, 0, &PSTex, 0);
	s_pDevice->CreateVertexShader(VSTex->GetBufferPointer(), VSTex->GetBufferSize(), NULL, &s_pVSTex);
	s_pDevice->CreatePixelShader(PSTex->GetBufferPointer(), PSTex->GetBufferSize(), NULL, &s_pPSTex);
	s_pContext->VSSetShader(s_pVSTex, 0, 0);
	s_pContext->PSSetShader(s_pPSTex, 0, 0);
	D3D11_INPUT_ELEMENT_DESC iedTex[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	s_pDevice->CreateInputLayout(iedTex, ARRAYSIZE(iedTex), VSTex->GetBufferPointer(), VSTex->GetBufferSize(), &s_pLayoutTex);

	VS->Release();
	PS->Release();
	VSTex->Release();
	PSTex->Release();
	float_t x_coo = 0.8f;
	{
		TexVertex square[] =
		{
			{DirectX::XMFLOAT4(+1.0f, +1.0f, +0.0f, 1.f) , DirectX::XMFLOAT2(1.0f, 0.0f)},
			{DirectX::XMFLOAT4(+1.0f, -1.0f, +0.0f, 1.f) , DirectX::XMFLOAT2(1.0f, 1.0f)},
			{DirectX::XMFLOAT4(x_coo, -1.0f, +0.0f, 1.f) , DirectX::XMFLOAT2(0.0f, 1.0f)},
			{DirectX::XMFLOAT4(x_coo, -1.0f, +0.0f, 1.f) , DirectX::XMFLOAT2(0.0f, 1.0f)},
			{DirectX::XMFLOAT4(x_coo, +1.0f, +0.0f, 1.f) , DirectX::XMFLOAT2(0.0f, 0.0f)},
			{DirectX::XMFLOAT4(+1.0f, +1.0f, +0.0f, 1.f) , DirectX::XMFLOAT2(1.0f, 0.0f)}
		};



		D3D11_BUFFER_DESC bdTex;
		ZeroMemory(&bdTex, sizeof(bdTex));
		bdTex.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
		bdTex.ByteWidth = sizeof(square);             // size is the VERTEX struct * 3
		bdTex.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
		bdTex.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer
		s_pDevice->CreateBuffer(&bdTex, NULL, &s_pVBufferTex);


		D3D11_MAPPED_SUBRESOURCE ms;
		s_pContext->Map(s_pVBufferTex, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer
		memcpy(ms.pData, square, sizeof(square));                // copy the data
		s_pContext->Unmap(s_pVBufferTex, NULL);                                     // unmap the buffer


		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; //D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		s_pDevice->CreateSamplerState(&samplerDesc, &s_pSampler);



		D3D11_TEXTURE2D_DESC readTexDesc;
		ZeroMemory(&readTexDesc, sizeof(readTexDesc));
		readTexDesc.Format = DXGI_FORMAT_R32_FLOAT;
		readTexDesc.Width = 32;
		readTexDesc.Height = 320;
		readTexDesc.MipLevels = 1;
		readTexDesc.ArraySize = 1;
		readTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		readTexDesc.SampleDesc.Count = 1;
		readTexDesc.SampleDesc.Quality = 0;
		readTexDesc.Usage = D3D11_USAGE_DYNAMIC; //need staging flag for read
		readTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		readTexDesc.MiscFlags = 0;

		s_pDevice->CreateTexture2D(&readTexDesc, NULL, &s_pTexResourceStaging);

		D3D11_SHADER_RESOURCE_VIEW_DESC trv_desc;
		trv_desc.Format = DXGI_FORMAT_R32_FLOAT;
		trv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		trv_desc.Texture2D.MostDetailedMip = 0;
		trv_desc.Texture2D.MipLevels = 1;
		s_pDevice->CreateShaderResourceView(s_pTexResourceStaging, &trv_desc, &s_pTextureRV);

	}

	{
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

		s_pDevice->CreateBuffer(&bd, NULL, &s_pVBuffer);
	}


}

void RendererDX11::OnUpdate()
{
	static int32_t current_x = 0;
	static float_t max_y = 1.f;
	++current_x;
	// clear the back buffer to a deep blue
	float color[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	s_pContext->ClearRenderTargetView(s_pRenderTargetView, color);

	{
		UINT stride = sizeof(TexVertex);
		UINT offset = 0;
		s_pContext->IASetVertexBuffers(0, 1, &s_pVBufferTex, &stride, &offset);
		s_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		s_pContext->IASetInputLayout(s_pLayoutTex);
		s_pContext->VSSetShader(s_pVSTex, 0, 0);
		s_pContext->PSSetShader(s_pPSTex, 0, 0);
		s_pContext->PSSetShaderResources(0, 1, &s_pTextureRV);
		s_pContext->PSSetSamplers(0, 1, &s_pSampler);

		float_t* tex = DR->GetWeights();
		std::vector<float_t> copy;
		float_t min = tex[0];
		float_t max = tex[0];

		for (size_t i = 0; i < 10 * 1024; ++i)
		{
			copy.push_back(tex[i]);
			if (tex[i] > max)
				max = tex[i];
			if (tex[i] < min)
				min = tex[i];
		}
		for (auto& m : copy)
			m = (m + min) / (max - min) * 1.f;

		D3D11_MAPPED_SUBRESOURCE msT;
		s_pContext->Map(s_pTexResourceStaging, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &msT);   // map the buffer
		memcpy(msT.pData, copy.data(), copy.size() * sizeof(float_t));                // copy the data
		s_pContext->Unmap(s_pTexResourceStaging, NULL);                                     // unmap the buffer
		s_pContext->Draw(6, 0);
	}
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		s_pContext->IASetVertexBuffers(0, 1, &s_pVBuffer, &stride, &offset);
		s_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		s_pContext->IASetInputLayout(s_pLayout);
		s_pContext->VSSetShader(s_pVS, 0, 0);
		s_pContext->PSSetShader(s_pPS, 0, 0);
		D3D11_MAPPED_SUBRESOURCE ms;
		s_pContext->Map(s_pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);   // map the buffer

		float_t value = DR->TestNN();
		std::string winName("DigitRecognizer: " + std::to_string(value));
		SetWindowTextA(hW, winName.c_str());
		DR->TrainNN();
		OurVertices[current_x].Pos.y = (value * 2.f - 1.f); //sin(static_cast<float_t>(current_x) / static_cast<float_t>(SCREEN_WIDTH) * 6.28f) / 2.f + .5f;

		if (current_x >= SCREEN_WIDTH - 100)
			current_x = 0;
		memcpy(ms.pData, OurVertices, sizeof(OurVertices));                // copy the data
		s_pContext->Unmap(s_pVBuffer, NULL);                                     // unmap the buffer
		s_pContext->Draw(current_x, 0);
	}
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
