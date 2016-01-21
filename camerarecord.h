#ifndef CAMERARECORD_H
#define CAMERARECORD_H

#include <QWidget>

#include <QCamera>
#include <QCameraInfo>

#include <QMediaRecorder>

#include <QTimer>

namespace Ui {
class CameraRecord;
}

class CameraRecord : public QWidget
{
    Q_OBJECT

public:
    explicit CameraRecord(QWidget *parent = 0);
    ~CameraRecord();

    QString pathToSaveMovie() const;
    void setPathToSaveMovie(const QString &pathToSaveMovie);

    void startCamera();
    void stopCamera();

signals:
    void recordFinished(QString a_path);

private slots:
    void on_tbRecord_clicked();
    void stopRecorder();

    void loadCamera(int a_index);

    void on_comboBox_currentIndexChanged(int index);

    void on_tbStop_clicked();

private:
    Ui::CameraRecord *ui;

    QCamera *camera;
    QMediaRecorder *m_recorder;

    QString m_pathToSaveMovie;
};

#endif // CAMERARECORD_H
