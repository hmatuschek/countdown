#ifndef COUNTDOWN_HH
#define COUNTDOWN_HH

#include <QWidget>
#include <QMenu>
#include <QVector>
#include "application.hh"


class Countdown : public QWidget
{
  Q_OBJECT

public:
  explicit Countdown(Application &app, QWidget *parent = 0);
  virtual ~Countdown();

protected slots:
  void showMenu(const QPoint &pos);
  void updateTicks();

protected:

  virtual void paintEvent(QPaintEvent *evt);
  void mouseReleaseEvent(QMouseEvent *evt);

protected:
  Application &_app;
  QMenu *_menu;
  QVector<int> _ticks;
};

#endif // COUNTDOWN_HH
