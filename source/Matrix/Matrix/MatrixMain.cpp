#include "pch.h"
#include "MatrixMain.h"
#include "Common\DirectXHelper.h"

using namespace Matrix;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// 加载应用程序时加载并初始化应用程序资产。
MatrixMain::MatrixMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources) {
	m_deviceResources->RegisterDeviceNotify(this);
	m_codeRain = std::unique_ptr<CodeRain>(new CodeRain(m_deviceResources));

	// TODO: 如果需要默认的可变时间步长模式之外的其他模式，请更改计时器设置。
	// 例如，对于 60 FPS 固定时间步长更新逻辑，请调用:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

MatrixMain::~MatrixMain() {
	// 取消注册设备通知
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// 在窗口大小更改(例如，设备方向更改)时更新应用程序状态
void MatrixMain::CreateWindowSizeDependentResources() {
	// TODO: 将此替换为应用程序内容的与大小相关的初始化。
	//m_sceneRenderer->CreateWindowSizeDependentResources();
}

void MatrixMain::Update() {
	m_timer.Tick([&]() {
		m_codeRain->Update(m_timer);
	});
}

bool MatrixMain::Render() {
	if (m_timer.GetFrameCount() == 0) {
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// 将视区重置为针对整个屏幕。
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// 将呈现目标重置为屏幕。
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// 清除后台缓冲区和深度模具视图。
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_codeRain->Render();

	return true;
}

// 通知呈现器，需要释放设备资源。
void MatrixMain::OnDeviceLost() {
	m_codeRain->ReleaseDeviceDependentResources();
}

// 通知呈现器，现在可重新创建设备资源。
void MatrixMain::OnDeviceRestored() {
	m_codeRain->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
