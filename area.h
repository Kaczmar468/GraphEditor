#ifndef AREA_H
#define AREA_H

#include <gtkmm/drawingarea.h>
#include <vector>

class Area : public Gtk::DrawingArea
{
public:
  Area();
  virtual ~Area(); 
  bool on_button_press_event(GdkEventButton *event);

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  std::vector <std::pair<double,double> > points;
  double cur_width=1000,cur_height=500;
};

#endif
