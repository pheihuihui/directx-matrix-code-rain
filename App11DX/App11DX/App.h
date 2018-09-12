#pragma once

//#include "pch.h"
//#include "Common\DeviceResources.h"
//#include "App11DXMain.h"
#include "DirectXBase.h"
#include "SampleOverlay.h"


// 我们的应用程序的主入口点。使用 Windows shell 连接应用程序并处理应用程序生命周期事件。
ref class App sealed : public DirectXBase, public Windows::ApplicationModel::Core::IFrameworkView {

internal:
	App();

	// DirectXBase Methods
	virtual void CreateDeviceIndependentResources() override;
	virtual void CreateDeviceResources() override;
	virtual void CreateWindowSizeDependentResources() override;
	virtual void Render() override;

public:

	// IFrameworkView 方法。
	virtual void Initialize(_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
	virtual void SetWindow(_In_ Windows::UI::Core::CoreWindow^ window);
	virtual void Load(_In_ Platform::String^ entryPoint);
	virtual void Run();
	virtual void Uninitialize();

	//protected:
	//	// 应用程序生命周期事件处理程序。
	//	void OnActivated(Windows::ApplicationModel::Core::CoreApplicationView^ applicationView, Windows::ApplicationModel::Activation::IActivatedEventArgs^ args);
	//	void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ args);
	//	void OnResuming(Platform::Object^ sender, Platform::Object^ args);

	//	// 窗口事件处理程序。
	//	void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
	//	void OnVisibilityChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::VisibilityChangedEventArgs^ args);
	//	void OnWindowClosed(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::CoreWindowEventArgs^ args);

	//	// DisplayInformation 事件处理程序。
	//	void OnDpiChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
	//	void OnOrientationChanged(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);
	//	void OnDisplayContentsInvalidated(Windows::Graphics::Display::DisplayInformation^ sender, Platform::Object^ args);

private:
	// Event Handlers
	void OnWindowSizeChanged(
		_In_ Windows::UI::Core::CoreWindow^ sender,
		_In_ Windows::UI::Core::WindowSizeChangedEventArgs^ args
	);

	void OnDpiChanged(_In_ Windows::Graphics::Display::DisplayInformation^ sender, _In_ Platform::Object^ args);

	void OnDisplayContentsInvalidated(_In_ Windows::Graphics::Display::DisplayInformation^ sender, _In_ Platform::Object^ args);

	void OnActivated(
		_In_ Windows::ApplicationModel::Core::CoreApplicationView^ applicationView,
		_In_ Windows::ApplicationModel::Activation::IActivatedEventArgs^ args
	);

	void OnSuspending(
		_In_ Platform::Object^ sender,
		_In_ Windows::ApplicationModel::SuspendingEventArgs^ args
	);

	void OnResuming(
		_In_ Platform::Object^ sender,
		_In_ Platform::Object^ args
	);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>                    m_blackBrush;
	Microsoft::WRL::ComPtr<IDWriteTextFormat>                       m_textFormat;
	Microsoft::WRL::ComPtr<IDWriteTypography>                       m_textTypography;
	Microsoft::WRL::ComPtr<IDWriteTextLayout>                       m_textLayout;
	SampleOverlay^													m_sampleOverlay;
};


ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource {
public:
	virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
};
