#include "application.hh"
#include "screensaver.hh"

int main(int argc, char *argv[])
{
  Application app(argc,argv);
  app.setQuitOnLastWindowClosed(false);

  ScreenSaver screenSaver;
  screenSaver.enable(false);

  app.exec();

  screenSaver.enable(true);

  return 0;
}
