
#include "ICommand.h"
#include "Service.h"

class RemoveCommand : public ICommand {
private:
    Service* movieServ;
    Movie mov;
    int id;

public:
    RemoveCommand(Service& mv, Movie m, int id);
    void execute() override;
    void undo() override;
    void redo() override;


};


