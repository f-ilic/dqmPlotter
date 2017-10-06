#include "../include/Browser.h"
#include "../include/StatusBar.h"

#include <iostream>
#include "TGSplitter.h"

Browser::Browser() {

    file_filter.Connect("FilesSelected(map<string*, string*>*)",
                        "FileViewer",
                        &file_view,
                        "DisplayInTreeView(map<string*, string*>*)");

    file_view.Connect("ItemDoubleClicked(TH1*)", "Browser", this, "ReceiveItem(TH1*)");
}

void Browser::DrawInFrame(TGCompositeFrame *mf) {
    TGVerticalFrame* top_frame = new TGVerticalFrame(mf);

    TGHorizontalFrame* up = new TGHorizontalFrame(top_frame,10, 300, kFixedHeight);
    TGHorizontalFrame* down = new TGHorizontalFrame(top_frame);

    TGHSplitter* hsplitter = new TGHSplitter(top_frame,2,2);
    hsplitter->SetFrame(up, kTRUE);

    file_filter.DrawInFrame(up);
    file_view.DrawInFrame(down);

    top_frame->AddFrame(up,  new TGLayoutHints(kLHintsExpandX));
    top_frame->AddFrame(hsplitter,  new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    top_frame->AddFrame(down,  new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

    mf->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
}

void Browser::UpdateLists() {
    StatusBar::GetStatusBar().GetStatusBarControl()->SetText("Updating GUI content. Please wait", 0);

    file_filter.FillFromFile(Configuration::GetConfiguration().GetValue(Configuration::DATABASEPATH), true);
    file_filter.FillModuleFilters(Configuration::GetConfiguration().GetValue(Configuration::DATABASEFILTERSPATH),true) ;
    
    StatusBar::GetStatusBar().GetStatusBarControl()->SetText("GUI content updated", 0);

}

void Browser::ReceiveItem(TH1* t){
    cout << "Browser::ReceiveItem()" << endl;
    Emit("OpenItemDoubleClicked(TH1*)", t);
}

