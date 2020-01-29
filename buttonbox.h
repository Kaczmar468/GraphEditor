#ifndef BUTTONBOX_H
#define BUTTONBOX_H

#include <gtkmm.h>

class ButtonBox : public Gtk::ButtonBox
{
public:
  ButtonBox();
  ~ButtonBox();
  void on_click_Move();
  void on_click_Path();
  void on_click_Vertex();
  void on_click_Remove();

protected:
  Gtk::Image *m_image_Move,*m_image_Path, *m_image_Vertex, *m_image_Remove;
  Gtk::ToggleButton m_Button_Move, m_Button_Path, m_Button_Vertex, m_Button_Remove;
  Gtk::ComboBox m_Combo_Algorithms;
};

#endif