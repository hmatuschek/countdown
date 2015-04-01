#include "application.hh"
#include "screensaver.hh"

int main(int argc, char *argv[])
{
  Application app(argc,argv);
  app.setQuitOnLastWindowClosed(false);

  ScreenSaver screenSaver;
  screenSaver.disable();

  app.exec();

  screenSaver.enable();

  return 0;
}
