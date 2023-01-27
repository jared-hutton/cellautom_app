/***************************************************************
 * Name:      LifeApp.cpp
 * Purpose:   Code for Application Class
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

#include "LifeApp.h"
#include "LifeMain.h"

IMPLEMENT_APP(LifeApp);

bool LifeApp::OnInit()
{
    LifeFrame* frame = new LifeFrame( _(APP_TITLE), wxDefaultPosition, wxSize(640, 480));
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();

    return true;
}

void ValidateDefs()
{
	assert(CAINIT_RATIO_AXIS == 0 || CAINIT_RATIO_AXIS == 1);
}
