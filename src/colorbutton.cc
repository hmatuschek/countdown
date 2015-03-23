#include "colorbutton.hh"
#include <QPainter>
#include <QColorDialog>

ColorButton::ColorButton(const QColor &color, QWidget *parent)
  : QPushButton(parent), _color(color)
{
  setMinimumSize(128+10, 32+10);
  QObject::connect(this, SIGNAL(clicked()), this, SLOT(onSelectColor()));
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
  _color = dialog.currentColor();
}


