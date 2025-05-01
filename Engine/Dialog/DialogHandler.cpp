#include "DialogHandler.h"

DialogHandler::DialogHandler()
{
    //ctor
}

DialogHandler::~DialogHandler()
{
    //dtor
}

void DialogHandler::HandleInput()
{
    if (!dialogboxes.empty())
    {
        auto& top = dialogboxes.back();
        top->HandleInput();
    }
}

void DialogHandler::Draw()
{
    if (!dialogboxes.empty())
    {
        auto& top = dialogboxes.back();
        top->x = 1920;
        top->y = 1080;
        top->Draw();
        if (top->closed)
        {
            dialogboxes.pop_back();
        }
    }

    // Remove closed dialog boxes
    dialogboxes.remove_if([](const std::unique_ptr<PataDialogBox>& dialog) {
        return dialog->closed;
    });
}