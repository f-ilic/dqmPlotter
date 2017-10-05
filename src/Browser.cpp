#include "../include/Browser.h"

#include <iostream>
#include "TGSplitter.h"

Browser::Browser() {

    file_filter.Connect("FilesSelected(map<string*, string*>*)",
                        "FileViewer",
                        &file_view,
                        "DisplayInTreeView(map<string*, string*>*)");
}

void Browser::DrawInFrame(TGCompositeFrame *mf) {
    TGVerticalFrame* top_frame = new TGVerticalFrame(mf);

    TGHorizontalFrame* up = new TGHorizontalFrame(top_frame,10, 150, kFixedHeight);
    TGHorizontalFrame* down = new TGHorizontalFrame(top_frame);

    TGHSplitter *hsplitter = new TGHSplitter(top_frame,2,2);
    hsplitter->SetFrame(up, kTRUE);

    file_filter.DrawInFrame(up);
    file_view.DrawInFrame(down);

    top_frame->AddFrame(up,  new TGLayoutHints(kLHintsExpandX));
    top_frame->AddFrame(hsplitter,  new TGLayoutHints(kLHintsTop | kLHintsExpandX));
    top_frame->AddFrame(down,  new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));


    mf->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
}

void Browser::UpdateLists()
{
  cout << "Browser::UpdateLists()" << endl;
  file_filter.FillFromFile(Configuration::GetConfiguration().GetValue(Configuration::DATABASEPATH), true);
  file_filter.FillModuleFilters(Configuration::GetConfiguration().GetValue(Configuration::DATABASEFILTERSPATH),true) ;
}
