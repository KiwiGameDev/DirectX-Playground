#include <memory>

class DeviceContext;
class SwapChain;
class ConstantBuffer;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
class ResourceManager;
class TextureManager;
class SamplerState;
class Resource;
class Texture;
class Mesh;

typedef std::shared_ptr<DeviceContext> DeviceContextPtr;
typedef std::shared_ptr<SwapChain> SwapChainPtr;
typedef std::shared_ptr<ConstantBuffer> ConstantBufferPtr;
typedef std::shared_ptr<VertexBuffer> VertexBufferPtr;
typedef std::shared_ptr<IndexBuffer> IndexBufferPtr;
typedef std::shared_ptr<VertexShader> VertexShaderPtr;
typedef std::shared_ptr<PixelShader> PixelShaderPtr;
typedef std::shared_ptr<SamplerState> SamplerStatePtr;
typedef std::shared_ptr<Resource> ResourcePtr;
typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<Mesh> MeshPtr;
