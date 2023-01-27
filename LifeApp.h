/***************************************************************
 * Name:      LifeApp.h
 * Purpose:   Defines Application Class
 * Author:    Jared Hutton ()
 * Created:   2022-12-24
 * Copyright: Jared Hutton ()
 * License:
 **************************************************************/

#ifndef LIFEAPP_H
#define LIFEAPP_H

#include <wx/app.h>

class LifeApp : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(LifeApp)

#endif // LIFEAPP_H
