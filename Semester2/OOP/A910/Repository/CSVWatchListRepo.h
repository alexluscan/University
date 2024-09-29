
#include "Repository\WatchListRepository.h"

class CSVWatchListRepo : public WatchList {
private:
    void readFromFile() override;
    void writeToFile() override;
    std::string _filename;

public:
    CSVWatchListRepo(const std::string& repo_file_name, const std::string& csv_file_name);

    void addMovie(Movie mov, int flag) override;

    void deleteMovie(Movie mov) override;


    ~CSVWatchListRepo();
};


