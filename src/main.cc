#include "application.hh"
#include <QSystemTrayIcon>
#include <QIcon>
#include <QUrl>


int main(int argc, char *argv[])
{
  Application app(argc,argv);
  app.setQuitOnLastWindowClosed(false);

  app.exec();

  return 0;
}
