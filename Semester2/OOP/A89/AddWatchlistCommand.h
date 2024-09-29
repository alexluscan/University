
#include "ICommand.h"
#include "Service.h"

class AddWatchlistCommand : public ICommand{
private:
    Service* movieServ;
    Movie mov;

public:
    AddWatchlistCommand(Service& mv, Movie m);
    void execute() override;
    void undo() override;
    void redo() override;

};


