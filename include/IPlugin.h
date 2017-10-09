#ifndef IPLUGIN__H_
#define IPLUGIN__H_

#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"
#include "TGListBox.h"
#include "TGTextEntry.h"
#include "TGLabel.h"

class IPlugin {
    RQ_OBJECT("IPlugin")

public:
    virtual ~IPlugin() {}
    virtual void DrawInFrame(TGCompositeFrame* frame) = 0;
    virtual void Receive(TH1* t) = 0;
    virtual void ReceiveNewFileOpen(string* filepath) = 0;

    // TODO: MAYBE>?? DESIGN IT PROPERLY BEFORE IMPLEMENTING
//    vector<TFile*> GetOpenedFiles() {
//        Emit()
//    }

    void SignalStatus(string*);
};


class PreviewPlugin : public IPlugin {
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

    void ReceiveNewFileOpen(string* filepath) override {
        cout << "PreviewPlugin::ReceiveNewFileOpen(string*) NOT IMPLEMENTED" << endl;
    }


private:
    TRootEmbeddedCanvas* embedded_canvas;
};

class SuperimposePlugin : public IPlugin {
    RQ_OBJECT("SuperimposePlugin")

public:
    SuperimposePlugin() {}

    void DrawInFrame(TGCompositeFrame* frame) override {

        TGHorizontalFrame* preview_frame = new TGHorizontalFrame(frame);
            TGVerticalFrame* selection_frame = new TGVerticalFrame(preview_frame, 202, 250, kFixedWidth);
            TGVerticalFrame* selection_buttons_frame = new TGVerticalFrame(preview_frame);
                TGTextButton* remove_selected_item_button = new TGTextButton(selection_buttons_frame, "Remove Selected");
                TGTextButton* clear_button = new TGTextButton(selection_buttons_frame, "Clear All");

        TGHorizontalFrame* superimpose_frame = new TGHorizontalFrame(frame);
            TGVerticalFrame* option_frame = new TGVerticalFrame(superimpose_frame, 200, 250, kFixedWidth);
                TGHorizontalFrame* controlFrameXRange = new TGHorizontalFrame(option_frame, 200, 40);
                TGHorizontalFrame* controlFrameYRange = new TGHorizontalFrame(option_frame, 200, 40);
                TGTextButton* apply_custom_button = new TGTextButton(option_frame, "Apply");


        preview_canvas = new TRootEmbeddedCanvas("Preview", frame);
        superimpose_canvas = new TRootEmbeddedCanvas("Superimpose", superimpose_frame);
        selection_box  = new TGListBox(selection_frame);

        selection_buttons_frame->AddFrame(remove_selected_item_button, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));
        selection_buttons_frame->AddFrame(clear_button, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        selection_frame->AddFrame(selection_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2,2,2,2));
        selection_frame->AddFrame(selection_buttons_frame, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        preview_frame->AddFrame(selection_frame, new TGLayoutHints(kLHintsExpandY, 2,2,2,2));
        preview_frame->AddFrame(preview_canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2,2,2,2));

        // X Axis
        xRangeCheckbox    = new TGCheckButton(option_frame,"Use Custom X Range");
        xminNumbertextbox = new TGNumberEntryField(controlFrameXRange);
        xmaxNumbertextbox = new TGNumberEntryField(controlFrameXRange);

        controlFrameXRange->AddFrame(xminNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));
        controlFrameXRange->AddFrame(xmaxNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));

        // Y Axis
        yRangeCheckbox    = new TGCheckButton(option_frame,"Use Custom Y Range");
        yminNumbertextbox = new TGNumberEntryField(controlFrameYRange);
        ymaxNumbertextbox = new TGNumberEntryField(controlFrameYRange);

        controlFrameYRange->AddFrame(yminNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));
        controlFrameYRange->AddFrame(ymaxNumbertextbox, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 2,2,2,2));

        // ------- Add to Custom
        option_frame->AddFrame(xRangeCheckbox,     new TGLayoutHints(kLHintsExpandX, 2,2,2,2));
        option_frame->AddFrame(controlFrameXRange, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        option_frame->AddFrame(yRangeCheckbox,     new TGLayoutHints(kLHintsExpandX, 2,2,2,2));
        option_frame->AddFrame(controlFrameYRange, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        option_frame->AddFrame(apply_custom_button, new TGLayoutHints(kLHintsExpandX, 2,2,2,2));

        superimpose_frame->AddFrame(option_frame, new TGLayoutHints(kLHintsBottom, 2,2,2,2));
        superimpose_frame->AddFrame(superimpose_canvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2,2,2,2));

        frame->AddFrame(preview_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2,2,2,2));
        frame->AddFrame(superimpose_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2,2,2,2));

        remove_selected_item_button->Connect("Clicked()", "SuperimposePlugin", this, "RemoveSelected()");
        clear_button->Connect("Clicked()", "SuperimposePlugin", this, "ClearAll()");
        apply_custom_button->Connect("Clicked()", "SuperimposePlugin", this, "SuperimposeAll()");
    }

    void Receive(TH1* t) override {
        // dont allow duplicates in list:
        // FIXME: This does not work like its supposed to.
//        for(auto& e : selected_objects) {
//            if (t->Compare(e.second) == 0)
//                return;
//        }

        int num_selected = selected_objects.size();
        selected_objects[num_selected] = t;

        selection_box->AddEntry(t->GetTitle(), num_selected);
        selection_box->Layout();

        PreviewAll();
        SuperimposeAll();
    }

    void ReceiveNewFileOpen(string* filepath) override {
        cout << "SuperimposePlugin::ReceiveNewFileOpen(string*) NOT IMPLEMENTED" << endl;
    }



    void PreviewAll() {
        TCanvas* canvas = preview_canvas->GetCanvas();
        canvas->Clear();
        canvas->Divide(selected_objects.size(), 1);

        // Dont work on originals so that chaging something in the preview
        // does not affect superimpose and v.v.
        vector<TH1*> copies;
        for(auto& elem : selected_objects){
            copies.push_back((TH1*)elem.second->Clone());
        }

        int i = 1;
        for(auto& elem : copies) {
            canvas->cd(i++);
            elem->Draw();
        }
        canvas->Update();
    }

    void SuperimposeAll() {
        TCanvas* canvas = superimpose_canvas->GetCanvas();

        // Dont work on originals so that chaging something in the preview
        // does not affect superimpose and v.v.
        vector<TH1*> copies;
        for(auto& elem : selected_objects){
            copies.push_back((TH1*)elem.second->Clone());
        }

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
        for(auto& elem : copies) {

            if(xRangeCheckbox->IsOn())
                elem->SetAxisRange(xminNumbertextbox->GetNumber(), xmaxNumbertextbox->GetNumber(),"X");

            if(yRangeCheckbox->IsOn())
                elem->SetAxisRange(yminNumbertextbox->GetNumber(), ymaxNumbertextbox->GetNumber(),"Y");

            elem->SetLineColor(colors[idx]);
            legend->AddEntry(elem, elem->GetTitle());
            elem->SetTitle("Superimposed");

            if(idx==0) elem->Draw();
            else       elem->Draw("SAME");

            elem->SetStats(false);
            idx++;
        }

        legend->Draw();
        canvas->Update();
    }

    void ClearAll() {
        Emit("SignalStatus(string*)", new string("Cleared all"));

        preview_canvas->GetCanvas()->Clear();
        preview_canvas->GetCanvas()->Update();

        superimpose_canvas->GetCanvas()->Clear();
        superimpose_canvas->GetCanvas()->Update();

        selection_box->RemoveAll();
        selected_objects.clear();
    }

    void RemoveSelected() {
        TGLBEntry* elem = selection_box->GetSelectedEntry();

        if(!elem)
            return;

        int selected_id = selection_box->GetSelected();
        selected_objects.erase(selected_id);
        selection_box->RemoveEntry(selected_id);

        PreviewAll();
        SuperimposeAll();
    }

private:
    TGListBox*         selection_box;
    map<int, TH1*>     selected_objects;

    TRootEmbeddedCanvas* preview_canvas;
    TRootEmbeddedCanvas* superimpose_canvas;

    TGCheckButton* xRangeCheckbox;
    TGCheckButton* yRangeCheckbox;
    TGNumberEntryField* xminNumbertextbox;
    TGNumberEntryField* xmaxNumbertextbox;
    TGNumberEntryField* yminNumbertextbox;
    TGNumberEntryField* ymaxNumbertextbox;
};

class ComparisonPlugin : public IPlugin {
    RQ_OBJECT("ComparisonPlugin")

public:
    ComparisonPlugin() {}

    void DrawInFrame(TGCompositeFrame* frame) override {
         files_frame = new TGHorizontalFrame(frame);
         TGHorizontalFrame* search_frame = new TGHorizontalFrame(frame);

         TGHorizontalFrame* out_frame = new TGHorizontalFrame(frame);
         selection_box = new TGListBox(out_frame);

         search_box = new TGTextEntry(search_frame);
         applyfilter_button = new TGTextButton(search_frame, "Apply Filter");

         search_frame->AddFrame(search_box, new TGLayoutHints(kLHintsExpandX));
         search_frame->AddFrame(applyfilter_button);

         out_frame->AddFrame(selection_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

         frame->AddFrame(search_frame, new TGLayoutHints(kLHintsExpandX));
         frame->AddFrame(files_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
         frame->AddFrame(out_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
    }

    void Receive(TH1* t) override {
        TGVerticalFrame* group_frame = new TGVerticalFrame(files_frame);
        TGListBox* selection_box = new TGListBox(group_frame);
        TGLabel*   selection_label = new TGLabel(group_frame, t->GetTitle());

        selection_box->AddEntry("TEST1", 0);
        listboxes.push_back(selection_box);

        group_frame->AddFrame(selection_label);
        group_frame->AddFrame(selection_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        files_frame->AddFrame(group_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));

        files_frame->MapSubwindows();
        files_frame->MapWindow();
        files_frame->Layout();
    }

    void ReceiveNewFileOpen(string* filepath) override {
        cout << "ComparisonPlugin::ReceiveNewFileOpen(string*) NOT IMPLEMENTED" << endl;
    }


private:
    vector<TGListBox*> listboxes;
    vector<TGLabel*>   labels;

    TGHorizontalFrame* files_frame;
    TGTextEntry*       search_box;
    TGTextButton*      applyfilter_button;
    TGListBox*         selection_box;
};


#endif
