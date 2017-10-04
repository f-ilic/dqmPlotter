#ifndef BROWSER__H_
#define BROWSER__H_

#include "../include/RemoteFileFilter.h"
#include "../include/FileViewer.h"

#include "TGTextEntry.h"
#include "TGComboBox.h"
#include "RQ_OBJECT.h"
#include <iostream>

class Browser {
    RQ_OBJECT("RemoteFileFilter")

public:
    Browser();
    void DrawInFrame(TGCompositeFrame* mf);
    void UpdateLists();

private:
    RemoteFileFilter   file_filter;
    FileViewer         file_view;
};
#endif
