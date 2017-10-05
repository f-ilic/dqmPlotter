#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"

class IPlugin {
public:
    virtual ~IPlugin() {}
    virtual void DrawInFrame(TGCompositeFrame* frame) = 0;
    virtual void Receive(TH1* t) = 0;
};


class PreviewPlugin : IPlugin{
    RQ_OBJECT("PreviewPlugin")

public:
    PreviewPlugin() {}

    void DrawInFrame(TGCompositeFrame* frame) override {
        embedded_canvas = new TRootEmbeddedCanvas("name", frame);
        frame->AddFrame(embedded_canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    }

    void Receive(TH1* t) override {
        if(t) {
            embedded_canvas->GetCanvas()->cd(1);
            t->Draw();
            embedded_canvas->GetCanvas()->Update();
        }
    }

private:
    TRootEmbeddedCanvas* embedded_canvas;
};
