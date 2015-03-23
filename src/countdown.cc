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
#include <QDebug>


Countdown::Countdown(Application &app, QWidget *parent)
  : QWidget(parent), _app(app)
{
  setMouseTracking(true);

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
  _ticks.clear();
  _ticks.reserve(Nticks);
  for (int i=_app.duration()/Nticks; i<=_app.duration(); i+=_app.duration()/Nticks) {
    _ticks.push_back(i);
  }
}

void
Countdown::showMenu(const QPoint &pos) {
  _app.menu()->popup(mapToGlobal(pos));
}

void
Countdown::mouseReleaseEvent(QMouseEvent *evt) {
  QWidget::mouseReleaseEvent(evt);
  if (Qt::LeftButton == evt->button() && !_app.running()) {
    QPoint center(rect().width()/2, rect().height()/2);
    float scale = float(std::min(width(),height()))/200;
    QRect playBB(center.x()-10*scale,center.y()-20*scale, 30*scale,40*scale);
    if (playBB.contains(evt->pos())) {
      unsetCursor(); _app.startTimer(true);
    }
  }
}

void
Countdown::mouseMoveEvent(QMouseEvent *evt) {
  QWidget::mouseMoveEvent(evt);
  if (! _app.running()) {
    QPoint center(rect().width()/2, rect().height()/2);
    float scale = float(std::min(width(),height()))/200;
    QRect playBB(center.x()-10*scale,center.y()-20*scale, 30*scale,40*scale);
    if (cursor().shape() == Qt::PointingHandCursor) {
      if (! playBB.contains(evt->pos())) { unsetCursor(); }
    } else {
      if (playBB.contains(evt->pos())) { setCursor(Qt::PointingHandCursor); }
    }
  }
}

void
Countdown::paintEvent(QPaintEvent *evt)
{
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  painter.fillRect(rect(), "white");

  float side = qMin(width(), height());
  painter.translate(width()/2, height()/2);
  painter.scale(side/200, side/200);

  // Draw pie
  if (_app.isInLastMinutes()) {
    painter.setBrush(QBrush(_app.lastMinutesColor()));
    painter.setPen(QPen(_app.lastMinutesColor()));
  } else {
    painter.setBrush(QBrush(_app.timeColor()));
    painter.setPen(QPen(_app.timeColor()));
  }

  int angle = (16*360.*_app.ticksLeft())/(10*_app.duration());
  painter.drawPie(QRectF(-90,-90,180,180), 16*90, angle);

  // Update pen to draw ticks and center dot
  painter.setPen(QPen(QBrush("black"), 5,
                      Qt::SolidLine, Qt::RoundCap, Qt::BevelJoin));

  // Draw center dot
  painter.drawEllipse(QPointF(0,0), 5,5);

  // Draw ticks if enabled
  if (_app.showTicks()) {
    int Nticks = _ticks.size();
    for (float angle=0; angle<2*M_PI; angle+=2*M_PI/Nticks) {
      QPointF p1(85*std::sin(angle), -85*std::cos(angle));
      QPointF p2(95*std::sin(angle), -95*std::cos(angle));
      painter.drawLine(p1,p2);
    }
  }

  // Draw time left if enabled
  if (_app.showTimeLeft()) {
    QString text = QString("%1 min").arg(_app.timeLeft());
    QRectF bb = painter.boundingRect(QRect(0,40,0,0), Qt::AlignCenter|Qt::AlignCenter, text);
    painter.drawText(bb, text, Qt::AlignCenter|Qt::AlignCenter);
  }

  // Draw play "button" if not running
  if (! _app.running()) {
    QColor bg(255,255,255,200);
    painter.setPen(bg); painter.setBrush(bg);
    painter.drawRect(-100,-100, 200,200);

    QColor fg(0,0,0);
    QPainterPath path;
    path.moveTo(-10,-20); path.lineTo(20,0); path.lineTo(-10,20); path.lineTo(-10,-20);
    painter.setPen(QPen(fg, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPath(path);
    painter.fillPath(path,fg);
  }
}
