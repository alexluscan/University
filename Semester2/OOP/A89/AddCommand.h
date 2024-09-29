
#include "ICommand.h"
#include "Service.h"


class AddCommand : public ICommand{
private:
    Service* movieServ;
    std::vector<Movie> oldList;
    std::vector<Movie> newList;
    Movie mov;
    int id;

public:
    AddCommand(Service& mv, Movie m, int id);
    void execute() override;
    void undo() override;
    void redo() override;

};


