#ifndef BROWSER__H_
#define BROWSER__H_

#include "../include/RemoteFileFilter.h"
#include "../include/FileViewer.h"

#include "TGTextEntry.h"
#include "TGComboBox.h"
#include "RQ_OBJECT.h"
#include <iostream>

class Browser {
    RQ_OBJECT("Browser")

public:
    Browser();
    void DrawInFrame(TGCompositeFrame* frame);
    void UpdateLists();

    // signal: INTERFACE
    void SignalStatus(string* t); //*SIGNAL*
    void OpenItemDoubleClicked(TH1* t); //*SIGNAL*
    void FileLoaded(map<string*, string*>* t); //*SIGNAL*
    void CloseFile(string* t); //*SIGNAL* t==displayname NOT the path

    // slot: INTERNAL
    void ReceiveItem(TH1* t); //*SLOT*

private:
    RemoteFileFilter   file_filter;
    FileViewer         file_view;
};
#endif
