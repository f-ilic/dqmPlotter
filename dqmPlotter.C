#include "Browser.cpp"
#include "TopMenu.cpp"

void dqmPlotter() {
    int width = 400;
    int height = 800;
    
    // FIRST OF ALL: LOAD CONFIGURATION DATA
    Configuration::GetConfiguration("DATA/con.fig");
    
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);

    main_frame->SetWindowName("Browser");
    Menu*  menu = new Menu();
    Browser* browser = new Browser();

    menu->DrawInFrame(main_frame);
    browser->DrawInFrame(main_frame);

    main_frame->MapSubwindows();
    main_frame->MapWindow();

    main_frame->Layout();
    main_frame->MoveResize(100, 100, width, height);
}


