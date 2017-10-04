#include "../include/Browser.h"

Browser::Browser() {
    // set up all the connection stuff between filter and viewer
}

void Browser::DrawInFrame(TGCompositeFrame *mf) {
    file_filter.DrawInFrame(mf);
    file_view.DrawInFrame(mf);

    file_filter.Connect("FilesSelected(int)", "FileViewer", &file_view, "PrintSomething(int)");
}
