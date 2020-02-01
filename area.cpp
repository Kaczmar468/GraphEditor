#include "area.h"
#include <cairomm/context.h>
#include <iostream>
#include <vector>

Area::Area()
{  
  add_events(Gdk::BUTTON_PRESS_MASK);
  std::cerr << "HOMO LUDENS\n\n\n";
}

Area::~Area()
{
}

bool Area::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  cur_width=width;
  cur_height=height;

  std::cerr << width << " " << height << "\n";
  cr->scale(width/2,height);
  double r=0.04;

  cr->set_line_width(0.02);

  cr->set_source_rgb(0.0, 0.6, 0.2);
  
  //cr->arc(0.5, 0.5, 0.4, 0, 2 * M_PI);
  for(auto u:points){
    //cr->save();
    cr->arc(u.first, u.second, r, 0.0, 2.0 * M_PI);
  cr->fill_preserve();
    //cr->restore();
    cr->stroke();
  }
  /*cr->move_to(0, 0);
  cr->line_to(width, height);
  cr->move_to(0, height);
  cr->line_to(width, 0);*/
  cr->stroke();
  return true;
}

bool Area::on_button_press_event(GdkEventButton *event){
  std::cerr << "Myszka w ( " << event->x << " , " << event->y << " )";
  points.push_back({event->x*2.0/cur_width,event->y/cur_height});
  queue_draw();
  return true;
}
