#include <string>
#include <vector>
#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

namespace Matrix {
	class CodeRain {
	public:
		CodeRain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		void CreateDeviceDependentResources();
		void ReleaseDeviceDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();

	private:


		UINT	m_columnCount;
		UINT	m_lineCount;
		UINT	m_chanceCount;
		float	m_fontSize;
		
		std::shared_ptr<DX::DeviceResources>						m_deviceResources;
		std::wstring												m_charSet;
		uint32														m_charSetLength;
		std::vector<std::wstring>									m_textLines;
		DWRITE_TEXT_METRICS											m_textMetrics;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>				m_blackBrush;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>				m_whiteBrush;
		std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>>	m_greenBrushes;
		Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1>				m_stateBlock;
		std::vector<Microsoft::WRL::ComPtr<IDWriteTextLayout3>>		m_multipleLinesTextLayout;
		Microsoft::WRL::ComPtr<IDWriteTextFormat2>					m_textFormat;
	};

	class SingleRainDrop {
	public:
		DWRITE_TEXT_RANGE								dropRange;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	colorBrush;
	};

	class RainDrop {
	public:
		RainDrop(UINT height, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> head, std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>>& tail, UINT time);
		void getNextRainDrop();
		void initRainDrop();
		UINT totHeight;
		UINT actHeight;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> headBrush;
		std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>> tailBrushes;
		std::vector<Microsoft::WRL::ComPtr<SingleRainDrop>> drop;
		UINT nextDropTime;
	};
}
