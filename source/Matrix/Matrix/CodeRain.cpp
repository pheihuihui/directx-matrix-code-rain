#include "pch.h"
#include "CodeRain.h"
#include <stdlib.h>
#include "Common/DirectXHelper.h"
#include <vector>
#include "RainDrop.h"


using namespace Matrix;
using namespace Microsoft::WRL;

Matrix::CodeRain::CodeRain(const std::shared_ptr<DX::DeviceResources>& deviceResources)
{
	m_deviceResources = deviceResources;
	m_fontSize = 16.0f;
	m_charSet = L"QWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%^&*()_+-=,.<>;:[]{}?";
	m_charSetLength = m_charSet.size();
	m_chanceCount = 100;

	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
	m_columnCount = 100;
	m_lineCount = 50;

	for (UINT i = 0; i < m_columnCount; i++) {
		std::wstring tmp;
		for (UINT j = 0; j < m_lineCount; j++) {
			int index = rand() % m_charSetLength;
			tmp.push_back(m_charSet[index]);
		}
		m_textLines.push_back(tmp);
	}

	ComPtr<IDWriteTextFormat> textFormat;
	m_deviceResources->GetDWriteFactory()->CreateTextFormat(
		L"Consolas",
		nullptr,
		DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		m_fontSize,
		L"en-US",
		&textFormat
	);

	textFormat.As(&m_textFormat);
	m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
	m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock);
	CreateDeviceDependentResources();

	for (UINT i = 0; i < m_columnCount; i++) {
		std::shared_ptr<RainDrop> singleDrop = std::make_shared<RainDrop>(m_lineCount, m_whiteBrush.Get(), m_greenBrushes);
		m_rainDrops.push_back(singleDrop);
	}
}

void Matrix::CodeRain::CreateDeviceDependentResources() {
	m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &m_blackBrush);
	m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &m_whiteBrush);
	for (int i = 0; i < 50; i++) {
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> tmp;
		m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green, ((float)i) / 50.0f), &tmp);
		m_greenBrushes.push_back(tmp);
	}
}

void Matrix::CodeRain::ReleaseDeviceDependentResources() {
	m_blackBrush.Reset();
	m_whiteBrush.Reset();
	for (auto a = m_greenBrushes.begin(); a != m_greenBrushes.end(); a++)
		a->Reset();
}

void Matrix::CodeRain::Update(DX::StepTimer const & timer) {

	for (UINT i = 0; i < m_columnCount; i++) {
		for (UINT j = 0; j < m_lineCount; j++) {
			if (rand() % m_chanceCount == 0)
				m_textLines[i][j] = m_charSet[rand() % m_charSetLength];
		}
	}

	ComPtr<IDWriteTextLayout> textLayout;
	for (UINT i = 0; i < m_columnCount; i++)
		m_multipleLinesTextLayout.push_back(nullptr);
	for (UINT i = 0; i < m_columnCount; i++) {
		m_deviceResources->GetDWriteFactory()->CreateTextLayout(m_textLines[i].c_str(), (uint32)m_textLines[i].length(), m_textFormat.Get(), 16.0f, 200.0f, &textLayout);
		textLayout.As(&m_multipleLinesTextLayout[i]);
		m_multipleLinesTextLayout[i]->GetMetrics(&m_textMetrics);
	}

	for (auto a = m_rainDrops.begin(); a != m_rainDrops.end(); a++)
		(*a)->GetNextDrop();
}

void Matrix::CodeRain::Render() {
	ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();
	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(0, 0);
	context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());

	m_textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
	m_textFormat->SetReadingDirection(DWRITE_READING_DIRECTION::DWRITE_READING_DIRECTION_LEFT_TO_RIGHT);
	m_textFormat->SetFlowDirection(DWRITE_FLOW_DIRECTION::DWRITE_FLOW_DIRECTION_TOP_TO_BOTTOM);

	//for (auto a = m_multipleLinesTextLayout.begin(); a != m_multipleLinesTextLayout.end(); a++)
	//	StartRaining(*a, );
	for (UINT i = 0; i < m_columnCount; i++)
		StartRaining(m_multipleLinesTextLayout[i], m_rainDrops[i]);

	for (UINT i = 0; i < m_columnCount; i++)
		context->DrawTextLayout(D2D1::Point2F(16.0f * i, 10.0f), m_multipleLinesTextLayout[i].Get(), m_blackBrush.Get());

	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET) {
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

void Matrix::CodeRain::StartRaining(Microsoft::WRL::ComPtr<IDWriteTextLayout3> layout, std::shared_ptr<RainDrop> rdrop) {
	for (auto a = rdrop->brushesMap.begin(); a != rdrop->brushesMap.end(); a++)
		layout.Get()->SetDrawingEffect(a->second.second.Get(), a->second.first);
}
