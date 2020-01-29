#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
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

  Area area;
  Gtk::Box m_Box_Main, m_Top_Box, m_Bottom_Box;

  Glib::RefPtr<Gtk::Builder> m_refBuilder;
  Glib::RefPtr<Gio::SimpleActionGroup> m_refActionGroup;
};

#endif
