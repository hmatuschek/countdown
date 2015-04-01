#ifndef SCREENSAVER_HH
#define SCREENSAVER_HH
#include <Qt>
#include <QObject>
#include <QTimer>

#ifdef Q_OS_MAC
#import <IOKit/pwr_mgt/IOPMLib.h>
#endif


/** This class implements the system specific inhibition of the screen-saver of the computer. This
 * prevents that the display or the computer gets into stand-by while the clock is running. */
class ScreenSaver: public QObject
{
  Q_OBJECT

public:
  /** Constructor. */
  ScreenSaver(QObject *parent=0);

public slots:
  /** Enables the screen-saver. */
  void enable();
  /** Disables the screen-saver. */
  void disable();
  /** Enables or disables the screen saver. */
  void setEnabled(bool enabled);

protected:
#ifdef Q_OS_MACX
  /// Holds the ID of the process assertion avoiding the stand-by mode of the monitor and system.
  IOPMAssertionID _assertionID;
#endif
};


#endif // SCREENSAVER_HH
