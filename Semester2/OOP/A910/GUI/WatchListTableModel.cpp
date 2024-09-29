
#include <QFont>
#include <QBrush>
#include "WatchListTableModel.h"

WatchListTableModel::WatchListTableModel(WatchList &r, QObject *parent): repo{r}, QAbstractTableModel{parent}{

}

WatchListTableModel::~WatchListTableModel() {

}

int WatchListTableModel::rowCount(const QModelIndex &parent) const {

    int movieNumber = this->repo.list_size();

    return movieNumber;
}

int WatchListTableModel::columnCount(const QModelIndex &parent) const {
    return 5;
}

QVariant WatchListTableModel::data(const QModelIndex &index, int role) const {

    int row = index.row();
    int column = index.column();

    std::vector<Movie> movies = this->repo.getWatchList();

    if(row == movies.size()){
        return QVariant();
    }

    Movie m = movies[row];
    std::cout << column<<std::endl;

    if(role == Qt::DisplayRole || role == Qt::EditRole){
        switch(column){
            case 0:
                return QString::fromStdString(m.getTitle());
            case 1:
                return QString::fromStdString(m.getGenre());
            case 2:
                return QString::number(m.getLikesNumber());
            case 3:
                return QString::number(m.getReleaseYear());
            case 4:
                return QString::fromStdString(m.getTrailer());
            default:
                break;
        }
    }
    if(role == Qt::FontRole){
        QFont font("Courier New",10,10, true);

        if(m.getLikesNumber() >= 20){
            font.setItalic(true);
        return font;}
    }
    if(role == Qt::BackgroundRole){
        if(row%2 == 1){
 //           QBrush my_brush;
           // QColor red(Qt::red);
          //  my_brush.setColor(red);
            return QBrush{ QColor{0, 250, 154} };
        }
    }

    return QVariant{};
}

QVariant WatchListTableModel::headerData(int section, Qt::Orientation orientation, int role) const {

    if(role == Qt::DisplayRole){
        if(orientation == Qt::Horizontal){
            switch(section){
                case 0:
                    return QString{"Title"};
                case 1:
                    return QString{"Genre"};
                case 2:
                    return QString{"Number of likes"};
                case 3:
                    return QString{"Release year"};
                case 4:
                    return QString{"Trailer"};
                default:
                    break;
            }
        }
    }
    if(role == Qt::FontRole){
        QFont font("Courier New",10,10, true);
        font.setItalic(false);
        font.setBold(true);
        return font;
    }
    return QVariant{};
}

bool WatchListTableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
   // beginResetModel();
   // endResetModel();
    if(!index.isValid() || role != Qt::EditRole)
        return false;

    int movieIndex = index.row();

    std::vector<Movie> movies = this->repo.getWatchList();


    if(movieIndex == movies.size()){
        this->beginInsertRows(QModelIndex{}, movieIndex, movieIndex);

        switch(index.column()){
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
        }

        this->endInsertRows();
       // endResetModel();
       // this->update();
        return true;
    }

    this->update();
    return true;

}

Qt::ItemFlags WatchListTableModel::flags(const QModelIndex &index) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void WatchListTableModel::update() {
    beginResetModel();
    endResetModel();

}


