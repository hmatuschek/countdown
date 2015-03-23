#include "countdown.hh"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRectF>
#include <QPoint>
#include <cmath>
#include <QMouseEvent>
#include <QString>
#include <QFontMetrics>


Countdown::Countdown(Application &app, QWidget *parent)
  : QWidget(parent), _app(app)
{
  // Assemble Menu
  _menu = new QMenu();
  _menu->addAction(app.actStartStop());
  _menu->addSeparator();
  _menu->addAction(app.actShowFullScreen());
  _menu->addAction(app.actShowSettings());
  _menu->addSeparator();
  _menu->addAction(app.actQuit());

  updateTicks();

  setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)),
                   this, SLOT(showMenu(QPoint)));

  QObject::connect(&app, SIGNAL(durationChanged()), this, SLOT(updateTicks()));
  QObject::connect(&app, SIGNAL(updateClock()), this, SLOT(update()));
}


Countdown::~Countdown() {
  // pass...
}

void
Countdown::updateTicks() {
  /* Compute Ticks. */
  int Nticks = 9;
  for (; Nticks>0; Nticks--) {
    if (0 == _app.duration()%Nticks) { break; }
  }
  _ticks.reserve(Nticks);
  for (int i=_app.duration()/Nticks; i<=_app.duration(); i+=_app.duration()/Nticks) {
    _ticks.push_back(i);
  }
}

void
Countdown::showMenu(const QPoint &pos) {
  _menu->popup(mapToGlobal(pos));
}

void
Countdown::mouseReleaseEvent(QMouseEvent *evt) {
  QWidget::mouseReleaseEvent(evt);
}

void
Countdown::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  float side = qMin(width(), height());
  painter.translate(width()/2, height()/2);
  painter.scale(side/200, side/200);

  // Draw pie
  if (_app.isInLastMinutes()) {
    painter.setBrush(QBrush("red"));
    painter.setPen(QPen("red"));
  } else {
    painter.setBrush(QBrush("blue"));
    painter.setPen(QPen("blue"));
  }

  int angle = (16*360.*_app.ticksLeft())/(10*_app.duration());
  painter.drawPie(QRectF(-90,-90,180,180), 16*90, angle);

  // Update pen to draw ticks and center dot
  painter.setPen(QPen(QBrush("black"), 5,
                      Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));

  // Draw center dot
  painter.drawEllipse(QPointF(0,0), 5,5);

  // Draw ticks.
  int Nticks = _ticks.size();
  for (float angle=0; angle<2*M_PI; angle+=2*M_PI/Nticks) {
    QPointF p1(85*std::sin(angle), -85*std::cos(angle));
    QPointF p2(95*std::sin(angle), -95*std::cos(angle));
    painter.drawLine(p1,p2);
  }

  // Draw time left if enabled
  if (_app.showTimeLeft()) {
    QString text = QString("%1 min").arg(_app.timeLeft());
    QRectF bb = painter.boundingRect(QRect(0,40,0,0), Qt::AlignCenter|Qt::AlignCenter, text);
    painter.drawText(bb, text, Qt::AlignCenter|Qt::AlignCenter);
  }
}
