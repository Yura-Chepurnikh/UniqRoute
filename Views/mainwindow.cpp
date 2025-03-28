#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unordered_map>
#include <QPainter>
#include <QDebug>
#include <QLabel>
#include <QPen>
#include <QLine>
#include <QBrush>

#include <QLayout>
#include <QLineEdit>
#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>

#include <QString>

#include "../Core/node.hpp"
#include "../Core/edge.hpp"
#include "../Core/node_edge_table.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const size_t number_of_graphs = 4;
    const size_t number_of_graph_properties = 3;

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setMaximumHeight(250);
    centralWidget->setMaximumWidth(350);

    QGridLayout* grid = new QGridLayout(centralWidget);

    for (size_t row = 1; row <= number_of_graphs; ++row) {
        for (size_t col = 0; col < number_of_graph_properties; ++col) {
            QLineEdit* cell = new QLineEdit();
            grid->addWidget(cell, row, col, Qt::AlignLeft | Qt::AlignTop);
        }
    }

    QLabel* name = new QLabel("Name");
    QLabel* x = new QLabel("Coordinate X");
    QLabel* y = new QLabel("Coordinate Y");

    QHBoxLayout* names_of_properties = new QHBoxLayout();

    names_of_properties->addWidget(name);
    names_of_properties->addWidget(x);
    names_of_properties->addWidget(y);

    QWidget* names_of_properties_widget = new QWidget();
    names_of_properties_widget->setLayout(names_of_properties);

    grid->addWidget(names_of_properties_widget, 0, 0, 1, 3);

    this->gridLayout = grid;

    QPushButton* enter = new QPushButton("Enter");
    grid->addWidget(enter);
    centralWidget->setLayout(grid);
    this->setCentralWidget(centralWidget);

    connect(enter, &QPushButton::clicked, this, &MainWindow::ReadFromAdjacencyMatrix);
}

void MainWindow::ReadFromAdjacencyMatrix() {
    if (!gridLayout)
        return;

    std::vector<QString> all_data_about_nodes;

    for (size_t i = 0; i < gridLayout->count(); ++i) {
        QLayoutItem* item = gridLayout->itemAt(i);
        if (!item)
            continue;

        QWidget* widget = item->widget();
        if (!widget)
            continue;

        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget);
        if (lineEdit) {
            QString data = lineEdit->text();
            all_data_about_nodes.push_back(data);
            qDebug() << "cell: " << i << "contain: " << data << '\n';
        }
    }
    for (size_t i = 0; i < all_data_about_nodes.size(); i += 3) {
        Node* new_node = new Node();
        for (size_t j = 0; j < 3; ++j) {
            qDebug() << "a" << all_data_about_nodes[i+j] << '\n';
            switch(j) {
                case 0:
                {
                    new_node->name = all_data_about_nodes[i+j].toStdString();
                    break;
                }
                case 1:
                {
                    new_node->x = all_data_about_nodes[i+j].toDouble();
                    break;
                }
                case 2:
                {
                    new_node->y = all_data_about_nodes[i+j].toDouble();
                    break;
                }
            }
        }
        m_graphes.push_back(new_node);
    }

    for (auto begin = m_graphes.begin(); begin != m_graphes.end(); ++begin)
        qDebug() << QString::fromStdString((*begin)->name) << " " << (*begin)->x << " " << (*begin)->y << "\n";

    while (QLayoutItem* item = gridLayout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->hide();
            delete widget;
        }
        delete item;
    }
    EstablishConnections();
}

void MainWindow::EstablishConnections() {
    QGridLayout* grid = new QGridLayout();

    int col = 1;
    for (auto begin = m_graphes.begin(); begin != m_graphes.end(); ++begin, ++col) {
        auto name = QString::fromStdString((*begin)->name);
        QLabel* graph_name = new QLabel(name);
        grid->addWidget(graph_name, 0, col);
    }

    int row = 1;
    for (auto begin = m_graphes.begin(); begin != m_graphes.end(); ++begin, ++row) {
        auto name = QString::fromStdString((*begin)->name);
        QLabel* graph_name = new QLabel(name);
        grid->addWidget(graph_name, row, 0, Qt::AlignCenter);
    }

    const size_t number_of_graphs = m_graphes.size();
    for (size_t row = 1; row <= number_of_graphs; ++row) {
        for (size_t col = 1; col <= number_of_graphs; ++col) {
            QPushButton* isConnected = new QPushButton("0");
            isConnected->setStyleSheet("background-color: gray");

            grid->addWidget(isConnected, row, col, Qt::AlignLeft | Qt::AlignTop);

            connect(isConnected, &QPushButton::clicked, [isConnected](){
                if (isConnected->text() == "1") {
                    isConnected->setStyleSheet("background-color: gray");
                    isConnected->setText("0");
                } else {
                    isConnected->setStyleSheet("background-color: orange");
                    isConnected->setText("1");
                }
            });
        }
    }

    QWidget* central_widget = new QWidget();
    central_widget->setMaximumHeight(200);
    central_widget->setMaximumWidth(200);

    central_widget->setLayout(grid);
    this->setCentralWidget(central_widget);

}


void MainWindow::paintEvent(QPaintEvent* event) {
    // Node* node1 = new Node("node1", 40, 40);
    // Node* node2 = new Node("node2", 200, 200);
    // Node* node3 = new Node("node3", 60, 500);
    // Node* node4 = new Node("node4", 400, 400);

    // Edge* e12 = new Edge(100, node1, node2);
    // Edge* e23 = new Edge(200, node2, node3);
    // Edge* e31 = new Edge(300, node3, node1);

    // std::unordered_map<Node*, std::list<Edge*>> table;

    // std::list<Edge*> node1_edges {e12, e31};
    // std::list<Edge*> node2_edges {e12, e23};
    // std::list<Edge*> node3_edges {e23, e31};

    // table.insert({node1, {}});
    // table.insert({node2, {}});
    // table.insert({node3, {node3_edges}});
    // table.insert({node4, {}});

    // NodeEdgeTable* nodeEdgeTable = NodeEdgeTable::GetInstance(table);

    // QPainter painter(this);
    // QPen pen(Qt::red);
    // painter.setPen(pen);

    // for (const auto& [key, value] : nodeEdgeTable->GetTable()) {
    //     painter.setPen(Qt::red);

    //     QLabel* name = new QLabel(key->name, this);
    //     name->show();
    //     name->move(key->x, key->y);

    //     painter.drawEllipse(QPoint(key->x, key->y), 5, 5);

    //     painter.setPen(Qt::blue);

    //     for (auto i = value.begin(); i != value.end(); ++i) {
    //         QPoint point1((*i)->nodes.first->x, (*i)->nodes.first->y);
    //         QPoint point2((*i)->nodes.second->x, (*i)->nodes.second->y);
    //         painter.drawLine(point1, point2);
    //     }
    // }
}

MainWindow::~MainWindow()
{
    delete ui;
}
