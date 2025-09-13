#include "gui.hpp"

class TickList : public wxApp
{
  public:
    bool OnInit() override
    {
        TickListFrame *frame = new TickListFrame;
        frame->Show();
        CORE_LOG_FMT("exited: {}", "with a smile");
        return true;
    }
};

wxIMPLEMENT_APP(TickList);
