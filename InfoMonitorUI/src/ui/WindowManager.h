#pragma once
#include <QObject>
#include <QMainWindow>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>
#include <QResizeEvent>

class UIConfigManager;

// ques(wangwenxi)：这里是定时器保存，还是有个接口自动保存，因为每一列的详细信息也需要用这个保存
// 控制窗口类
class WindowManager : public QObject {
    Q_OBJECT

public:
    explicit WindowManager(QMainWindow* window, UIConfigManager* pConfigManager, QObject* parent = nullptr);
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
    QMainWindow* m_pWindow = nullptr;
    UIConfigManager* m_pConfigMgr = nullptr;
    QTimer* m_pSaveTimer = nullptr;
};
