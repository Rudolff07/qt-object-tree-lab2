#include <QApplication>
#include "mainwindow.h"
#include <QDebug>

// --- ДЕМОНСТРАЦИЯ ОШИБКИ 1 ---
void demoError1() {
    qDebug() << "--- Старт демонстрации Ошибки 1 (стековый ребенок) ---";
    QWidget *parent = new QWidget; // Родитель в куче
    QLabel child("Я ребенок", parent); // Ребенок на стеке

    // При выходе из функции child умрет.
    // А parent останется жить с указателем на мертвый объект.

    delete parent; // Пытаемся удалить родителя, он попытается удалить мертвого ребенка -> CRASH!
}
// ------------------------------

// -- ДЕМОНСТРАЦИЯ ОШИБКИ 2 --
void demoError2() {
    qDebug() << "=== Старт демонстрации Ошибки 2 ===";
    QWidget *parent = new QWidget;
    QLabel *child = new QLabel("Я - забытый ребенок", parent); // Ребенок в куче

    qDebug() << "Ребенок создан. Его родитель:" << parent;
    qDebug() << "Отвязываем ребенка: child->setParent(nullptr)...";

    child->setParent(nullptr); // Теперь у ребенка НЕТ родителя

    qDebug() << "Удаляем родителя (delete parent)...";
    delete parent;

    qDebug() << "Родитель удален. Но ребенок все еще в памяти!";
    qDebug() << "Произошла УТЕЧКА ПАМЯТИ. Указатель child больше никто не удалит.";

    // ЗДЕСЬ ДОЛЖНА БЫТЬ СТРОКА: child->deleteLater();
    // Но мы ее специально не пишем, чтобы показать ошибку.

    qDebug() << "Конец функции demoError2. Утечка памяти зафиксирована.";
}

// ---------------------------


// -- ДЕМОНСТРАЦИЯ ОШИБКИ 3 --
void demoError3() {
    qDebug() << "=== Старт демонстрации Ошибки 3 ===";

    // Создаем окна и компоновщики
    QWidget *window = new QWidget;
    window->setWindowTitle("Демо Ошибки 3");
    window->resize(400, 300);

    QVBoxLayout *layout1 = new QVBoxLayout(window);
    QWidget *panel = new QWidget;
    QVBoxLayout *layout2 = new QVBoxLayout(panel);

    QPushButton *btn = new QPushButton("Я - кнопка");
    layout2->addWidget(btn); // Кнопка в панели (layout2)

    layout1->addWidget(panel); // Панель в окне (layout1)
    window->show();

    qDebug() << "Кнопка находится в layout2 (внутри панели).";
    qDebug() << "ТЕПЕРЬ ДЕЛАЕМ НЕПРАВИЛЬНО: просто меняем родителя без работы с layout...";

    // ОШИБКА: Меняем родителя, но НЕ удаляем из старого layout и НЕ добавляем в новый!
    btn->setParent(window);

    qDebug() << "Визуально кнопка может застрять или исчезнуть, так как layout1 о ней не знает.";

}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Демонстрация 1 ошибки
    //demoError1();

    //Демонстрация 2 ошибки
    //demoError2();

    //Демонстрация 3 ошибки
    //demoError3();

    MainWindow window;
    window.show();
    return app.exec();
}