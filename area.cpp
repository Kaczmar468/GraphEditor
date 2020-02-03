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
  m_Col_Vertex.set_rgba(0.0, 0.6, 0.2, 0.8); 
  m_Col_Vertex_s.set_rgba(0.2, 0.0, 0.6,0.9); 
  m_Col_Path.set_rgba(0.6, 0.2, 0.0, 0.6); 
  m_Col_Path_s.set_rgba(0.2, 0.2, 0.5, 0.8); 
  m_Col_Back.set_rgba(1.0,1.0,1.0,0.0);
}

void Area::finish_reading(){
  queue_draw();
}

void Area::new_graph(unsigned int n){
  MAX_V=n;
  was_first_click=0;
  points.clear();
  paths.clear();
  special_paths.clear();
  double delta=1.0/(1.0+sqrt((double)(n/2.0))),cur_x=delta,cur_y=delta;
  for(int i=1;i<=n;i++){
    points[i]={cur_x,cur_y};
    cur_x+=delta;
    if(cur_x>2.0-delta/2.0){
      cur_x=delta;
    cur_y+=delta;
    }
  }  
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
  return -1;
}

bool Area::on_button_press_event(GdkEventButton *event){
  std::pair<double,double> mouse_point={event->x*2.0/cur_width,event->y/cur_height};
  if(MODE=="add_vertex"){
    points[++MAX_V]=mouse_point;
    queue_draw();
    return true;
  }
  int found_point_id=find_vertex(mouse_point);
  std::pair<double,double> found_point;
  special_paths.clear();
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
    }else{
      was_first_click=0;
      std::pair<int,int> new_path = {std::min(found_point_id,last_point_id),std::max(found_point_id,last_point_id)};
      if(paths.find(new_path)==paths.end() && found_point!=last_point){
        paths.insert(new_path);
      }
    }
    queue_draw();
    return true;
  }
  if(MODE=="move_vertex"){
    if(was_first_click){
      was_first_click=0;
      points[last_point_id]=mouse_point;
      queue_draw();
      return true;
    }
    if(found_point_id==-1){
       was_first_click=0;
       queue_draw();
       return true;
    }
    last_point=found_point;
    last_point_id=found_point_id;
    was_first_click=1;
    queue_draw();
    return true;
  }
  if(MODE=="remove_vertex"){
    if(found_point_id==-1){
       queue_draw();
       return true;
    }
    points.erase(found_point_id);
    std::set <std::pair<int,int> > temp=paths;
    paths.clear();
    for(auto u:temp)
      if(u.first!=found_point_id && u.second!=found_point_id)
        paths.insert(u);
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
    }else{
      was_first_click=0;
      std::pair<int,int> new_path = {std::min(found_point_id,last_point_id),std::max(found_point_id,last_point_id)};
      if(found_point_id==last_point_id || paths.find(new_path)==paths.end()){
          queue_draw();
          return true;
      }
      paths.erase(paths.find(new_path));
    }
    queue_draw();
    return true;
  }
  if(MODE=="option_off"){
    queue_draw();
    return 0;
  }
  if(MODE=="BFS"||MODE=="DFS"){
    if(found_point_id==-1){
       queue_draw();
       return true;
    }
    m_Algo.set_graph(borrow_graph(found_point_id),paths);
    if(MODE=="BFS")
      special_paths=m_Algo.BFS();
    else
      special_paths=m_Algo.DFS();
    queue_draw();
    return true;
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

  cr->set_line_cap(Cairo::LINE_CAP_ROUND);
  cr->set_line_width(LINE_WIDTH);

  //BACKGROUND
  cr->scale(width/2,height);        
  cr->set_source_rgba(m_Col_Back.get_red(),m_Col_Back.get_green(),m_Col_Back.get_blue(),m_Col_Back.get_alpha());
  cr->rectangle(0, 0, get_width(), get_height());
  cr->fill();

  //SPECIAL PATHS
  cr->set_source_rgba(m_Col_Path.get_red(),m_Col_Path.get_green(),m_Col_Path.get_blue(),m_Col_Path.get_alpha());
  
  for(auto u:paths)
    if(special_paths.find(u)==special_paths.end()){
      std::pair<double,double> v_1=points[u.first],v_2=points[u.second];
      cr->move_to(v_1.first,v_1.second);
      cr->line_to(v_2.first,v_2.second);
      cr->stroke();
    }

  //PATHS
  cr->set_source_rgba(m_Col_Path_s.get_red(),m_Col_Path_s.get_green(),m_Col_Path_s.get_blue(),m_Col_Path_s.get_alpha());
  
  for(auto u:special_paths){
    std::pair<double,double> v_1=points[u.first],v_2=points[u.second];
    cr->move_to(v_1.first,v_1.second);
    cr->line_to(v_2.first,v_2.second);
    cr->stroke();
  }

  //POINTS
  cr->set_line_width(0);
  for(auto u:points){
    cr->set_source_rgba(m_Col_Vertex.get_red(),m_Col_Vertex.get_green(),m_Col_Vertex.get_blue(),m_Col_Vertex.get_alpha());
    if(was_first_click && u.first == last_point_id)
      cr->set_source_rgba(m_Col_Vertex_s.get_red(),m_Col_Vertex_s.get_green(),m_Col_Vertex_s.get_blue(),m_Col_Vertex_s.get_alpha());
    cr->arc(u.second.first, u.second.second, R, 0.0, 2.0 * M_PI);
    cr->fill_preserve();  
    cr->stroke();
  }
  cr->stroke();
  return true;
}

bool Area::export_graph(std::string filename){
  std::ofstream file(filename);
  if(!file.good())
    return false;
  file << "p edge " << points.size() << " " << paths.size() << std::endl;
  for(auto u: paths)
    file << "e " << u.first << " " << u.second << std::endl;
  file.close();
  return true;
}

std::vector<int> Area::borrow_graph(int a){
  std::vector<int> ans;
  ans.push_back(a);
  for(auto u:points)
    ans.push_back(u.first);
  return ans;
}

int Area::nocc(){
  m_Algo.set_graph(borrow_graph(0),paths);
  return m_Algo.nocc();
}

void Area::set_color(Gdk::RGBA n_Col,int a){
  switch(a){
    case(1):{
      m_Col_Vertex=n_Col;
      break;
    }
    case(2):{
      m_Col_Path=n_Col;
      break;
    }
    case(3):{
      m_Col_Vertex_s=n_Col;
      break;
    }
    case(4):{
      m_Col_Path_s=n_Col;
      break;
    }
    case(5):{
      m_Col_Back=n_Col;
      break;
    }
    default:{
      std::cerr << "Didn't mach a color" << std::endl;
    }
  }
}