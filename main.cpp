#include "gui.hpp"

class TickList : public wxApp
{
  public:
    bool OnInit() override
    {
        TickListFrame *frame = new TickListFrame;
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(TickList);
