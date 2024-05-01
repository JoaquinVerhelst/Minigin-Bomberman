#pragma once

#include <glm/glm.hpp>
#include <memory>
//#include <vector>
#include <unordered_map>
#include "Singleton.h"
#include "GameObject.h"


namespace std {
	template <>
	struct hash<glm::ivec2> {
		size_t operator()(const glm::ivec2& v) const {
			// Combine the hash values of the x and y components
			// You can use any suitable hash function here
			return hash<int>()(v.x) ^ (hash<int>()(v.y) << 1);
		}
	};

	template <>
	struct equal_to<glm::ivec2> {
		bool operator()(const glm::ivec2& v1, const glm::ivec2& v2) const {
			// Compare the x and y components for equality
			return v1.x == v2.x && v1.y == v2.y;
		}
	};
}

namespace Jotar
{


	struct GridCell
	{
		GridCell()
			:WorldPosition{}, CellSize{}, CenterCellPosition{}, Index{} {}

		GridCell(glm::vec2 worldPos, int size, glm::ivec2 index)
			:WorldPosition{ worldPos }, CellSize{ size }, CenterCellPosition{}, Index{ index }
		{
			CenterCellPosition = glm::vec2(worldPos.x + CellSize / 2, worldPos.y + CellSize / 2);
		}

		glm::vec2 WorldPosition;
		int CellSize;
		glm::vec2 CenterCellPosition;
		glm::ivec2 Index;

		std::weak_ptr<GameObject> ObjectOnCell;

		bool IsConnected(glm::ivec2 cellIndex) const
		{
			if (std::abs(cellIndex.y - Index.y) == 1)
				return true;
			else if (std::abs(cellIndex.x - Index.x) == 1)
				return true;

			return false;
		}
	};




	class WorldGrid final : public Singleton<WorldGrid>
	{
	public:
		void Init(int rows, int columns, int size);
		void Render() const;

		WorldGrid(const WorldGrid& other) = delete;
		WorldGrid(WorldGrid&& other) = delete;
		WorldGrid& operator=(const WorldGrid& other) = delete;
		WorldGrid& operator=(WorldGrid&& other) = delete;

		std::unordered_map<glm::ivec2, GridCell>& GetWorldGrid();

		const int GetCellSize() const;
		const glm::vec2& GetGridSize() const;
		const GridCell& GetGridCellByID(const glm::ivec2& ID) const;
		GridCell& GetGridCellByID(const glm::ivec2& ID);
		const GridCell& GetGridCellByPosition(const glm::vec2& position) const;
		GridCell& GetGridCellByPosition(const glm::vec2& position);


		std::vector<GridCell> GetConnectedCellsFromIndex(glm::ivec2 cellIndex) const;

	private:
		friend class Singleton<WorldGrid>;
		WorldGrid() = default;
		std::unordered_map<glm::ivec2, GridCell> m_Grid;
		
		glm::vec2 m_GridSize;
		int m_CellSize;
	};
}
