#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/graphics.h>
#include <wx/dcbuffer.h>
#include <wx/app.h>

#include <algorithm>

#include "../LifeMain.h"
#include "LifeCanvas.h"

LifeCanvas::LifeCanvas(wxWindow *parent, const wxWindowID id, const wxPoint &position, /*const wxSize &size,*/ CellularAutomaton *cellularAutomaton, const uint16_t cellsDisplayedX = CA_CELLS_X, const uint16_t cellsDisplayedY = CA_CELLS_Y, const wxColour &gridColor, const wxColour &deadColor, const wxColour &aliveColor)
	: wxWindow(parent, id, position, wxSize(cellsDisplayedX * CANVAS_CELL_WITH_GRID_PIXELS_X, cellsDisplayedY * CANVAS_CELL_WITH_GRID_PIXELS_Y), wxFULL_REPAINT_ON_RESIZE | wxBORDER_THEME), m_cellularAutomaton(cellularAutomaton)
{
	SetGridColor(gridColor);
	SetDeadColor(deadColor);
	SetAliveColor(aliveColor);

	// Indicate that the program will be painting directly to the background and set the background color (this will ultimately be the grid
	// color)
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	SetBackgroundColour(m_gridColor);

	// Bind event handlers
	Bind(wxEVT_SIZE, &LifeCanvas::OnSize, this);
	Bind(wxEVT_PAINT, &LifeCanvas::OnPaint, this);
	Bind(wxEVT_MOTION, &LifeCanvas::OnMouseHover, this);
	Bind(wxEVT_LEFT_DOWN, &LifeCanvas::OnMouseClick, this);
		
	m_displayScopeLoX = m_cellularAutomaton->GetScopeLoX();
	m_displayScopeLoY = m_cellularAutomaton->GetScopeLoY();

	m_displayScopeHiX = m_cellularAutomaton->GetScopeHiX();
	m_displayScopeHiY = m_cellularAutomaton->GetScopeHiY();
}

LifeCanvas::~LifeCanvas()
{
    //dtor
}

void LifeCanvas::OnSize(wxSizeEvent &event)
{
	int screenX, screenY;
	int16_t diffScopeX, diffScopeY;

	GetSize(&screenX, &screenY);	

	m_displayScopeHiX = m_displayScopeLoX + screenX / CANVAS_CELL_WITH_GRID_PIXELS_X;
	m_displayScopeHiY = m_displayScopeLoY + screenY / CANVAS_CELL_WITH_GRID_PIXELS_Y;

	diffScopeX = m_displayScopeHiX - m_cellularAutomaton->GetScopeHiX();
	diffScopeY = m_displayScopeHiY - m_cellularAutomaton->GetScopeHiY();

	if (diffScopeX > 0)
		m_cellularAutomaton->AddCols(1, diffScopeX);
	if (diffScopeY > 0)
		m_cellularAutomaton->AddRows(1, diffScopeY);
}

void LifeCanvas::MoveOrigin(int32_t t_moveX, int32_t t_moveY) 
{
	int16_t diffScopeX, diffScopeY;

	m_displayScopeLoX += t_moveX;
	m_displayScopeLoY += t_moveY;

	diffScopeX = m_displayScopeHiX - m_cellularAutomaton->GetScopeHiX();
	diffScopeY = m_displayScopeHiY - m_cellularAutomaton->GetScopeHiY();

	if (diffScopeX > 0)
		m_cellularAutomaton->AddCols(1, diffScopeX);
	if (diffScopeY > 0)
		m_cellularAutomaton->AddRows(1, diffScopeY);
}

void LifeCanvas::OnMouseHover(wxMouseEvent &event)
{
	wxInt32 cellx = PxToCell_X(event.GetX());
	wxInt32 celly = PxToCell_Y(event.GetY());

	wxString msg;
	msg.Printf("X: %d Y: %d", cellx, celly);
	((LifeFrame*)wxGetApp().GetTopWindow())->SetStatusText(msg, 1);
}

void LifeCanvas::OnMouseClick(wxMouseEvent &event)
{
	uint32_t cellx = PxToCell_X(event.GetX());
	uint32_t celly = PxToCell_Y(event.GetY());

	((LifeFrame*)GetParent())->DamageCell(cellx, celly);
}

// Event Handlers
void LifeCanvas::OnPaint(wxPaintEvent &event)
{
	int32_t cellx, celly;

	wxPoint rectOrigin;
	wxSize rectSize;

	wxGraphicsContext *gc;

	wxAutoBufferedPaintDC dc(this);

	if (m_gridColorChanged)
	{
		SetBackgroundColour(m_gridColor);
		dc.SetBackground(m_gridColor);
		dc.Clear();
		m_gridColorChanged = false;;
	}

	gc = wxGraphicsContext::Create(dc);

	if (gc)
	{
		rectSize = FromDIP(wxSize(CANVAS_CELL_PIXELS_X, CANVAS_CELL_PIXELS_Y));

		m_displayScopeHiX = (int)m_cellularAutomaton->GetScopeHiX();
		m_displayScopeHiY = (int)m_cellularAutomaton->GetScopeHiY();

		for (celly = m_displayScopeLoY; celly < m_displayScopeHiY; ++celly)
		for (cellx = m_displayScopeLoX; cellx < m_displayScopeHiX; ++cellx)
		{
			rectOrigin = FromDIP(wxPoint((cellx - m_displayScopeLoX) * CANVAS_CELL_WITH_GRID_PIXELS_X, (celly - m_displayScopeLoY) * CANVAS_CELL_WITH_GRID_PIXELS_Y));

			gc->SetBrush(m_cellularAutomaton->GetCellState(cellx, celly) ? m_aliveColor : m_deadColor);
			gc->DrawRectangle(rectOrigin.x, rectOrigin.y, rectSize.GetWidth(), rectSize.GetHeight());
		}

	}

	delete gc;
}
