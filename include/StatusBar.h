#ifndef STATUSBAR__H_
#define STATUSBAR__H_

#include "TGFrame.h"
#include "TGStatusBar.h"
#include "TGProgressBar.h"

class StatusBar final{
    public:
        void DrawInFrame(TGMainFrame *main_frame, Int_t width = 1000);
        
        static StatusBar& GetStatusBar(TGMainFrame *main_frame = nullptr, Int_t width = 1000)
        {
            static StatusBar statusBar(main_frame, width);
            
            return statusBar;
        }
        
        TGStatusBar* GetStatusBarControl() { return this->statusBar; }
        TGProgressBar* GetProgressBar() { return this->progress; }
        
    protected:
        StatusBar(TGMainFrame *main_frame, Int_t width = 1000) {
            DrawInFrame(main_frame, width);
        }
    private:
        TGStatusBar* statusBar;
        TGCompositeFrame* subFrame;
        TGHProgressBar* progress;
};

#endif