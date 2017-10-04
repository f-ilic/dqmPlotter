R__LOAD_LIBRARY(lib/Browser_cpp.so)
R__LOAD_LIBRARY(lib/FileTable_cpp.so)
R__LOAD_LIBRARY(lib/RemoteFileFilter_cpp.so)
R__LOAD_LIBRARY(lib/FileViewer_cpp.so)
R__LOAD_LIBRARY(lib/Configuration_cpp.so)
R__LOAD_LIBRARY(lib/TopMenu_cpp.so)


#include "include/Browser.h"
#include "include/TopMenu.h"

void dqmPlotter() {
  
    int width = 1000;
    int height = 600;
    
    // FIRST OF ALL: LOAD CONFIGURATION DATA
    Configuration::GetConfiguration("DATA/con.fig");
    
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);

    main_frame->SetWindowName("DQM Plotter");
    TGHorizontalFrame* left = new TGHorizontalFrame(main_frame);

    TopMenu*  menu = new TopMenu();    
    Browser* browser = new Browser();

    menu->DrawInFrame(main_frame);
    browser->DrawInFrame(left);

    main_frame->AddFrame(left, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

    main_frame->MapSubwindows();
    main_frame->MapWindow();

    main_frame->Layout();
    main_frame->MoveResize(100, 100, width, height);
    
    // MENU -> BROWSER CONNECTION
    menu->Connect("UpdateIndex(void)", "Browser", browser, "UpdateLists(void)");
}


