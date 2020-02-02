#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
/*#include <gtkmm/comboboxtext.h>
#include <gtkmm/liststore.h>*/
#include "area.h"

class MainWindow : public Gtk::Window
{
public:
  MainWindow(const Glib::RefPtr<Gtk::Application>& app);
  virtual ~MainWindow();

protected:
  void on_action_quit();
  void on_action_import();
  void on_action_export();
  void on_action_settings();
  void on_click_Move();
  void on_click_Path();
  void on_click_Vertex();
  void on_click_Remove_Vertex();
  void on_click_Remove_Path();
  void on_combo_changed();
  void option_off();

  Area area;
  Gtk::Box m_Box_Main, m_Top_Box, m_Bottom_Box;
  Gtk::Label m_Label;
  Gtk::AspectFrame m_AspectFrame;
  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;
  Gtk::Image *m_image_Move,*m_image_Path, *m_image_Vertex, *m_image_Remove_Vertex, *m_image_Remove_Path;
  Gtk::ToggleButton m_Button_Move, m_Button_Path, m_Button_Vertex, m_Button_Remove_Vertex, m_Button_Remove_Path;
  Gtk::ComboBoxText m_Combo_Algorithms;
  Gtk::ButtonBox m_ButtonBox;
};

#endif
