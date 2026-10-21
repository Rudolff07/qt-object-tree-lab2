#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
}

MainWindow::~MainWindow()
{
    qDebug() << "[DESTROY] MainWindow";
}

/* -------------------------------------------------------------
   UI‑инициализация
   ------------------------------------------------------------- */
void MainWindow::setupUi()
{
    setObjectName("MainWindow");
    setWindowTitle("Qt Object Tree Demo");
    resize(520, 640);

    // ---------- корневой layout ----------
    m_rootLayout = new QVBoxLayout(this);

    // ---------- заголовок ----------
    m_title = new LogLabel("Дерево объектов Qt: владелец и потомки", this);
    m_rootLayout->addWidget(m_title.data());

    // ---------- поле ввода ----------
    m_input = new LogLineEdit(this);
    m_input->setPlaceholderText("Виджет‑ребёнок главного окна");
    m_rootLayout->addWidget(m_input.data());

    // ---------- статус ----------
    m_status = new LogLabel("Статус: окно создано", this);
    m_status->setWordWrap(true);
    m_rootLayout->addWidget(m_status.data());

    // ---------- панель‑владелец ----------
    m_panel = new LogPanel("Панель‑владелец", this);
    m_rootLayout->addWidget(m_panel.data());

    // layout внутри панели
    m_panelLayout = new QVBoxLayout(m_panel.data());

    // Статическая кнопка внутри панели
    LogButton *staticButton = new LogButton("Статическая кнопка внутри панели", m_panel.data());
    m_panelLayout->addWidget(staticButton);

    // Перемещаемая кнопка (по умолчанию в панели)
    m_movable = new LogButton("Перемещаемый виджет (в панели)", m_panel.data());
    m_panelLayout->addWidget(m_movable.data());

    // Кнопка «Переместить» (в корневой layout)
    m_moveButton = new LogButton("Переместить виджет: панель ↔ окно", this);
    connect(m_moveButton.data(), &QPushButton::clicked,
            this, &MainWindow::moveMovableWidget);
    m_rootLayout->addWidget(m_moveButton.data());

    // Кнопка «Удалить панель»
    m_deleteButton = new LogButton("Удалить панель (deleteLater)", this);
    connect(m_deleteButton.data(), &QPushButton::clicked,
            this, &MainWindow::deletePanel);
    m_rootLayout->addWidget(m_deleteButton.data());

    // Кнопка «Demo setParent(nullptr)»
    LogButton *detachButton = new LogButton("Демо setParent(nullptr)", this);
    connect(detachButton, &QPushButton::clicked,
            this, &MainWindow::demoDetaching);
    m_rootLayout->addWidget(detachButton);

    m_rootLayout->addStretch();

    setStatus(
        "Готово. Все виджеты созданы с родителями. "
        "Удаление родителя автоматически удалит детей."
        );
}

/* -------------------------------------------------------------
   Вспомогательная функция для вывода статуса
   ------------------------------------------------------------- */
void MainWindow::setStatus(const QString &text)
{
    if (!m_status.isNull())
        m_status->setText(text);
}

/* -------------------------------------------------------------
   Перемещение кнопки между панелью и главным окном
   ------------------------------------------------------------- */
void MainWindow::moveMovableWidget()
{
    if (m_movable.isNull()) {
        setStatus("Перемещаемый виджет уже уничтожен.");
        return;
    }

    if (m_movable->parentWidget() == m_panel.data()) {
        // из панели → окно
        if (!m_panelLayout.isNull())
            m_panelLayout->removeWidget(m_movable.data());

        m_movable->setParent(this);
        if (!m_rootLayout.isNull())
            m_rootLayout->addWidget(m_movable.data());

        m_movable->setText("Перемещаемый виджет (вне панели)");
        setStatus(
            "setParent(this) сделал главным окном новым владельцем. "
            "Панель больше не удалит этот виджет автоматически."
            );
    } else {
        // из окна → панель
        if (!m_rootLayout.isNull())
            m_rootLayout->removeWidget(m_movable.data());

        m_movable->setParent(m_panel.data());
        if (!m_panelLayout.isNull())
            m_panelLayout->addWidget(m_movable.data());

        m_movable->setText("Перемещаемый виджет (в панели)");
        setStatus(
            "Виджет снова стал ребёнком панели. "
            "Теперь он будет удалён вместе с панелью."
            );
    }

    m_movable->show();
    qDebug() << "[SET_PARENT] Новый владелец виджета:"
             << m_movable->parentWidget()->objectName();
}

/* -------------------------------------------------------------
   Удаление панели (deleteLater)
   ------------------------------------------------------------- */
void MainWindow::deletePanel()
{
    if (m_panel.isNull()) {
        setStatus("Панель уже удалена.");
        return;
    }

    if (!m_movable.isNull() && m_movable->parentWidget() == m_panel.data()) {
        setStatus(
            "Панель будет удалена. Перемещаемый виджет – её ребёнок, "
            "поэтому будет уничтожен автоматически."
            );
    } else {
        setStatus(
            "Панель будет удалена. Перемещаемый виджет НЕ её ребёнок, "
            "поэтому останется жив."
            );
    }

    // Отключаем кнопки, чтобы пользователь не нажал их повторно
    if (!m_moveButton.isNull())
        m_moveButton->setEnabled(false);
    if (!m_deleteButton.isNull())
        m_deleteButton->setEnabled(false);

    qDebug() << "[DELETE_LATER] Запрошено удаление панели:"
             << m_panel->objectName();
    m_panel->deleteLater();   // безопасное отложенное удаление
}

/* -------------------------------------------------------------
   Демонстрация setParent(nullptr)
   ------------------------------------------------------------- */
void MainWindow::demoDetaching()
{
    LogPanel *tempOwner = new LogPanel("TempOwner", this);
    tempOwner->hide();   // не показываем в UI, только для логов

    // Два дочерних QLabel
    new LogLabel("Ребёнок TempOwner", tempOwner);
    LogLabel *detachedChild = new LogLabel("Отвязанный ребёнок TempOwner", tempOwner);

    qDebug() << "[DEMO] Дети tempOwner до detach:"
             << tempOwner->children().count();

    // Отвязываем один из них
    detachedChild->setParent(nullptr);

    qDebug() << "[DEMO] Дети tempOwner после detach:"
             << tempOwner->children().count();

    setStatus(
        "Демо: один QLabel остался ребёнком TempOwner, "
        "другой отвязан через setParent(nullptr). "
        "При удалении TempOwner уничтожается только первый."
        );

    // Удаляем оба объекта – второй нужно удалить вручную
    tempOwner->deleteLater();
    detachedChild->deleteLater();
}