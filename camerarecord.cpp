#include "camerarecord.h"
#include "ui_camerarecord.h"

#include <QUrl>
#include <QCameraViewfinder>
#include <QDir>
#include <QFileDialog>

CameraRecord::CameraRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraRecord)
{
    ui->setupUi(this);

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    if (cameras.count() > 0) {
        foreach (const QCameraInfo &cameraInfo, cameras) {
            ui->comboBox->addItem(cameraInfo.description());
        }
    }
    foreach (QWidget *widget , ui->stackedWidget->findChildren<QWidget*>()) {
        if (widget->inherits("QCameraViewfinder")) {
            widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            widget->setFixedHeight(480);
            widget->setFixedWidth(640);
        }
    }
}

void CameraRecord::loadCamera(int a_index)
{
    QString l_camDescription = ui->comboBox->itemText(a_index);
    foreach (const QCameraInfo &cameraInfo, QCameraInfo::availableCameras()) {
        if (cameraInfo.description().compare(l_camDescription) == 0) {
            camera = new QCamera(cameraInfo);
            m_recorder = new QMediaRecorder(camera);
        }
    }

    QVideoEncoderSettings settings = m_recorder->videoSettings();
    settings.setQuality(QMultimedia::VeryHighQuality);
    settings.setCodec("video/x-h264");

    m_recorder->setVideoSettings(settings);

    camera->setCaptureMode(QCamera::CaptureVideo);

    QCameraViewfinder *viewFinder = new QCameraViewfinder();
    camera->setViewfinder(viewFinder);
    ui->verticalLayout->addWidget(viewFinder);

    camera->start();
}

CameraRecord::~CameraRecord()
{
    delete ui;
}

void CameraRecord::on_tbRecord_clicked()
{
    if (pathToSaveMovie().isEmpty()) {
        setPathToSaveMovie(QFileDialog::getSaveFileName(this, "Escolha o local onde o "
                                                              "vídeo será salvo",
                                                        QDir::currentPath(),
                                                        "Vídeos"));
    }
    m_recorder->setOutputLocation(QUrl(pathToSaveMovie() + ".avi"));
    m_recorder->record();
}

void CameraRecord::stopRecorder()
{
    m_recorder->stop();
    emit recordFinished(pathToSaveMovie());
}

QString CameraRecord::pathToSaveMovie() const
{
    return m_pathToSaveMovie;
}

void CameraRecord::setPathToSaveMovie(const QString &pathToSaveMovie)
{
    m_pathToSaveMovie = pathToSaveMovie;
}

void CameraRecord::stopCamera()
{
    if (camera->state() == QCamera::ActiveState) {
        camera->stop();
    }
}

void CameraRecord::on_comboBox_currentIndexChanged(int a_index)
{
    loadCamera(a_index);
}

void CameraRecord::startCamera()
{
    if (camera->state() != QCamera::ActiveState) {
        camera->start();
    }
}

void CameraRecord::on_tbStop_clicked()
{
    stopRecorder();
}
