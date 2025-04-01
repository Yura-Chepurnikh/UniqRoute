#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QObject>

#include "../Core/node.hpp"
#include "../Core/edge.hpp"

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
    void CheckReadNumberNodesCharacterisitics();
    void RecordNodesCharacteristics();
    void CheckReadFields();
    void SetConnections();

private:
    int m_node_number {0};
    int m_characteristics_number {0};

    std::vector<Node*> m_graphes;
    std::list<Edge*> m_edges;

    QGridLayout* m_node_metrics {nullptr};
    QGridLayout* m_node_characteristics {nullptr};
    QGridLayout* m_node_connections {nullptr};

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
