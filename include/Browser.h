#ifndef BROWSER__H_
#define BROWSER__H_

#include "../include/RemoteFileFilter.h"
#include "../include/FileViewer.h"

#include "TGTextEntry.h"
#include "TGComboBox.h"
#include <iostream>

class Browser {

public:
    Browser();
    void SetCertificatePath(string certpath);
    void DrawInFrame(TGCompositeFrame* mf);

private:
    RemoteFileFilter   file_filter;
    FileViewer         file_view;
};
#endif
