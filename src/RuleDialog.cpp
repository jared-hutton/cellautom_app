#include "RuleDialog.h"

#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/valtext.h>

RuleDialog::RuleDialog(wxWindow *parent, wxWindowID id, const wxString &title, const uint32_t defaultRule = 0)
	: wxDialog(parent, id, title)
{
	wxBoxSizer *sizer1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer *sizer4 = new wxBoxSizer(wxHORIZONTAL);

	wxTextValidator numericValidator(wxFILTER_DIGITS);

	wxStaticText *staticText1 = new wxStaticText(this, wxID_ANY, _("B"));
	textCtrl1 = new wxTextCtrl(this, wxID_ANY, BirthStringFromRule(defaultRule), wxDefaultPosition, wxDefaultSize, 0, numericValidator);

	wxStaticText *staticText2 = new wxStaticText(this, wxID_ANY, _("S"));
	textCtrl2 = new wxTextCtrl(this, wxID_ANY, SurviveStringFromRule(defaultRule), wxDefaultPosition, wxDefaultSize, 0, numericValidator);

	wxButton *button1 = new wxButton(this, idOK, _("OK"));
	wxButton *button2 = new wxButton(this, idCancel, _("Cancel"));

	sizer1->Add(sizer2, wxEXPAND);
	sizer1->Add(sizer3, wxEXPAND);
	sizer1->Add(sizer4, wxEXPAND);

	sizer2->Add(staticText1, wxEXPAND);
	sizer2->Add(textCtrl1, wxEXPAND);

	sizer3->Add(staticText2, wxEXPAND);
	sizer3->Add(textCtrl2, wxEXPAND);

	sizer4->Add(button1, wxEXPAND);
	sizer4->Add(button2, wxEXPAND);

	SetSizerAndFit(sizer1);

	Bind(wxEVT_BUTTON, &RuleDialog::OnOK, this, idOK);
	Bind(wxEVT_BUTTON, &RuleDialog::OnCancel, this, idCancel);
}

RuleDialog::~RuleDialog()
{
    //dtor
}

uint32_t RuleDialog::GetRule()
{
	uint32_t rule = 0x000;

	char *brthStr = textCtrl1->GetValue().char_str();
	char *survStr = textCtrl2->GetValue().char_str();

	char *c;

	for (c = brthStr; *c; ++c)
		rule = rule | (0x1 << (*c - 48));

	for (c = survStr; *c; ++c)
		rule = rule | (0x1 << (*c - 39));

	return rule;
}

void RuleDialog::OnOK(wxCommandEvent &event)
{
	EndModal(wxID_OK);
}

void RuleDialog::OnCancel(wxCommandEvent &event)
{
	EndModal(wxID_CANCEL);
}

const wxString RuleDialog::BirthStringFromRule(const uint32_t t_rule)
{
	wxString outString("");
	uint8_t biti;

	for (biti = 0; biti < 9; ++biti)
		if ((t_rule >> biti) & 0x1)
			outString.Append((char)(biti + 48));

	return outString;
}

const wxString RuleDialog::SurviveStringFromRule(const uint32_t t_rule)
{
	wxString outString("");
	uint8_t biti;

	for (biti = 0; biti < 9; ++biti)
		if ((t_rule >> (biti + 9)) & 0x1)
			outString.Append((char)(biti + 48));

	return outString;
}
