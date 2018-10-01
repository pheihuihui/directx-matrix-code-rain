#pragma once

#include <map>
#include <vector>

namespace Matrix {
	class RainDrop {
	public:
		RainDrop(UINT length, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> head, std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>>& tail);
		void GetNextDrop();
		void InitDrop();
		void FinishDrop();

		std::map<int, std::pair<DWRITE_TEXT_RANGE, Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>>> brushesMap;

	private:
		void updateMap();

		std::vector<Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>>	dropBrushes;
		Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>				headBrush;
		std::vector<DWRITE_TEXT_RANGE>								dropRanges;

		UINT	actualLength;
		UINT	initChance;
		UINT	dropLength;
		UINT	rainSpeed;
		UINT	rainSpeedCount;
		bool	started;
		int		startIndex;
		int		endIndex;
		int		headIndex;
	};
}