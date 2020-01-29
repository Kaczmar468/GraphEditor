#include "buttonbox.h"
#include <iostream>

ButtonBox::ButtonBox()
: m_Button_Move("Move"),
  m_Button_Path("Add path"),
  m_Button_Vertex("Add vertex"),
  m_Button_Remove("Remove"),
  m_Combo_Algorithms("Algorithms")
{
  set_border_width(1);
  set_layout(Gtk::BUTTONBOX_START);
  set_spacing(2);

  m_image_Move=new Gtk::Image("icons/move20.png");
  m_Button_Move.set_image_position(Gtk::POS_LEFT);
  m_Button_Move.set_image(*m_image_Move);
  m_image_Path=new Gtk::Image("icons/path20.png");
  m_Button_Path.set_image_position(Gtk::POS_LEFT);
  m_Button_Path.set_image(*m_image_Path);
  m_image_Vertex=new Gtk::Image("icons/vertex20.png");
  m_Button_Vertex.set_image_position(Gtk::POS_LEFT);
  m_Button_Vertex.set_image(*m_image_Vertex);
  m_image_Remove=new Gtk::Image("icons/remove20.png");
  m_Button_Remove.set_image_position(Gtk::POS_LEFT);
  m_Button_Remove.set_image(*m_image_Remove);

  m_Button_Move.signal_clicked().connect( sigc::mem_fun(*this,
              &ButtonBox::on_click_Move) );
  add(m_Button_Move);
  add(m_Button_Vertex);
  add(m_Button_Path);
  add(m_Button_Remove);
  add(m_Combo_Algorithms);
}

ButtonBox::~ButtonBox()
{
  delete m_image_Move;
  delete m_image_Path;
  delete m_image_Vertex;
  delete m_image_Remove;
}

void ButtonBox::on_click_Move(){
   std::cout << "Move clicked." << std::endl;
}

void ButtonBox::on_click_Path(){

}

void ButtonBox::on_click_Vertex(){

}

void ButtonBox::on_click_Remove(){

}
