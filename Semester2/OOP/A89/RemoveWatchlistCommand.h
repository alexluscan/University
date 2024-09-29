
#include "ICommand.h"
#include "Service.h"

class RemoveWatchlistCommand : public ICommand{
private:
    Service* movieServ;
    Movie mov;
    bool isRated;

public:
    RemoveWatchlistCommand(Service& mv, Movie& m, bool fl);
    void execute() override;
    void undo() override;
    void redo() override;

};


