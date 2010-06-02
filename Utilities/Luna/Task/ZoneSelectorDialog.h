#pragma once

#include "API.h"

#include "Common/Types.h"

namespace Luna 
{
  class ZoneSelectorDialog : public wxDialog 
  {
  protected:
    wxListBox* m_listBox1;
    wxStaticText* m_messageText;
    wxButton* m_button2;
    wxButton* m_button3;
    wxButton* m_button1;

    S_u32& m_SelectedZones;
    u32 m_NumZones;

  public:
    ZoneSelectorDialog( wxWindow* parent, const std::string& title, const std::string& msg, const V_string& zoneNames, S_u32& selectedZones, int id = -1, wxPoint pos = wxDefaultPosition, 
      wxSize size = wxSize( -1, -1 ), int style = wxCAPTION );
    virtual ~ZoneSelectorDialog();

  private:
    void OnOK( wxCommandEvent& event );
    void OnCancel( wxCommandEvent& event );
    void OnAll( wxCommandEvent& event );

    DECLARE_EVENT_TABLE();
  };
}