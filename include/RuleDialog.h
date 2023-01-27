#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include <cstdint>

#include <wx/dialog.h>
#include <wx/textctrl.h>

class RuleDialog : public wxDialog
{
    public:
        RuleDialog(wxWindow *parent, wxWindowID id, const wxString &title, const uint32_t defaultRule);
        virtual ~RuleDialog();

	uint32_t GetRule();

    protected:

    private:
	enum 
	{
		idOK = 2000,
		idCancel	
	};

	// Event Handlers
	void OnOK(wxCommandEvent &event);
	void OnCancel(wxCommandEvent &event);

	const wxString BirthStringFromRule(const uint32_t t_rule);
	const wxString SurviveStringFromRule(const uint32_t t_rule);

	wxTextCtrl *textCtrl1;
	wxTextCtrl *textCtrl2;

	uint32_t m_defRule;
};

#endif // RULEDIALOG_H
