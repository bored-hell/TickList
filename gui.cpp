#include "gui.hpp"

enum
{
    ID_LISTCTRL = 1,
    ID_ADDBUTTON,
    ID_EDITBUTTON,
    ID_REMOVEBUTTON,
};

TaskEditDialog::TaskEditDialog(const wxString &name, wxWindow *parent) : wxDialog(parent, wxID_ANY, name)
{
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer *grid_sizer = new wxFlexGridSizer(2, wxSize(10, 10));
    grid_sizer->Add(new wxStaticText(this, wxID_ANY, "Task Name:"), 0, wxALIGN_RIGHT);

    task_ctrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, -1));
    task_ctrl->SetMaxLength(50);

    grid_sizer->Add(task_ctrl, 1, wxEXPAND);

    grid_sizer->Add(new wxStaticText(this, wxID_ANY, "Priority:"), 0, wxALIGN_RIGHT);

    wxArrayString priorities;
    priorities.Add("NONE");
    priorities.Add("LOW");
    priorities.Add("MEDIUM");
    priorities.Add("HIGH");
    priorities.Add("URGENT!");

    priority_choice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, priorities);
    priority_choice->SetSelection(priorities.Index("MEDIUM"));
    grid_sizer->Add(priority_choice, 1, wxEXPAND);

    main_sizer->Add(grid_sizer, 0, wxALL | wxEXPAND, 15);

    wxStdDialogButtonSizer *button_sizer = new wxStdDialogButtonSizer;
    button_sizer->AddButton(new wxButton(this, wxID_OK));
    button_sizer->AddButton(new wxButton(this, wxID_CANCEL));
    button_sizer->Realize();

    main_sizer->Add(button_sizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);
    SetSizerAndFit(main_sizer);
}

TickListFrame::TickListFrame()
    : wxFrame(nullptr, wxID_ANY, "TickList APP", wxDefaultPosition, wxDefaultSize,
              wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION),
      n_complete(0)
{
    CreateMenuAndStatusBar();
    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);

    CreateListCtrl();
    main_sizer->Add(list_ctrl, 1, wxEXPAND | wxALL, 10);

    main_sizer->Add(InitButtons(), 0, wxALIGN_CENTER, 5);

    BindAllEvents();
    SetSizerAndFit(main_sizer);
    SetSize(800, 600);
}

void TickListFrame::CreateMenuAndStatusBar()
{
    wxMenu *menu_file = new wxMenu;
    menu_file->Append(wxID_ABOUT);
    menu_file->Append(wxID_EXIT);

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(menu_file, "&File");
    SetMenuBar(menu_bar);

    CreateStatusBar();
    SetStatusText("Welcome to TickList APP!");
}

static int wxCALLBACK ListItemCompare(wxIntPtr item1, wxIntPtr item2, wxIntPtr sort_data)
{
    size_t id1 = static_cast<size_t>(item1);
    size_t id2 = static_cast<size_t>(item2);
    const TaskList *tasklist = reinterpret_cast<const TaskList *>(sort_data);

    auto *task1 = tasklist->GetTaskFromId(id1);
    auto *task2 = tasklist->GetTaskFromId(id2);

    if (task1->priority < task2->priority)
        return 1;
    else if (task1->priority > task2->priority)
        return -1;
    else
    {
        if (id1 < id2)
            return -1;
        else if (id1 > id2)
            return 1;
        else
            return 0;
    }
}

void TickListFrame::CreateListCtrl()
{
    list_ctrl = new wxListCtrl(this, ID_LISTCTRL, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
    if (!list_ctrl)
    {
        CORE_LOG("Invalid ListCtrl");
        std::abort();
    }

    list_ctrl->EnableCheckBoxes();
    list_ctrl->InsertColumn(1, "TASK TO COMPLETE", wxLIST_FORMAT_CENTER);
    list_ctrl->InsertColumn(2, "COMPLETE", wxLIST_FORMAT_CENTER);
    list_ctrl->InsertColumn(3, "PRIORITY", wxLIST_FORMAT_CENTER);

    tasklist.LoadTasks();

    size_t idx = 0;
    for (const auto &tp : tasklist.GetData())
    {
        list_ctrl->InsertItem(idx, tp.second.task_name);
        list_ctrl->SetItem(idx, 1, (tp.second.complete) ? "YES" : "NO");
        list_ctrl->SetItem(idx, 2, PriorityToStr(tp.second.priority).data());
        list_ctrl->SetItemData(idx, tp.first);
        ++idx;
    }

    list_ctrl->SortItems(&ListItemCompare, reinterpret_cast<wxIntPtr>(&tasklist));
}

void TickListFrame::AdjustListColumnWidths()
{
    list_ctrl->SetColumnWidth(1, wxLIST_AUTOSIZE_USEHEADER);
    list_ctrl->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER);

    auto total_list_width = list_ctrl->GetSize().GetWidth();
    auto col1_size = list_ctrl->GetColumnWidth(1);
    auto col2_size = list_ctrl->GetColumnWidth(2);

    // idk: list was using vertical scroll, I removed 40px to disable it
    auto col0_size = total_list_width - (col1_size + col2_size) - 40;

    if (col0_size < 0)
        col0_size = 0;
    list_ctrl->SetColumnWidth(0, col0_size);
}

wxBoxSizer *TickListFrame::InitButtons()
{
    add_button = new wxButton(this, ID_ADDBUTTON, "Add Task");

    edit_button = new wxButton(this, ID_EDITBUTTON, "Edit Task");
    edit_button->Disable();

    remove_button = new wxButton(this, ID_REMOVEBUTTON, "Remove Task");
    remove_button->Disable();

    wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);
    button_sizer->Add(add_button, 0, wxALIGN_CENTER | wxALL, 5);
    button_sizer->Add(edit_button, 0, wxALIGN_CENTER | wxALL, 5);
    button_sizer->Add(remove_button, 0, wxALIGN_CENTER | wxALL, 5);

    return button_sizer;
}

void TickListFrame::BindAllEvents()
{
    Bind(wxEVT_MENU, &TickListFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &TickListFrame::OnExit, this, wxID_EXIT);

    Bind(wxEVT_BUTTON, &TickListFrame::OnAddButtonClick, this, ID_ADDBUTTON);
    Bind(wxEVT_BUTTON, &TickListFrame::OnEditButtonClick, this, ID_EDITBUTTON);
    Bind(wxEVT_BUTTON, &TickListFrame::OnRemoveButtonClick, this, ID_REMOVEBUTTON);

    Bind(wxEVT_SIZE, &TickListFrame::OnSize, this);

    list_ctrl->Bind(wxEVT_LIST_ITEM_SELECTED, &TickListFrame::OnListSelectionChange, this);
    list_ctrl->Bind(wxEVT_LIST_ITEM_DESELECTED, &TickListFrame::OnListSelectionChange, this);
    list_ctrl->Bind(wxEVT_LIST_ITEM_CHECKED, &TickListFrame::OnListItemCheck, this);
    list_ctrl->Bind(wxEVT_LIST_ITEM_UNCHECKED, &TickListFrame::OnListItemUnCheck, this);
}

void TickListFrame::OnSize(wxSizeEvent &event)
{
    AdjustListColumnWidths();
    event.Skip();
}

void TickListFrame::OnListItemCheck(wxListEvent &event)
{
    auto idx = event.GetIndex();
    list_ctrl->SetItem(idx, 1, "YES");

    ++n_complete;
    auto total = list_ctrl->GetItemCount();
    auto progress = (n_complete / static_cast<float>(total)) * 100;
    SetStatusText(wxString::Format("Progress: %.1f%%", progress));
    event.Skip();
}

void TickListFrame::OnListItemUnCheck(wxListEvent &event)
{
    auto idx = event.GetIndex();
    list_ctrl->SetItem(idx, 1, "NO");

    --n_complete;
    auto total = list_ctrl->GetItemCount();
    auto progress = (n_complete / static_cast<float>(total)) * 100;
    SetStatusText(wxString::Format("Progress: %.1f%%", progress));
}

void TickListFrame::OnAbout(wxCommandEvent &)
{
    wxMessageBox("TickList APP\n\n A simple todo list application.", "About TickList APP", wxOK | wxICON_INFORMATION);
}

void TickListFrame::OnExit(wxCommandEvent &)
{
    Close(true);
}

void TickListFrame::OnListSelectionChange(wxListEvent &)
{
    auto items = list_ctrl->GetSelectedItemCount();

    edit_button->Enable(items == 1);
    remove_button->Enable(items >= 1);
}

void TickListFrame::OnAddButtonClick(wxCommandEvent &)
{
    SetStatusText("Add button clicked");

    TaskEditDialog dialog("Add Task", this);
    if (dialog.ShowModal() == wxID_OK)
    {
        wxString task_name = dialog.GetTaskName();
        int priority = dialog.GetPriority();
        auto tp = tasklist.AddTask(task_name.ToStdString(), priority);

        auto idx = list_ctrl->InsertItem(list_ctrl->GetItemCount(), task_name);
        list_ctrl->SetItem(idx, 1, "NO");
        list_ctrl->SetItem(idx, 2, PriorityToStr(priority).data());
        list_ctrl->SetItemData(idx, tp.first);

        list_ctrl->SortItems(&ListItemCompare, reinterpret_cast<wxIntPtr>(&tasklist));
    }
}

void TickListFrame::OnEditButtonClick(wxCommandEvent &)
{
    SetStatusText("Edit button clicked");

    auto idx = list_ctrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

    Task *task = tasklist.GetTaskFromId(list_ctrl->GetItemData(idx));

    wxListItem item_task_name, item_priority;
    item_task_name.SetId(idx);
    item_task_name.SetColumn(0);
    list_ctrl->GetItem(item_task_name);

    item_priority.SetId(idx);
    item_priority.SetColumn(2);
    list_ctrl->GetItem(item_priority);

    TaskEditDialog dialog("Edit Task", this);
    dialog.SetTaskName(task->task_name);
    dialog.SetPriority(wxString::Format("%d", task->priority));

    if (dialog.ShowModal() == wxID_OK)
    {
        task->task_name = dialog.GetTaskName().ToStdString();
        task->priority = dialog.GetPriority();

        list_ctrl->SetItem(idx, 0, dialog.GetTaskName());
        list_ctrl->SetItem(idx, 2, PriorityToStr(dialog.GetPriority()).data());
    }
    list_ctrl->SortItems(&ListItemCompare, reinterpret_cast<wxIntPtr>(&tasklist));
}

void TickListFrame::OnRemoveButtonClick(wxCommandEvent &)
{
    auto total_items = list_ctrl->GetItemCount() - 1;
    for (int i = total_items; i >= 0; --i)
    {
        if ((list_ctrl->GetItemState(i, wxLIST_STATE_SELECTED) & wxLIST_STATE_SELECTED) != 0)
        {
            tasklist.RemoveTask(list_ctrl->GetItemData(i));
            list_ctrl->DeleteItem(i);
        }
    }

    list_ctrl->SortItems(&ListItemCompare, reinterpret_cast<wxIntPtr>(&tasklist));

    if (list_ctrl->GetItemCount() == 0)
      remove_button->Disable();
}
