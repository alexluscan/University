#include <vector>
#include "Domain/Movie.h"

class ICommand {
public:
    virtual void execute()=0;
    virtual void undo()=0;
    virtual void redo()=0;
};

