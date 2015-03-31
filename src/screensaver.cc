#include "screensaver.hh"
#include <QtConfig>

#ifdef Q_WS_WIN32
#include <Windows.h>

ScreenSaver::ScreenSaver(QObject *parent)
  : QObject(parent)
{
  // pass...
}

void
ScreenSaver::enable(bool enable) {
  if (enable) {
    SetThreadExecutionState(ES_CONTINUOUS);
  } else {
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
  }
}
#endif


#ifdef Q_OS_MAC
ScreenSaver::ScreenSaver(QObject *parent)
  : QObject(parent)
{
  // pass...
}

void
ScreenSaver::enable(bool enable) {
  if (enable) {
    IOPMAssertionRelease(_assertionID);
  } else {
    IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,
                                kIOPMAssertionLevelOn, CFSTR("Countdown Clock"), &_assertionID);
  }
}
#endif


#ifdef Q_WS_X11
ScreenSaver::ScreenSaver(QObject *parent)
  : Object(parent)
{
  // pass...
}

void
ScreenSaver::enable(bool enable) {
  /// @todo Implement
}
#endif
