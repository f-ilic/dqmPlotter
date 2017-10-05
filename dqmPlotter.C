R__LOAD_LIBRARY(lib/Browser_cpp.so)
R__LOAD_LIBRARY(lib/FileTable_cpp.so)
R__LOAD_LIBRARY(lib/RemoteFileFilter_cpp.so)
R__LOAD_LIBRARY(lib/FileViewer_cpp.so)
R__LOAD_LIBRARY(lib/Configuration_cpp.so)
R__LOAD_LIBRARY(lib/MenuBar_cpp.so)


#include "include/Browser.h"
#include "include/MenuBar.h"

void dqmPlotter() {
  
    int width = 400;
    int height = 1000;
    
    // FIRST OF ALL: LOAD CONFIGURATION DATA
    Configuration::GetConfiguration("DATA/con.fig");
    
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);

    main_frame->SetWindowName("DQM Plotter");

    TGVerticalFrame* left = new TGVerticalFrame(main_frame);

    MenuBar*  menu = new MenuBar();

    Browser* browser = new Browser();

    menu->DrawInFrame(main_frame);
    browser->DrawInFrame(left);

    main_frame->AddFrame(left, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

    main_frame->MapSubwindows();
    main_frame->MapWindow();

    main_frame->Layout();
    main_frame->MoveResize(100, 100, width, height);
    
    // MENU -> BROWSER CONNECTION
    menu->Connect("IndexUpdated()", "Browser", browser, "UpdateLists()");
    
    // CREATE LOCAL DATA DIRECTORY
    string cmd = "mkdir " + Configuration::GetConfiguration().GetValue(Configuration::TMPDATADIRECTORY);
    system(cmd.c_str());
}


class DQMPlotter {
public:

private:
    MenuBar        menu;
    Browser     browser;

};
