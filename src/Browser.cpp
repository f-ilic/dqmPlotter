#include "../include/Browser.h"

#include <iostream>

Browser::Browser() {
    // set up all the connection stuff between filter and viewer
}

void Browser::DrawInFrame(TGCompositeFrame *mf) {
    file_filter.DrawInFrame(mf);
    file_view.DrawInFrame(mf);

    file_filter.Connect("FilesSelected(int)", "FileViewer", &file_view, "PrintSomething(int)");
}

void Browser::UpdateLists()
{
  cout << "Hello world!" << endl;
  cout << "sadsafssgergteg" << endl;
  cout << "sadsafssgergteg" << endl;
  // file_filter.FillFromFile(Configuration::GetConfiguration().GetValue(Configuration::DATABASEPATH), true);
  // file_filter.FillModuleFilters(Configuration::GetConfiguration().GetValue(Configuration::DATABASEFILTERSPATH),true) ;
}
