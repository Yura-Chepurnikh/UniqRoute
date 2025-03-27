#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QObject>

#include "../Core/node.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    void paintEvent(QPaintEvent* event) override;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void ReadFromAdjacencyMatrix();

private:
    std::list<Node*> m_nodes;
    QGridLayout* gridLayout = nullptr;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
