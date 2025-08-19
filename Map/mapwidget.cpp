#include "mapwidget.h"
#include "ui_mapwidget.h"
#include "AutoHideDockContainer.h"
#include "DockAreaTabBar.h"
#include "DockAreaTitleBar.h"
#include "DockAreaWidget.h"
#include "DockComponentsFactory.h"
#include "Eigen/Dense"
#include "FloatingDockContainer.h"
#include "signalmanager.h"
#include "manager/channel_manager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

using namespace ads;

MapWidget::MapWidget(QWidget *parent) : QWidget(parent)
    , ui(new Ui::MapWidget)
{
    ui->setupUi(this);
    init();
}

MapWidget::~MapWidget()
{
    delete ui;
}

void MapWidget::init()
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

    initUI();
}

void MapWidget::initUI()
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

    // slam 地图
    QHBoxLayout *slamTabWidgetLayout = new QHBoxLayout();
    slamTabWidgetLayout->setContentsMargins(0, 0, 0, 0);
    ui->slamTabWidget->setLayout(slamTabWidgetLayout);

    m_dock_manager = new CDockManager();
    slamTabWidgetLayout->addWidget(m_dock_manager);

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
    m_battery_bar = new QProgressBar();
    m_battery_bar->setObjectName(QString::fromUtf8("battery_bar_"));
    m_battery_bar->setMaximumSize(QSize(90, 16777215));
    m_battery_bar->setAutoFillBackground(true);
    m_battery_bar->setStyleSheet(QString::fromUtf8(
        "QProgressBar#m_battery_bar\n"
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

    m_battery_bar->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);
    horizontalLayout_tools->addWidget(m_battery_bar);

    QLabel *label_11 = new QLabel();
    label_11->setObjectName(QString::fromUtf8("label_11"));
    label_11->setMinimumSize(QSize(32, 32));
    label_11->setMaximumSize(QSize(32, 32));
    label_11->setPixmap(QPixmap(QString::fromUtf8(":/images/power-v.png")));

    horizontalLayout_tools->addWidget(label_11);

    m_label_power = new QLabel();
    m_label_power->setObjectName(QString::fromUtf8("label_power_"));
    m_label_power->setMinimumSize(QSize(50, 32));
    m_label_power->setMaximumSize(QSize(50, 32));
    m_label_power->setStyleSheet(QString::fromUtf8(""));

    horizontalLayout_tools->addWidget(m_label_power);
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
    m_display_manager = new Display::DisplayManager();
    center_h_layout->addWidget(m_display_manager->GetViewPtr());


    /***************************地图下方的坐标***************************/
    QHBoxLayout *horizontalLayout_12 = new QHBoxLayout();
    horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));

    QLabel *label = new QLabel();
    label->setText("map:");
    label->setObjectName(QString::fromUtf8("label"));
    horizontalLayout_12->addWidget(label);

    m_label_pos_map = new QLabel();
    m_label_pos_map->setObjectName(QString::fromUtf8("m_label_pos_map"));
    m_label_pos_map->setStyleSheet(QString::fromUtf8(""));
    horizontalLayout_12->addWidget(m_label_pos_map);

    QLabel *label_5 = new QLabel();
    label_5->setText("scene:");
    label_5->setObjectName(QString::fromUtf8("label_5"));
    horizontalLayout_12->addWidget(label_5);

    m_label_pos_scene = new QLabel();
    m_label_pos_scene->setObjectName(QString::fromUtf8("m_label_pos_scene"));
    horizontalLayout_12->addWidget(m_label_pos_scene);

    horizontalLayout_12->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    center_layout->addLayout(horizontalLayout_12);


    /***************************中心主窗体***************************/
    QWidget *center_widget = new QWidget();
    center_widget->setLayout(center_layout);
    CDockWidget *CentralDockWidget = new ads::CDockWidget("CentralWidget");
    CentralDockWidget->setWidget(center_widget);
    m_center_docker_area = m_dock_manager->setCentralWidget(CentralDockWidget);
    m_center_docker_area->setAllowedAreas(DockWidgetArea::OuterDockAreas);


    /***************************相机窗口***************************/
    // for (auto one_image : ConfigManager->GetRootConfig().images)
    // {
    //     LOG_INFO("init image window location:" << one_image.location << " topic:" << one_image.topic);
    //     m_image_frame_map[one_image.location] = new RatioLayoutedFrame();
    //     ads::CDockWidget *dock_widget = new ads::CDockWidget(std::string("image/" + one_image.location).c_str());
    //     dock_widget->setWidget(m_image_frame_map[one_image.location]);
    //     m_dock_manager->addDockWidget(ads::DockWidgetArea::RightDockWidgetArea, dock_widget, m_center_docker_area);
    //     dock_widget->toggleView(true);
    // }


    /******************************连接******************************/

    connect(reloc_btn, &QToolButton::clicked, this, [=]() {
        m_display_manager->StartReloc();
    });
    connect(save_map_btn, &QToolButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Map files", "", "Map files (*.yaml,*.pgm,*.pgm.json)");
        if (!fileName.isEmpty())
        {
            // 用户选择了文件夹，可以在这里进行相应的操作
            LOG_INFO("用户选择的保存地图路径：" << fileName.toStdString());
            m_display_manager->SaveMap(fileName.toStdString());
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
            m_display_manager->OpenMap(fileName.toStdString());
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
            m_display_manager->SetEditMapMode(Display::MapEditMode::kNormal);
            edit_map_btn->setText("结束编辑");
            tools_edit_map_widget->show();
        }
        else
        {
            m_display_manager->SetEditMapMode(Display::MapEditMode::kStop);
            edit_map_btn->setText("编辑地图");
            tools_edit_map_widget->hide();
        }
    });
    connect(add_point_btn, &QToolButton::clicked, this, [=]() {
        m_display_manager->SetEditMapMode(Display::MapEditMode::kAddPoint);
    });
    connect(normal_cursor_btn, &QToolButton::clicked, this, [=]() {
        m_display_manager->SetEditMapMode(Display::MapEditMode::kNormal);
    });
    connect(erase_btn, &QToolButton::clicked, this, [=]() {
        m_display_manager->SetEditMapMode(Display::MapEditMode::kErase);
    });
    connect(draw_line_btn, &QToolButton::clicked, this, [=]() {
        m_display_manager->SetEditMapMode(Display::MapEditMode::kDrawLine);
    });
    connect(draw_pen_btn, &QToolButton::clicked, this, [=]() {
        m_display_manager->SetEditMapMode(Display::MapEditMode::kDrawWithPen);
    });
    connect(SigManager, &SignalManager::sigRecvChannelData, this, &MapWidget::onSigRecvChannelData, Qt::BlockingQueuedConnection);
    connect(SigManager, &SignalManager::sigSendNavGoal, this, &MapWidget::onSigSendNavGoal);
    connect(SigManager, &SignalManager::sigPub2DPose, this, &MapWidget::onSigPub2DPose);
    connect(SigManager, &SignalManager::sigPub2DGoal, this, &MapWidget::onSigPub2DGoal);
    connect(SigManager, &SignalManager::sigCursorPose, this, &MapWidget::onSigCursorPose);
}

// 发送 ROS2 数据的接口
void MapWidget::SendChannelMsg(const MsgId &id, const std::any &data)
{
    ChannelManager::instance()->SendMessage(id, data);
}






/********************************************槽函数********************************************/

// 发送目标点位的槽函数
void MapWidget::onSigSendNavGoal(const std::any &data)
{
    SendChannelMsg(MsgId::kSetNavGoalPose, data);
}

// 发送重定位的槽函数
void MapWidget::onSigPub2DPose(const RobotPose &pose)
{
    SendChannelMsg(MsgId::kSetRelocPose, pose);
}

// 发送目标点位的槽函数
void MapWidget::onSigPub2DGoal(const RobotPose &pose)
{
    SendChannelMsg(MsgId::kSetNavGoalPose, pose);
}

// 对接收到的 ROS2 数据进行分发
void MapWidget::onSigRecvChannelData(const MsgId &id, const std::any &data)
{
    switch (id)
    {
    case MsgId::kOdomPose:
        // 里程计数器（暂未启用）
        updateOdomInfo(std::any_cast<RobotState>(data));
        break;
    case MsgId::kRobotPose:
    {
        // 坐标变换
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
    m_display_manager->UpdateTopicData(id, data);
}

// 接收当前光标的坐标
void MapWidget::onSigCursorPose(const std::string &display_name, QPointF pos)
{
    if(display_name != DISPLAY_MAP)
    {
        return;
    }

    basic::Point mapPos = m_display_manager->mapPose2Word(basic::Point(pos.x(), pos.y()));
    m_label_pos_map->setText("( x:" + QString::number(mapPos.x).mid(0, 4) + " y:" + QString::number(mapPos.y).mid(0, 4) + ") ");
    m_label_pos_scene->setText("(x:" + QString::number(pos.x()).mid(0, 4) + " y:" + QString::number(pos.y()).mid(0, 4) + ")");
}

void MapWidget::updateOdomInfo(RobotState state)
{

    // qDebug() << "速度：" << abs(state.vx * 100);

}

void MapWidget::SlotSetBatteryStatus(double percent, double voltage)
{
    m_battery_bar->setValue(percent);
    m_label_power->setText(QString::number(voltage, 'f', 2) + "V");
}

// 接收相机数据
void MapWidget::SlotRecvImage(const std::string &location, std::shared_ptr<cv::Mat> data)
{
    if (m_image_frame_map.count(location))
    {
        QImage image(data->data, data->cols, data->rows, data->step[0], QImage::Format_RGB888);
        m_image_frame_map[location]->setImage(image);
    }
}
