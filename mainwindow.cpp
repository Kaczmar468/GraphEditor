#include "mainwindow.h"
#include "buttonbox.h"
#include "area.h"
#include <iostream>

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app)
: m_Box_Main(Gtk::ORIENTATION_VERTICAL),
  m_Top_Box(Gtk::ORIENTATION_HORIZONTAL)
{
  set_title("GRAPHEDITOR");
  add(m_Box_Main);
  m_Box_Main.pack_start(m_Top_Box, Gtk::PACK_EXPAND_WIDGET, 0);
  m_Box_Main.pack_start(m_Bottom_Box, Gtk::PACK_EXPAND_WIDGET, 0);


  m_refActionGroup = Gio::SimpleActionGroup::create();

  m_refActionGroup->add_action("import",
    sigc::mem_fun(*this, &MainWindow::on_action_import) );
  m_refActionGroup->add_action("export",
    sigc::mem_fun(*this, &MainWindow::on_action_export) );
  m_refActionGroup->add_action("settings",
    sigc::mem_fun(*this, &MainWindow::on_action_settings) );
  m_refActionGroup->add_action("quit",
    sigc::mem_fun(*this, &MainWindow::on_action_quit) );

  insert_action_group("menubar", m_refActionGroup);

  m_refBuilder = Gtk::Builder::create();
  const char* ui_info =
      "<interface>"
      "  <menu id='menubar'>"
      "    <submenu>"
      "      <attribute name='label' translatable='yes'>_Menu</attribute>"
      "      <section>"
      "        <item>"
      "          <attribute name='label' translatable='yes'>_Import</attribute>"
      "          <attribute name='action'>menubar.import</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;o</attribute>"
      "        </item>"
      "        <item>"
      "          <attribute name='label' translatable='yes'>_Export</attribute>"
      "          <attribute name='action'>menubar.export</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;e</attribute>"
      "        </item>"
      "      </section>"
      "      <section>"
      "        <item>"
      "          <attribute name='label' translatable='yes'>_Settings</attribute>"
      "          <attribute name='action'>menubar.settings</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;s</attribute>"
      "        </item>"
      "      </section>"
      "      <section>"
      "        <item>"
      "          <attribute name='label' translatable='yes'>_Quit</attribute>"
      "          <attribute name='action'>menubar.quit</attribute>"
      "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
      "        </item>"
      "      </section>"
      "    </submenu>"
      "  </menu>"
      "</interface>";
  app->set_accel_for_action("menubar.import", "<Primary>o");
  app->set_accel_for_action("menubar.export", "<Primary>e");
  app->set_accel_for_action("menubar.settings", "<Primary>s");
  app->set_accel_for_action("menubar.quit", "<Primary>q");
  m_refBuilder->add_from_string(ui_info);
  auto object = m_refBuilder->get_object("menubar");
  auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
  auto pMenuBar = Gtk::make_managed<Gtk::MenuBar>(gmenu);
  m_Top_Box.pack_start(*pMenuBar, Gtk::PACK_SHRINK);

  m_Top_Box.set_border_width(10);
  m_Bottom_Box.set_border_width(10);

  m_Top_Box.pack_start(*Gtk::make_managed<ButtonBox>(), Gtk::PACK_EXPAND_WIDGET);

  m_Bottom_Box.set_size_request(800,500);
  m_Bottom_Box.pack_start(area, Gtk::PACK_EXPAND_WIDGET);

  show_all_children();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_action_quit()
{
  hide();
}

void MainWindow::on_action_import()
{
   std::cout << "Importing a file." << std::endl;
}

void MainWindow::on_action_export()
{
  std::cout << "Exporting a file." << std::endl;
}

void MainWindow::on_action_settings()
{
  std::cout << "Settings" << std::endl;
}
