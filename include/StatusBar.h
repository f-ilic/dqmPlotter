#ifndef STATUSBAR__H_
#define STATUSBAR__H_

#include "TGFrame.h"
#include "TGStatusBar.h"
#include "TGProgressBar.h"

class StatusBar {
    RQ_OBJECT("StatusBar")

public:
    StatusBar() {}

    void DrawInFrame(TGCompositeFrame* frame) {
        status_bar = new TGStatusBar(frame, 50, 50, kHorizontalFrame);
        frame->AddFrame(status_bar, new TGLayoutHints(kLHintsExpandX));
        status_bar->SetText("Program started!", 0);
    }

    // slot: INTERFACE
    void ReceiveStatus(string* t) {
        status_bar->SetText(t->c_str(), 0);
        delete t;
    }

private:
    TGStatusBar* status_bar;
};

#endif
