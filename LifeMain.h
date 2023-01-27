/***************************************************************
 * Name:      LifeMain.h
 * Purpose:   Defines Application Frame
 * Author:    Jared Hutton ()
 * Created:   2022-12-24
 * Copyright: Jared Hutton ()
 * License:
 **************************************************************/

#ifndef LIFEMAIN_H
#define LIFEMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <bitset>

#include "LifeApp.h"
#include "LifeCanvas.h"
#include "CellularAutomaton.h"

class LifeFrame: public wxFrame
{
    public:
        LifeFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
        ~LifeFrame();

	// Cellular Automaton Interactions
	void CANextTick();
	void RandomizeCells();
	void DamageCell(int32_t t_cellx, int32_t t_celly);

    private:
        enum
        {
        	idMenuQuit = 1000,
		idMenuNew,
		idMenuRule,
		idMenuRandomize,
        	idMenuAbout,
		idMenuGridColor,
		idMenuDeadColor,
		idMenuAliveColor,
		idTimer,
		idNext,
		idPlay,
		idStop,
		idRestart,
		idMoveUp,
		idMoveRight,
		idMoveDown,
		idMoveLeft
        };

	// Event Handlers
	void OnClose(wxCloseEvent &event);
	void OnNew(wxCommandEvent &event);
	void OnQuit(wxCommandEvent &event);

	void OnGridColor(wxCommandEvent &event);
	void OnDeadColor(wxCommandEvent &event);
	void OnAliveColor(wxCommandEvent &event);

	void OnRule(wxCommandEvent &event);
	void OnRandomize(wxCommandEvent &event);

	void OnAbout(wxCommandEvent &event);

	void OnLifeNext(wxCommandEvent &event);
	void OnLifePlay(wxCommandEvent &event);
	void OnLifeStop(wxCommandEvent &event);
	void OnRestart(wxCommandEvent &event);

	void OnMoveUp(wxCommandEvent &event);
	void OnMoveRight(wxCommandEvent &event);
	void OnMoveDown(wxCommandEvent &event);
	void OnMoveLeft(wxCommandEvent &event);

	void OnTimer(wxTimerEvent &event);

	LifeCanvas *m_canvas;

	wxTimer *m_timer;

	CellularAutomaton *m_cellularAutomaton;

	std::array<std::vector<bool>, 4> m_restartMap;

	bool m_caRunning = false;
	bool m_caStarted = false;

	uint32_t m_caTick = 0;
	uint32_t m_rngReseeds = 0;
};


#endif // LIFEMAIN_H
