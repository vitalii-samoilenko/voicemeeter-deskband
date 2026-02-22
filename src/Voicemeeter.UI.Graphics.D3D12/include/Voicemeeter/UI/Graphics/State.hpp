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
			template<
				typename TSurface,
				typename TLoader>
			class State final {
			public:
				inline State(
					TSurface &surface
					TLoader &loader)
					: _slots_current{ 0 }
					, _slots_commandAllocators{}
					, _slots_commandLists{}
					, _slots_fences{}
					, _slots_hEvents{}
					, _slots_counts{}
					, _blender_hRenderTargetHeap{ nullptr }
					, _blender_renderTargets{}
					, _blender_hRenderTargets{}
					, _blender_hTextureHeap{ nullptr }
					, _blender_defaultState{ nullptr }
					, _blender_rootSignature{ nullptr }
					, _layers_hRenderTargetHeap{ nullptr }
					, _layers_renderTarget{ nullptr }
					, _layers_hRenderTarget{}
					, _layers_hTextureHeap{ nullptr }
					, _layers_defaultState{ nullptr }
					, _layers_blendState{ nullptr }
					, _layers_rootSignature{ nullptr }
					, _atlas{ nullptr }
					, _squareBuffer{ nullptr }
					, _hSquareBuffer{} {
					bool failed{ true };
					auto guardEvents = ::estd::make_guard([
							&failed,
							&hEvents = _slots_hEvents
						]()->void {
							if (!failed) {
								return;
							}
							for (HANDLE hEvent : hEvents) {
								::CloseHandle(hEvent);
							}
						});
					{
						for (size_t slot{ 0 }; slot < SlotsSize; ++slot) {
							::Windows::ThrowIfFailed(surface.get_Device()
								->CreateCommandAllocator(
									D3D12_COMMAND_LIST_TYPE_DIRECT,
									IID_PPV_ARGS(&_slots_commandAllocators[slot])
							), "Command allocator creation failed");
							::Windows::ThrowIfFailed(surface.get_Device()
								->CreateCommandList1(
									0, D3D12_COMMAND_LIST_TYPE_DIRECT,
									D3D12_COMMAND_LIST_FLAG_NONE,
									IID_PPV_ARGS(&_slots_commandLists[slot])
							), "Command list creation failed");
							_slots_hEvents[slot] = ::Windows::CreateEventW(NULL, FALSE, FALSE, NULL);
							::Windows::ThrowIfFailed(surface.get_Device()
								->CreateFence(
									_slots_counts[slot],
									D3D12_FENCE_FLAG_NONE,
									IID_PPV_ARGS(&_slots_fences[slot])
							), "Fence creation failed");
						}
					}
					::Microsoft::WRL::ComPtr<ID3D12Resource> squareUploadBuffer{ nullptr };
					{
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
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateCommittedResource(
								&squareHeapProps, D3D12_HEAP_FLAG_NONE, &squareDesc,
								D3D12_RESOURCE_STATE_COPY_DEST,
								nullptr,
								IID_PPV_ARGS(&_squareBuffer)
						), "Vertex buffer creation failed");
						squareHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateCommittedResource(
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
						_commandLists[0]->CopyResource(_squareBuffer.Get(), squareUploadBuffer.Get());
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
						_commandLists[0]->ResourceBarrier(1, &barrier);
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
							D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE,
							D3D12_MEMORY_POOL_UNKNOWN,
							0, 0
						};
						D3D12_RESOURCE_DESC textureDesc{
							D3D12_RESOURCE_DIMENSION_TEXTURE2D,
							0,
							pop(Layouts::Atlas::Width), pop(Layouts::Atlas::Height), 1, 1,
							DXGI_FORMAT_R32G32B32_FLOAT,
							DXGI_SAMPLE_DESC{
								1, 0
							},
							D3D12_TEXTURE_LAYOUT_UNKNOWN,
							D3D12_RESOURCE_FLAG_NONE
						};
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateCommittedResource(
								&textureHeapProps, D3D12_HEAP_FLAG_NONE, &textureDesc,
								D3D12_RESOURCE_STATE_COPY_DEST,
								nullptr,
								IID_PPV_ARGS(&_atlas)
						), "Texture creation failed");
						UINT rowSize{ pop(Layouts::Atlas::Width) * 4 * 3 };
						UINT srcRowPitch{ rowSize };
						UINT remainder{ rowSize % D3D12_TEXTURE_DATA_PITCH_ALIGNMENT };
						UINT dstRowPitch{
							remainder
								? rowSize - remainder + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT
								: rowSize
						};
						textureHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
						textureHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
						textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
						textureDesc.Width = dstRowPitch * pop(Layouts::Atlas::Height);
						textureDesc.Height = 1;
						textureDesc.Format = DXGI_FORMAT_UNKNOWN;
						textureDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateCommittedResource(
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
									DXGI_FORMAT_R32G32B32_FLOAT,
									pop(Layouts::Atlas::Width), pop(Layouts::Atlas::Height), 1,
									dstRowPitch
								}
							}
						};
						D3D12_TEXTURE_COPY_LOCATION dstLoc{
							_atlas.Get(),
							D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX
						};
						dstLoc.SubresourceIndex = 0;
						_commandLists[0]->CopyTextureRegion(
							&dstLoc,
							0U, 0U, 0U,
							&srcLoc,
							nullptr);
						D3D12_RESOURCE_BARRIER barrier{
							D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
							D3D12_RESOURCE_BARRIER_FLAG_NONE,
							D3D12_RESOURCE_TRANSITION_BARRIER{
								_atlas.Get(),
								D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
								D3D12_RESOURCE_STATE_COPY_DEST,
								D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE
							}
						};
						_commandLists[0]->ResourceBarrier(1U, &barrier);
						D3D12_DESCRIPTOR_HEAP_DESC hTextureHeapDesc{
							D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
							1,
							D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
							0
						};
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateDescriptorHeap(
								&hTextureHeapDesc,
								IID_PPV_ARGS(&_layers_hTextureHeap)
						), "SRV heap descriptor creation failed");
						D3D12_SHADER_RESOURCE_VIEW_DESC hTextureDesc{
							DXGI_FORMAT_R32G32B32_FLOAT,
							D3D12_SRV_DIMENSION_TEXTURE2D,
							D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
						};
						hTextureDesc.Texture2D.MipLevels = 1U;
						surface.get_Device()
							->CreateShaderResourceView(
								_texture.Get(),
								&hTextureDesc,
								_layers_hTextureHeap->GetCPUDescriptorHandleForHeapStart());
					}
					{
						::Windows::ThrowIfFailed(_commandLists[0]->Close(
						), "Command list close failed");
						surface.get_CommandQueue()
							->ExecuteCommandLists(1, &_commandLists[0]);
						surface.get_CommandQueue()
							->Signal(_fences[0].Get(), ++_counts[0]);
					}
					{
						D3D12_DESCRIPTOR_HEAP_DESC hRenderTargetHeapDesc{
							D3D12_DESCRIPTOR_HEAP_TYPE_RTV, TSurface::BuffersSize,
							D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
							0
						};
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateDescriptorHeap(
								&hRenderTargetHeapDesc,
								IID_PPV_ARGS(&_blender_hRenderTargetHeap)
						), "RTV heap descriptor creation failed");
						D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{
							_blender_hRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
						};
						UINT hRenderTargetSize{
							surface.get_Device()
								->GetDescriptorHandleIncrementSize(
									D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
						};
						for (size_t buffer{ 0 }; buffer < TSurface::BuffersSize; ++buffer) {
							::Windows::ThrowIfFailed(surface.get_SwapChain()
								->GetBuffer(
									static_cast<UINT>(buffer),
									IID_PPV_ARGS(&_blender_renderTargets[buffer])
							), "Failed to get swap chain buffer");
							_blender_hRenderTargets[buffer].ptr = SIZE_T(INT64(hRenderTarget.ptr)
								+ INT64(buffer * hRenderTargetSize));
							surface.get_Device()
								->CreateRenderTargetView(
									_blender_renderTargets[buffer].Get(),
									nullptr, _blender_hRenderTargets[buffer]);
						}
					}
					{
						D3D12_DESCRIPTOR_HEAP_DESC hRenderTargetHeapDesc{
							D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 1U,
							D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
							0
						};
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateDescriptorHeap(
								&hRenderTargetHeapDesc,
								IID_PPV_ARGS(&_layers_hRenderTargetHeap)
						), "RTV heap descriptor creation failed");
						D3D12_HEAP_PROPERTIES textureHeapProps{
							D3D12_HEAP_TYPE_DEFAULT,
							D3D12_CPU_PAGE_PROPERTY_NOT_AVAILABLE,
							D3D12_MEMORY_POOL_UNKNOWN,
							0, 0
						};
						D3D12_RESOURCE_DESC textureDesc{
							D3D12_RESOURCE_DIMENSION_TEXTURE2D,
							0,
							8U, 8U,
							1, 1,
							DXGI_FORMAT_R8G8B8A8_UNORM,
							DXGI_SAMPLE_DESC{
								1, 0
							},
							D3D12_TEXTURE_LAYOUT_UNKNOWN,
							D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
						};
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateCommittedResource(
								&textureHeapProps, D3D12_HEAP_FLAG_NONE, &textureDesc,
								D3D12_RESOURCE_STATE_RENDER_TARGET,
								nullptr,
								IID_PPV_ARGS(&_layers_renderTarget)
						), "Texture creation failed");
						D3D12_CPU_DESCRIPTOR_HANDLE hRenderTarget{
							_layers_hRenderTargetHeap->GetCPUDescriptorHandleForHeapStart()
						};
						_layers_hRenderTarget.ptr = SIZE_T(INT64(hRenderTarget.ptr);
						surface.get_Device()
							->CreateRenderTargetView(
								_layers_renderTarget.Get(),
								nullptr, _layers_hRenderTarget);
						D3D12_DESCRIPTOR_HEAP_DESC hTextureHeapDesc{
							D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
							1,
							D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
							0
						};
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateDescriptorHeap(
								&hTextureHeapDesc,
								IID_PPV_ARGS(&_blender_hTextureHeap)
						), "SRV heap descriptor creation failed");
						D3D12_SHADER_RESOURCE_VIEW_DESC hTextureDesc{
							DXGI_FORMAT_R8G8B8A8_UNORM,
							D3D12_SRV_DIMENSION_TEXTURE2D,
							D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING
						};
						hTextureDesc.Texture2D.MipLevels = 1U;
						surface.get_Device()
							->CreateShaderResourceView(
								_layers_renderTarget.Get(),
								&hTextureDesc,
								_blender_hTextureHeap->GetCPUDescriptorHandleForHeapStart());
					}
					{
						D3D12_DESCRIPTOR_RANGE hTextureRange{
							D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
							1,
							0, 0,
							0
						};
						::std::array<D3D12_ROOT_PARAMETER, 3> params{};
						params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
						params[0].Constants.ShaderRegister = 0;
						params[0].Constants.Num32BitValues = 4;
						params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
						params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
						params[1].Constants.ShaderRegister = 0;
						params[1].Constants.Num32BitValues = 5;
						params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
						params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
						params[2].DescriptorTable.NumDescriptorRanges = 1;
						params[2].DescriptorTable.pDescriptorRanges = &hTextureRange;
						params[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
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
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateRootSignature(
								0,
								temp->GetBufferPointer(), temp->GetBufferSize(),
								IID_PPV_ARGS(&_layers_rootSignature)
						), "Root signature creation failed");
					}
					{
						D3D12_DESCRIPTOR_RANGE hTextureRange{
							D3D12_DESCRIPTOR_RANGE_TYPE_SRV,
							1,
							0, 0,
							0
						};
						::std::array<D3D12_ROOT_PARAMETER, 4> params{};
						params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
						params[0].Constants.ShaderRegister = 0;
						params[0].Constants.Num32BitValues = 4;
						params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
						params[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
						params[1].Constants.ShaderRegister = 0;
						params[1].Constants.Num32BitValues = 1;
						params[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
						params[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
						params[2].Constants.ShaderRegister = 0;
						params[2].Constants.Num32BitValues = 1;
						params[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
						params[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
						params[3].DescriptorTable.NumDescriptorRanges = 1;
						params[3].DescriptorTable.pDescriptorRanges = &hTextureRange;
						params[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
						D3D12_STATIC_SAMPLER_DESC samplerDesc{
							D3D12_FILTER_MIN_MAG_MIP_POINT,
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
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateRootSignature(
								0,
								temp->GetBufferPointer(), temp->GetBufferSize(),
								IID_PPV_ARGS(&_blender_rootSignature)
						), "Root signature creation failed");
					}
					::Microsoft::WRL::ComPtr<ID3DBlob> vertexShader{ nullptr };
					::Microsoft::WRL::ComPtr<ID3DBlob> pixelShader{ nullptr };
					::Microsoft::WRL::ComPtr<ID3DBlob> blenderShader{ nullptr };
					{
						typename TLoader::resource vertexCode{
							loader.LoadResource(
								Layouts::Loader::hlsl::Vertex)
						};
						typename TLoader::resource pixelCode{
							loader.LoadResource(
								Layouts::Loader::hlsl::Pixel)
						};
						typename TLoader::resource blenderCode{
							loader.LoadResource(
								Layouts::Loader::hlsl::Blender)
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
						::Windows::ThrowIfFailed(::D3DCompile(
							pixelCode.data(), pixelCode.size(),
							NULL,
							nullptr, nullptr,
							"Main", "ps_5_0",
							compileFlags, 0,
							&blenderShader, nullptr
						), "Pixel shader compilation failed");
					}
					{
						::std::array<D3D12_INPUT_ELEMENT_DESC, 2> inputDescs{};
						inputDescs[0].SemanticName = "POSITION";
						inputDescs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
						inputDescs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
						inputDescs[1].SemanticName = "SV_VertexID";
						inputDescs[1].Format = DXGI_FORMAT_R32_UINT;
						inputDescs[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
						D3D12_INPUT_ELEMENT_DESC positionDesc{
							"POSITION", 0,
							DXGI_FORMAT_R32G32_FLOAT,
							0, 0,
							D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
							0
						};
						D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{
							_layers_rootSignature.Get(),
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
										FALSE, FALSE,
										D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
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
								FALSE,
								FALSE, FALSE, 0,
								D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
							},
							D3D12_DEPTH_STENCIL_DESC{
								FALSE, D3D12_DEPTH_WRITE_MASK_ZERO, D3D12_COMPARISON_FUNC_NEVER,
								FALSE, 0, 0,
								D3D12_DEPTH_STENCILOP_DESC{}, D3D12_DEPTH_STENCILOP_DESC{}
							},
							D3D12_INPUT_LAYOUT_DESC{
								&inputDescs[0], 2U
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
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateGraphicsPipelineState(
								&pipelineStateDesc,
								IID_PPV_ARGS(&_layers_defaultState)
						), "Pipeline state creation failed");
						pipelineStateDesc.BlendState.RenderTarget[0].BlendEnable = TRUE;
						pipelineStateDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
						pipelineStateDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
						pipelineStateDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ZERO;
						pipelineStateDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ONE;
						::Windows::ThrowIfFailed(_d3dDevice->CreateGraphicsPipelineState(
							&pipelineStateDesc,
							IID_PPV_ARGS(&_layers_blendState)
						), "Pipeline state creation failed");
					}
					{
						::std::array<D3D12_INPUT_ELEMENT_DESC, 2> inputDescs{};
						inputDescs[0].SemanticName = "POSITION";
						inputDescs[0].Format = DXGI_FORMAT_R32G32_FLOAT;
						inputDescs[0].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
						inputDescs[1].SemanticName = "SV_VertexID";
						inputDescs[1].Format = DXGI_FORMAT_R32_UINT;
						inputDescs[1].InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
						D3D12_INPUT_ELEMENT_DESC positionDesc{
							"POSITION", 0,
							DXGI_FORMAT_R32G32_FLOAT,
							0, 0,
							D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
							0
						};
						D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineStateDesc{
							_blender_rootSignature.Get(),
							D3D12_SHADER_BYTECODE{
								vertexShader->GetBufferPointer(),
								vertexShader->GetBufferSize()
							},
							D3D12_SHADER_BYTECODE{
								blenderShader->GetBufferPointer(),
								blenderShader->GetBufferSize()
							},
							D3D12_SHADER_BYTECODE{}, D3D12_SHADER_BYTECODE{}, D3D12_SHADER_BYTECODE{},
							D3D12_STREAM_OUTPUT_DESC{},
							D3D12_BLEND_DESC{
								FALSE, FALSE,
								{
									D3D12_RENDER_TARGET_BLEND_DESC{
										TRUE, FALSE,
										D3D12_BLEND_SRC_ALPHA, D3D12_BLENDINV_SRC_ALPHA, D3D12_BLEND_OP_ADD,
										D3D12_BLEND_ZERO, D3D12_BLEND_ONE, D3D12_BLEND_OP_ADD,
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
								FALSE,
								FALSE, FALSE, 0,
								D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF
							},
							D3D12_DEPTH_STENCIL_DESC{
								FALSE, D3D12_DEPTH_WRITE_MASK_ZERO, D3D12_COMPARISON_FUNC_NEVER,
								FALSE, 0, 0,
								D3D12_DEPTH_STENCILOP_DESC{}, D3D12_DEPTH_STENCILOP_DESC{}
							},
							D3D12_INPUT_LAYOUT_DESC{
								&inputDescs[0], 2U
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
						::Windows::ThrowIfFailed(surface.get_Device()
							->CreateGraphicsPipelineState(
								&pipelineStateDesc,
								IID_PPV_ARGS(&_blender_defaultState)
						), "Pipeline state creation failed");
					}
					if (_slots_fences[0]->GetCompletedValue() < _slots_counts[0]) {
						::Windows::ThrowIfFailed(_slots_fences[0]->SetEventOnCompletion(
							_slots_counts[0], _slots_hEvents[0]
						), "Event signaling failed");
						::Windows::WaitForSingleObject(
							_slots_hEvents[0], INFINITE);
					}
					failed = false;
				};
				State() = delete;
				State(State const &) = delete;
				State(State &&) = delete;

				inline ~State() {
					for (HANDLE hEvent : _slots_hEvents) {
						::CloseHandle(hEvent);
					}
				};

				State & operator=(State const &) = delete;
				State & operator=(State &&) = delete;

				static constexpr size_t SlotsSize{ 3 };

				// slots
				inline size_t get_slots_Current() const {
					return _slots_current;
				}
				inline size_t inc_slots_Current() {
					return ++(_slots_current) % SlotsSize;
				};
				inline ID3D12CommandAllocator * get_slots_CommandAllocator(size_t slot) const {
					return _slots_commandAllocators[slot].Get();
				};
				inline ID3D12GraphicsCommandList * get_slots_CommandList(size_t slot) const {
					return _slots_commandLists[slot].Get();
				};
				inline ID3D12Fence * get_slots_Fence(size_t slot) const {
					return _slots_fences[slot].Get();
				};
				inline HANDLE get_slots_hEvent(size_t slot) const {
					return _slots_hEvents[slot];
				};
				inline UINT64 get_slots_Count(size_t slot) const {
					return _slots_counts[slot];
				};
				inline UINT64 inc_slots_Count(size_t slot) {
					return ++(_slots_counts[slot]);
				};
				// blender
				inline ID3D12Resource * get_blender_RenderTarget(size_t buffer) const {
					return _blender_renderTargets[buffer].Get();
				};
				inline ID3D12Resource ** geta_blender_RenderTarget(size_t buffer) {
					return &_blender_renderTargets[buffer];
				};
				inline D3D12_CPU_DESCRIPTOR_HANDLE get_blender_hRenderTarget(size_t buffer) const {
					return _blender_hRenderTargets[buffer];
				};
				inline ID3D12DescriptorHeap * get_blender_hTextureHeap() const {
					return _blender_hTextureHeap.Get();
				};
				inline ID3D12PipelineState * get_blender_DefaultState() const {
					return _blender_defaultState.Get();
				};
				inline ID3D12RootSignature * get_blender_RootSignature() const {
					return _blender_rootSignature.Get();
				};
				// layers
				inline ID3D12Resource * get_layers_RenderTarget() const {
					return _layers_renderTarget.Get();
				};
				inline ID3D12Resource ** geta_layers_RenderTarget() {
					return &_layers_renderTarget;
				};
				inline D3D12_CPU_DESCRIPTOR_HANDLE get_layers_hRenderTarget() const {
					return _layers_hRenderTarget;
				};
				inline ID3D12DescriptorHeap * get_layers_hTextureHeap() const {
					return _layers_hTextureHeap.Get();
				};
				inline ID3D12PipelineState * get_layers_DefaultState() const {
					return _layers_defaultState.Get();
				};
				inline ID3D12PipelineState * get_layers_BlendState() const {
					return _layers_blendState.Get();
				};
				inline ID3D12RootSignature * get_layers_RootSignature() const {
					return _layers_rootSignature.Get();
				};
				// ------
				inline ID3D12Resource * get_Atlas() const {
					return _atlas.Get();
				};
				inline D3D12_VERTEX_BUFFER_VIEW get_hSquareBuffer() const {
					return _hSquareBuffer;
				};

			private:
				// slots
				size_t _slots_current;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12CommandAllocator>, SlotsSize> _slots_commandAllocators;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>, SlotsSize> _slots_commandLists;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12Fence>, SlotsSize> _slots_fences;
				::std::array<HANDLE, SlotsSize> _slots_hEvents;
				::std::array<UINT64, SlotsSize> _slots_counts;
				// blender
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _blender_hRenderTargetHeap;
				::std::array<::Microsoft::WRL::ComPtr<ID3D12Resource>, TSurface::BuffersSize> _blender_renderTargets;
				::std::array<D3D12_CPU_DESCRIPTOR_HANDLE, TSufrace::BuffersSize> _blender_hRenderTargets;
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _blender_hTextureHeap;
				::Microsoft::WRL::ComPtr<ID3D12PipelineState> _blender_defaultState;
				::Microsoft::WRL::ComPtr<ID3D12RootSignature> _blender_rootSignature;
				// layers
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _layers_hRenderTargetHeap;
				::Microsoft::WRL::ComPtr<ID3D12Resource> _layers_renderTarget;
				D3D12_CPU_DESCRIPTOR_HANDLE _layers_hRenderTarget;
				::Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _layers_hTextureHeap;
				::Microsoft::WRL::ComPtr<ID3D12PipelineState> _layers_defaultState;
				::Microsoft::WRL::ComPtr<ID3D12PipelineState> _layers_blendState;
				::Microsoft::WRL::ComPtr<ID3D12RootSignature> _layers_rootSignature;
				// ------
				::Microsoft::WRL::ComPtr<ID3D12Resource> _atlas;
				::Microsoft::WRL::ComPtr<ID3D12Resource> _squareBuffer;
				D3D12_VERTEX_BUFFER_VIEW _hSquareBuffer;
			};
		}
	}
}

#endif
