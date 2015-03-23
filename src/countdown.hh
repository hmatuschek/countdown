#ifndef COUNTDOWN_HH
#define COUNTDOWN_HH

#include <QWidget>
#include <QMenu>
#include <QVector>
#include "application.hh"

/** Count-down clock widget. */
class Countdown : public QWidget
{
  Q_OBJECT

public:
  /** Constructor. */
  explicit Countdown(Application &app, QWidget *parent = 0);

protected slots:
  /** Shows the context menu. */
  void showMenu(const QPoint &pos);
  /** Updates the clock ticks. */
  void updateTicks();

protected:
  /** Draws the clock. */
  virtual void paintEvent(QPaintEvent *evt);
  /** Handles mouse (button release) events. */
  void mouseReleaseEvent(QMouseEvent *evt);
  /** Handles mous (move) events. */
  void mouseMoveEvent(QMouseEvent *evt);

protected:
  /** A weak reference to the application instance. */
  Application &_app;
  /** Hols the value of the clock ticks to be drawn. */
  QVector<int> _ticks;
};

#endif // COUNTDOWN_HH
