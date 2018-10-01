#include "pch.h"
#include "RainDrop.h"

using namespace Microsoft::WRL;

Matrix::RainDrop::RainDrop(UINT length, ComPtr<ID2D1SolidColorBrush> head, std::vector<ComPtr<ID2D1SolidColorBrush>>& tail) {
	headBrush = head;
	dropBrushes = tail;
	initChance = 100;
	started = false;
	dropLength = length;
	rainSpeedCount = 0;

	int speedMin = 3;
	int speedMax = 10;
	rainSpeed = rand() % (speedMax - speedMin) + speedMin;

	FinishDrop();
}

void Matrix::RainDrop::GetNextDrop() {
	if (!started) {
		if (rand() % initChance == 0) {
			InitDrop();
			return;
		}
	}
	else {
		UINT tmp = brushesMap.begin()->second.first.startPosition;
		if (tmp > dropLength && tmp < 1000) {
			FinishDrop();
			return;
		}
		else {
			updateMap();
		}
	}
}

void Matrix::RainDrop::InitDrop() {
	int tmpMin = 5;
	int tmpMax = (dropLength * 2 / 3);
	actualLength = rand() % (tmpMax - tmpMin) + tmpMin;
	for (UINT i = 0; i < actualLength; i++) {
		DWRITE_TEXT_RANGE tmpRange = {i - actualLength, 1};
		int tmpIndex = int((float)i * dropLength / actualLength);
		ComPtr<ID2D1SolidColorBrush> tmpBrush = dropBrushes[tmpIndex];
		std::pair<DWRITE_TEXT_RANGE, ComPtr<ID2D1SolidColorBrush>> tmpPair = std::make_pair(tmpRange, tmpBrush);
		brushesMap.insert(std::make_pair(i, tmpPair));
	}
	DWRITE_TEXT_RANGE headRange = { 0, 1 };
	std::pair<DWRITE_TEXT_RANGE, ComPtr<ID2D1SolidColorBrush>> headPair = std::make_pair(headRange, headBrush);
	brushesMap.insert(std::make_pair(actualLength, headPair));
	started = true;
}

void Matrix::RainDrop::FinishDrop() {
	started = false;
	startIndex = -1;
	endIndex = -1;
	headIndex = -1;
	actualLength = 0;
	brushesMap.clear();
	dropRanges.clear();
}

void Matrix::RainDrop::updateMap() {
	rainSpeedCount += 1;
	for (auto a = brushesMap.begin(); a != brushesMap.end(); a++) {
		if (rainSpeedCount == rainSpeed) {
			a->second.first.startPosition += 1;
		}
	}
	if (rainSpeedCount == rainSpeed)
		rainSpeedCount = 0;
}
