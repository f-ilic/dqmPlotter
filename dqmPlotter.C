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

#include <iostream>

class DQMPlotter {
    RQ_OBJECT("DQMPlotter")
public:

    DQMPlotter(const char* plugin_name) {

        // Instantiate the correct plugin
        if(std::string(plugin_name).compare("PreviewPlugin") == 0){
            std::cout << "PreviewPlugin" << std::endl;
            plugin = new PreviewPlugin();
        }
        else if (std::string(plugin_name).compare("SuperimposePlugin") == 0){
            std::cout << "SuperimposePlugin" << std::endl;
            plugin = new SuperimposePlugin();
        } else {
            std::cout << std::endl;
            std::cout << " =============== INVALID ===============" << std::endl;
            printHelp();
            return;
        }

        int width = 1000;
        int height = 700;

        // FIRST OF ALL: LOAD CONFIGURATION DATA
        Configuration::Instance("DATA/con.fig");

        main_frame = new TGMainFrame(gClient->GetRoot(), width, height);
        main_frame->SetWindowName("DQM Plotter");
        menu = new MenuBar();
        browser = new Browser();
        status_bar = new StatusBar();

        containter_frame = new TGHorizontalFrame(main_frame);

        browser_area = new TGVerticalFrame(containter_frame, 300, 10, kFixedWidth | kRaisedFrame);
        plugin_area  = new TGCompositeFrame(containter_frame, 700, 10, kRaisedFrame);
        vsplitter = new TGVSplitter(containter_frame, 2, 2);
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

        browser->Connect("FileLoaded(map<string*, string*>*)", "IPlugin", plugin, "ReceiveFileLoaded(map<string*, string*>*)");
        browser->Connect("OpenItemDoubleClicked(TH1*)", "IPlugin", plugin, "Receive(TH1*)");
        browser->Connect("CloseFile(string*)", "IPlugin", plugin, "ReceiveFileClose(string*)");

        menu->Connect("IndexUpdated()", "Browser", browser, "UpdateLists()");

        browser->Connect("SignalStatus(string*)", "StatusBar", status_bar, "ReceiveStatus(string*)");
        plugin->Connect("SignalStatus(string*)", "StatusBar", status_bar, "ReceiveStatus(string*)");
        menu->Connect("SignalStatus(string*)", "StatusBar", status_bar, "ReceiveStatus(string*)");

        // CREATE LOCAL DATA DIRECTORY
        string cmd = "mkdir -p " + Configuration::Instance().GetValue(Configuration::TMPDATADIRECTORY);
        system(cmd.c_str());
    }

public:
    void replaceActivePlugin(std::string* plugin_name) {
        //todo: dynamic swapping
    }
    static void printHelp() {
        std::cout << "The following plugins are available: " << std::endl;
        std::cout << "  * PreviewPlugin" << std::endl;
        std::cout << "  * SuperimposePlugin" << std::endl;

        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << "Example call:" << std::endl;
        std::cout << "root 'dqmPlotter.C(\"PreviewPlugin\")'" << std::endl ;
        std::cout << std::endl;
    }

private:

    MenuBar*   menu;
    Browser*   browser;
    IPlugin*  plugin;
    StatusBar* status_bar;

    TGHorizontalFrame* containter_frame;
    TGVerticalFrame*  browser_area;
    TGCompositeFrame* plugin_area;
    TGVSplitter* vsplitter;
    TGMainFrame* main_frame;
};

void dqmPlotter(const char* plugin_name) {
    DQMPlotter* plotter = new DQMPlotter(plugin_name);
}

void dqmPlotter() {
    DQMPlotter::printHelp();
}
















