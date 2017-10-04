#include "../include/Browser.h"

Browser::Browser() {

    file_filter.Connect("FilesSelected(map<string*, string*>*)",
                        "FileViewer",
                        &file_view,
                        "DisplayInTreeView(map<string*, string*>*)");
}

void Browser::DrawInFrame(TGCompositeFrame *mf) {
    file_filter.DrawInFrame(mf);
    file_view.DrawInFrame(mf);
}
