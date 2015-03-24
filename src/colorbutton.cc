#include "colorbutton.hh"
#include <QPainter>
#include <QColorDialog>
#include "mainwindow.hh"

ColorButton::ColorButton(const QColor &color, QWidget *parent)
  : QPushButton(parent), _color(color)
{
  setMinimumSize(64+10, 16+10);
  QObject::connect(this, SIGNAL(clicked()), this, SLOT(onSelectColor()));
}

void
ColorButton::setColor(const QColor &color) {
  _color = color; update();
}

void
ColorButton::paintEvent(QPaintEvent *evt) {
  QPushButton::paintEvent(evt);
  QPainter painter(this);

  painter.setPen("black");
  painter.setBrush(_color);
  painter.drawRect(5,5, width()-10, height()-10);
}

void
ColorButton::onSelectColor() {
  QColorDialog dialog(_color);
  if (QDialog::Accepted != dialog.exec()) { return; }
  setColor(dialog.currentColor());
}


