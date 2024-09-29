
#include "ICommand.h"
#include "UndoRedoException.h"
#include <stack>
#include <memory>

typedef std::stack<std::shared_ptr<ICommand>> commandStack;

class UndoRedo {
private:
    commandStack undoStack;
    commandStack redoStack;

public:

    UndoRedo();
    void redo();
    void undo();

    void executeCommand(std::shared_ptr<ICommand> command);



};


