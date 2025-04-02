#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QObject>

#include "../Core/node.hpp"
#include "../Core/edge.hpp"
#include "../Core/node_edge_table.hpp"

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
    void ReadConnections();

private:
    bool m_isAllowedDraw {false};

    int m_node_number {0};
    int m_characteristics_number {0};

    std::vector<Node*> m_graphes;
    std::vector<Edge*> m_edges;

    QGridLayout* m_node_metrics {nullptr};
    QGridLayout* m_node_characteristics {nullptr};
    QGridLayout* m_node_connections {nullptr};

    NodeEdgeTable* m_nodeEdgeTable;

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
