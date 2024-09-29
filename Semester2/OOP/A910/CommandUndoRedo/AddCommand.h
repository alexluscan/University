
#include "CommandUndoRedo/ICommand.h"
#include "Service/MovieService.h"


class AddCommand : public ICommand{
private:
    MovieService* movieServ;
    std::vector<Movie> oldList;
    std::vector<Movie> newList;
    Movie mov;

public:
    AddCommand(MovieService& mv, Movie m);
    void execute() override;
    void undo() override;
    void redo() override;

};


