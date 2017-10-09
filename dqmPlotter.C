R__LOAD_LIBRARY(lib/Browser_cpp.so)
R__LOAD_LIBRARY(lib/FileTable_cpp.so)
R__LOAD_LIBRARY(lib/RemoteFileFilter_cpp.so)
R__LOAD_LIBRARY(lib/FileViewer_cpp.so)
R__LOAD_LIBRARY(lib/Configuration_cpp.so)
R__LOAD_LIBRARY(lib/MenuBar_cpp.so)

#include "include/Browser.h"
#include "include/MenuBar.h"
#include "include/StatusBar.h"
#include "include/IPlugin.h"
#include "TGSplitter.h"

void dqmPlotter() {
    int width = 1000;
    int height = 700;
    
    // FIRST OF ALL: LOAD CONFIGURATION DATA
    Configuration::Instance("DATA/con.fig");
    
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);
    main_frame->SetWindowName("DQM Plotter");


    MenuBar*  menu = new MenuBar();
    Browser* browser = new Browser();
    IPlugin* plugin = new ComparisonPlugin();
    StatusBar* status_bar = new StatusBar();

    TGHorizontalFrame* containter_frame = new TGHorizontalFrame(main_frame);

    TGVerticalFrame*  browser_area = new TGVerticalFrame(containter_frame, 300, 10, kFixedWidth | kRaisedFrame);
    TGCompositeFrame* plugin_area  = new TGCompositeFrame(containter_frame, 700, 10, kRaisedFrame);
    TGVSplitter* vsplitter = new TGVSplitter(containter_frame, 2, 2);
    vsplitter->SetFrame(browser_area, kTRUE);

    menu->DrawInFrame(main_frame);
    browser->DrawInFrame(browser_area);
    plugin->DrawInFrame(plugin_area);

    containter_frame->AddFrame(browser_area, new TGLayoutHints(kLHintsExpandY));
    containter_frame->AddFrame(vsplitter,  new TGLayoutHints(kLHintsExpandY, 5, 5, 5, 5));
    containter_frame->AddFrame(plugin_area, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    main_frame->AddFrame(containter_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    status_bar->DrawInFrame(main_frame);

    main_frame->MapSubwindows();
    main_frame->MapWindow();
    main_frame->Layout();
    
    browser->Connect("OpenItemDoubleClicked(TH1*)", "IPlugin", plugin, "Receive(TH1*)");
    menu->Connect("IndexUpdated()", "Browser", browser, "UpdateLists()");
    
    browser->Connect("SignalStatus(string*)", "StatusBar", status_bar, "ReceiveStatus(string*)");
    plugin->Connect("SignalStatus(string*)", "StatusBar", status_bar, "ReceiveStatus(string*)");

    // CREATE LOCAL DATA DIRECTORY
    string cmd = "mkdir -p " + Configuration::Instance().GetValue(Configuration::TMPDATADIRECTORY);
    system(cmd.c_str());
}


class DQMPlotter {
public:

private:
    MenuBar   menu;
    Browser   browser;
//    IPlugin*  plugin;
    StatusBar status_bar;
};
