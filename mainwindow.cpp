#include "mainwindow.h"
#include "area.h"
#include <iostream>
#include <string>
#include <fstream>

MainWindow::MainWindow(const Glib::RefPtr<Gtk::Application>& app):
  m_Box_Main(Gtk::ORIENTATION_VERTICAL),
  m_Top_Box(Gtk::ORIENTATION_HORIZONTAL),
  m_Button_Move("Move"),
  m_Button_Path("Add path"),
  m_Button_Vertex("Add vertex"),
  m_Button_Remove_Vertex("Remove vertex"),
  m_Button_Remove_Path("Remove path")
{
  set_title("GRAPHEDITOR");
  add(m_Box_Main);
  m_Button_Vertex.set_active(1);
  MainWindow::on_click_Vertex();
  m_Label.set_margin_top(8);
  m_Label.set_margin_bottom(0);
  m_AspectFrame.set(Gtk::ALIGN_FILL, Gtk::ALIGN_FILL, 2.0, false);
  m_Box_Main.pack_start(m_Top_Box, Gtk::PACK_SHRINK, 0);
  m_Box_Main.pack_start(m_Label, Gtk::PACK_SHRINK, 0);
  m_Box_Main.pack_start(m_AspectFrame, Gtk::PACK_EXPAND_WIDGET, 0);
  m_AspectFrame.add(m_Bottom_Box);
  m_refActionGroup = Gio::SimpleActionGroup::create();

  m_refActionGroup->add_action("import",
    sigc::mem_fun(*this, &MainWindow::on_action_import) );
  m_refActionGroup->add_action("export",
    sigc::mem_fun(*this, &MainWindow::on_action_export) );
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
  app->set_accel_for_action("menubar.quit", "<Primary>q");
  m_refBuilder->add_from_string(ui_info);
  auto object = m_refBuilder->get_object("menubar");
  auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
  auto pMenuBar = Gtk::make_managed<Gtk::MenuBar>(gmenu);
  m_Top_Box.pack_start(*pMenuBar, Gtk::PACK_SHRINK);

  m_ButtonBox.set_border_width(0);
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

  m_image_Remove_Vertex=new Gtk::Image("icons/remove20.png");
  m_Button_Remove_Vertex.set_image_position(Gtk::POS_LEFT);
  m_Button_Remove_Vertex.set_image(*m_image_Remove_Vertex);

  m_image_Remove_Path=new Gtk::Image("icons/remove20.png");
  m_Button_Remove_Path.set_image_position(Gtk::POS_LEFT);
  m_Button_Remove_Path.set_image(*m_image_Remove_Path);

  m_Button_Move.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Move) );
  m_Button_Path.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Path) );
  m_Button_Vertex.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Vertex) );
  m_Button_Remove_Vertex.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Remove_Vertex) );
  m_Button_Remove_Path.signal_clicked().connect( sigc::mem_fun(*this,&MainWindow::on_click_Remove_Path) );

  m_ButtonBox.add(m_Button_Move);
  m_ButtonBox.add(m_Button_Vertex);
  m_ButtonBox.add(m_Button_Path);
  m_ButtonBox.add(m_Button_Remove_Vertex);
  m_ButtonBox.add(m_Button_Remove_Path);

  m_Combo_Algorithms.append("Choose Algorithm");
  m_Combo_Algorithms.append("BFS");
  m_Combo_Algorithms.append("DFS");
  m_Combo_Algorithms.append("Connected components");
  m_Combo_Algorithms.set_active(0);

  m_ButtonBox.add(m_Combo_Algorithms);
  m_Combo_Algorithms.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_combo_changed) );

  m_Combo_Colors.append("Change color");
  m_Combo_Colors.append("Vertex");
  m_Combo_Colors.append("Path");
  m_Combo_Colors.append("Special vertex");
  m_Combo_Colors.append("Special path");
  m_Combo_Colors.append("Backgroung");
  m_Combo_Colors.set_active(0);

  m_ButtonBox.add(m_Combo_Colors);
  m_Combo_Colors.signal_changed().connect( sigc::mem_fun(*this, &MainWindow::on_combo_colors_changed) );

  m_Top_Box.pack_start(m_ButtonBox, Gtk::PACK_EXPAND_WIDGET);

  m_Bottom_Box.set_size_request(1100,550);
  m_Bottom_Box.pack_start(area, Gtk::PACK_EXPAND_WIDGET, 0);

  show_all_children();
}

MainWindow::~MainWindow()
{
  delete m_image_Move;
  delete m_image_Path;
  delete m_image_Vertex;
  delete m_image_Remove_Path;
  delete m_image_Remove_Vertex;
}

void MainWindow::option_off(){
  if(m_Button_Move.get_active()==0 && m_Button_Vertex.get_active()==0 && m_Button_Path.get_active()==0
    && m_Button_Remove_Vertex.get_active()==0 && m_Button_Remove_Path.get_active()==0){
   m_Label.set_text("Click a button to choose a tool");
   area.set_mode("option_off");
  }
}

void MainWindow::on_click_Move(){
   if(m_Button_Move.get_active()==false){
    option_off();
    return;
  }
  m_Combo_Algorithms.set_active(0);
   if(m_Button_Move.get_active()){
     if(m_Button_Path.get_active())m_Button_Path.set_active(0);
     if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
     if(m_Button_Remove_Vertex.get_active())m_Button_Remove_Vertex.set_active(0);
     if(m_Button_Remove_Path.get_active())m_Button_Remove_Path.set_active(0);
   }
   m_Label.set_text("Click to move a vertex");
   area.set_mode("move_vertex");
}

void MainWindow::on_click_Path(){
   if(m_Button_Path.get_active()==false){
    option_off();
    return;
  }
  m_Combo_Algorithms.set_active(0);
   if(m_Button_Path.get_active()){
     if(m_Button_Move.get_active())m_Button_Move.set_active(0);
     if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
     if(m_Button_Remove_Vertex.get_active())m_Button_Remove_Vertex.set_active(0);
     if(m_Button_Remove_Path.get_active())m_Button_Remove_Path.set_active(0);
   }
   m_Label.set_text("Click to add a path");
   area.set_mode("add_path");
}

void MainWindow::on_click_Vertex(){
   if(m_Button_Vertex.get_active()==false){
    option_off();
    return;
  }
  m_Combo_Algorithms.set_active(0);
   if(m_Button_Vertex.get_active()){
     if(m_Button_Path.get_active())m_Button_Path.set_active(0);
     if(m_Button_Move.get_active())m_Button_Move.set_active(0);
     if(m_Button_Remove_Vertex.get_active())m_Button_Remove_Vertex.set_active(0);
     if(m_Button_Remove_Path.get_active())m_Button_Remove_Path.set_active(0);
   }
   m_Label.set_text("Click to add vertex");
   area.set_mode("add_vertex");
}

void MainWindow::on_click_Remove_Vertex(){
   if(m_Button_Remove_Vertex.get_active()==false){
    option_off();
    return;
  }
  m_Combo_Algorithms.set_active(0);
   if(m_Button_Remove_Vertex.get_active()){
     if(m_Button_Path.get_active())m_Button_Path.set_active(0);
     if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
     if(m_Button_Move.get_active())m_Button_Move.set_active(0);
     if(m_Button_Remove_Path.get_active())m_Button_Remove_Path.set_active(0);
   }
   m_Label.set_text("Click on vertex to remove it");
   area.set_mode("remove_vertex");
}

void MainWindow::on_click_Remove_Path(){
   if(m_Button_Remove_Path.get_active()==false){
    option_off();
    return;
  }
  m_Combo_Algorithms.set_active(0);
   if(m_Button_Remove_Path.get_active()){
     if(m_Button_Path.get_active())m_Button_Path.set_active(0);
     if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
     if(m_Button_Move.get_active())m_Button_Move.set_active(0);
     if(m_Button_Remove_Vertex.get_active())m_Button_Remove_Vertex.set_active(0);
   }
   m_Label.set_text("Click on two vertexes to remove path between them");
   area.set_mode("remove_path");
}

void MainWindow::on_combo_changed()
{
  Glib::ustring text = m_Combo_Algorithms.get_active_text();
  if(text!="Choose Algorithm"){
    if(m_Button_Path.get_active())m_Button_Path.set_active(0);
    if(m_Button_Vertex.get_active())m_Button_Vertex.set_active(0);
    if(m_Button_Move.get_active())m_Button_Move.set_active(0);
    if(m_Button_Remove_Vertex.get_active())m_Button_Remove_Vertex.set_active(0);
    if(m_Button_Remove_Path.get_active())m_Button_Remove_Path.set_active(0);
    if(text=="BFS"){
      m_Label.set_text("Choose a BFS source");
      area.set_mode("BFS");
    }else if(text=="DFS"){
      area.set_mode("DFS");
      m_Label.set_text("Choose a DFS source");
    }else if(text=="Connected components"){
      Glib::ustring temp="Your graph has " ;
      temp+=std::to_string(area.nocc());
      temp+=" connected components";
      m_Label.set_text(temp);
    }else
      std::cout << "ComboBox didn't recognize text: " << text << std::endl;
  }
}

void MainWindow::on_action_quit()
{
  hide();
}

void MainWindow::on_action_import()
{
  std::cout << "Importing a file." << std::endl;  
  Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(*this);
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Open", Gtk::RESPONSE_OK);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("DIMACS");
  filter_any->add_pattern("*");
  filter_any->add_pattern("*.bliss");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  std::string filename; 
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      std::cout << "Open clicked." << std::endl;
      filename = dialog.get_filename();
      std::cout << "File selected: " <<  filename << std::endl;
      break;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      std::cout << "Cancel clicked." << std::endl;
      return;
    }
    default:
    {
      std::cout << "Unexpected button clicked." << std::endl;
      return;
    }
  }
  std::ifstream file(filename);
  if(!file.good()){
      std::cout << "Can't open a file: " << filename << std::endl;
      return;
  }
  std::string temp, comment;
  while(file >> temp){
    if(temp=="e"){
      int a,b;
      file >> a >> b;
      if(area.add_path(a,b)==false){
        std::cerr << "Didn't add a path from " << a << " to " << b << std::endl;
      }
    }else if(temp=="c"){
      getline(file,comment);
      std::cout << "Comment: " << comment << std::endl;
    }else if(temp=="p"){
      int n,m;
      file >> comment >> n >> m;
      area.new_graph(n);
    }else{
      std::cerr << "Didn't recognize: " << temp;
    }
  }
  area.finish_reading();
  m_Button_Move.set_active(1);
  file.close();
}

void MainWindow::on_action_export()
{
  std::cout << "Exporting a file." << std::endl;
  Gtk::FileChooserDialog dialog("Please choose a file",
          Gtk::FILE_CHOOSER_ACTION_SAVE);
  dialog.set_transient_for(*this);
  dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
  dialog.add_button("_Save", Gtk::RESPONSE_OK);

  auto filter = Gtk::FileFilter::create();
  filter->set_name("DIMACS");
  filter->add_pattern("*");
  filter->add_pattern("*.bliss");
  dialog.add_filter(filter);

  int result = dialog.run();
  switch(result)
  {
    case(Gtk::RESPONSE_OK):
    {
      std::cout << "Save clicked." << std::endl;
      std::string filename = dialog.get_filename();
      std::cout << "File selected: " <<  filename << std::endl;
      if(area.export_graph(filename)==false){
        std::cout << "Can't open a file: " << filename << std::endl;
      }
      return;
    }
    case(Gtk::RESPONSE_CANCEL):
    {
      std::cout << "Cancel clicked." << std::endl;
      return;
    }
    default:
    {
      std::cout << "Unexpected button clicked." << std::endl;
      return;
    }
  }
}


void MainWindow::on_combo_colors_changed(){
  Glib::ustring text = m_Combo_Colors.get_active_text();
  if(text=="Change color")return;
  Gdk::RGBA m_Color;
  Gtk::ColorChooserDialog dialog("Choose a color");
  dialog.set_transient_for(*this);

  const int result = dialog.run();

  switch(result)
  {
    case Gtk::RESPONSE_OK:
    {
      m_Color = dialog.get_rgba();
      break;
    }
    case Gtk::RESPONSE_CANCEL:
    {
      std::cout << "Cancel clicked." << std::endl;
      break;
    }
    default:
    {
      std::cout << "Unexpected button clicked: " << result << std::endl;
      break;
    }
  }
  if(text=="Vertex"){
    area.set_color(m_Color,1);
  }else if(text=="Path"){
    area.set_color(m_Color,2);
  }else if(text=="Special vertex"){
    area.set_color(m_Color,3);
  }else if(text=="Special path"){
    area.set_color(m_Color,4);
  }else if(text=="Backgroung"){
    area.set_color(m_Color,5);
  }else
    std::cout << "ComboBox didn't recognize text: " << text << std::endl;
  m_Combo_Colors.set_active(0);
  area.finish_reading();
}
