#include "pch.h"
#include "RainDrop.h"

using namespace Microsoft::WRL;

Matrix::RainDrop::RainDrop(UINT length, ComPtr<ID2D1SolidColorBrush> head, std::vector<ComPtr<ID2D1SolidColorBrush>>& tail) {
	headBrush = head;
	dropBrushes = tail;
	initChance = 100;
	started = false;
	dropLength = length;
	UINT min = 5;
	UINT max = length / 2 > min ? length / 2 : 5;
	actualLength = rand() % (max - min) + min;
	startIndex = -1;
	endIndex = startIndex - actualLength;
}

void Matrix::RainDrop::GetNextDrop() {
	if (!started) {
		if (rand() % initChance == 0) {
			InitDrop();
			return;
		}
	}
	else {
		if (rand() % initChance == 0) {
			brushesMap[0].first.startPosition += 1;
		}
	}
}

void Matrix::RainDrop::InitDrop() {
	startIndex = 0;
	endIndex = startIndex - actualLength;
	started = true;
	
	DWRITE_TEXT_RANGE tmpRange = {0, 1};
	std::pair<DWRITE_TEXT_RANGE, ComPtr<ID2D1SolidColorBrush>> tmpPair = std::make_pair(tmpRange, headBrush);
	brushesMap.insert(std::make_pair(tmpPair.first.startPosition, tmpPair));
}