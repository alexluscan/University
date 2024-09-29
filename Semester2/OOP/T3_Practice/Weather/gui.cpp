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
    this->searchByStartLineEdit = new QLineEdit();
    this->searchByCategoryOrName = new QLineEdit();
    this->searchByCategoryButton = new QPushButton("Display");
    this->labelForSearchByCategory = new QLabel("Display by description and start time");
    this->labelForSearchByCategoryOrName = new QLabel("Filter the initial list");
    mainLayout->addWidget(this->listWidgetAllItems);
    mainLayout->addWidget(this->labelForSearchByCategoryOrName);
    mainLayout->addWidget(this->searchByCategoryOrName);
    mainLayout->addWidget(this->labelForSearchByCategory);
    mainLayout->addWidget(this->searchByCategoryLineEdit);
    mainLayout->addWidget(this->searchByStartLineEdit);
    mainLayout->addWidget(this->searchByCategoryButton);
    mainLayout->addWidget(this->listWidgetFilteredItems);
    this->setLayout(mainLayout);
    this->populateListWidgetAllItems();
    QObject::connect(this->searchByCategoryOrName, &QLineEdit::textChanged, this, &GUI::populateListWidgetBasedOnSearch);
    QObject::connect(this->searchByCategoryButton, &QPushButton::clicked, this, &GUI::filterByCategory);
    QObject::connect(this->searchByCategoryLineEdit, &QLineEdit::returnPressed, this, &GUI::filterByCategory);
    QObject::connect(this->listWidgetAllItems, &QListWidget::itemSelectionChanged, this, &GUI::printSelectedItem);
}

void GUI::populateListWidgetBasedOnSearch()
{
    std::string categoryOrName = this->searchByCategoryOrName->text().toStdString();
    if (categoryOrName == "")
    {
        this->populateListWidgetAllItems();
        return;
    }
    this->listWidgetAllItems->clear();
    std::vector<Item> itemsFiltered = this->service.get_prob(stoi(categoryOrName));
    for (auto item : itemsFiltered)
        this->listWidgetAllItems->addItem(QString::fromStdString(item.toString()));
}

void GUI::populateListWidgetAllItems()
{
    this->listWidgetAllItems->clear();
    std::vector<Item> allItems = this->service.get_sorted();
    for (auto item : allItems) {
        this->listWidgetAllItems->addItem(QString::fromStdString(item.toString()));
    }
}

void GUI::filterByCategory() {
    std::string Name = this->searchByCategoryLineEdit->text().toStdString();
    std::string start = this->searchByStartLineEdit->text().toStdString();
    show_by_f3(Name, stoi(start));
}

void GUI::printSelectedItem() {
    QListWidgetItem *selectedItem = this->listWidgetAllItems->currentItem();
    if (selectedItem == nullptr)
        return;
    std::string itemAsString = selectedItem->text().toStdString();
    this->searchByCategoryLineEdit->setText(QString::fromStdString(itemAsString));
}


void GUI::show_by_f3(string nm, int st) {
    auto [els, total] = service.get_f3(nm, st);

    QWidget *window = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout();

    QLabel *label = new QLabel(
        els.size() == 0 ? QString("No weather in that interval")
                          : QString("Total hours: ") + QString::number(total));

    QListWidget *list = new QListWidget();

    for (auto t : els) {
        list->addItem(QString::fromStdString(t.toString()));
    }

    layout->addWidget(label);
    if (els.size() != 0) {
        layout->addWidget(list);
    }

    window->setLayout(layout);

    return window->show();
}