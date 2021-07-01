#pragma once
#include <d3d11.h>

class DeviceContext;
class SwapChain;
class ConstantBuffer;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;

class RenderSystem
{
	friend class SwapChain;
	friend class ConstantBuffer;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class VertexShader;
	friend class PixelShader;

public:
	RenderSystem();
	bool init();

	SwapChain* createSwapChain(HWND hwnd, UINT width, UINT height);
	VertexBuffer* createVertexBuffer(void* list_vertices, UINT size_vertices, UINT size_list, void* shader_byte_code, UINT size_byte_shader);
	IndexBuffer* createIndexBuffer(void* list_indices, UINT size_list);
	ConstantBuffer* createConstantBuffer(void* buffer, UINT size_buffer);

	VertexShader* createVertexShader(const void* shader_byte_code, size_t size_shader_byte_code);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t size_shader_byte_code);
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* size_shader_byte_code);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* size_shader_byte_code);
	void releaseCompiledShader();

	DeviceContext* getImmediateDeviceContext();

	bool release();
	~RenderSystem();

private:
	DeviceContext* m_imm_device_context;

	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;

	ID3DBlob* m_blob = nullptr;

	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;
};
