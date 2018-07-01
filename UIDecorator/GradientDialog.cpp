#include "GradientDialog.h"
#include "ui_GradientDialog.h"
#include <QColorDialog>

GradientDialog::GradientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradientDialog)
{
    ui->setupUi(this);
    initialize();
}

GradientDialog::~GradientDialog()
{
    delete ui;
}

QString GradientDialog::getGradient()
{
    exec();
    QString res = mResult;
    mResult = "";
    return res;
}

void GradientDialog::initialize()
{
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    mColors.push_back(Qt::white);
    mColors.push_back(Qt::black);
    connect(ui->mButtonOk,&QPushButton::clicked,                                                        [&](){ drawGradient();mResult = mGradient;close(); });
    connect(ui->mButtonCancel,&QPushButton::clicked,                                                    [&](){ close(); });
    connect(ui->mButtonColor1,&QPushButton::clicked,                                                    [&](){ selectColor(0); });
    connect(ui->mButtonColor2,&QPushButton::clicked,                                                    [&](){ selectColor(1); });
    connect(ui->mButtonSwitch,&QPushButton::clicked,                                                    [&](){ switchColors(); });
    connect(ui->mPointSlider1,&QSlider::valueChanged,                                                   [&](){ drawGradient(); });
    connect(ui->mPointSlider2,&QSlider::valueChanged,                                                   [&](){ drawGradient(); });
    connect(ui->mTypeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),   [&](){ drawGradient(); });
    connect(ui->mAngleSpin,static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),               [&](){ drawGradient(); });

    drawGradient();
}

void GradientDialog::selectColor(int aIndex)
{
    mColors[aIndex] = QColorDialog::getColor(mColors[aIndex]);
    drawGradient();
}

void GradientDialog::switchColors()
{
    QColor color = mColors[0];
    mColors[0] = mColors[1];
    mColors[1] = color;
    drawGradient();
}

void GradientDialog::drawGradient()
{
    QString colors[2];
    double points[2],value;
    colors[0] = mColors[0].name(QColor::NameFormat::HexArgb);
    colors[1] = mColors[1].name(QColor::NameFormat::HexArgb);
    value = ui->mAngleSpin->value();
    points[0] = ui->mPointSlider1->value() / 100.0;
    points[1] = ui->mPointSlider2->value() / 100.0;

    ui->mButtonColor1->setStyleSheet( QString("background-color: %1;border: none;").arg(colors[0]) );
    ui->mButtonColor2->setStyleSheet( QString("background-color: %1;border: none;").arg(colors[1]) );

    switch (ui->mTypeComboBox->currentIndex()) {
        case 0: mGradient = QString("qlineargradient(x1:0, y1:1, x2:1, y2:1, stop:%1 %2, stop:%3 %4)").arg(points[0]).arg(colors[0]).arg(points[1]).arg(colors[1]); break;
        case 1: mGradient = QString("qlineargradient(x1:1, y1:0, x2:1, y2:1, stop:%1 %2, stop:%3 %4)").arg(points[0]).arg(colors[0]).arg(points[1]).arg(colors[1]); break;
        case 2: mGradient = QString("qradialgradient(cx:0.5, cy:0.5, radius:%1, fx:0.5, fy:0.5, stop:%2 %3, stop:%4 %5)").arg(value / 360.0f).arg(points[0]).arg(colors[0]).arg(points[1]).arg(colors[1]); break;
        case 3: mGradient = QString("qconicalgradient(cx:0.5, cy:0.5, angle:%1, stop:%2 %3, stop:%4 %5)").arg(value).arg(points[0]).arg(colors[0]).arg(points[1]).arg(colors[1]); break;
    }
    ui->mDemoWidget->setStyleSheet("background: " + mGradient);
}
