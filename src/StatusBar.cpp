#include "../include/StatusBar.h"


void StatusBar::DrawInFrame(TGMainFrame *main_frame, Int_t width){
    statusBar = new TGStatusBar(main_frame, width, 50, kHorizontalFrame);
    
    Int_t parts[] = {75, 25};
    statusBar->SetParts(parts, 2);
    
    statusBar->SetText("Program started!", 0);
    
    subFrame = statusBar->GetBarPart(1);
    
    progress = new TGHProgressBar(subFrame, width * parts[1] / 100, 50);
    progress->SetRange(1.0, 100.0);
    // progress->SetPosition(44.0);
    // progress->SetBarType(TGProgressBar::kFancy);
    // progress->SetFillType(TGProgressBar::kSolidFill);
    // progress->SetBarColor();
    
    main_frame->AddFrame(statusBar, new TGLayoutHints(kLHintsExpandX));
}