#ifndef LIFECANVAS_H
#define LIFECANVAS_H

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include <cstdint>

#include "CellularAutomaton.h"

class LifeCanvas : public wxWindow
{
    public:
	LifeCanvas(wxWindow *parent, const wxWindowID id, const wxPoint &position, /*const wxSize &size,*/ CellularAutomaton *cellularAutomaton, const uint16_t cellsDisplayedX, const uint16_t cellsDisplayedY, const wxColour &gridColor = *wxBLACK, const wxColour &deadColor = *wxBLACK, const wxColour &aliveColor = *wxWHITE);
	virtual ~LifeCanvas();

	// Visual control methods
	void SetGridColor(const wxColor &t_gridColor) { m_gridColor = t_gridColor; m_gridColorChanged = true; }
	void SetDeadColor(const wxColor &t_deadColor) { m_deadColor = t_deadColor; }
	void SetAliveColor(const wxColor &t_aliveColor) { m_aliveColor = t_aliveColor; }

	void MoveOrigin(int32_t t_moveX, int32_t t_moveY);

    private:
	// Event Handlers
	void OnSize(wxSizeEvent &event);
	void OnMouseHover(wxMouseEvent &event);
	void OnMouseClick(wxMouseEvent &event);
	void OnPaint(wxPaintEvent &event);

	// Translate pixel locations to cells
	wxInt32 PxToCell_X(uint32_t px_x) { return m_displayScopeLoX + px_x / CANVAS_CELL_WITH_GRID_PIXELS_X; }
	wxInt32 PxToCell_Y(uint32_t px_y) { return m_displayScopeLoY + px_y / CANVAS_CELL_WITH_GRID_PIXELS_Y; }

	wxColour m_gridColor;
	wxColour m_deadColor;
	wxColour m_aliveColor;

	CellularAutomaton *m_cellularAutomaton;

	int32_t m_displayScopeLoX = 0;
	int32_t m_displayScopeLoY = 0; 

	int32_t m_displayScopeHiX = 16;
	int32_t m_displayScopeHiY = 16;

	uint8_t m_cellDisplayPixelsX;
	uint8_t m_cellDisplayPixelsY;

	bool m_gridColorChanged = false;
};

#endif // LIFECANVAS_H
