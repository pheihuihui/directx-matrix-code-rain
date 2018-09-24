#include "pch.h"
#include "CodeRain.h"
#include <stdlib.h>
#include "Common/DirectXHelper.h"

using namespace Matrix;
using namespace Microsoft::WRL;

Matrix::CodeRain::CodeRain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_fontSize(16.0f),
	m_charSet(L"QWERTYUIOPASDFGHJKLZXCVBNM1234567890!@#$%^&*()_+-=,.<>;:[]{}?"),
	m_charSetLength(m_charSet.size()),
	m_chanceCount(100)
{
	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
	m_columnCount = 16;
	m_lineCount = 9;

	for (UINT i = 0; i < m_columnCount; i++) {
		std::wstring tmp;
		for (UINT j = 0; j < m_lineCount; j++) {
			tmp.push_back(m_charSet[rand() % m_charSetLength]);
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

	//context->DrawTextLayout(D2D1::Point2F(10.f, 10.f), m_textLayout.Get(), m_greenBrush.Get());
	DWRITE_TEXT_RANGE white_range = { 2, 10 };
	DWRITE_TEXT_RANGE green_range = { 0, 2 };

	m_multipleLinesTextLayout[2].Get()->SetDrawingEffect(m_whiteBrush.Get(), white_range);
	m_multipleLinesTextLayout[2].Get()->SetDrawingEffect(m_greenBrushes[40].Get(), green_range);
	for (UINT i = 0; i < m_columnCount; i++)
		context->DrawTextLayout(D2D1::Point2F(16.0f * i, 10.0f), m_multipleLinesTextLayout[i].Get(), m_greenBrushes[10].Get());

	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET) {
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}