#include <QDebug>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "define.h"
#include "signalmanager.h"
#include "AutoHideDockContainer.h"
#include "DockAreaTabBar.h"
#include "DockAreaTitleBar.h"
#include "DockAreaWidget.h"
#include "DockComponentsFactory.h"
#include "Eigen/Dense"
#include "FloatingDockContainer.h"
#include "logger.h"
#include "speed_ctrl.h"


using namespace ads;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    LOG_INFO("mico_app 程序启动" << QThread::currentThreadId());

    initData();
    initUI();

    QTimer::singleShot(50, this, [=]() {
        RestoreState();
    });
}

void MainWindow::initData()
{
    Q_INIT_RESOURCE(images);
    Q_INIT_RESOURCE(media);

    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<RobotPose>("RobotPose");
    qRegisterMetaType<RobotSpeed>("RobotSpeed");
    qRegisterMetaType<RobotState>("RobotState");
    qRegisterMetaType<OccupancyMap>("OccupancyMap");
    qRegisterMetaType<LaserScan>("LaserScan");
    qRegisterMetaType<RobotPath>("RobotPath");
    qRegisterMetaType<MsgId>("MsgId");
    qRegisterMetaType<std::any>("std::any");
    qRegisterMetaType<TopologyMap>("TopologyMap");
    qRegisterMetaType<TopologyMap::PointInfo>("TopologyMap::PointInfo");
}

void MainWindow::initUI()
{
    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    CDockManager::setConfigFlag(CDockManager::DockAreaHasUndockButton, false);
    CDockManager::setConfigFlag(CDockManager::DockAreaHasTabsMenuButton, false);
    CDockManager::setConfigFlag(CDockManager::MiddleMouseButtonClosesTab, true);
    CDockManager::setConfigFlag(CDockManager::EqualSplitOnInsertion, true);
    CDockManager::setConfigFlag(CDockManager::ShowTabTextOnlyForActiveTab, true);
    CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);

    dock_manager_ = new CDockManager(this);
    QVBoxLayout *center_layout = new QVBoxLayout();    //垂直
    QHBoxLayout *center_h_layout = new QHBoxLayout();  //水平


    /***************************地图工具栏***************************/
    QHBoxLayout *horizontalLayout_tools = new QHBoxLayout();
    horizontalLayout_tools->setSpacing(0);
    horizontalLayout_tools->setObjectName(QString::fromUtf8(" horizontalLayout_tools"));

    // 重定位按钮
    QToolButton *reloc_btn = new QToolButton();
    reloc_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    reloc_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:hover {"
        "   background-color: lightblue;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");

    QIcon icon4;
    icon4.addFile(QString::fromUtf8(":/images/reloc2.svg"), QSize(64, 64), QIcon::Normal, QIcon::Off);
    reloc_btn->setIcon(icon4);
    reloc_btn->setText("重定位");
    reloc_btn->setIconSize(QSize(32, 32));
    horizontalLayout_tools->addWidget(reloc_btn);

    // 编辑地图按钮
    QToolButton *edit_map_btn = new QToolButton();
    edit_map_btn->setIconSize(QSize(32, 32));
    edit_map_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    edit_map_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:hover {"
        "   background-color: lightblue;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    QIcon icon5;
    icon5.addFile(QString::fromUtf8(":/images/edit.svg"), QSize(64, 64), QIcon::Normal, QIcon::Off);
    edit_map_btn->setIcon(icon5);
    edit_map_btn->setText("编辑地图");
    horizontalLayout_tools->addWidget(edit_map_btn);

    // 打开地图按钮
    QToolButton *open_map_btn = new QToolButton();
    open_map_btn->setIconSize(QSize(32, 32));
    open_map_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    open_map_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:hover {"
        "   background-color: lightblue;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    QIcon icon6;
    icon6.addFile(QString::fromUtf8(":/images/open.svg"), QSize(64, 64), QIcon::Normal, QIcon::Off);
    open_map_btn->setIcon(icon6);
    open_map_btn->setText("打开地图");
    horizontalLayout_tools->addWidget(open_map_btn);

    // 保存地图按钮
    QToolButton *save_map_btn = new QToolButton();
    save_map_btn->setIconSize(QSize(32, 32));
    save_map_btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    save_map_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:hover {"
        "   background-color: lightblue;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    QIcon icon7;
    icon7.addFile(QString::fromUtf8(":/images/save.svg"), QSize(64, 64), QIcon::Normal, QIcon::Off);
    save_map_btn->setIcon(icon7);
    save_map_btn->setText("保存地图");
    horizontalLayout_tools->addWidget(save_map_btn);
    horizontalLayout_tools->addItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Minimum));
    center_layout->addLayout(horizontalLayout_tools);

    // 电池电量
    battery_bar_ = new QProgressBar();
    battery_bar_->setObjectName(QString::fromUtf8("battery_bar_"));
    battery_bar_->setMaximumSize(QSize(90, 16777215));
    battery_bar_->setAutoFillBackground(true);
    battery_bar_->setStyleSheet(QString::fromUtf8(
        "QProgressBar#battery_bar_\n"
        "{\n"
        "      border:none;   /*\346\227\240\350\276\271\346\241\206*/\n"
        "      background:rgb(211, 215, 207);\n"
        "      border-radius:5px;\n"
        "      text-align:center;   "
        "/*\346\226\207\346\234\254\347\232\204\344\275\215\347\275\256*/\n"
        "      color: rgb(229, 229, 229);  "
        "/*\346\226\207\346\234\254\351\242\234\350\211\262*/\n"
        "}\n"
        " \n"
        "QProgressBar::chunk \n"
        "{\n"
        "      background-color:rgb(115, 210, 22);\n"
        "      border-radius:4px;\n"
        "}\n"
        ""));

    battery_bar_->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

    horizontalLayout_tools->addWidget(battery_bar_);

    QLabel *label_11 = new QLabel();
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setMinimumSize(QSize(32, 32));
    label_11->setMaximumSize(QSize(32, 32));
    label_11->setPixmap(QPixmap(QString::fromUtf8(":/images/power-v.png")));

    horizontalLayout_tools->addWidget(label_11);

    label_power_ = new QLabel();
    label_power_->setObjectName(QString::fromUtf8("label_power_"));
    label_power_->setMinimumSize(QSize(50, 32));
    label_power_->setMaximumSize(QSize(50, 32));
    label_power_->setStyleSheet(QString::fromUtf8(""));

    horizontalLayout_tools->addWidget(label_power_);
    SlotSetBatteryStatus(0, 0);


    /***************************编辑地图工具栏***************************/
    QWidget *tools_edit_map_widget = new QWidget();
    QVBoxLayout *layout_tools_edit_map = new QVBoxLayout();
    tools_edit_map_widget->setLayout(layout_tools_edit_map);
    layout_tools_edit_map->setSpacing(0);
    layout_tools_edit_map->setObjectName(QString::fromUtf8(" layout_tools_edit_map"));

    // 鼠标按钮
    QToolButton *normal_cursor_btn = new QToolButton();
    normal_cursor_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    normal_cursor_btn->setToolTip("鼠标");
    normal_cursor_btn->setCursor(Qt::PointingHandCursor);
    normal_cursor_btn->setIconSize(QSize(32, 32));

    QIcon pose_tool_btn_icon;
    pose_tool_btn_icon.addFile(QString::fromUtf8(":/images/cursor_point_btn.svg"), QSize(), QIcon::Normal, QIcon::Off);
    normal_cursor_btn->setIcon(pose_tool_btn_icon);
    layout_tools_edit_map->addWidget(normal_cursor_btn);

    // 添加点位按钮
    QToolButton *add_point_btn = new QToolButton();
    add_point_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    add_point_btn->setToolTip("添加工位点");
    add_point_btn->setCursor(Qt::PointingHandCursor);
    add_point_btn->setIconSize(QSize(32, 32));

    QIcon add_point_btn_icon;
    add_point_btn_icon.addFile(QString::fromUtf8(":/images/point_btn.svg"), QSize(), QIcon::Normal, QIcon::Off);
    add_point_btn->setIcon(add_point_btn_icon);
    layout_tools_edit_map->addWidget(add_point_btn);

    // 连接工位点按钮（未启用）
    QToolButton *add_topology_path_btn = new QToolButton();
    add_topology_path_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    add_topology_path_btn->setToolTip("连接工位点");
    add_topology_path_btn->setCursor(Qt::PointingHandCursor);
    add_topology_path_btn->setIconSize(QSize(32, 32));

    QIcon add_topology_path_btn_icon;
    add_topology_path_btn_icon.addFile(QString::fromUtf8(":/images/topo_link_btn.svg"), QSize(), QIcon::Normal, QIcon::Off);
    add_topology_path_btn->setIcon(add_topology_path_btn_icon);
    layout_tools_edit_map->addWidget(add_topology_path_btn);
    add_topology_path_btn->setEnabled(false);

    // 添加区域按钮
    QToolButton *add_region_btn = new QToolButton();
    add_region_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    add_region_btn->setToolTip("添加区域");
    add_region_btn->setCursor(Qt::PointingHandCursor);
    add_region_btn->setIconSize(QSize(32, 32));

    QIcon add_region_btn_icon;
    add_region_btn_icon.addFile(QString::fromUtf8(":/images/region_btn.svg"), QSize(), QIcon::Normal, QIcon::Off);
    add_region_btn->setIcon(add_region_btn_icon);
    add_region_btn->setEnabled(false);
    layout_tools_edit_map->addWidget(add_region_btn);

    // 分隔
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout_tools_edit_map->addWidget(separator);

    // 橡皮擦按钮
    QToolButton *erase_btn = new QToolButton();
    erase_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    erase_btn->setToolTip("橡皮擦");
    erase_btn->setCursor(Qt::PointingHandCursor);
    erase_btn->setIconSize(QSize(32, 32));

    QIcon erase_btn_icon;
    erase_btn_icon.addFile(QString::fromUtf8(":/images/erase_btn.svg"), QSize(), QIcon::Normal, QIcon::Off);
    erase_btn->setIcon(erase_btn_icon);
    layout_tools_edit_map->addWidget(erase_btn);

    // 画笔按钮
    QToolButton *draw_pen_btn = new QToolButton();
    draw_pen_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    draw_pen_btn->setToolTip("线条");
    draw_pen_btn->setCursor(Qt::PointingHandCursor);
    draw_pen_btn->setIconSize(QSize(32, 32));

    QIcon draw_pen_btn_icon;
    draw_pen_btn_icon.addFile(QString::fromUtf8(":/images/pen.svg"), QSize(), QIcon::Normal, QIcon::Off);
    draw_pen_btn->setIcon(draw_pen_btn_icon);

    layout_tools_edit_map->addWidget(draw_pen_btn);

    // 线段按钮
    QToolButton *draw_line_btn = new QToolButton();
    draw_line_btn->setStyleSheet(
        "QToolButton {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
        "QToolButton:pressed {"
        "   background-color: lightblue;"
        "}");
    draw_line_btn->setToolTip("线条");
    draw_line_btn->setCursor(Qt::PointingHandCursor);
    draw_line_btn->setIconSize(QSize(32, 32));

    QIcon draw_line_btn_icon;
    draw_line_btn_icon.addFile(QString::fromUtf8(":/images/line_btn.svg"), QSize(), QIcon::Normal, QIcon::Off);
    draw_line_btn->setIcon(draw_line_btn_icon);

    layout_tools_edit_map->addWidget(draw_line_btn);

    layout_tools_edit_map->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding));
    tools_edit_map_widget->hide();
    center_h_layout->addWidget(tools_edit_map_widget);
    center_layout->addLayout(center_h_layout);


    /***************************地图显示***************************/
    display_manager_ = new Display::DisplayManager();
    center_h_layout->addWidget(display_manager_->GetViewPtr());


    /***************************地图下方的坐标***************************/
    QHBoxLayout *horizontalLayout_12 = new QHBoxLayout();
    horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));

    QLabel *label = new QLabel();
    label->setText("map:");
    label->setObjectName(QString::fromUtf8("label"));
    horizontalLayout_12->addWidget(label);

    label_pos_map_ = new QLabel();
    label_pos_map_->setObjectName(QString::fromUtf8("label_pos_map_"));
    label_pos_map_->setStyleSheet(QString::fromUtf8(""));
    horizontalLayout_12->addWidget(label_pos_map_);

    QLabel *label_5 = new QLabel();
    label_5->setText("scene:");
    label_5->setObjectName(QString::fromUtf8("label_5"));
    horizontalLayout_12->addWidget(label_5);

    label_pos_scene_ = new QLabel();
    label_pos_scene_->setObjectName(QString::fromUtf8("label_pos_scene_"));
    horizontalLayout_12->addWidget(label_pos_scene_);

    horizontalLayout_12->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    center_layout->addLayout(horizontalLayout_12);


    /***************************中心主窗体***************************/
    QWidget *center_widget = new QWidget();
    center_widget->setLayout(center_layout);
    CDockWidget *CentralDockWidget = new ads::CDockWidget("CentralWidget");
    CentralDockWidget->setWidget(center_widget);
    center_docker_area_ = dock_manager_->setCentralWidget(CentralDockWidget);
    center_docker_area_->setAllowedAreas(DockWidgetArea::OuterDockAreas);


    /***************************速度仪表盘***************************/
    ads::CDockWidget *DashBoardDockWidget = new ads::CDockWidget("DashBoard");
    QWidget *speed_dashboard_widget = new QWidget();
    DashBoardDockWidget->setWidget(speed_dashboard_widget);
    speed_dash_board_ = new DashBoard(speed_dashboard_widget);
    auto dashboard_area = dock_manager_->addDockWidget(ads::DockWidgetArea::LeftDockWidgetArea, DashBoardDockWidget, center_docker_area_);
    ui->menuView->addAction(DashBoardDockWidget->toggleViewAction());


    /***************************速度控制***************************/
    speed_ctrl_widget_ = new SpeedCtrlWidget();
    connect(speed_ctrl_widget_, &SpeedCtrlWidget::signalControlSpeed, [this](const RobotSpeed &speed) {
        SendChannelMsg(MsgId::kSetRobotSpeed, speed);
    });
    ads::CDockWidget *SpeedCtrlDockWidget = new ads::CDockWidget("SpeedCtrl");
    SpeedCtrlDockWidget->setWidget(speed_ctrl_widget_);
    auto speed_ctrl_area = dock_manager_->addDockWidget(ads::DockWidgetArea::BottomDockWidgetArea, SpeedCtrlDockWidget, dashboard_area);
    ui->menuView->addAction(SpeedCtrlDockWidget->toggleViewAction());


    /***************************导航任务列表***************************/
    QWidget *task_list_widget = new QWidget();
    nav_goal_table_view_ = new NavGoalTableView();
    QVBoxLayout *horizontalLayout_13 = new QVBoxLayout();
    horizontalLayout_13->addWidget(nav_goal_table_view_);
    task_list_widget->setLayout(horizontalLayout_13);
    ads::CDockWidget *nav_goal_list_dock_widget = new ads::CDockWidget("Task");
    QPushButton *btn_add_one_goal = new QPushButton("Add Point");
    QHBoxLayout *horizontalLayout_15 = new QHBoxLayout();
    QPushButton *btn_start_task_chain = new QPushButton("Start Task Chain");
    QCheckBox *loop_task_checkbox = new QCheckBox("Loop Task");
    QHBoxLayout *horizontalLayout_14 = new QHBoxLayout();
    horizontalLayout_15->addWidget(btn_add_one_goal);
    horizontalLayout_14->addWidget(btn_start_task_chain);
    horizontalLayout_14->addWidget(loop_task_checkbox);
    QPushButton *btn_load_task_chain = new QPushButton("Load Task Chain");
    QPushButton *btn_save_task_chain = new QPushButton("Save Task Chain");
    QHBoxLayout *horizontalLayout_16 = new QHBoxLayout();
    horizontalLayout_16->addWidget(btn_load_task_chain);
    horizontalLayout_16->addWidget(btn_save_task_chain);

    horizontalLayout_13->addLayout(horizontalLayout_15);
    horizontalLayout_13->addLayout(horizontalLayout_14);
    horizontalLayout_13->addLayout(horizontalLayout_16);
    nav_goal_list_dock_widget->setWidget(task_list_widget);
    nav_goal_list_dock_widget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    nav_goal_list_dock_widget->setMinimumSize(200, 150);
    nav_goal_list_dock_widget->setMaximumSize(480, 9999);
    dock_manager_->addDockWidget(ads::DockWidgetArea::RightDockWidgetArea, nav_goal_list_dock_widget, center_docker_area_);
    nav_goal_list_dock_widget->toggleView(false);

    // nav_goal_list_dock_widget->toggleView(false);
    ui->menuView->addAction(nav_goal_list_dock_widget->toggleViewAction());

    for (auto one_image : Config::ConfigManager::Instacnce()->GetRootConfig().images)
    {
        LOG_INFO("init image window location:" << one_image.location << " topic:" << one_image.topic);
        image_frame_map_[one_image.location] = new RatioLayoutedFrame();
        ads::CDockWidget *dock_widget = new ads::CDockWidget(std::string("image/" + one_image.location).c_str());
        dock_widget->setWidget(image_frame_map_[one_image.location]);

        dock_manager_->addDockWidget(ads::DockWidgetArea::RightDockWidgetArea, dock_widget, center_docker_area_);
        dock_widget->toggleView(true);
        ui->menuView->addAction(dock_widget->toggleViewAction());
    }

    /******************************连接******************************/

    connect(btn_load_task_chain, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Open JSON file", "", "JSON files (*.json)");
        if (!fileName.isEmpty())
        {
            qDebug() << "Selected file:" << fileName;
            nav_goal_table_view_->LoadTaskChain(fileName.toStdString());
        }
    });
    connect(btn_save_task_chain, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save JSON file", "", "JSON files (*.json)");
        if (!fileName.isEmpty())
        {
            qDebug() << "Selected file:" << fileName;
            if (!fileName.endsWith(".json"))
            {
                fileName += ".json";
                nav_goal_table_view_->SaveTaskChain(fileName.toStdString());
            }
        }
    });
    connect(btn_add_one_goal, &QPushButton::clicked, this, [=]() {
        nav_goal_table_view_->AddItem();
    });
    connect(btn_start_task_chain, &QPushButton::clicked, this, [=]() {
        if (btn_start_task_chain->text() == "Start Task Chain")
        {
            btn_start_task_chain->setText("Stop Task Chain");
            nav_goal_table_view_->StartTaskChain(loop_task_checkbox->isChecked());
        }
        else
        {
            btn_start_task_chain->setText("Start Task Chain");
            nav_goal_table_view_->StopTaskChain();
        }
    });
    connect(reloc_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->StartReloc();
    });
    connect(save_map_btn, &QToolButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Map files", "", "Map files (*.yaml,*.pgm,*.pgm.json)");
        if (!fileName.isEmpty())
        {
            // 用户选择了文件夹，可以在这里进行相应的操作
            LOG_INFO("用户选择的保存地图路径：" << fileName.toStdString());
            display_manager_->SaveMap(fileName.toStdString());
        }
        else
        {
            // 用户取消了选择
            LOG_INFO("取消保存地图");
        }
    });
    connect(open_map_btn, &QToolButton::clicked, this, [=]() {
        QStringList filters;
        filters << "地图(*.yaml)" << "拓扑地图(*.topology)";

        QString fileName = QFileDialog::getOpenFileName(nullptr, "OPen Map files", "", filters.join(";;"));
        if (!fileName.isEmpty())
        {
            // 用户选择了文件夹，可以在这里进行相应的操作
            LOG_INFO("用户选择的打开地图路径：" << fileName.toStdString());
            display_manager_->OpenMap(fileName.toStdString());
        }
        else
        {
            // 用户取消了选择
            LOG_INFO("取消打开地图");
        }
    });
    connect(edit_map_btn, &QToolButton::clicked, this, [=]() {
        if (edit_map_btn->text() == "编辑地图")
        {
            display_manager_->SetEditMapMode(Display::MapEditMode::kNormal);
            edit_map_btn->setText("结束编辑");
            tools_edit_map_widget->show();
        }
        else
        {
            display_manager_->SetEditMapMode(Display::MapEditMode::kStop);
            edit_map_btn->setText("编辑地图");
            tools_edit_map_widget->hide();
        }
    });
    connect(add_point_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->SetEditMapMode(Display::MapEditMode::kAddPoint);
    });
    connect(normal_cursor_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->SetEditMapMode(Display::MapEditMode::kNormal);
    });
    connect(erase_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->SetEditMapMode(Display::MapEditMode::kErase);
    });
    connect(draw_line_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->SetEditMapMode(Display::MapEditMode::kDrawLine);
    });
    connect(add_region_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->SetEditMapMode(Display::MapEditMode::kRegion);
    });
    connect(draw_pen_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->SetEditMapMode(Display::MapEditMode::kDrawWithPen);
    });
    connect(add_topology_path_btn, &QToolButton::clicked, this, [=]() {
        display_manager_->SetEditMapMode(Display::MapEditMode::kLinkTopology);
    });
    connect(SigManager, &SignalManager::sigTaskFinish, this, [=]() {
        LOG_INFO("task finish!");
        btn_start_task_chain->setText("Start Task Chain");
    });
    connect(SigManager, &SignalManager::sigRecvChannelData, this, &MainWindow::onSigRecvChannelData, Qt::BlockingQueuedConnection);
    connect(SigManager, &SignalManager::sigSendNavGoal, this, &MainWindow::onSigSendNavGoal);
    connect(SigManager, &SignalManager::sigPub2DPose, this, &MainWindow::onSigPub2DPose);
    connect(SigManager, &SignalManager::sigPub2DGoal, this, &MainWindow::onSigPub2DGoal);
    connect(SigManager, &SignalManager::sigCursorPose, this, &MainWindow::onSigCursorPose);
}

void MainWindow::SlotRecvImage(const std::string &location, std::shared_ptr<cv::Mat> data)
{
    if (image_frame_map_.count(location))
    {
        QImage image(data->data, data->cols, data->rows, data->step[0], QImage::Format_RGB888);
        image_frame_map_[location]->setImage(image);
    }
}

// 发送 ROS2 数据的接口
void MainWindow::SendChannelMsg(const MsgId &id, const std::any &data)
{
    ChannelManager::instance()->SendMessage(id, data);
}

void MainWindow::closeChannel()
{
    ChannelManager::instance()->CloseChannel();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    disconnect(SigManager, &SignalManager::sigRecvChannelData, this, &MainWindow::onSigRecvChannelData);
    SaveState();
    dock_manager_->deleteLater();
    QMainWindow::closeEvent(event);
    LOG_INFO("micao app close!");
}

void MainWindow::SaveState()
{
    QSettings settings("state.ini", QSettings::IniFormat);
    settings.setValue("mainWindow/Geometry", this->saveGeometry());
    settings.setValue("mainWindow/State", this->saveState());
    dock_manager_->addPerspective("history");
    dock_manager_->savePerspectives(settings);
}

// 恢复主窗口（包括窗口大小、位置、布局和自定义界面状态）**到上一次关闭时保存的样子
void MainWindow::RestoreState()
{
    QSettings settings("state.ini", QSettings::IniFormat);
    this->restoreGeometry(settings.value("mainWindow/Geometry").toByteArray());
    this->restoreState(settings.value("mainWindow/State").toByteArray());
    dock_manager_->loadPerspectives(settings);
    dock_manager_->openPerspective("history");
}

void MainWindow::updateOdomInfo(RobotState state)
{
    // 转向灯
    //   if (state.w > 0.1) {
    //     ui->label_turnLeft->setPixmap(
    //         QPixmap::fromImage(QImage("://images/turnLeft_hl.png")));
    //   } else if (state.w < -0.1) {
    //     ui->label_turnRight->setPixmap(
    //         QPixmap::fromImage(QImage("://images/turnRight_hl.png")));
    //   } else {
    //     ui->label_turnLeft->setPixmap(
    //         QPixmap::fromImage(QImage("://images/turnLeft_l.png")));
    //     ui->label_turnRight->setPixmap(
    //         QPixmap::fromImage(QImage("://images/turnRight_l.png")));
    //   }
    // 仪表盘
    speed_dash_board_->set_speed(abs(state.vx * 100));
    if (state.vx > 0.001)
    {
        speed_dash_board_->set_gear(DashBoard::kGear_D);
    }
    else if (state.vx < -0.001)
    {
        speed_dash_board_->set_gear(DashBoard::kGear_R);
    }
    else
    {
        speed_dash_board_->set_gear(DashBoard::kGear_N);
    }
    //   QString number = QString::number(abs(state.vx * 100)).mid(0, 2);
    //   if (number[1] == ".") {
    //     number = number.mid(0, 1);
    //   }
    //  ui->label_speed->setText(number);
    //  ui->mapViz->grab().save("/home/chengyangkj/test.jpg");
    //  QImage image(mysize,QImage::Format_RGB32);
    //           QPainter painter(&image);
    //           myscene->render(&painter);   //关键函数
}

void MainWindow::SlotSetBatteryStatus(double percent, double voltage)
{
    battery_bar_->setValue(percent);
    label_power_->setText(QString::number(voltage, 'f', 2) + "V");
}




/********************************************槽函数********************************************/

// 发送目标点位的槽函数
void MainWindow::onSigSendNavGoal(const std::any &data)
{
    SendChannelMsg(MsgId::kSetNavGoalPose, data);
}

// 发送重定位的槽函数
void MainWindow::onSigPub2DPose(const RobotPose &pose)
{
    SendChannelMsg(MsgId::kSetRelocPose, pose);
}

// 发送目标点位的槽函数
void MainWindow::onSigPub2DGoal(const RobotPose &pose)
{
    SendChannelMsg(MsgId::kSetNavGoalPose, pose);
}

// 对接收到的 ROS2 数据进行分发
void MainWindow::onSigRecvChannelData(const MsgId &id, const std::any &data)
{
    switch (id)
    {
    case MsgId::kOdomPose:
        // 里程计数器
        updateOdomInfo(std::any_cast<RobotState>(data));
        break;
    case MsgId::kRobotPose:
    {
        // 坐标变化
        nav_goal_table_view_->UpdateRobotPose(std::any_cast<RobotPose>(data));
        break;
    }
    case MsgId::kBatteryState:
    {
        // 当前电量
        std::map<std::string, std::string> map = std::any_cast<std::map<std::string, std::string>>(data);
        this->SlotSetBatteryStatus(std::stod(map["percent"]), std::stod(map["voltage"]));
        break;
    }
    case MsgId::kImage:
    {
        // 相机图片信息
        auto location_to_mat = std::any_cast<std::pair<std::string, std::shared_ptr<cv::Mat>>>(data);
        this->SlotRecvImage(location_to_mat.first, location_to_mat.second);
        break;
    }
    default:
        break;
    }
    display_manager_->UpdateTopicData(id, data);
}

// 接收当前光标的坐标
void MainWindow::onSigCursorPose(const std::string &display_name, QPointF pos)
{
    if(display_name != DISPLAY_MAP)
    {
        return;
    }

    basic::Point mapPos = display_manager_->mapPose2Word(basic::Point(pos.x(), pos.y()));
    label_pos_map_->setText("( x:" + QString::number(mapPos.x).mid(0, 4) + " y:" + QString::number(mapPos.y).mid(0, 4) + ") ");
    label_pos_scene_->setText("(x:" + QString::number(pos.x()).mid(0, 4) + " y:" + QString::number(pos.y()).mid(0, 4) + ")");
}
