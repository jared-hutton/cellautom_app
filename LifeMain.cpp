/***************************************************************
 * Name:      LifeMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Jared Hutton ()
 * Created:   2022-12-24
 * Copyright: Jared Hutton ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <random>
#ifdef DEBUG
	#include <cstdio>
#endif

#include <wx/colordlg.h>

#include "RuleDialog.h"
#include "LifeMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

LifeFrame::LifeFrame(const wxString &title, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize)
    : wxFrame(NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
	// Menu initialization
	wxMenuBar *menuBar = new wxMenuBar();
	wxMenu *fileMenu = new wxMenu();
	wxMenu *automatonMenu = new wxMenu();
	wxMenu *styleMenu = new wxMenu();
	wxMenu *helpMenu = new wxMenu();

	fileMenu->Append(idMenuNew, _("&New\tCtrl-N"), _("Clear the current automaton state"));
	fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
	automatonMenu->Append(idMenuRule, _("Rule"), _("Change automaton rule"));
	automatonMenu->Append(idMenuRandomize, _("Randomize"), _("Randomize cells"));
	styleMenu->Append(idMenuGridColor, _("Grid Color"), _("Change grid color"));
	styleMenu->Append(idMenuDeadColor, _("Dead Color"), _("Change color of dead cells"));
	styleMenu->Append(idMenuAliveColor, _("Alive Color"), _("Change color of living cells"));
    	helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));

	menuBar->Append(fileMenu, _("&File"));
	menuBar->Append(automatonMenu, _("Automaton"));
	menuBar->Append(styleMenu, _("&Style"));
	menuBar->Append(helpMenu, _("&Help"));

	SetMenuBar(menuBar);

	Bind(wxEVT_MENU, &LifeFrame::OnNew, this, idMenuNew);
	Bind(wxEVT_MENU, &LifeFrame::OnQuit, this, idMenuQuit);

	Bind(wxEVT_MENU, &LifeFrame::OnRule, this, idMenuRule);
	Bind(wxEVT_MENU, &LifeFrame::OnRandomize, this, idMenuRandomize);

	Bind(wxEVT_MENU, &LifeFrame::OnGridColor, this, idMenuGridColor);
	Bind(wxEVT_MENU, &LifeFrame::OnDeadColor, this, idMenuDeadColor);
	Bind(wxEVT_MENU, &LifeFrame::OnAliveColor, this, idMenuAliveColor);

	Bind(wxEVT_MENU, &LifeFrame::OnAbout, this, idMenuAbout);

	// Tool Images initialization
	wxImage toolImages[NUM_TOOL_IMAGES];

	toolImages[0] = wxImage(IMGPTH_TLNEXT, wxBITMAP_TYPE_BMP);
	toolImages[1] = wxImage(IMGPTH_TLPLAY, wxBITMAP_TYPE_BMP);
	toolImages[2] = wxImage(IMGPTH_TLSTOP, wxBITMAP_TYPE_BMP);
	toolImages[3] = wxImage(IMGPTH_TLRESTART, wxBITMAP_TYPE_BMP);
	toolImages[4] = wxImage(IMGPTH_TLMVUP, wxBITMAP_TYPE_BMP);
	toolImages[5] = wxImage(IMGPTH_TLMVRIGHT, wxBITMAP_TYPE_BMP);
	toolImages[6] = wxImage(IMGPTH_TLMVDOWN, wxBITMAP_TYPE_BMP);
	toolImages[7] = wxImage(IMGPTH_TLMVLEFT, wxBITMAP_TYPE_BMP);

	// Setting default masks since bitmaps do not support transparency
	for (size_t i = 0; i < NUM_TOOL_IMAGES; ++i)
		toolImages[i].SetMaskColour(toolImages[i].GetRed(0, 0), toolImages[i].GetGreen(0, 0), toolImages[i].GetBlue(0, 0));

	// Toolbar initialization
	wxToolBar *toolBar = CreateToolBar(wxTB_HORIZONTAL);

	toolBar->SetMargins(5, 5);
	toolBar->SetToolBitmapSize(wxSize(16, 16));

	toolBar->AddTool(idNext, _("Next Tick"), wxBitmap(toolImages[0]));
	toolBar->AddTool(idPlay, _("Play"), wxBitmap(toolImages[1]));
	toolBar->AddTool(idStop, _("Stop"), wxBitmap(toolImages[2]));
	toolBar->AddTool(idRestart, _("Restart"), wxBitmap(toolImages[3]));
	toolBar->AddTool(idMoveUp, _("Move Up"), wxBitmap(toolImages[4]));
	toolBar->AddTool(idMoveRight, _("Move Right"), wxBitmap(toolImages[5]));
	toolBar->AddTool(idMoveDown, _("Move Down"), wxBitmap(toolImages[6]));
	toolBar->AddTool(idMoveLeft, _("Move Left"), wxBitmap(toolImages[7]));

	toolBar->Realize();

	toolBar->EnableTool(idStop, false);

	Bind(wxEVT_TOOL, &LifeFrame::OnLifeNext, this, idNext);
	Bind(wxEVT_TOOL, &LifeFrame::OnLifePlay, this, idPlay);
	Bind(wxEVT_TOOL, &LifeFrame::OnLifeStop, this, idStop);
	Bind(wxEVT_TOOL, &LifeFrame::OnRestart, this, idRestart);
	Bind(wxEVT_TOOL, &LifeFrame::OnMoveUp, this, idMoveUp);
	Bind(wxEVT_TOOL, &LifeFrame::OnMoveRight, this, idMoveRight);
	Bind(wxEVT_TOOL, &LifeFrame::OnMoveDown, this, idMoveDown);
	Bind(wxEVT_TOOL, &LifeFrame::OnMoveLeft, this, idMoveLeft);

	// Status Bar Initialization
	CreateStatusBar(2);
	SetStatusText(_("Hello Code::Blocks user!"),0);
	SetStatusText(wxbuildinfo(short_f), 1);

	// Initialize cellular automaton
	m_cellularAutomaton = new CellularAutomaton(16, 16, 16, 16);

	// Initialize sizer and canvas
	wxSizer *sizer = new wxBoxSizer(wxVERTICAL);

	m_canvas = new LifeCanvas(this, wxID_ANY, wxDefaultPosition, /*FromDIP(wxSize(CANVAS_SIZE_X, CANVAS_SIZE_Y)),*/ m_cellularAutomaton, 16, 16, CANVAS_GRID_COLOR, CANVAS_CELL_DEAD_COLOR, CANVAS_CELL_ALIVE_COLOR);
	sizer->Add(m_canvas, 1, wxEXPAND | wxALL, 0);
	SetSizerAndFit(sizer);

	// Initialize timer
	m_timer = new wxTimer(this, idTimer);

	Bind(wxEVT_TIMER, &LifeFrame::OnTimer, this, idTimer);

	// Bind default event handlers
	Bind(wxEVT_CLOSE_WINDOW, &LifeFrame::OnClose, this);
}

LifeFrame::~LifeFrame()
{
	delete m_canvas;
}

void LifeFrame::OnClose(wxCloseEvent &event)
{
	Destroy();
}

void LifeFrame::OnNew(wxCommandEvent &event)
{
	m_caStarted = false;

	m_cellularAutomaton->SetCellStates(0);

	m_canvas->Refresh();
}

void LifeFrame::OnQuit(wxCommandEvent &event)
{
	Destroy();
}

void LifeFrame::OnRule(wxCommandEvent &event)
{
	if (!m_caStarted)
	{
		RuleDialog ruleDialog(this, wxID_ANY, "Automaton Rule", m_cellularAutomaton->GetRule());
	
		if (ruleDialog.ShowModal() == wxID_OK)
		{
			m_cellularAutomaton->SetRule(ruleDialog.GetRule());	
		}
	}
}

void LifeFrame::OnRandomize(wxCommandEvent &event)
{
	if (!m_caStarted)
	{
		RandomizeCells();
		
		m_canvas->Refresh();
	}
}

void LifeFrame::OnGridColor(wxCommandEvent &event)
{
	wxColourDialog dialog(this);
	
	if (dialog.ShowModal() == wxID_OK)
		m_canvas->SetGridColor(dialog.GetColourData().GetColour());

	m_canvas->Refresh();
}

void LifeFrame::OnDeadColor(wxCommandEvent &event)
{
	wxColourDialog dialog(this);
	
	if (dialog.ShowModal() == wxID_OK)
		m_canvas->SetDeadColor(dialog.GetColourData().GetColour());

	m_canvas->Refresh();
}

void LifeFrame::OnAliveColor(wxCommandEvent &event)
{
	wxColourDialog dialog(this);
	
	if (dialog.ShowModal() == wxID_OK)
		m_canvas->SetAliveColor(dialog.GetColourData().GetColour());

	m_canvas->Refresh();
}


void LifeFrame::OnAbout(wxCommandEvent &event)
{
	wxString msg = wxbuildinfo(long_f);
	wxMessageBox(msg, _("Welcome to..."));
}

void LifeFrame::OnLifeNext(wxCommandEvent &event)
{
	if (!m_caStarted)
	{
		m_restartMap = m_cellularAutomaton->GetCellMap();
		m_caStarted = true;
	}

	CANextTick();
}

void LifeFrame::OnLifePlay(wxCommandEvent &event)
{
	if (!m_caStarted)
	{
		m_restartMap = m_cellularAutomaton->GetCellMap();
		m_caStarted = true;
	}

	m_caRunning = true;

	m_timer->Start(TIMER_INTERVAL);

	GetToolBar()->EnableTool(idPlay, false);
	GetToolBar()->EnableTool(idNext, false);
	GetToolBar()->EnableTool(idStop, true);
	GetToolBar()->EnableTool(idRestart, false);

	CANextTick();
}

void LifeFrame::OnLifeStop(wxCommandEvent &event)
{
	m_caRunning = false;

	m_timer->Stop();

	GetToolBar()->EnableTool(idPlay, true);
	GetToolBar()->EnableTool(idNext, true);
	GetToolBar()->EnableTool(idStop, false);
	GetToolBar()->EnableTool(idRestart, true);

	m_canvas->Refresh();
}

void LifeFrame::OnRestart(wxCommandEvent &event)
{
	m_cellularAutomaton->SetCellMap(m_restartMap);
	m_caStarted = false;

	m_canvas->Refresh();
}

void LifeFrame::OnMoveUp(wxCommandEvent& event)
{
	m_canvas->MoveOrigin(0, -1);
	m_canvas->Refresh();
}

void LifeFrame::OnMoveRight(wxCommandEvent& event)
{
	m_canvas->MoveOrigin(1, 0);
	m_canvas->Refresh();
}


void LifeFrame::OnMoveDown(wxCommandEvent& event)
{
	m_canvas->MoveOrigin(0, 1);
	m_canvas->Refresh();
}


void LifeFrame::OnMoveLeft(wxCommandEvent& event)
{
	m_canvas->MoveOrigin(-1, 0);
	m_canvas->Refresh();
}


void LifeFrame::RandomizeCells()
{
}

void LifeFrame::OnTimer(wxTimerEvent &event)
{
	CANextTick();
}

void LifeFrame::CANextTick()
{
	m_cellularAutomaton->NextTick();
	m_canvas->Refresh();
}

/* DamageCell
 *
 * Sets the CA cell at the given coordinates based on the current damage rule
 *
 */
void LifeFrame::DamageCell(int32_t t_cellx, int32_t t_celly)
{
	if (!m_caRunning)
	{
	}
	m_cellularAutomaton->IncrementCellState(t_cellx, t_celly);
	m_canvas->Refresh();
}
