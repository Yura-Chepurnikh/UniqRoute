#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unordered_map>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QPen>
#include <QLine>
#include <QBrush>

#include "../Core/node.hpp"
#include "../Core/edge.hpp"
#include "../Core/node_edge_table.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    Node* node1 = new Node("node1", 40, 40);
    Node* node2 = new Node("node2", 200, 200);
    Node* node3 = new Node("node3", 60, 500);
    Node* node4 = new Node("node4", 400, 400);


    Edge* e12 = new Edge(100, node1, node2);
    Edge* e23 = new Edge(200, node2, node3);
    Edge* e31 = new Edge(300, node3, node1);

    std::unordered_map<Node*, std::list<Edge*>> table;

    std::list<Edge*> node1_edges {e12, e31};
    std::list<Edge*> node2_edges {e12, e23};
    std::list<Edge*> node3_edges {e23, e31};

    table.insert({node1, {}});
    table.insert({node2, {}});
    table.insert({node3, {node3_edges}});
    table.insert({node4, {}});

    NodeEdgeTable* nodeEdgeTable = NodeEdgeTable::GetInstance(table);

    QPainter painter(this);
    QPen pen(Qt::red);
    painter.setPen(pen);

    for (const auto& [key, value] : nodeEdgeTable->GetTable()) {
        painter.setPen(Qt::red);

        QLabel* name = new QLabel(key->name, this);
        name->show();
        name->move(key->x, key->y);

        painter.drawEllipse(QPoint(key->x, key->y), 5, 5);

        painter.setPen(Qt::blue);

        for (auto i = value.begin(); i != value.end(); ++i) {
            QPoint point1((*i)->nodes.first->x, (*i)->nodes.first->y);
            QPoint point2((*i)->nodes.second->x, (*i)->nodes.second->y);
            painter.drawLine(point1, point2);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
