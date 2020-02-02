#include "area.h"
#include <cairomm/context.h>
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>

Area::Area()
{  
  add_events(Gdk::BUTTON_PRESS_MASK);
}

Area::~Area()
{
}

void Area::finish_reading(){
  queue_draw();
}

void Area::new_graph(unsigned int n){
  MAX_V=n;
  was_first_click=0;
  points.clear();
  paths.clear();
  double delta=1.0/(1.0+sqrt((double)(n/2.0))),cur_x=delta,cur_y=delta;
  for(int i=1;i<=n;i++){
  std::cout << cur_x << " " << cur_y << " " << delta << std::endl;
    points[i]={cur_x,cur_y};
    cur_x+=delta;
    if(cur_x>2.0-delta/2.0){
      cur_x=delta;
    cur_y+=delta;
    }
  }  
  std::cout << cur_x << " " << cur_y << " " << delta << std::endl;
}

bool Area::add_path(int a,int b){
  if(a<=MAX_V && a>0 && b>0 && b<=MAX_V){
    paths.insert({std::min(a,b),std::max(a,b)});
    return 1;
  }
  return 0;
}

double dist(std::pair<double,double> a, std::pair<double,double> b){
  return sqrt((a.first-b.first)*(a.first-b.first)+(a.second-b.second)*(a.second-b.second));
}

void Area::set_mode(std::string mode)
{
  MODE=mode;
  was_first_click=0;
}

int Area::find_vertex(std::pair<double,double> point){
  for(auto u:points){
    if(dist(u.second,point)<=R){
      return u.first;
    }
  }
  //std::cerr << "nie znalazl\n";
  return -1;
}

bool Area::on_button_press_event(GdkEventButton *event){
  std::pair<double,double> mouse_point={event->x*2.0/cur_width,event->y/cur_height};
  if(MODE=="add_vertex"){
    std::cerr << "New point in { " << mouse_point.first << " , " << mouse_point.second << " }" << std::endl;
    points[++MAX_V]=mouse_point;
    queue_draw();
    return true;
  }
  int found_point_id=find_vertex(mouse_point);
  //std::cerr << "Found point with id = " << found_point_id << std::endl;
  std::pair<double,double> found_point={11.0,11.0};
  if(found_point_id!=-1)
    found_point=points[found_point_id];
  if(MODE=="add_path"){
    if(found_point_id==-1){
       was_first_click=0;
       queue_draw();
       return true;
    }
    if(was_first_click==0){
      last_point=found_point;
      last_point_id=found_point_id;
      was_first_click=1;
      std::cerr << "First vertex { " << last_point.first << " , " << last_point.second << " } for mouse_point { "
        << mouse_point.first << " , " << mouse_point.second << " } " << std::endl;
    }else{
      was_first_click=0;
      std::pair<int,int> new_path = {std::min(found_point_id,last_point_id),std::max(found_point_id,last_point_id)};
      if(paths.find(new_path)==paths.end() && found_point!=last_point){
        paths.insert(new_path);
        std::cerr << "Added path from { " << last_point.first << " , " << last_point.second << " } to { "
          << found_point.first << " , " << found_point.second << " } " << std::endl;
      }else{
        std::cerr << "A path from { " << last_point.first << " , " << last_point.second << " } to { "
          << found_point.first << " , " << found_point.second << " } was NOT added" << std::endl;
      }
    }
    queue_draw();
    return true;
  }
  if(MODE=="move_vertex"){
  //std::cerr << "Found point222 " << found_point_id << std::endl;
    if(was_first_click){
      was_first_click=0;
      //points.erase(last_point_id);
      points[last_point_id]=mouse_point;
      std::cerr << "Moved vertex from { " << last_point.first << " , " << last_point.second << " } to { "
        << mouse_point.first << " , " << mouse_point.second << " } " << std::endl;
      queue_draw();
      return true;
    }
  //std::cerr << "Found point 333" << found_point_id << std::endl;
    if(found_point_id==-1){
       was_first_click=0;
       queue_draw();
       return true;
    }
    last_point=found_point;
    last_point_id=found_point_id;
    was_first_click=1;
    std::cerr << "Detected vertex { " << last_point.first << " , " << last_point.second << " } for mouse_point { "
      << mouse_point.first << " , " << mouse_point.second << " } " << std::endl;
    queue_draw();
    return true;
  }
  if(MODE=="remove_vertex"){
    if(found_point_id==-1){
       queue_draw();
       return true;
    }
    std:: cerr << points.size();
    points.erase(found_point_id);
    std::set <std::pair<int,int> > temp=paths;
    paths.clear();
    for(auto u:temp)
      if(u.first!=found_point_id && u.second!=found_point_id)
        paths.insert(u);
    std::cerr << "Erased a point width id = " << found_point_id << std::endl;
    queue_draw();
    return true;
  }
  if(MODE=="remove_path"){
    if(found_point_id==-1){
       was_first_click=0;
       queue_draw();
       return true;
    }
    if(was_first_click==0){
      last_point=found_point;
      last_point_id=found_point_id;
      was_first_click=1;
      std::cerr << "First vertex { " << last_point.first << " , " << last_point.second << " } for mouse_point { "
        << mouse_point.first << " , " << mouse_point.second << " } " << std::endl;
    }else{
      was_first_click=0;
      std::pair<int,int> new_path = {std::min(found_point_id,last_point_id),std::max(found_point_id,last_point_id)};
      if(found_point_id==last_point_id || paths.find(new_path)==paths.end()){
        std::cerr << "A path from { " << last_point.first << " , " << last_point.second << " } to { "
          << found_point.first << " , " << found_point.second << " } was NOT removed" << std::endl;
          queue_draw();
          return true;
      }
      paths.erase(paths.find(new_path));
        std::cerr << "Removed path from { " << last_point.first << " , " << last_point.second << " } to { "
          << found_point.first << " , " << found_point.second << " } " << std::endl;
    }
    queue_draw();
    return true;
  }
  if(MODE=="option_off"){
    std::cerr << "Click a button to choose a tool" << std::endl;
    queue_draw();
    return 0;
  }
  std::cerr << "No activity for mode " << MODE << std::endl;
  queue_draw();
  return true;
}

bool Area::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
  Gtk::Allocation allocation = get_allocation();
  const int width = allocation.get_width();
  const int height = allocation.get_height();
  cur_width=width;
  cur_height=height;

  //std::cerr << width << " " << height << std::endl;
  cr->scale(width/2,height);
  

  cr->set_line_width(LINE_WIDTH);

  cr->set_source_rgb(0.6, 0.2, 0.0);
  
  for(auto u:paths){
    std::pair<double,double> v_1=points[u.first],v_2=points[u.second];
    cr->move_to(v_1.first,v_1.second);
    cr->line_to(v_2.first,v_2.second);
    cr->stroke();
  }

  cr->set_source_rgb(0.0, 0.6, 0.2);

  for(auto u:points){
    //std::cerr << u.first << " = { " << u.second.first << " , " << u.second.second << " }" << std::endl;
    //cr->save();
    cr->set_source_rgb(0.0, 0.6, 0.2);
    if(was_first_click && u.first == last_point_id)
      cr->set_source_rgb(0.2, 0.0, 0.6);
    cr->arc(u.second.first, u.second.second, R, 0.0, 2.0 * M_PI);
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

bool Area::export_graph(std::string filename){
  std::ofstream file(filename);
  if(!file.good())
    return false;
  file << "p edge " << points.size() << " " << paths.size() << std::endl;
  for(auto u: paths){
    file << "e " << u.first << " " << u.second << std::endl;
  }
  file.close();
  return true;
}