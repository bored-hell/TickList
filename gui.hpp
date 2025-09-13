#pragma once
#ifndef _GUI_HPP
#define _GUI_HPP

#include "core.hpp"
#include <map>

#include <wx/wx.h>
#include <wx/listctrl.h>

class TaskEditDialog : public wxDialog
{
  public:
    TaskEditDialog(const wxString &name, wxWindow *parent);
    wxString GetTaskName() const
    {
        return task_ctrl->GetValue();
    }
    int GetPriority() const
    {
        return priority_choice->GetSelection();
    }
    void SetTaskName(const wxString &name)
    {
        task_ctrl->SetValue(name);
    }
    void SetPriority(const wxString &priority)
    {
        int idx = priority_choice->FindString(priority);
        if (idx != wxNOT_FOUND)
            priority_choice->SetSelection(idx);
    }

  private:
    wxTextCtrl *task_ctrl;
    wxChoice *priority_choice;
};

class TickListFrame : public wxFrame
{
  public:
    TickListFrame();

  private:
    void CreateMenuAndStatusBar();
    void CreateListCtrl();
    void AdjustListColumnWidths();
    wxBoxSizer *InitButtons();
    void BindAllEvents();

  private:
    void OnSize(wxSizeEvent &event);
    void OnListItemCheck(wxListEvent &event);
    void OnListItemUnCheck(wxListEvent &event);
    void OnAbout(wxCommandEvent &);
    void OnExit(wxCommandEvent &);
    void OnListSelectionChange(wxListEvent &);
    void OnAddButtonClick(wxCommandEvent &);
    void OnEditButtonClick(wxCommandEvent &);
    void OnRemoveButtonClick(wxCommandEvent &);

  private:
    wxListCtrl *list_ctrl;
    wxButton *add_button;
    wxButton *edit_button;
    wxButton *remove_button;
    wxButton *save_button;

  private:
    TaskList tasklist;
    size_t n_complete;
};

#endif //!_GUI_HPP
