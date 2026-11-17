module;

#define UNICODE 1
#define _UNICODE 1
#include <d3d11.h>
#include <dxgi1_2.h>

module DirectUI.platform;

import std;

namespace DirectUI {

namespace {

constexpr std::uint32_t kErrInvalidId{0xDEAD0001};

struct DeviceImpl {
  ID3D11Device* ptr_device{};
  ID3D11DeviceContext* ptr_context{};
};

struct SwapChainImpl {
  IDXGISwapChain* ptr_swap{};
  ID3D11RenderTargetView* ptr_rtv{};
  std::uint64_t uint8_device_id{};
};

auto device_registry() -> std::deque<DeviceImpl>& {
  static std::deque<DeviceImpl> deq_reg;
  return deq_reg;
}

auto swap_chain_registry() -> std::deque<SwapChainImpl>& {
  static std::deque<SwapChainImpl> deq_reg;
  return deq_reg;
}

auto alloc_device_id() -> std::uint64_t {
  static std::uint64_t uint8_next{0};
  return uint8_next++;
}

auto alloc_swap_chain_id() -> std::uint64_t {
  static std::uint64_t uint8_next{0};
  return uint8_next++;
}

auto hresult_error(HRESULT int4_hr, std::string_view str_op) -> Error {
  return Error{static_cast<std::uint32_t>(int4_hr), std::string{str_op}};
}

template <typename T>
auto release(T*& ptr_res) -> void {
  if (ptr_res != nullptr) {
    ptr_res->Release();
    ptr_res = nullptr;
  }
}

auto rebuild_buffers(SwapChainImpl& impl, DeviceImpl& dev_impl) -> std::expected<void, Error> {
  release(impl.ptr_rtv);
  auto const int4_hr = impl.ptr_swap->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
  if (FAILED(int4_hr)) {
    return std::unexpected{hresult_error(int4_hr, "IDXGISwapChain1::ResizeBuffers")};
  }
  auto* ptr_texture = static_cast<ID3D11Texture2D*>(nullptr);
  auto const int4_hr_buf = impl.ptr_swap->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&ptr_texture));
  if (FAILED(int4_hr_buf)) {
    return std::unexpected{hresult_error(int4_hr_buf, "IDXGISwapChain1::GetBuffer")};
  }
  auto const int4_hr_rtv = dev_impl.ptr_device->CreateRenderTargetView(ptr_texture, nullptr, &impl.ptr_rtv);
  ptr_texture->Release();
  if (FAILED(int4_hr_rtv)) {
    return std::unexpected{hresult_error(int4_hr_rtv, "ID3D11Device::CreateRenderTargetView")};
  }
  return {};
}

}

auto create_device() -> std::expected<Device, Error> {
  auto const uint4_flags = static_cast<UINT>(D3D11_CREATE_DEVICE_BGRA_SUPPORT);
  auto* ptr_device = static_cast<ID3D11Device*>(nullptr);
  auto* ptr_context = static_cast<ID3D11DeviceContext*>(nullptr);
  auto int4_hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, uint4_flags, nullptr, 0, D3D11_SDK_VERSION, &ptr_device, nullptr, &ptr_context);
  if (FAILED(int4_hr)) {
    int4_hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, uint4_flags, nullptr, 0, D3D11_SDK_VERSION, &ptr_device, nullptr, &ptr_context);
    if (FAILED(int4_hr)) {
      return std::unexpected{hresult_error(int4_hr, "D3D11CreateDevice(WARP fallback)")};
    }
  }
  auto const uint8_id = alloc_device_id();
  device_registry().emplace_back(DeviceImpl{ptr_device, ptr_context});
  return Device{uint8_id};
}

auto create_swap_chain(Device const& dev, Window const& win) -> std::expected<SwapChain, Error> {
  auto& deq_dev = device_registry();
  if (dev.uint8_id >= deq_dev.size()) {
    return std::unexpected{Error{kErrInvalidId, "create_swap_chain: invalid device"}};
  }
  auto& dev_impl = deq_dev[dev.uint8_id];
  auto* ptr_hwnd = static_cast<HWND>(native_handle(win));
  if (ptr_hwnd == nullptr) {
    return std::unexpected{Error{kErrInvalidId, "create_swap_chain: invalid window"}};
  }
  auto* ptr_factory = static_cast<IDXGIFactory2*>(nullptr);
  auto int4_hr = CreateDXGIFactory1(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&ptr_factory));
  if (FAILED(int4_hr)) {
    return std::unexpected{hresult_error(int4_hr, "CreateDXGIFactory1")};
  }
  auto desc_swap = DXGI_SWAP_CHAIN_DESC1{};
  desc_swap.Width = 0;
  desc_swap.Height = 0;
  desc_swap.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  desc_swap.Stereo = FALSE;
  desc_swap.SampleDesc.Count = 1;
  desc_swap.SampleDesc.Quality = 0;
  desc_swap.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  desc_swap.BufferCount = 2;
  desc_swap.Scaling = DXGI_SCALING_STRETCH;
  desc_swap.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
  desc_swap.AlphaMode = DXGI_ALPHA_MODE_IGNORE;
  desc_swap.Flags = 0;
  auto* ptr_swap = static_cast<IDXGISwapChain1*>(nullptr);
  int4_hr = ptr_factory->CreateSwapChainForHwnd(dev_impl.ptr_device, ptr_hwnd, &desc_swap, nullptr, nullptr, &ptr_swap);
  ptr_factory->Release();
  if (FAILED(int4_hr)) {
    return std::unexpected{hresult_error(int4_hr, "CreateSwapChainForHwnd")};
  }
  auto& deq_sc = swap_chain_registry();
  auto const uint8_id = alloc_swap_chain_id();
  auto& impl = deq_sc.emplace_back(SwapChainImpl{ptr_swap, nullptr, dev.uint8_id});
  auto const res_rebuild = rebuild_buffers(impl, dev_impl);
  if (!res_rebuild) {
    release(impl.ptr_rtv);
    release(impl.ptr_swap);
    deq_sc.pop_back();
    return std::unexpected{res_rebuild.error()};
  }
  return SwapChain{uint8_id};
}

auto render_clear_color(SwapChain& sc, std::array<float, 3> const& arr_rgb) -> std::expected<void, Error> {
  auto& deq_sc = swap_chain_registry();
  if (sc.uint8_id >= deq_sc.size()) {
    return std::unexpected{Error{kErrInvalidId, "render_clear_color: invalid swap chain"}};
  }
  auto& sc_impl = deq_sc[sc.uint8_id];
  if (sc_impl.ptr_rtv == nullptr) {
    return std::unexpected{Error{kErrInvalidId, "render_clear_color: swap chain destroyed"}};
  }
  auto& deq_dev = device_registry();
  if (sc_impl.uint8_device_id >= deq_dev.size()) {
    return std::unexpected{Error{kErrInvalidId, "render_clear_color: invalid device"}};
  }
  auto& dev_impl = deq_dev[sc_impl.uint8_device_id];
  auto const arr_color = std::array<float, 4>{arr_rgb[0], arr_rgb[1], arr_rgb[2], 1.0F};
  dev_impl.ptr_context->ClearRenderTargetView(sc_impl.ptr_rtv, arr_color.data());
  auto int4_hr = sc_impl.ptr_swap->Present(1, 0);
  if (FAILED(int4_hr)) {
    auto const res_rebuild = rebuild_buffers(sc_impl, dev_impl);
    if (!res_rebuild) {
      return std::unexpected{res_rebuild.error()};
    }
    dev_impl.ptr_context->ClearRenderTargetView(sc_impl.ptr_rtv, arr_color.data());
    int4_hr = sc_impl.ptr_swap->Present(1, 0);
    if (FAILED(int4_hr)) {
      return std::unexpected{hresult_error(int4_hr, "IDXGISwapChain1::Present")};
    }
  }
  return {};
}

auto native_handle(Device const& dev) -> void* {
  auto& deq = device_registry();
  if (dev.uint8_id >= deq.size()) {
    return nullptr;
  }
  return deq[dev.uint8_id].ptr_device;
}

auto native_handle(SwapChain const& sc) -> void* {
  auto& deq = swap_chain_registry();
  if (sc.uint8_id >= deq.size()) {
    return nullptr;
  }
  return deq[sc.uint8_id].ptr_swap;
}

auto destroy_swap_chain(SwapChain& sc) -> void {
  auto& deq = swap_chain_registry();
  if (sc.uint8_id >= deq.size()) {
    return;
  }
  auto& impl = deq[sc.uint8_id];
  release(impl.ptr_rtv);
  release(impl.ptr_swap);
}

auto destroy_device(Device& dev) -> void {
  auto& deq = device_registry();
  if (dev.uint8_id >= deq.size()) {
    return;
  }
  auto& impl = deq[dev.uint8_id];
  release(impl.ptr_context);
  release(impl.ptr_device);
}

}
