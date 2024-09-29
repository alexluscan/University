
#ifndef A10_913_DIACONU_ANA_1_ICOMMAND_H
#define A10_913_DIACONU_ANA_1_ICOMMAND_H
#include <vector>
#include "Domain/Movie.h"

class ICommand {
public:
    virtual void execute()=0;
    virtual void undo()=0;
    virtual void redo()=0;
};

