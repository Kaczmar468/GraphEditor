#include "mainwindow.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "autyzm.k.pl.grapheditor");

  MainWindow window(app);

  return app->run(window);
  //area bug
}
