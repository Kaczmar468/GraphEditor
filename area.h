#ifndef AREA_H
#define AREA_H

#include <gtkmm/drawingarea.h>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class Area : public Gtk::DrawingArea
{
public:
  Area();
  virtual ~Area(); 
  void set_mode(std::string mode);
  bool add_path(int a,int b);
  void new_graph(unsigned int n);
  void finish_reading();
  bool export_graph(std::string);

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_button_press_event(GdkEventButton *event);
  int find_vertex(std::pair<double,double> point);

  int MAX_V=0,last_point_id;
  bool was_first_click=1;
  std::pair<double,double> last_point={22.0,22.0};
  std::string MODE="initial";
  std::unordered_map <int,std::pair<double,double> > points;
  std::set <std::pair<int,int> > paths;
  double cur_width=1000,cur_height=500, R=0.04, LINE_WIDTH=0.02;
};

#endif
