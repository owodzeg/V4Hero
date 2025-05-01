#ifndef DIALOGHANDLER_H
#define DIALOGHANDLER_H

#include <memory>
#include "DialogBox.h"

class DialogHandler
{
public:
    std::list<std::unique_ptr<PataDialogBox>> dialogboxes;

    DialogHandler();
    ~DialogHandler();
    void HandleInput();
    void Draw();
};

#endif