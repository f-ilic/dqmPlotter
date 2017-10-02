#include "Browser.h"

Browser::Browser(TGMainFrame* main_frame) {

    table.FillFromFile("./rootfiles.txt");

    top_frame  = new TGVerticalFrame(main_frame);
    bot_frame  = new TGVerticalFrame(main_frame);

    search_frame = new TGHorizontalFrame(top_frame);
    datamode_dropdown = new TGComboBox(search_frame, 100);

    module_dropdown = new TGComboBox(search_frame, 100);
    search_box = new TGTextEntry(search_frame);
    applyfilter_button = new TGTextButton(search_frame, "Apply Filter");

    search_frame->AddFrame(datamode_dropdown);
    search_frame->AddFrame(module_dropdown);
    search_frame->AddFrame(search_box, new TGLayoutHints(kLHintsExpandX));
    search_frame->AddFrame(applyfilter_button);

    available_files_box = new TGListBox(top_frame);
    selectfiles_button = new TGTextButton(top_frame, "Choose selected file(s)");

    selected_files_box = new TGListBox(bot_frame);
    clear_selected_button = new TGTextButton(bot_frame, "Clear Selection");

    top_frame->AddFrame(search_frame, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    top_frame->AddFrame(available_files_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    top_frame->AddFrame(selectfiles_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));

    bot_frame->AddFrame(selected_files_box, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    bot_frame->AddFrame(clear_selected_button, new TGLayoutHints(kLHintsExpandX, 2, 2, 2, 2));
    main_frame->AddFrame(top_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));
    main_frame->AddFrame(bot_frame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 2, 2, 2, 2));

    datamode_dropdown->AddEntry("NOT IMPLEMENTED YET", 0);
    datamode_dropdown->AddEntry("Online", 1);
    datamode_dropdown->AddEntry("Offline", 2);
    datamode_dropdown->AddEntry("Relval", 3);

    module_dropdown->AddEntry("All", 0);
    module_dropdown->AddEntry("Ecal", 1);
    module_dropdown->AddEntry("Pixel", 2);
    module_dropdown->AddEntry("PixelPhase1", 3);
    module_dropdown->AddEntry("SiStrip", 4);

    datamode_dropdown->Resize(150, 20);
    module_dropdown->Resize(150, 20);

    datamode_dropdown->Select(0);
    module_dropdown->Select(0);

    available_files_box->SetMultipleSelections(true);

    applyfilter_button->SetToolTipText("Click here to print the selection you made");
    applyfilter_button->Connect("Clicked()", "Browser", this, "ApplyFilter()");

    selectfiles_button->Connect("Clicked()", "Browser", this, "SelectFiles()");
    clear_selected_button->Connect("Clicked()", "Browser", this, "ClearSelectedFiles()");

    table.DisplayInListBox(available_files_box);

    selection_in_box = new TList;
}

void Browser::ApplyFilter() {
    available_files_box->RemoveAll();
    string module_str = module_dropdown->GetSelectedEntry()->GetTitle();
    string query_str = search_box->GetText();

    vector<string> filters;
    filters.push_back("_" + module_str + "_");
    filters.push_back(query_str);

    FileTable result = table.FilterNamesBy(filters);
    result.DisplayInListBox(available_files_box);
}

//void Browser::ClearSelectedFiles() {
//    selected_files_box->RemoveAll();
//}

void Browser::SelectFiles() {
    selection_in_box->Clear();
    available_files_box->GetSelectedEntries(selection_in_box);
    selection_in_box->ls();

    FileTable selected_files;

    for(const auto&& obj: *selection_in_box) {
        string obj_name = obj->GetTitle();
        string val = table.GetPathFromName(obj_name);
        cout << "found value " << val << endl;
        selected_files.AddEntry(obj_name, val);
    }

    selected_files.DisplayInListBox(selected_files_box);
    selected_files.PrintDebug();
}
