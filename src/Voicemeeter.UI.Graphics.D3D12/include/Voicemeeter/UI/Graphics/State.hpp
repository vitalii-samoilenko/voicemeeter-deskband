#ifndef VOICEMEETER_UI_GRAPHICS_STATE_HPP
#define VOICEMEETER_UI_GRAPHICS_STATE_HPP

#include <array>

#include "memory.hpp"
#include "wheel.hpp"

#include "Windows/API.hpp"
#include "Windows/COM.hpp"
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dcomp.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "Voicemeeter/UI/Layouts/Atlas.hpp"
#include "Voicemeeter/UI/Layouts/Loader.hpp"

namespace Voicemeeter {
	namespace UI {
		namespace Graphics {
			template<typename TLoader>
			class State final {
			public:
				inline State(
					HWND hWnd,
					TLoader &loader)
					: _hWnd{ hWnd }
					, _d3dDevice{ nullptr }
					, _commandQueue{ nullptr }
					, _commandAllocator{ nullptr }
					, _commandList{ nullptr }
					, _rootSignature{ nullptr }
					, _defaultState{ nullptr }
					, _blendState{ nullptr }
					, _hTextureHeap{ nullptr }
					, _texture{ nullptr }
					, _hSquareBuffer{}
					, _squareBuffer{ nullptr }
					, _fence{ nullptr }
					, _hEvent{ NULL }
					, _count{ 0 }
					, _swapChain{ nullptr }
					, _hRenderTargetHeap{ nullptr }
					, _hRenderTargets{}
					, _renderTargets{ nullptr, nullptr }
					, _commandAllocators{ nullptr, nullptr }
					, _commandLists{ nullptr, nullptr }
					, _fences{ nullptr, nullptr }
					, _hEvents{ NULL, NULL }
					, _counts{ 0, 0 }
					, _compositionTarget{ nullptr } {
					bool failed{ true };
					auto guardEvents = ::estd::make_guard([
							&failed,
							&hEvents = _hEvents,
							&hEvent = _hEvent
						]()->void {
							if (!failed) {
								return;
							}
							for (HANDLE hEvent : hEvents) {
								::CloseHandle(hEvent);
							}
							::CloseHandle(hEvent);
						});
					::Windows::ThrowIfFailed(::CoInitialize(
						NULL
					), "COM initialization failed");
					{
						UINT dxgiFactoryFlags{ 0 };
#ifndef NDEBUG
						::Microsoft::WRL::ComPtr<ID3D12Debug> d3dDebug{ nullptr };
						::Windows::ThrowIfFailed(::D3D12GetDebugInterface(
							IID_PPV_ARGS(&d3dDebug)
						), "Could not get debug interface");
						d3dDebug->EnableDebugLayer();
						dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
						::Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory{ nullptr };
						::Windows::ThrowIfFailed(::CreateDXGIFactory2(
							dxgiFactoryFlags,
							IID_PPV_ARGS(&dxgiFactory)
						), "DXGI factory creation failed");
						::Windows::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(
							_hWnd,
							DXGI_MWA_NO_ALT_ENTER
						), "Failed to disable fullscreen transition");
						::Microsoft::WRL::ComPtr<IDXGIAdapter4> dxgiAdapter{ nullptr };
						::Windows::ThrowIfFailed(dxgiFactory->EnumAdapterByGpuPreference(
							0,
							DXGI_GPU_PREFERENCE_UNSPECIFIED,
							IID_PPV_ARGS(&dxgiAdapter)
						), "Could not get DXGI adapter");
						::Windows::ThrowIfFailed(::D3D12CreateDevice(
							dxgiAdapter.Get(),
							D3D_FEATURE_LEVEL_12_1,
							IID_PPV_ARGS(&_d3dDevice)
						), "D3D12 device creation failed");
						D3D12_COMMAND_QUEUE_DESC queueDesc{
							D3D12_COMMAND_LIST_TYPE_DIRECT,
							D3D12_COMMAND_QUEUE_PRIORITY_HIGH,
							D3D12_COMMAND_QUEUE_FLAG_NONE,
							0
						};
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommandQueue(
							&queueDesc,
							IID_PPV_ARGS(&_commandQueue)
						), "Command queue creation failed");
						DXGI_SWAP_CHAIN_DESC1 swapChainDesc{
							8, 8,
							DXGI_FORMAT_R8G8B8A8_UNORM,
							FALSE,
							DXGI_SAMPLE_DESC{
								1, 0
							},
							DXGI_USAGE_RENDER_TARGET_OUTPUT, FrameCount,
							DXGI_SCALING_STRETCH,
							DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL,
							DXGI_ALPHA_MODE_PREMULTIPLIED,
							0
						};
						::Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain{ nullptr };
						::Windows::ThrowIfFailed(dxgiFactory->CreateSwapChainForComposition(
							_commandQueue.Get(),
							&swapChainDesc,
							nullptr,
							&swapChain
						), "DXGI swap chain creation failed");
						::Windows::ThrowIfFailed(swapChain.As(
							&_swapChain
						), "Could not get swap chain");
					}
					{
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommandAllocator(
							D3D12_COMMAND_LIST_TYPE_DIRECT,
							IID_PPV_ARGS(&_commandAllocator)
						), "Command allocator creation failed");
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommandList(
							0, D3D12_COMMAND_LIST_TYPE_DIRECT,
							_commandAllocator.Get(), nullptr,
							IID_PPV_ARGS(&_commandList)
						), "Command list creation failed");
						::Windows::ThrowIfFailed(_d3dDevice->CreateFence(
							_count,
							D3D12_FENCE_FLAG_NONE,
							IID_PPV_ARGS(&_fence)
						), "Fence creation failed");
						_hEvent = ::Windows::CreateEventW(NULL, FALSE, FALSE, NULL);
					}
					::Microsoft::WRL::ComPtr<ID3D12Resource> squareUploadBuffer{ nullptr };
					{
						// TODO: vertex identifier for texture alignment
						::std::array<FLOAT, 2 * 4> square{
							-1.F, -1.F,
							-1.F, 1.F,
							1.F, -1.F,
							1.F, 1.F
						};
						D3D12_HEAP_PROPERTIES squareHeapProps{
							D3D12_HEAP_TYPE_DEFAULT,
							D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
							D3D12_MEMORY_POOL_UNKNOWN,
							0, 0
						};
						D3D12_RESOURCE_DESC squareDesc{
							D3D12_RESOURCE_DIMENSION_BUFFER,
							0, sizeof(square), 1, 1, 1,
							DXGI_FORMAT_UNKNOWN,
							DXGI_SAMPLE_DESC{
								1, 0
							},
							D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
							D3D12_RESOURCE_FLAG_NONE
						};
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommittedResource(
							&squareHeapProps, D3D12_HEAP_FLAG_NONE, &squareDesc,
							D3D12_RESOURCE_STATE_COPY_DEST,
							nullptr,
							IID_PPV_ARGS(&_squareBuffer)
						), "Vertex buffer creation failed");
						squareHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommittedResource(
							&squareHeapProps, D3D12_HEAP_FLAG_NONE, &squareDesc,
							D3D12_RESOURCE_STATE_GENERIC_READ,
							nullptr,
							IID_PPV_ARGS(&squareUploadBuffer)
						), "Upload buffer creation failed");
						D3D12_RANGE range{
							0, 0
						};
						void *data{ nullptr };
						::Windows::ThrowIfFailed(squareUploadBuffer->Map(
							0, &range,
							&data
						), "Upload buffer map failed");
						::memcpy(data, square.data(), sizeof(square));
						squareUploadBuffer->Unmap(0, nullptr);
						_commandList->CopyResource(_squareBuffer.Get(), squareUploadBuffer.Get());
						D3D12_RESOURCE_BARRIER barrier{
							D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
							D3D12_RESOURCE_BARRIER_FLAG_NONE,
							D3D12_RESOURCE_TRANSITION_BARRIER{
								_squareBuffer.Get(),
								D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
								D3D12_RESOURCE_STATE_COPY_DEST,
								D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER
							}
						};
						_commandList->ResourceBarrier(1, &barrier);
						_hSquareBuffer.BufferLocation = _squareBuffer->GetGPUVirtualAddress();
						_hSquareBuffer.StrideInBytes = 2 * sizeof(FLOAT);
						_hSquareBuffer.SizeInBytes = sizeof(square);
					}
					::Microsoft::WRL::ComPtr<ID3D12Resource> textureUploadBuffer{ nullptr };
					{
						typename TLoader::resource atlas{
							loader.LoadResource(
								Layouts::Loader::bit::Atlas)
						};
						D3D12_HEAP_PROPERTIES textureHeapProps{
							D3D12_HEAP_TYPE_DEFAULT,
							D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
							D3D12_MEMORY_POOL_UNKNOWN,
							0, 0
						};
						D3D12_RESOURCE_DESC textureDesc{
							D3D12_RESOURCE_DIMENSION_TEXTURE2D,
							0,
							pop(Layouts::Atlas::Width), pop(Layouts::Atlas::Height), 1, 1,
							DXGI_FORMAT_R32_FLOAT,
							DXGI_SAMPLE_DESC{
								1, 0
							},
							D3D12_TEXTURE_LAYOUT_UNKNOWN,
							D3D12_RESOURCE_FLAG_NONE
						};
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommittedResource(
								&textureHeapProps, D3D12_HEAP_FLAG_NONE, &textureDesc,
								D3D12_RESOURCE_STATE_COPY_DEST,
								nullptr,
								IID_PPV_ARGS(&_texture)
						), "Texture creation failed");
						UINT rowSize{ pop(Layouts::Atlas::Width) * 4 };
						UINT srcRowPitch{ rowSize };
						UINT remainder{ rowSize % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT };
						UINT dstRowPitch{
							remainder
								? rowSize - remainder + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
								: rowSize
						};
						textureHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
						textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
						textureDesc.Width = dstRowPitch * pop(Layouts::Atlas::Height);
						textureDesc.Height = 1;
						textureDesc.Format = DXGI_FORMAT_UNKNOWN;
						textureDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
						::Windows::ThrowIfFailed(_d3dDevice->CreateCommittedResource(
							&textureHeapProps, D3D12_HEAP_FLAG_NONE, &textureDesc,
							D3D12_RESOURCE_STATE_GENERIC_READ,
							nullptr,
							IID_PPV_ARGS(&textureUploadBuffer)
						), "Upload buffer creation failed");
						D3D12_RANGE range{
							0, 0
						};
						BYTE *dst{ nullptr };
						::Windows::ThrowIfFailed(textureUploadBuffer->Map(
							0, &range,
							reinterpret_cast<void **>(&dst)
						), "Upload buffer map failed");
						for (UINT row{ 0 }; row < pop(Layouts::Atlas::Height); ++row) {
							memcpy(dst + dstRowPitch * row,
								reinterpret_cast<BYTE *>(atlas.data()) + srcRowPitch * row,
								rowSize);
						}
						textureUploadBuffer->Unmap(0, nullptr);
						D3D12_TEXTURE_COPY_LOCATION srcLoc{
							textureUploadBuffer.Get(),
							D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT,
							D3D12_PLACED_SUBRESOURCE_FOOTPRINT{
								0,
								D3D12_SUBRESOURCE_FOOTPRINT{
									DXGI_FORMAT_R32_FLOAT,
									pop(Layouts::Atlas::Width), pop(Layouts::Atlas::Height), 1,
									dstRowPitch
								}
							}
						};
						D3D12_TEXTURE_COPY_LOCATION dstLoc{
							_texture.Get(),
							D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX
						};
						dstLoc.SubresourceIndex = 0;
						_commandList->CopyTextureRegion(
							&dstLoc,
							0U, 0U, 0U,
							&srcLoc,
							nullptr);
						D3D12_RESOURCE_BARRIER barrier{
							D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
							D3D12_RESOURCE_BARRIER_FLAG_NONE,
							D3D12_RESOURCE_TRANSITION_BARRIER{
								_texture.Get(),
								D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
								D3D12_RESOURCE_STATE_COPY_DEST,
								D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
							}
						};
						_commandList->ResourceBarrier(1U, &barrier);
						D3D12_DESCRIPTOR_HEAP_DESC hTextureHeapDesc{
							D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
							1,
							D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
							0
						};
						::Windows::ThrowIfFailed(_d3dDevice->CreateDescriptorHeap(
							&hTextureHeapDesc,
							IID_PPV_ARGS(&_hTextureHeap)
						), "SRV heap descriptor creation failed");
						D3D12_SHADER_RESOURCE_VIEW_DESC hTextureDesc{
							DXGI_FORMAT_R32_FLOAT,
							D3D12_SRV_DIMENSION_TEXTURE2D,
							D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
						};
						hTextureDesc.Texture2D.MipLevels = 1U;
						_d3dDevice->CreateShaderResourceView(
							_texture.Get(),
							&hTextureDesc,
							_hTextureHeap->GetCPUDescriptorHandleForHeapStart());
					}
					{
						::Windows::ThrowIfFailed(_commandList->Close(
						), "Command list close failed");
						ID3D12CommandList *commandList{ _commandList.Get() };
						_commandQueue->ExecuteCommandLists(1, &commandList);
						_commandQueue->Signal(_fence.Get(), ++_count);
					}
					{
						D3D12_DESCRIPTOR_HEAP_DESC hRenderTargetHeapDesc{
							D3D12_DESCRIPTOR_HEAP_TYPE_RTV, FrameCount,
							D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
							0
						};
						::Windows::ThrowIfFailed(_d3dDevice->CreateDescriptorHeap(
							&hRenderTargetHeapDesc,
							IID_PPV_ARGS(&_hRenderTargetHeap)
						), "RTV heap descriptor creation failed");
						UINT hRenderTargetSize{
							_d3dDevice->GetDescriptorHandleIncrementSize(
								D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
						};
						D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{
							_hRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
						};
						for (size_t frame{ 0 }; frame < FrameCount; ++frame) {
							_hRenderTargets[frame].ptr = SIZE_T(INT64(hRenderTarget.ptr)
								+ INT64(frame * hRenderTargetSize));
							::Windows::ThrowIfFailed(_swapChain->GetBuffer(
								static_cast<UINT>(frame),
								IID_PPV_ARGS(&_renderTargets[frame])
							), "Failed to get swap chain buffer");
							_d3dDevice->CreateRenderTargetView(
								_renderTargets[frame].Get(),
								nullptr, _hRenderTargets[frame]);
							::Windows::ThrowIfFailed(_d3dDevice->CreateCommandAllocator(
								D3D12_COMMAND_LIST_TYPE_DIRECT,
								IID_PPV_ARGS(&_commandAllocators[frame])
							), "Command allocator creation failed");
							::Windows::ThrowIfFailed(_d3dDevice->CreateCommandList1(
								0, D3D12_COMMAND_LIST_TYPE_DIRECT,
								D3D12_COMMAND_LIST_FLAG_NONE,
								IID_PPV_ARGS(&_commandLists[frame])
							), "Command list creation failed");
							::Windows::ThrowIfFailed(_d3dDevice->CreateFence(
								_counts[frame],
								D3D12_FENCE_FLAG_NONE,
								IID_PPV_ARGS(&_fences[frame])
							), "Fence creation failed");
							_hEvents[frame] = ::Windows::CreateEventW(NULL, FALSE, FALSE, NULL);
						}
					}
					{
						D3D12_DESCRIPTOR_RANGE hTextureRange{
							D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
							1,
							0, 0,
							0
						};
						::std::array<D3D12_ROOT_PARAMETER, 2> params{};
						params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
						params[0].DescriptorTable.NumDescriptorRanges = 1;
						params[0].DescriptorTable.pDescriptorRanges = &hTextureRange;
						params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
						params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
						params[1].Constants.ShaderRegister = 0;
						params[1].Constants.Num32BitValues = 8;
						params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
						D3D12_STATIC_SAMPLER_DESC samplerDesc{
							D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT,
							D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_TEXTURE_ADDRESS_MODE_BORDER, D3D12_TEXTURE_ADDRESS_MODE_BORDER,
							0.F, 0, D3D12_COMPARISON_FUNC_NEVER,
							D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK,
							0.F, D3D12_FLOAT32_MAX,
							0, 0,
							D3D12_SHADER_VISIBILITY_PIXEL
						};
						D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{
							static_cast<UINT>(params.size()), params.data(),
							1, &samplerDesc,
							D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT
						};
						::Microsoft::WRL::ComPtr<ID3DBlob> temp{ nullptr };
						::Windows::ThrowIfFailed(::D3D12SerializeRootSignature(
							&rootSignatureDesc,
							D3D_ROOT_SIGNATURE_VERSION_1,
							&temp, nullptr
						), "Root signature serialization failed");
						::Windows::ThrowIfFailed(_d3dDevice->CreateRootSignature(
							0,
							temp->GetBufferPointer(), temp->GetBufferSize(),
							IID_PPV_ARGS(&_rootSignature)
						), "Root signature creation failed");
					}
					{
						::Microsoft::WRL::ComPtr<ID3DBlob> vertexShader{ nullptr };
						::Microsoft::WRL::ComPtr<ID3DBlob> pixelShader{ nullptr };
						typename TLoader::resource vertexCode{
							loader.LoadResource(
								Layouts::Loader::hlsl::Vertex)
						};
						typename TLoader::resource pixelCode{
							loader.LoadResource(
								Layouts::Loader::hlsl::Pixel)
						};
						UINT compileFlags{ 0 };
#ifndef NDEBUG
						compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
						::Windows::ThrowIfFailed(::D3DCompile(
							vertexCode.data(), vertexCode.size(),
							NULL,
							nullptr, nullptr,
							"Main", "vs_5_0",
							compileFlags, 0,
							&vertexShader, nullptr
						), "Vertex shader compilation failed");
						::Windows::ThrowIfFailed(::D3DCompile(
							pixelCode.data(), pixelCode.size(),
							NULL,
							nullptr, nullptr,
							"Main", "ps_5_0",
							compileFlags, 0,
							&pixelShader, nullptr
						), "Pixel shader compilation failed");
						D3D12_INPUT_ELEMENT_DESC positionDesc{
							"POSITION", 0,
							DXGI_FORMAT_R32G32_FLOAT,
							0, 0,
							D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
							0
						};
						D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{
							_rootSignature.Get(),
							D3D12_SHADER_BYTECODE{
								vertexShader->GetBufferPointer(),
								vertexShader->GetBufferSize()
							},
							D3D12_SHADER_BYTECODE{
								pixelShader->GetBufferPointer(),
								pixelShader->GetBufferSize()
							},
							D3D12_SHADER_BYTECODE{}, D3D12_SHADER_BYTECODE{}, D3D12_SHADER_BYTECODE{},
							D3D12_STREAM_OUTPUT_DESC{},
							D3D12_BLEND_DESC{
								FALSE, FALSE,
								{
									D3D12_RENDER_TARGET_BLEND_DESC{
										TRUE, FALSE,
										D3D12_BLEND_SRC_ALPHA, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
										D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
										D3D12_LOGIC_OP_NOOP,
										D3D12_COLOR_WRITE_ENABLE_ALL
									}
								}
							},
							UINT_MAX,
							D3D12_RASTERIZER_DESC{
								D3D12_FILL_MODE_SOLID, D3D12_CULL_MODE_NONE,
								FALSE,
								D3D12_DEFAULT_DEPTH_BIAS, D3D12_DEFAULT_DEPTH_BIAS_CLAMP, D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS,
								TRUE,
								FALSE, FALSE, 0,
								D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
							},
							D3D12_DEPTH_STENCIL_DESC{
								FALSE, D3D12_DEPTH_WRITE_MASK_ZERO, D3D12_COMPARISON_FUNC_NEVER,
								FALSE, 0, 0,
								D3D12_DEPTH_STENCILOP_DESC{}, D3D12_DEPTH_STENCILOP_DESC{}
							},
							D3D12_INPUT_LAYOUT_DESC{
								&positionDesc, 1
							},
							D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED,
							D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,
							1, { DXGI_FORMAT_R8G8B8A8_UNORM },
							DXGI_FORMAT_UNKNOWN,
							DXGI_SAMPLE_DESC{
								1, 0
							},
							0,
							D3D12_CACHED_PIPELINE_STATE{},
							D3D12_PIPELINE_STATE_FLAG_NONE
						};
						::Windows::ThrowIfFailed(_d3dDevice->CreateGraphicsPipelineState(
							&pipelineStateDesc,
							IID_PPV_ARGS(&_defaultState)
						), "Pipeline state creation failed");
						pipelineStateDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
						pipelineStateDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_DEST_ALPHA;
						pipelineStateDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
						::Windows::ThrowIfFailed(_d3dDevice->CreateGraphicsPipelineState(
							&pipelineStateDesc,
							IID_PPV_ARGS(&_blendState)
						), "Pipeline state creation failed");
					}
					{
						::Microsoft::WRL::ComPtr<IDCompositionDevice> compositionDevice{ nullptr };
						::Windows::ThrowIfFailed(::DCompositionCreateDevice(
							nullptr,
							IID_PPV_ARGS(&compositionDevice)
						), "Composition device creation failed");
						::Windows::ThrowIfFailed(compositionDevice->CreateTargetForHwnd(
							_hWnd, TRUE,
							&_compositionTarget
						), "Composition target creation failed");
						::Microsoft::WRL::ComPtr<IDCompositionVisual> compositionVisual{ nullptr };
						::Windows::ThrowIfFailed(compositionDevice->CreateVisual(
							&compositionVisual
						), "Composition visual creation failed");
						::Windows::ThrowIfFailed(compositionVisual->SetContent(
							_swapChain.Get()
						), "Could not set swap chain content");
						::Windows::ThrowIfFailed(_compositionTarget->SetRoot(
							compositionVisual.Get()
						), "Could not set composition target root");
						::Windows::ThrowIfFailed(compositionDevice->Commit(
						), "Could not commit composition device");
					}
					if (_fence->GetCompletedValue() < _count) {
						::Windows::ThrowIfFailed(_fence->SetEventOnCompletion(
							_count, _hEvent
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_hEvent, INFINITE);
					}
					failed = false;
				};
				State() = delete;
				State(State const &) = delete;
				State(State &&) = delete;

				inline ~State() {
					for (HANDLE hEvent : _hEvents) {
						::CloseHandle(hEvent);
					}
					::CloseHandle(_hEvent);
				};

				State & operator=(State const &) = delete;
				State & operator=(State &&) = delete;

				static constexpr size_t FrameCount{ 2 };

				inline HWND get_hWnd() const {
					return _hWnd;
				};
				inline ID3D12Device8 * get_D3dDevice() const {
					return _d3dDevice.Get();
				};
				inline ID3D12CommandQueue * get_CommandQueue() const {
					return _commandQueue.Get();
				};
				inline ID3D12CommandAllocator * get_CommandAllocator() const {
					return _commandAllocator.Get();
				};
				inline ID3D12GraphicsCommandList * get_CommandList() const {
					return _commandList.Get();
				};
				inline ID3D12RootSignature * get_RootSignature() const {
					return _rootSignature.Get();
				};
				inline ID3D12PipelineState * get_DefaultState() const {
					return _defaultState.Get();
				};
				inline ID3D12PipelineState * get_BlendState() const {
					return _blendState.Get();
				};
				inline ID3D12DescriptorHeap * get_hTextureHeap() const {
					return _hTextureHeap.Get();
				};
				inline ID3D12Resource * get_Texture() const {
					return _texture.Get();
				};
				inline ID3D12Resource ** geta_Texture() {
					return &_texture;
				};
				inline D3D12_VERTEX_BUFFER_VIEW get_hSquareBuffer() const {
					return _hSquareBuffer;
				};
				inline ID3D12Fence * get_Fence() const {
					return _fence.Get();
				};
				inline HANDLE get_hEvent() const {
					return _hEvent;
				};
				inline UINT64 get_Count() const {
					return _count;
				};
				inline IDXGISwapChain4 * get_SwapChain() const {
					return _swapChain.Get();
				};
				inline D3D12_CPU_DESCRIPTOR_HANDLE get_hRenderTarget(size_t frame) const {
					return _hRenderTargets[frame];
				};
				inline ID3D12Resource * get_RenderTarget(size_t frame) const {
					return _renderTargets[frame].Get();
				};
				inline ID3D12Resource ** geta_RenderTarget(size_t frame) {
					return &_renderTargets[frame];
				};
				inline ID3D12CommandAllocator * get_CommandAllocator(size_t frame) const {
					return _commandAllocators[frame].Get();
				};
				inline ID3D12GraphicsCommandList * get_CommandList(size_t frame) const {
					return _commandLists[frame].Get();
				};
				inline ID3D12Fence * get_Fence(size_t frame) const {
					return _fences[frame].Get();
				};
				inline HANDLE get_hEvent(size_t frame) const {
					return _hEvents[frame];
				};
				inline UINT64 get_Count(size_t frame) const {
					return _counts[frame];
				};
				inline UINT64 inc_Count(size_t frame) {
					return ++(_counts[frame]);
				};

			private:
				HWND _hWnd;
				::Microsoft::WRL::ComPtr<ID3D12Device8> _d3dDevice;
				::Microsoft::WRL::ComPtr<ID3D12CommandQueue> _commandQueue;
				::Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _commandAllocator;
				::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> _commandList;
				::Microsoft::WRL::ComPtr<ID3D12RootSignature> _rootSignature;
				::Microsoft::WRL::ComPtr<ID3D12PipelineState> _defaultState;
				::Microsoft::WRL::ComPtr<ID3D12PipelineState> _blendState;
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _hTextureHeap;
				::Microsoft::WRL::ComPtr<ID3D12Resource> _texture;
				D3D12_VERTEX_BUFFER_VIEW _hSquareBuffer;
				::Microsoft::WRL::ComPtr<ID3D12Resource> _squareBuffer;
				::Microsoft::WRL::ComPtr<ID3D12Fence> _fence;
				HANDLE _hEvent;
				UINT64 _count;
				::Microsoft::WRL::ComPtr<IDXGISwapChain4> _swapChain;
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _hRenderTargetHeap;
				::std::array<D3D12_CPU_DESCRIPTOR_HANDLE, FrameCount> _hRenderTargets;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12Resource>, FrameCount> _renderTargets;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, FrameCount> _commandAllocators;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>, FrameCount> _commandLists;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12Fence>, FrameCount> _fences;
				::std::array<HANDLE, FrameCount> _hEvents;
				::std::array<UINT64, FrameCount> _counts;
				::Microsoft::WRL::ComPtr<IDCompositionTarget> _compositionTarget;
			};
		}
	}
}

#endif
