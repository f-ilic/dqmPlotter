#ifndef IPLUGIN__H_
#define IPLUGIN__H_

#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"
#include "TGListBox.h"

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

    void Receive(TH1* t) override {
        selection_obj.push_back(t);
        selection_box->AddEntry(t->GetTitle(), selection_obj.size());
        selection_box->Layout();

        PreviewAll();
        SuperimposeAll();
    }

    void DrawInFrame(TGCompositeFrame* frame) override {


        TGHorizontalFrame* preview_frame = new TGHorizontalFrame(frame);
                    TGVerticalFrame* selection_frame = new TGVerticalFrame(preview_frame, 200, 250, kFixedWidth);
                    TGVerticalFrame* selection_buttons_frame = new TGVerticalFrame(preview_frame);
                        TGTextButton* remove_selected_item_button = new TGTextButton(selection_buttons_frame, "Remove Selected");
                        TGTextButton* clear_button = new TGTextButton(selection_buttons_frame, "Clear All");

        TGHorizontalFrame* superimpose_frame = new TGHorizontalFrame(frame);

        preview_canvas = new TRootEmbeddedCanvas("Preview", frame);
        superimpose_canvas = new TRootEmbeddedCanvas("Superimpose", superimpose_frame);
        selection_box  = new TGListBox(selection_frame);


        selection_buttons_frame->AddFrame(remove_selected_item_button, new TGLayoutHints(kLHintsExpandX));
        selection_buttons_frame->AddFrame(clear_button, new TGLayoutHints(kLHintsExpandX));

        selection_frame->AddFrame(selection_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
        selection_frame->AddFrame(selection_buttons_frame, new TGLayoutHints(kLHintsExpandX));

        preview_frame->AddFrame(selection_frame, new TGLayoutHints(kLHintsExpandY));
        preview_frame->AddFrame(preview_canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        remove_selected_item_button->Connect("Clicked()", "SuperimposePlugin", this, "RemoveSelected()");
        clear_button->Connect("Clicked()", "SuperimposePlugin", this, "ClearAll()");

        // ------- Custom
        TGVerticalFrame* option_frame = new TGVerticalFrame(superimpose_frame, 200, 250, kFixedWidth);

        // X Axis
        TGHorizontalFrame* controlFrameXRange = new TGHorizontalFrame(option_frame, 200, 40);

        TGCheckButton* xRangeCheckbox         = new TGCheckButton(option_frame,"Use Custom X Range");
        TGNumberEntryField* xminNumbertextbox = new TGNumberEntryField(controlFrameXRange);
        TGNumberEntryField* xmaxNumbertextbox = new TGNumberEntryField(controlFrameXRange);

        controlFrameXRange->AddFrame(xminNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));
        controlFrameXRange->AddFrame(xmaxNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));

        // Y Axis
        TGHorizontalFrame* controlFrameYRange = new TGHorizontalFrame(option_frame, 200, 40);

        TGCheckButton* yRangeCheckbox    = new TGCheckButton(option_frame,"Use Custom Y Range");
        TGNumberEntryField* yminNumbertextbox = new TGNumberEntryField(controlFrameYRange);
        TGNumberEntryField* ymaxNumbertextbox = new TGNumberEntryField(controlFrameYRange);

        controlFrameYRange->AddFrame(yminNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));
        controlFrameYRange->AddFrame(ymaxNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));

        TGTextButton* apply_custom_button = new TGTextButton(option_frame, "Apply");

        // ------- Add to Custom
        option_frame->AddFrame(xRangeCheckbox,     new TGLayoutHints(kLHintsExpandX, 2,2,2,2));
        option_frame->AddFrame(controlFrameXRange, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        option_frame->AddFrame(yRangeCheckbox,     new TGLayoutHints(kLHintsExpandX, 2,2,2,2));
        option_frame->AddFrame(controlFrameYRange, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        option_frame->AddFrame(apply_custom_button, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        superimpose_frame->AddFrame(option_frame, new TGLayoutHints(kLHintsBottom, 2,2,2,2));
        superimpose_frame->AddFrame(superimpose_canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2,2,2,2));

        frame->AddFrame(preview_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
        frame->AddFrame(superimpose_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    }

    void SuperimposeAll() {
        TCanvas* canvas = superimpose_canvas->GetCanvas();

        canvas->cd();

        vector<Int_t> basic_colors = { kBlue, kGreen, kCyan, kMagenta, kRed };
        vector<Int_t> colors;
        for(auto c : basic_colors) colors.push_back(c);
        for(auto c : basic_colors) colors.push_back(c+2);
        for(auto c : basic_colors) colors.push_back(c-7);
        for(auto c : basic_colors) colors.push_back(c-4);
        for(auto c : basic_colors) colors.push_back(c-9);
        auto legend = new TLegend(0.65,0.8,0.85,0.9);

        int idx = 0;
        for(auto& elem : selection_obj) {
            elem->SetLineColor(colors[idx]);
            legend->AddEntry(elem, elem->GetTitle());

            if(idx==0) elem->Draw();
            else       elem->Draw("SAME");

            elem->SetStats(false);
            idx++;
        }

        legend->Draw();
        canvas->Update();
    }

    void PreviewAll() {
        TCanvas* canvas = preview_canvas->GetCanvas();
        canvas->Clear();
        canvas->Divide(selection_obj.size(), 1);

        int i = 1;
        for(auto& elem : selection_obj) {
            canvas->cd(i++);
            elem->Draw();
        }
        canvas->Update();
    }

    void ClearAll() {
        preview_canvas->GetCanvas()->Clear();
        preview_canvas->GetCanvas()->Update();

        superimpose_canvas->GetCanvas()->Clear();
        superimpose_canvas->GetCanvas()->Update();

        selection_box->RemoveAll();
        selection_obj.clear();
    }

    void RemoveSelected() {
        cout << "RemoveSelected" << endl;
    }

private:
    TGListBox*         selection_box;
    vector<TH1*>       selection_obj;

    TRootEmbeddedCanvas* preview_canvas;
    TRootEmbeddedCanvas* superimpose_canvas;

};




#endif
