//
// Created by Mircea Maierean on 24.05.2023.
//

#include "gui.h"

GUI::GUI(QWidget *parent) : QWidget(parent)
{
    this->service = Service();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->listWidgetAllItems = new QListWidget();
    this->listWidgetFilteredItems = new QListWidget();
    this->searchByCategoryLineEdit = new QLineEdit();
    this->searchByName = new QLineEdit();
    this->deleteByNameButton = new QPushButton("DELETE");
    this->labelForSearchByCategoryOrName = new QLabel("Delete item");
    this->searchByCategoryButton = new QPushButton("Search by category");
    this->labelForSearchByCategory = new QLabel("Display by category");
    mainLayout->addWidget(this->listWidgetAllItems);
    mainLayout->addWidget(this->labelForSearchByCategoryOrName);
    mainLayout->addWidget(this->searchByName);
    mainLayout->addWidget(this->deleteByNameButton);
    mainLayout->addWidget(this->labelForSearchByCategory);
    mainLayout->addWidget(this->searchByCategoryLineEdit);
    mainLayout->addWidget(this->searchByCategoryButton);
    mainLayout->addWidget(this->listWidgetFilteredItems);
    this->setLayout(mainLayout);
    this->populateListWidgetAllItems();
    QObject::connect(this->searchByCategoryButton, &QPushButton::clicked, this, &GUI::filterByCategory);
    QObject::connect(this->deleteByNameButton, &QPushButton::clicked, this, &GUI::del_item);
    QObject::connect(this->searchByCategoryLineEdit, &QLineEdit::returnPressed, this, &GUI::filterByCategory);
    QObject::connect(this->listWidgetAllItems, &QListWidget::itemSelectionChanged, this, &GUI::printSelectedItem);
}

void GUI::populateListWidgetAllItems()
{
    this->listWidgetAllItems->clear();
    std::vector<Item> allItems = this->service.getAllItems();
    for (auto item : allItems)
        this->listWidgetAllItems->addItem(QString::fromStdString(item.toString()));
}

void GUI::del_item() {
    std::string name = this->searchByName->text().toStdString();
    this->service.del_item(name);
    populateListWidgetAllItems();
}

void GUI::filterByCategory() {
    this->listWidgetFilteredItems->clear();
    std::string category = this->searchByCategoryLineEdit->text().toStdString();
    std::vector<Item> itemsFiltered = this->service.getItemsByCategory(category);
    for (auto item : itemsFiltered)
        this->listWidgetFilteredItems->addItem(QString::fromStdString(item.toString()));
}

void GUI::printSelectedItem() {
    QListWidgetItem *selectedItem = this->listWidgetAllItems->currentItem();
    if (selectedItem == nullptr)
        return;
    std::string itemAsString = selectedItem->text().toStdString();
    this->searchByCategoryLineEdit->setText(QString::fromStdString(itemAsString));
}