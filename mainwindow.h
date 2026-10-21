#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPointer>
#include <QVBoxLayout>
#include <QWidget>
#include "widgets.h"

class MainWindow : public QWidget {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void moveMovableWidget();   // переместить кнопку между панелью и окном
    void deletePanel();        // удалить панель (deleteLater)
    void demoDetaching();      // демонстрация setParent(nullptr)

private:
    void setupUi();
    void setStatus(const QString &text);

    // Layout‑ы
    QPointer<QVBoxLayout> m_rootLayout;
    QPointer<QVBoxLayout> m_panelLayout;

    // Виджеты
    QPointer<LogLabel>      m_title;
    QPointer<LogLineEdit>   m_input;
    QPointer<LogLabel>      m_status;
    QPointer<LogPanel>      m_panel;
    QPointer<LogButton>     m_movable;      // перемещаемая кнопка
    QPointer<LogButton>     m_moveButton;
    QPointer<LogButton>     m_deleteButton;
};

#endif // MAINWINDOW_H