//
// Created by Mircea Maierean on 24.05.2023.
//

#ifndef SHOPPING_LIST_GUI_H
#define SHOPPING_LIST_GUI_H
#include <QWidget>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "service.h"

class GUI : public QWidget
{
private:
    Service service;
    QListWidget* listWidgetAllItems;
    QListWidget* listWidgetFilteredItems;
    QLabel *labelForSearchByCategoryOrName;
    QLabel *labelForSearchByCategory;
    QLineEdit* searchByName;
    QLineEdit *searchByCategoryLineEdit;
    QPushButton *searchByCategoryButton;
    QPushButton *deleteByNameButton;
public:
    GUI(QWidget* parent = Q_NULLPTR);
    void populateListWidgetAllItems();
public slots:
    void del_item();
    void filterByCategory();
    void printSelectedItem();
};
#endif //SHOPPING_LIST_GUI_H
