#include "../include/Browser.h"
#include "../include/StatusBar.h"

#include <iostream>
#include "TGSplitter.h"

Browser::Browser() {
    file_filter.Connect("FilesSelected(map<string*, string*>*)",
                        "FileViewer",
                        &file_view,
                        "DisplayInTreeView(map<string*, string*>*)");

    file_view.Connect("FileLoaded(map<string*, string*>*)",
                      "Browser",
                      this,
                      "FileLoaded(map<string*, string*>*)");

    file_view.Connect("CloseFile(string*)",
                      "Browser",
                      this,
                      "CloseFile(string*)");


    file_view.Connect("ItemDoubleClicked(TH1*)", "Browser", this, "ReceiveItem(TH1*)");

    file_view.Connect("SignalStatus(string*)", "Browser", this, "SignalStatus(string*)");
}

void Browser::DrawInFrame(TGCompositeFrame* frame) {
    TGVerticalFrame* top_frame = new TGVerticalFrame(frame);

    TGHorizontalFrame* up = new TGHorizontalFrame(top_frame,10, 300, kFixedHeight);
    TGHorizontalFrame* down = new TGHorizontalFrame(top_frame);

    TGHSplitter* hsplitter = new TGHSplitter(top_frame,2,2);
    hsplitter->SetFrame(up, kTRUE);

    file_filter.DrawInFrame(up);
    file_view.DrawInFrame(down);

    top_frame->AddFrame(up,  new TGLayoutHints(kLHintsExpandX));
    top_frame->AddFrame(hsplitter,  new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    top_frame->AddFrame(down,  new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    frame->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
}

void Browser::UpdateLists() {
    Emit("SignalStatus(string*)", new string("Updating content ..."));

    file_filter.FillFromFile(Configuration::Instance().GetValue(Configuration::DATABASEPATH), true);
    file_filter.FillModuleFilters(Configuration::Instance().GetValue(Configuration::DATABASEFILTERSPATH), true) ;
    
    Emit("SignalStatus(string*)", new string("Ready!"));
}

void Browser::ReceiveItem(TH1* t){
    Emit("OpenItemDoubleClicked(TH1*)", t);
}

void Browser::SignalStatus(string* t) {
    Emit("SignalStatus(string*)", t);
}

void Browser::FileLoaded(map<string*, string*>* t) {
    Emit("FileLoaded(map<string*, string*>*)", t);
}

void Browser::CloseFile(string* t) {
    Emit("CloseFile(string*", t);
}
