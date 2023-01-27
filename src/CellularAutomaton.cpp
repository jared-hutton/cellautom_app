#include "CellularAutomaton.h"

#include <cstdint>
#include <cstdlib>

CellularAutomaton::CellularAutomaton() {}

CellularAutomaton::CellularAutomaton(uint32_t initCellScopeLoX, uint32_t initCellScopeLoY, uint32_t initCellScopeHiX, uint32_t initCellScopeHiY)
	: m_cellScopeLoX(initCellScopeLoX), m_cellScopeLoY(initCellScopeLoY), m_cellScopeHiX(initCellScopeHiX), m_cellScopeHiY(initCellScopeHiY)
{
	m_cellMap[0].resize(m_cellScopeHiX * m_cellScopeHiY, 0);
	m_cellMap[1].resize(m_cellScopeHiX * m_cellScopeLoY, 0);
	m_cellMap[2].resize(m_cellScopeLoX * m_cellScopeLoY, 0);
	m_cellMap[3].resize(m_cellScopeLoX * m_cellScopeHiY, 0);

	m_cellMapBuffer[0].resize(m_cellScopeHiX * m_cellScopeHiY, 0);
	m_cellMapBuffer[1].resize(m_cellScopeHiX * m_cellScopeLoY, 0);
	m_cellMapBuffer[2].resize(m_cellScopeLoX * m_cellScopeLoY, 0);
	m_cellMapBuffer[3].resize(m_cellScopeLoX * m_cellScopeHiY, 0);
}

CellularAutomaton::~CellularAutomaton()
{
}

/* NextTick
 *
 * Calculates the state of each cell in the next tick and places them into the tick buffer
 * Once the buffer is filled, its elements are moved back into the main cell state vectors
 *
 */
void CellularAutomaton::NextTick()
{
	int32_t cellx, celly;
	uint32_t celli;
	uint8_t curplane;
	uint8_t numNeighborsAlive;

	for (celly = -m_cellScopeLoY; celly < (int)m_cellScopeHiY; ++celly)
	for (cellx = -m_cellScopeLoX; cellx < (int)m_cellScopeHiX; ++cellx)
	{
		curplane = IndexCellToPlane(cellx, celly);
		celli = IndexCellInPlane(curplane, abs(cellx), abs(celly));

		numNeighborsAlive = GetNumAliveNeighbors(cellx, celly);

		m_cellMapBuffer[curplane][celli] =
			m_cellMap[curplane][celli] ?
				(m_rule >> (numNeighborsAlive + 9)) & 0x1
			:
				(m_rule >> numNeighborsAlive)       & 0x1;
	}

	for (celly = -m_cellScopeLoY; celly < (int)m_cellScopeHiY; ++celly)
	for (cellx = -m_cellScopeLoX; cellx < (int)m_cellScopeHiX; ++cellx)
	{
		curplane = IndexCellToPlane(cellx, celly);
		celli = IndexCellInPlane(curplane, abs(cellx), abs(celly));

		m_cellMap[curplane][celli] = m_cellMapBuffer[curplane][celli];
	}
}

void CellularAutomaton::SetCellStates(uint8_t t_state)
{
	uint32_t celli;
	uint8_t plane;

	for (plane = 0; plane < 4; ++plane)
	for (celli = 0; celli < m_cellMap[plane].size(); ++celli)
		m_cellMap[plane][celli] = t_state;
}

void CellularAutomaton::SetCellMap(std::array<std::vector<bool>, 4> &t_cellMap)
{
	m_cellMap = t_cellMap;
}
	

uint8_t CellularAutomaton::IndexCellToPlane(int32_t t_cellx, int32_t t_celly)
{
	if (t_cellx >= 0)
		return t_celly >= 0 ? 0 : 1;
	else
		return t_celly >= 0 ? 3 : 2;	
}

uint64_t CellularAutomaton::IndexCellInPlane(uint8_t t_plane, uint32_t t_cellx, uint32_t t_celly)
{
	switch(t_plane)
	{
	case 0:
		return t_cellx + t_celly * m_cellScopeHiX;	
		break;
	case 1:
		return t_cellx + (t_celly - 1) * m_cellScopeHiX;	
		break;
	case 2:
		return (t_cellx - 1) + (t_celly - 1) * m_cellScopeLoX;	
		break;
	case 3:
		return (t_cellx - 1) + t_celly * m_cellScopeLoX;	
		break;
	}

	return 4;
}

void CellularAutomaton::AddRows(bool hi, uint8_t t_count)
{
	if (hi)
	{
		m_cellScopeHiY += t_count;

		m_cellMap[0].resize(m_cellMap[0].size() + m_cellScopeHiX * t_count, 0);
		m_cellMap[3].resize(m_cellMap[3].size() + m_cellScopeLoX * t_count, 0);

		m_cellMapBuffer[0].resize(m_cellMapBuffer[0].size() + m_cellScopeHiX * t_count, 0);
		m_cellMapBuffer[3].resize(m_cellMapBuffer[3].size() + m_cellScopeLoX * t_count, 0);
	}
	else
	{
		m_cellScopeLoY += t_count;

		m_cellMap[1].resize(m_cellMap[1].size() + m_cellScopeHiX * t_count, 0);
		m_cellMap[2].resize(m_cellMap[2].size() + m_cellScopeLoX * t_count, 0);

		m_cellMapBuffer[1].resize(m_cellMapBuffer[1].size() + m_cellScopeHiX * t_count, 0);
		m_cellMapBuffer[2].resize(m_cellMapBuffer[2].size() + m_cellScopeLoX * t_count, 0);
	}
}

void CellularAutomaton::AddCols(bool hi, uint8_t t_count)
{
	uint32_t y;

	if (hi)
	{
		for (y = 1; y <= m_cellScopeHiY; ++y)
		{	
			m_cellMap[0].insert(m_cellMap[0].begin() + (y * m_cellScopeHiX) + (y - 1) * t_count, t_count, 0);	
			m_cellMapBuffer[0].insert(m_cellMapBuffer[0].begin() + (y * m_cellScopeHiX) + (y - 1) * t_count, t_count, 0);	
		}
		for (y = 1; y <= m_cellScopeLoY; ++y)
		{	
			m_cellMap[1].insert(m_cellMap[1].begin() + (y * m_cellScopeHiX) + (y - 1) * t_count, t_count, 0);	
			m_cellMapBuffer[1].insert(m_cellMapBuffer[1].begin() + (y * m_cellScopeHiX) + (y - 1) * t_count, t_count, 0);	
		}

		m_cellScopeHiX += t_count;
	}
	else
	{
		for (y = 1; y <= m_cellScopeLoY; ++y)
		{	
			m_cellMap[2].insert(m_cellMap[2].begin() + (y * m_cellScopeLoX) + (y - 1) * t_count, t_count, 0);	
			m_cellMapBuffer[2].insert(m_cellMapBuffer[2].begin() + (y * m_cellScopeLoX) + (y - 1) * t_count, t_count, 0);	
		}
		for (y = 1; y <= m_cellScopeHiY; ++y)
		{	
			m_cellMap[3].insert(m_cellMap[3].begin() + (y * m_cellScopeLoX) + (y - 1) * t_count, t_count, 0);	
			m_cellMapBuffer[3].insert(m_cellMapBuffer[3].begin() + (y * m_cellScopeLoX) + (y - 1) * t_count, t_count, 0);	
		}

		m_cellScopeLoX += t_count;
	}
}

uint8_t CellularAutomaton::GetCellState(int32_t t_cellx, int32_t t_celly)
{
	uint8_t plane = IndexCellToPlane(t_cellx, t_celly);
	uint32_t celli = IndexCellInPlane(plane, abs(t_cellx), abs(t_celly));

	if ((plane == 0 && t_cellx <   m_cellScopeHiX && t_celly <   m_cellScopeHiY) ||
	    (plane == 1 && t_cellx <   m_cellScopeHiX && t_celly >= -m_cellScopeLoY) ||
	    (plane == 2 && t_cellx >= -m_cellScopeLoX && t_celly >= -m_cellScopeLoY) ||
	    (plane == 3 && t_cellx >= -m_cellScopeLoX && t_celly <   m_cellScopeHiY))
	{
		return m_cellMap[plane][celli] ? 1 : 0;
	}
	else
	{
		return 0;
	}
}

uint8_t CellularAutomaton::GetNumAliveNeighbors(int32_t t_cellx, int32_t t_celly)
{
	return 
		GetCellState(t_cellx - 1, t_celly - 1) +
		GetCellState(t_cellx    , t_celly - 1) +
		GetCellState(t_cellx + 1, t_celly - 1) +

		GetCellState(t_cellx - 1, t_celly    ) +
		GetCellState(t_cellx + 1, t_celly    ) +
		
		GetCellState(t_cellx - 1, t_celly + 1) +
		GetCellState(t_cellx    , t_celly + 1) +
		GetCellState(t_cellx + 1, t_celly + 1);
}

void CellularAutomaton::IncrementCellState(int32_t t_cellx, int32_t t_celly)
{
	uint8_t plane = IndexCellToPlane(t_cellx, t_celly);
	uint32_t celli = IndexCellInPlane(plane, abs(t_cellx), abs(t_celly));

	m_cellMap[plane][celli] = !m_cellMap[plane][celli];
}
