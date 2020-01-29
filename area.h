#ifndef AREA_H
#define AREA_H

#include <gtkmm/drawingarea.h>

class Area : public Gtk::DrawingArea
{
public:
  Area();
  virtual ~Area();

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
};

#endif
