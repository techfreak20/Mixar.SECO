#include "MainFrame.h"
#include <wx/wx.h>
#include <wx/splitter.h>



wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_OPEN, MainFrame::OnOpenImage)
    EVT_MENU(wxID_SAVE, MainFrame::OnSaveImage)
    EVT_MENU(wxID_EXIT, MainFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()



MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);
   

    //MENU BAR

    //File Menu
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, "&Open Image...\tCtrl+O");
    fileMenu->Append(wxID_SAVE, "&Save Image...\tCtrl+S");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "&Exit\tAlt+F4");

    // Edit Menu
    wxMenu* editMenu = new wxMenu;
    editMenu->Append(wxID_UNDO, "&Undo\tCtrl+Z");
    editMenu->Append(wxID_REDO, "&Redo\tCtrl+Y");
    editMenu->AppendSeparator();
    editMenu->Append(wxID_ANY, "&Clear All Nodes");

    // Help Menu
    wxMenu* helpMenu = new wxMenu;
    helpMenu->Append(wxID_ABOUT, "&About");

    // Create the menu bar and attach the menus
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(fileMenu, "&File");
    menuBar->Append(editMenu, "&Edit");
    menuBar->Append(helpMenu, "&Help");

    // Attach menu bar to the frame
    SetMenuBar(menuBar);


    Bind(wxEVT_MENU, &MainFrame::OnOpenImage, this, wxID_OPEN);


    // === Splitter Window ===
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
    splitter->SetSashGravity(0.2); // 20% for left, 80% for right
    splitter->SetMinimumPaneSize(150); // minimum width for each pane

    // === Left Panel (Toolbox / Nodes) ===
    wxPanel* leftPanel = new wxPanel(splitter);
    leftPanel->SetBackgroundColour(wxColour(13, 27, 42));

    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    //Title Node
    wxStaticText* nodeLabel = new wxStaticText(leftPanel, wxID_ANY, "Nodes");
    wxFont nodeFont = nodeLabel->GetFont();
    nodeFont.SetPointSize(25);         // Increase size (e.g., 12pt)
    nodeFont.SetWeight(wxFONTWEIGHT_BOLD); // Make it bold
    nodeLabel->SetFont(nodeFont);
    nodeLabel->SetForegroundColour(*wxWHITE);  // White text on dark bg
    leftSizer->Add(nodeLabel, 0, wxALL, 10);


    //Basic Node
    wxStaticText* basicLabel = new wxStaticText(leftPanel, wxID_ANY, "Basic Nodes");
    wxFont basicFont = basicLabel->GetFont();
    basicFont.SetPointSize(15);         
    basicFont.SetWeight(wxFONTWEIGHT_BOLD); 
    basicLabel->SetFont(basicFont);
    basicLabel->SetForegroundColour(*wxWHITE); 
    leftSizer->Add(basicLabel, 0, wxALL, 10);


    leftSizer->Add(new wxButton(leftPanel, wxID_OPEN, "Open Image"), 0, wxALL | wxEXPAND, 5);
    leftSizer->Add(new wxButton(leftPanel, wxID_ANY, "Noise Generator"), 0, wxALL | wxEXPAND, 5);
    leftPanel->SetSizer(leftSizer);


    // PROCESSING SECTION
    wxStaticText* processingLabel = new wxStaticText(leftPanel, wxID_ANY, "Processing Nodes");
    wxFont processingFont = processingLabel->GetFont();
    processingFont.SetPointSize(15);
    processingFont.SetWeight(wxFONTWEIGHT_BOLD);
    processingLabel->SetFont(processingFont);
    processingLabel->SetForegroundColour(*wxWHITE);
    leftSizer->Add(processingLabel, 0, wxALL, 10);

    
    
    //Brightness
    wxButton* brightnessButton = new wxButton(leftPanel, wxID_ANY, "Brightness");
    leftSizer->Add(brightnessButton, 0, wxALL | wxEXPAND, 5);
    brightnessButton->Bind(wxEVT_BUTTON, &MainFrame::OnBrightnessImage, this);

    //Blur
    wxButton* blurButton = new wxButton(leftPanel, wxID_ANY, "Blur");
    leftSizer->Add(blurButton, 0, wxALL | wxEXPAND, 5);
    blurButton->Bind(wxEVT_BUTTON, &MainFrame::OnBlurImage, this);


    leftSizer->Add(new wxButton(leftPanel, wxID_ANY, "Threshold"), 0, wxALL | wxEXPAND, 5);
    leftSizer->Add(new wxButton(leftPanel, wxID_ANY, "Edge Detection"), 0, wxALL | wxEXPAND, 5);
    leftSizer->Add(new wxButton(leftPanel, wxID_ANY, "Convolution Filter"), 0, wxALL | wxEXPAND, 5);
    leftSizer->Add(new wxButton(leftPanel, wxID_ANY, "Color Channel Splitter"), 0, wxALL | wxEXPAND, 5);
    leftSizer->Add(new wxButton(leftPanel, wxID_ANY, "Blend"), 0, wxALL | wxEXPAND, 5);

    leftPanel->SetSizer(leftSizer);



    // === Right Panel (Canvas + Properties) ===
    wxPanel* rightPanel = new wxPanel(splitter);
    rightPanel->SetBackgroundColour(*wxWHITE);

    // Outer vertical sizer for right panel
    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

    // === Image Label ===
    wxStaticText* imageLabel = new wxStaticText(rightPanel, wxID_ANY, "Image Preview:");
    imageLabel->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    rightSizer->Add(imageLabel, 0, wxALL, 10);

    // === Image Canvas / Placeholder ===
    
    imageBox = new wxStaticBitmap(rightPanel, wxID_ANY, wxNullBitmap,
        wxPoint(20, 20), wxSize(400, 300));

    rightSizer->Add(imageBox, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 10); // Expand to use available space

    // === Properties Panel Section ===
    propertiesPanel = new wxPanel(rightPanel);
    propertiesPanel->SetBackgroundColour(wxColour(15, 20, 35)); // Dark background

    propertiesSizer = new wxBoxSizer(wxVERTICAL);

    // Properties Label
    wxStaticText* propertiesLabel = new wxStaticText(propertiesPanel, wxID_ANY, "Properties Panel");
    propertiesLabel->SetForegroundColour(*wxWHITE);
    wxFont labelFont = propertiesLabel->GetFont();
    labelFont.SetPointSize(11);
    labelFont.SetWeight(wxFONTWEIGHT_BOLD);
    propertiesLabel->SetFont(labelFont);
    propertiesSizer->Add(propertiesLabel, 0, wxALL, 10);

    // Placeholder for node properties
    wxStaticText* placeholderText = new wxStaticText(propertiesPanel, wxID_ANY, "No node selected.");
    placeholderText->SetForegroundColour(wxColour(220, 220, 220));
    propertiesSizer->Add(placeholderText, 0, wxLEFT | wxBOTTOM, 174);

    // Apply sizer to properties panel
    propertiesPanel->SetSizer(propertiesSizer);

    // Add properties panel to right side (bottom)
    rightSizer->Add(propertiesPanel, 0, wxEXPAND | wxBOTTOM, 10);  // Do not expand vertically

    // Apply final sizer to right panel
    rightPanel->SetSizer(rightSizer);

    

    // === Split the window horizontally ===
    splitter->SplitVertically(leftPanel, rightPanel, 200);


    // Wrap splitter inside a sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(splitter, 1, wxEXPAND, 0); // Fills the entire frame
    SetSizer(mainSizer);



    // Optional: Create a status bar
    CreateStatusBar();
    SetStatusText("Welcome to Node-Based Image Simulator!");
}




void MainFrame::OnOpenImage(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open Image file", "", "",
        "Image files (*.png;*.jpg;*.bmp)|*.png;*.jpg;*.bmp",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    wxString path = openFileDialog.GetPath();
    

    wxImage image;
    if (!image.LoadFile(path)) {
        wxLogError("Cannot load image at path: %s", path);
        return;
    }

    if (image.IsOk()) {
        originalImage = image.Copy();
        currentImage = originalImage.Copy();
        image = image.Scale(400, 300);
        wxBitmap bitmap(image);
        imageBox->SetBitmap(bitmap);
        Layout();  
    }

    
}

void MainFrame::OnSaveImage(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save Image File", "", "",
        "PNG files (*.png)|*.png|JPEG files (*.jpg)|*.jpg", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString path = saveFileDialog.GetPath();
        wxLogMessage("Saved image: %s", path);
        // TODO: Save image logic
    }
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);  // Close the window
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Node-Based Image Simulator\nUsing wxWidgets",
        "About", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnBlurImage(wxCommandEvent& event) {
    if (!originalImage.IsOk()) {
        wxLogError("No image loaded.");
        return;
    }

    
    propertiesSizer->Clear(true);

    
    wxStaticText* label = new wxStaticText(propertiesPanel, wxID_ANY, "Blur Intensity");
    label->SetForegroundColour(*wxWHITE);
    propertiesSizer->Add(label, 0, wxALL, 10);

   
    blurSlider = new wxSlider(propertiesPanel, wxID_ANY, 5, 1, 15,
        wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    propertiesSizer->Add(blurSlider, 0, wxALL | wxEXPAND, 10);

    // Bind slider event
    blurSlider->Bind(wxEVT_SLIDER, &MainFrame::OnBlurSliderChanged, this);

    propertiesPanel->Layout();
}


void MainFrame::OnBlurSliderChanged(wxCommandEvent& event) {
    int radius = blurSlider->GetValue();
    

    if (!originalImage.IsOk())
        return;

    currentImage = originalImage.Copy().Blur(radius);
    wxImage scaled = currentImage.Scale(400, 300);
    wxBitmap bitmap(scaled);
    imageBox->SetBitmap(bitmap);
    Layout();
}

void MainFrame::OnBrightnessImage(wxCommandEvent& event) {
    if (!originalImage.IsOk()) {
        wxLogError("No image loaded.");
        return;
    }

    // Clear previous controls in properties panel
    propertiesSizer->Clear(true);

    // Add title
    wxStaticText* label = new wxStaticText(propertiesPanel, wxID_ANY, "Brightness Adjustment");
    label->SetForegroundColour(*wxWHITE);
    propertiesSizer->Add(label, 0, wxALL, 10);

    // Create slider: min=-100, max=100, initial=0 (no adjustment)
    brightnessSlider = new wxSlider(propertiesPanel, wxID_ANY, 0, -100, 100,
        wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    propertiesSizer->Add(brightnessSlider, 0, wxALL | wxEXPAND, 10);

    // Bind slider event
    brightnessSlider->Bind(wxEVT_SLIDER, &MainFrame::OnBrightnessSliderChanged, this);

    propertiesPanel->Layout();
}


void MainFrame::OnBrightnessSliderChanged(wxCommandEvent& event) {
    int brightness = brightnessSlider->GetValue();

    if (!originalImage.IsOk())
        return;

    // Create a copy of the original image
    currentImage = originalImage.Copy();

    int width = currentImage.GetWidth();
    int height = currentImage.GetHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int r = currentImage.GetRed(x, y) + brightness;
            int g = currentImage.GetGreen(x, y) + brightness;
            int b = currentImage.GetBlue(x, y) + brightness;

            // Clamp values to 0-255
            r = std::max(0, std::min(255, r));
            g = std::max(0, std::min(255, g));
            b = std::max(0, std::min(255, b));

            currentImage.SetRGB(x, y, r, g, b);
        }
    }

    // Scale and display the modified image
    wxImage scaled = currentImage.Scale(400, 300);
    imageBox->SetBitmap(wxBitmap(scaled));
    Layout();
}



