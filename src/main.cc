#include "application.hh"
#include <QSystemTrayIcon>
#include <QIcon>
#include <QUrl>


int main(int argc, char *argv[])
{
  Application app(argc,argv);

  QSystemTrayIcon trayIcon;
  trayIcon.setIcon(QIcon("://icons/icon32.png"));
  trayIcon.setContextMenu(app.menu());
  trayIcon.show();

  app.exec();

  return 0;
}
