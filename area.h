#ifndef AREA_H
#define AREA_H

#include "algorithms.h"
#include <gtkmm/drawingarea.h>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class Area : public Gtk::DrawingArea
{
public:
  Area();
  void set_mode(std::string mode);
  bool add_path(int a,int b);
  void new_graph(unsigned int n);
  void finish_reading();
  bool export_graph(std::string);
  std::vector<int>  borrow_graph(int a);
  int nocc();
  void set_color(Gdk::RGBA n_Col,int a);

protected:
  bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
  bool on_button_press_event(GdkEventButton *event);
  int find_vertex(std::pair<double,double> point);

  int MAX_V=0,last_point_id;
  bool was_first_click=1;
  std::pair<double,double> last_point;
  std::string MODE;
  std::unordered_map <int,std::pair<double,double> > points;
  std::set <std::pair<int,int> > paths;
  std::set <std::pair<int,int> > special_paths;
  double cur_width=1000,cur_height=500, R=0.04, LINE_WIDTH=0.02;
  Algo m_Algo;
  Gdk::RGBA m_Col_Vertex, m_Col_Vertex_s, m_Col_Path, m_Col_Path_s, m_Col_Back;
};

#endif
