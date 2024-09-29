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
    QLineEdit* searchByCategoryOrName;
    QLineEdit *searchByCategoryLineEdit;
    QLineEdit *searchByStartLineEdit;
    QPushButton *searchByCategoryButton;
public:
    GUI(QWidget* parent = Q_NULLPTR);
    void populateListWidgetAllItems();
public slots:
    void populateListWidgetBasedOnSearch();
    void filterByCategory();
    void printSelectedItem();
    void show_by_f3(string nm, int st);
};
#endif //SHOPPING_LIST_GUI_H
