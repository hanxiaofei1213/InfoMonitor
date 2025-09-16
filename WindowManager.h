#pragma once

#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>
#include <QResizeEvent>

class ConfigManager;

class WindowManager : public QObject {
    Q_OBJECT

public:
    explicit WindowManager(QMainWindow* window, ConfigManager* configManager, QObject* parent = nullptr);
    ~WindowManager();

    // 初始化窗口大小
    void setupWindowSize();
    
    // 处理窗口大小变化
    void handleResize();
    
    // 保存当前窗口大小
    void saveCurrentSize();

protected slots:
    void onSaveTimer();

protected:
    void calculateDefaultSize(int& width, int& height);
    void centerWindow(int width, int height);

private:
    QMainWindow* m_window = nullptr;
    ConfigManager* m_configManager = nullptr;
    QTimer* m_saveTimer = nullptr;
};
