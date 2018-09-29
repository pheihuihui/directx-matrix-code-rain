#include <string>
#include <vector>
#include <map>
#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"
#include "RainDrop.h"

namespace Matrix {
	class CodeRain {
	public:
		CodeRain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();

	private:
		void StartRaining(Microsoft::WRL::ComPtr<IDWriteTextLayout3> layout, std::shared_ptr<RainDrop> rdrop);

		UINT				m_columnCount;
		UINT				m_lineCount;
		UINT				m_chanceCount;
		UINT				m_charSetLength;
		float				m_fontSize;
		DWRITE_TEXT_METRICS	m_textMetrics;

		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>				m_blackBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>				m_whiteBrush;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1>				m_stateBlock;
		Microsoft::WRL::ComPtr<IDWriteTextFormat2>					m_textFormat;
		std::shared_ptr<DX::DeviceResources>						m_deviceResources;
		std::wstring												m_charSet;
		std::vector<std::wstring>									m_textLines;
		std::vector<std::shared_ptr<RainDrop>>						m_rainDrops;
		std::vector<Microsoft::WRL::ComPtr<IDWriteTextLayout3>>		m_multipleLinesTextLayout;
		std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>>	m_greenBrushes;

	};
}
