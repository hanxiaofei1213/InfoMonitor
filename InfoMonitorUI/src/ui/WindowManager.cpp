#include "WindowManager.h"
#include <QRect>
#include "UIConfigManager.h"

namespace {
constexpr int MIN_WIDTH = 1000;
constexpr int MIN_HEIGHT = 600;
constexpr double SCREEN_RATIO = 2.0 / 3.0;
constexpr int SAVE_DELAY_MS = 1000;
}

WindowManager::WindowManager(QMainWindow* window, UIConfigManager* pConfigManager, QObject* parent)
    : QObject(parent)
    , m_pWindow(window)
    , m_pConfigMgr(pConfigManager) {
    // 创建保存定时器
    m_pSaveTimer = new QTimer(this);
    m_pSaveTimer->setSingleShot(true);
    m_pSaveTimer->setInterval(SAVE_DELAY_MS);
    connect(m_pSaveTimer, &QTimer::timeout, this, &WindowManager::onSaveTimer);
}

WindowManager::~WindowManager() = default;

void WindowManager::setupWindowSize() {
    if (!m_pWindow || !m_pConfigMgr) {
        return;
    }

    // 获取配置中的窗口大小
    int configWidth = m_pConfigMgr->GetWindowWidth();
    int configHeight = m_pConfigMgr->GetWindowHeight();

    // 如果配置中有有效的窗口大小，使用配置的大小
    if (configWidth > 0 && configHeight > 0) {
        m_pWindow->resize(configWidth, configHeight);
        return;
    }

    // 如果配置中没有窗口大小，计算屏幕的2/3大小
    int windowWidth, windowHeight;
    calculateDefaultSize(windowWidth, windowHeight);

    m_pWindow->resize(windowWidth, windowHeight);

    // 保存计算出的大小到配置
    m_pConfigMgr->SetWindowWidth(windowWidth);
    m_pConfigMgr->SetWindowHeight(windowHeight);

    // 窗口居中显示
    centerWindow(windowWidth, windowHeight);
}

void WindowManager::handleResize() {
    if (!m_pSaveTimer) {
        return;
    }

    // 重启定时器，避免频繁保存
    m_pSaveTimer->start();
}

void WindowManager::saveCurrentSize() {
    if (!m_pWindow || !m_pConfigMgr) {
        return;
    }

    m_pConfigMgr->SetWindowWidth(m_pWindow->width());
    m_pConfigMgr->SetWindowHeight(m_pWindow->height());
}

void WindowManager::onSaveTimer() {
    saveCurrentSize();
}

void WindowManager::calculateDefaultSize(int& width, int& height) {
    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen) {
        QRect screenGeometry = primaryScreen->availableGeometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();

        // 计算2/3大小
        width = static_cast<int>(screenWidth * SCREEN_RATIO);
        height = static_cast<int>(screenHeight * SCREEN_RATIO);

        // 确保不小于最小尺寸
        width = qMax(width, MIN_WIDTH);
        height = qMax(height, MIN_HEIGHT);
    } else {
        // 如果无法获取屏幕信息，使用默认大小
        width = 1800;
        height = 1200;
    }
}

void WindowManager::centerWindow(int width, int height) {
    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    if (primaryScreen && m_pWindow) {
        QRect screenGeometry = primaryScreen->availableGeometry();
        int screenWidth = screenGeometry.width();
        int screenHeight = screenGeometry.height();

        int x = (screenWidth - width) / 2;
        int y = (screenHeight - height) / 2;

        m_pWindow->move(x, y);
    }
}
