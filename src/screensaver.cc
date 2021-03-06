#include "screensaver.hh"
#include <QtConfig>


/* ******************************************************************************************** *
 * Microsoft Windows specific code.
 * ******************************************************************************************** */
#ifdef Q_OS_WIN32
#include <Windows.h>

ScreenSaver::ScreenSaver(QObject *parent)
  : QObject(parent)
{
  // pass...
}

void
ScreenSaver::enable() {
  SetThreadExecutionState(ES_CONTINUOUS);
}

void
ScreenSaver::disable() {
  SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
}
#endif


/* ******************************************************************************************** *
 * Apple MacOS X specific code.
 * ******************************************************************************************** */
#ifdef Q_OS_OSX
ScreenSaver::ScreenSaver(QObject *parent)
  : QObject(parent)
{
  // pass...
}

void
ScreenSaver::enable() {
  IOPMAssertionRelease(_assertionID);
}

void
ScreenSaver::disable() {
  IOPMAssertionCreateWithName(kIOPMAssertionTypeNoDisplaySleep,
                              kIOPMAssertionLevelOn, CFSTR("Countdown Clock"), &_assertionID);
}
#endif


/* ******************************************************************************************** *
 * Unix X11 specific code.
 * ******************************************************************************************** */
#ifdef Q_OS_UNIX
#ifndef Q_OS_OSX
ScreenSaver::ScreenSaver(QObject *parent)
  : QObject(parent)
{
  // pass...
}

void
ScreenSaver::enable() {
  /// @todo Implement.
}

void
ScreenSaver::disable() {
  /// @todo Implement.
}
#endif
#endif


/* ******************************************************************************************** *
 * Generic code.
 * ******************************************************************************************** */
void
ScreenSaver::setEnabled(bool enabled) {
  if (enabled) { enable(); }
  else { disable(); }
}
