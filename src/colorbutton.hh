#ifndef COLORBUTTON_HH
#define COLORBUTTON_HH

#include <QPushButton>

/** A simple color picker button. */
class ColorButton : public QPushButton
{
  Q_OBJECT
public:
  explicit ColorButton(const QColor &color, QWidget *parent = 0);

  inline const QColor &color() const { return _color; }

protected slots:
  void onSelectColor();

protected:
  void paintEvent(QPaintEvent *evt);

protected:
  QColor _color;
};

#endif // COLORBUTTON_HH
