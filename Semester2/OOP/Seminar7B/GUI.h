#pragma once

#include <QtWidgets/QWidget>
#include "ui_GUI.h"
#include "ChatSession.h"
#include <QPushButton>

class GUI : public QWidget, public Observer
{
    Q_OBJECT

public:
    GUI(ChatSession sessions, std::string user);
    void update() override;
    void sendMessage();

    void connectSignals();
    ~GUI();

private:
    Ui::GUIClass ui;
    ChatSession& session;
    std::string user;
};
