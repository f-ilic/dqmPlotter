#ifndef IPLUGIN__H_
#define IPLUGIN__H_

#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"

class IPlugin {
public:
    virtual ~IPlugin() {}
    virtual void DrawInFrame(TGCompositeFrame* frame) = 0;
    virtual void Receive(TH1* t) = 0;
};


class PreviewPlugin : IPlugin {
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


class SuperimposePlugin : IPlugin {
    RQ_OBJECT("SuperimposePlugin")

public:
    SuperimposePlugin() {}

    void DrawInFrame(TGCompositeFrame* frame) override {
        TGTextButton* clear_button = new TGTextButton(frame, "Clear Canvas");
        embedded_canvas = new TRootEmbeddedCanvas("name", frame);
        frame->AddFrame(embedded_canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
        frame->AddFrame(clear_button, new TGLayoutHints(kLHintsExpandX));

        clear_button->Connect("Clicked()", "SuperimposePlugin", this, "ClearCanvas()");
    }

    void Receive(TH1* t) override {
        if(t) {
            embedded_canvas->GetCanvas()->cd(1);

            if(is_emtpy_canvas) t->Draw();
            else                t->Draw("SAME");

            embedded_canvas->GetCanvas()->Update();
            is_emtpy_canvas = false;
        }
    }

    void ClearCanvas() {
        embedded_canvas->GetCanvas()->Clear();
        embedded_canvas->GetCanvas()->Update();
        is_emtpy_canvas = true;
    }

private:
    TRootEmbeddedCanvas* embedded_canvas;
    bool is_emtpy_canvas = true;
};




#endif
