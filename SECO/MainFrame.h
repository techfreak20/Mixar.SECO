#pragma once
#include <wx/wx.h>
#include <wx/statbmp.h>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    // Event handler functions
    void OnOpenImage(wxCommandEvent& event);
    void OnSaveImage(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnBlurImage(wxCommandEvent& event);
    void OnBlurSliderChanged(wxCommandEvent& event);
    void OnBrightnessImage(wxCommandEvent& event);
    void OnBrightnessSliderChanged(wxCommandEvent& event);



    wxStaticBitmap* imageBox;
    wxImage currentImage;
    wxPanel* propertiesPanel;
    wxBoxSizer* propertiesSizer = nullptr;
    wxSlider* blurSlider ;
    wxSlider* brightnessSlider;
    wxImage originalImage;

    // Declare the event table
    wxDECLARE_EVENT_TABLE();
};

