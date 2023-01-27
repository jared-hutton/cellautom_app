#ifndef CELLULARAUTOMATON_H
#define CELLULARAUTOMATON_H

#include <array>
#include <vector>

#include "defs.h"

class CellularAutomaton
{
    public:
        CellularAutomaton();
	CellularAutomaton(uint32_t initCellScopeLoX, uint32_t initCellScopeLoY, uint32_t initCellScopeHiX, uint32_t initCellScopeHiY);
        virtual ~CellularAutomaton();

	void NextTick();

	void Clear();

	const uint32_t GetRule() { return m_rule; }

	int32_t GetScopeLoX() { return -(int32_t)m_cellScopeLoX; };
	int32_t GetScopeLoY() { return -(int32_t)m_cellScopeLoY; };

	int32_t GetScopeHiX() { return m_cellScopeHiX; };
	int32_t GetScopeHiY() { return m_cellScopeHiY; };

	uint8_t GetCellState(int32_t t_cellx, int32_t t_celly);

	std::array<std::vector<bool>, 4> GetCellMap() { return m_cellMap; }

	void SetRule(uint32_t t_rule) { m_rule = t_rule; }
	void SetCellStates(uint8_t t_state);
	void IncrementCellState(int32_t t_cellx, int32_t t_celly);

	void SetCellMap(std::array<std::vector<bool>, 4> &t_cellMap);

	void AddRows(bool hi, uint8_t t_count);
	void AddCols(bool hi, uint8_t t_count);

    protected:

    private:
	std::array<std::vector<bool>, 4> m_cellMap;
	std::array<std::vector<bool>, 4> m_cellMapBuffer;

	uint32_t m_cellScopeLoX;
	uint32_t m_cellScopeLoY;

	uint32_t m_cellScopeHiX;
	uint32_t m_cellScopeHiY;

	uint32_t m_rule = 0;

	uint8_t IndexCellToPlane(int32_t t_cellx, int32_t t_celly);
	uint64_t IndexCellInPlane(uint8_t t_plane, uint32_t t_cellx, uint32_t t_celly);

	uint8_t GetNumAliveNeighbors(int32_t t_cellx, int32_t t_celly);
};

#endif // CELLULARAUTOMATON_H
