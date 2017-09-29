#include "Browser.cpp"

void dqmPlotter() {
    int width = 600;
    int height = 1000;
    TGMainFrame* main_frame = new TGMainFrame(gClient->GetRoot(), width, height);

    main_frame->SetWindowName("Browser");
    Browser* fs = new Browser(main_frame);

    main_frame->MapSubwindows();
    main_frame->MapWindow();

    main_frame->MoveResize(100, 100, width, height);
}
