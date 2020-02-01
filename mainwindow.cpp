#include "mainwindow.h"
#include "area.h"
#include <iostream>

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app):
  m_Box_Main(Gtk::ORIENTATION_VERTICAL),
  m_Top_Box(Gtk::ORIENTATION_HORIZONTAL),
  m_Button_Move("Move"),
  m_Button_Path("Add path"),
  m_Button_Vertex("Add vertex"),
  m_Button_Remove("Remove")
{
  set_title("GRAPHEDITOR");
  add(m_Box_Main);
  m_Button_Vertex.set_active(1);
  MainWindow::on_click_Vertex();
  m_Label.set_margin_top(8);
  m_Label.set_margin_bottom(3);
  m_AspectFrame.set(Gtk::ALIGN_FILL, Gtk::ALIGN_FILL, 2.0, false);
  /*m_AspectFrame.set_hexpand(true);
  m_AspectFrame.set_halign(Gtk::ALIGN_FILL);
  m_AspectFrame.set_vexpand(true);
  m_AspectFrame.set_valign(Gtk::ALIGN_FILL);*/
  m_Box_Main.pack_start(m_Top_Box, Gtk::PACK_SHRINK, 0);
  m_Box_Main.pack_start(m_Label, Gtk::PACK_SHRINK, 0);
  m_Box_Main.pack_start(m_AspectFrame, Gtk::PACK_EXPAND_WIDGET, 0);
  /*m_Bottom_Box.set_hexpand(true);
  m_Bottom_Box.set_halign(Gtk::ALIGN_FILL);
  m_Bottom_Box.set_vexpand(true);
  m_Bottom_Box.set_valign(Gtk::ALIGN_FILL);*/
  m_AspectFrame.add(m_Bottom_Box);
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
  //m_Top_Box.set_border_width(2);
  //m_Bottom_Box.set_border_width(10);

  m_ButtonBox.set_border_width(1);
  m_ButtonBox.set_layout(Gtk::BUTTONBOX_START);
  m_ButtonBox.set_spacing(2);

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

  m_Button_Move.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Move) );
  m_Button_Path.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Path) );
  m_Button_Vertex.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Vertex) );
  m_Button_Remove.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Remove) );

  m_ButtonBox.add(m_Button_Move);
  m_ButtonBox.add(m_Button_Vertex);
  m_ButtonBox.add(m_Button_Path);
  m_ButtonBox.add(m_Button_Remove);

  m_Combo_Algorithms.append("Choose Algorithm");
  m_Combo_Algorithms.append("BFS");
  m_Combo_Algorithms.append("DFS");
  m_Combo_Algorithms.append("Minimum spanning tree");
  m_Combo_Algorithms.append("Number of connected components");
  m_Combo_Algorithms.append("Shortest path");
  m_Combo_Algorithms.set_active(0);

  m_ButtonBox.add(m_Combo_Algorithms);
  m_Combo_Algorithms.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_combo_changed) );

  m_Top_Box.pack_start(m_ButtonBox, Gtk::PACK_EXPAND_WIDGET);

  m_Bottom_Box.set_size_request(1000,500);
  m_Bottom_Box.pack_start(area, Gtk::PACK_EXPAND_WIDGET, 0);

  show_all_children();
}

MainWindow::~MainWindow()
{
  delete m_image_Move;
  delete m_image_Path;
  delete m_image_Vertex;
  delete m_image_Remove;
}

void MainWindow::on_click_Move(){
   if(m_Button_Move.get_active()==false)return;
   if(m_Button_Move.get_active()){
     if(m_Button_Path.get_active())m_Button_Path.set_active(0);
     if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
     if(m_Button_Remove.get_active())m_Button_Remove.set_active(0);
   }
   m_Label.set_text("Click to move a vertex");
   std::cout << "Move clicked." << std::endl;
}

void MainWindow::on_click_Path(){
   if(m_Button_Path.get_active()==false)return;
   if(m_Button_Path.get_active()){
     if(m_Button_Move.get_active())m_Button_Move.set_active(0);
     if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
     if(m_Button_Remove.get_active())m_Button_Remove.set_active(0);
   }
   m_Label.set_text("Click to add a path");
   std::cout << "Path clicked." << std::endl;
}

void MainWindow::on_click_Vertex(){
   if(m_Button_Vertex.get_active()==false)return;
   if(m_Button_Vertex.get_active()){
     if(m_Button_Path.get_active())m_Button_Path.set_active(0);
     if(m_Button_Move.get_active())m_Button_Move.set_active(0);
     if(m_Button_Remove.get_active())m_Button_Remove.set_active(0);
   }
   m_Label.set_text("Click to add vertex");
   std::cout << "Vertex clicked." << std::endl;
}

void MainWindow::on_click_Remove(){
   if(m_Button_Remove.get_active()==false)return;
   if(m_Button_Remove.get_active()){
     if(m_Button_Path.get_active())m_Button_Path.set_active(0);
     if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
     if(m_Button_Move.get_active())m_Button_Move.set_active(0);
   }
   m_Label.set_text("Click to remove vertex or path");
   std::cout << "Remove clicked." << std::endl;
}

void MainWindow::on_combo_changed()
{
  Glib::ustring text = m_Combo_Algorithms.get_active_text();
  if(!(text.empty())){
    m_Label.set_text("Combo changed: " + text);
    std::cout << "Combo changed: " << text << std::endl;
  }
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
