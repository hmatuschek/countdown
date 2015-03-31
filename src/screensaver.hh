#ifndef SCREENSAVER_HH
#define SCREENSAVER_HH
#include <Qt>
#include <QObject>
#include <QTimer>
#ifdef Q_OS_MAC
#import <IOKit/pwr_mgt/IOPMLib.h>
#endif


class ScreenSaver: public QObject
{
  Q_OBJECT

public:
  ScreenSaver(QObject *parent=0);

  void enable(bool enable);


protected:
#ifdef Q_OS_MACX
  IOPMAssertionID _assertionID;
#endif
};


#endif // SCREENSAVER_HH
