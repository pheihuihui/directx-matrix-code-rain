#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "CodeRain.h"

// 在屏幕上呈现 Direct2D 和 3D 内容。
namespace Matrix {
	class MatrixMain : public DX::IDeviceNotify {
	public:
		MatrixMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~MatrixMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		std::shared_ptr<DX::DeviceResources>	m_deviceResources;
		std::unique_ptr<CodeRain>				m_codeRain;
		DX::StepTimer							m_timer;
	};
}