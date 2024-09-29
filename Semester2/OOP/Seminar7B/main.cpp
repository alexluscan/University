#include "GUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChatSession chatSession;
    std::string user = "Alice";
    std::string user2 = "BOB";
    GUI w{chatSession, user};
    GUI w2{ chatSession, user2 };
    chatSession.registerObserver(&w);
    chatSession.registerObserver(&w2);
    w.show();
    w2.show();
    return a.exec();
}
