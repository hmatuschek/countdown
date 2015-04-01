#ifndef COLORBUTTON_HH
#define COLORBUTTON_HH

#include <QPushButton>

/** A simple color picker button. It displays a color instead of a label and shows a color
 * selection dialog if clicked. */
class ColorButton : public QPushButton
{
  Q_OBJECT

public:
  /** Constructor.
   * @c param color Specifies the currenlty selected color. */
  explicit ColorButton(const QColor &color, QWidget *parent = 0);
  /** Returns the currently selected color. */
  inline const QColor &color() const { return _color; }
  /** Sets the currently selected color. */
  void setColor(const QColor &color);

protected slots:
  /** On click. */
  void onSelectColor();

protected:
  /** Draws the currently selected color. */
  void paintEvent(QPaintEvent *evt);

protected:
  /** Holds the currently selected color. */
  QColor _color;
};

#endif // COLORBUTTON_HH
