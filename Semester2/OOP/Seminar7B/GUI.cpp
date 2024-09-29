#include "GUI.h"

GUI::GUI(ChatSession sessions, std::string user)
    : session(sessions), user(user)
{
    ui.setupUi(this);
    this->connectSignals();
}

void GUI::update()
{
    this->ui.listWidget->clear();
    for (auto& m : this->session.getMessages())
        this->ui.listWidget->addItem(QString::fromStdString(m.second + " " + m.first));
}

void GUI::sendMessage()
{
    std::string msg = this->ui.textEdit->text().toStdString();
    this->session.addMessage(this->user, msg);
    this->ui.textEdit->clear();
}

void GUI::connectSignals()
{
    connect(this->ui.sendButton, &QPushButton::clicked, this, &GUI::sendMessage);
}

GUI::~GUI()
{}

