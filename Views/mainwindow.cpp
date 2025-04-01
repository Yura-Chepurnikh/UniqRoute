#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unordered_map>
#include <QtWidgets>
#include <exception>

#include "../Core/node.hpp"
#include "../Core/edge.hpp"
#include "../Core/node_edge_table.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString str = "Hi! Please Enter the number of Nodes and Characteristics";
    QLabel* welcoming_label = new QLabel(str);

    QLabel* m_node_number = new QLabel("number of nodes");
    QLabel* m_characteristics_number = new QLabel("number of characteristics");
    QLineEdit* node_field = new QLineEdit();
    QLineEdit* characteristics_field = new QLineEdit();
    QPushButton* enter = new QPushButton("Enter");

    QGridLayout* grid = new QGridLayout();

    grid->addWidget(welcoming_label, 0, 0, 2, 0);

    grid->addWidget(m_node_number, 1, 0);
    grid->addWidget(node_field, 1, 1);

    grid->addWidget(m_characteristics_number, 2, 0);
    grid->addWidget(characteristics_field, 2, 1);

    grid->addWidget(enter, 3, 0);

    grid->setSpacing(30);

    QWidget* centralWidget = new QWidget();
    centralWidget->setMaximumHeight(300);
    centralWidget->setMaximumWidth(500);

    centralWidget->setLayout(grid);
    this->setCentralWidget(centralWidget);

    m_node_metrics = grid;

    QObject::connect(enter, &QPushButton::clicked, this, &MainWindow::RecordNodesCharacteristics);
}

void MainWindow::CheckReadNumberNodesCharacterisitics() {
    try {
        if (!m_node_metrics) {
            throw std::runtime_error("node metrics does not exist");
        }

        for (size_t i = 0; i < m_node_metrics->count(); ++i) {
            QLayoutItem* item = m_node_metrics->itemAt(i);

            if (!item) {
                continue;
            }

            QWidget* widget = item->widget();
            if (!widget) {
                continue;
            }

            QLineEdit* field = qobject_cast<QLineEdit*>(widget);
            QString data;
            if (field) {
                data = field->text();
                if (data.isEmpty()) {
                    std::string error_message = "field: " + std::to_string(i) + " is empty.";
                    throw std::runtime_error(error_message);
                }
            }

            if (!m_node_number) {
                m_node_number = data.toInt();
            }
            else if (!m_characteristics_number) {
                m_characteristics_number = data.toInt();
            }
        }
    }
    catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

void MainWindow::RecordNodesCharacteristics() {
    CheckReadNumberNodesCharacterisitics();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setMaximumHeight(250);
    centralWidget->setMaximumWidth(350);

    QGridLayout* grid = new QGridLayout(centralWidget);

    for (size_t row = 1; row <= m_node_number; ++row) {
        for (size_t col = 0; col < m_characteristics_number; ++col) {
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

    this->m_node_characteristics = grid;

    QPushButton* enter = new QPushButton("Enter");
    grid->addWidget(enter);
    centralWidget->setLayout(grid);
    this->setCentralWidget(centralWidget);

    m_node_characteristics = grid;
    connect(enter, &QPushButton::clicked, this, &MainWindow::SetConnections);
}

void MainWindow::CheckReadFields() {
    try {
        if (!m_node_characteristics) {
            throw std::runtime_error("Characteristics does not exist!");
        }

        int step = m_characteristics_number;
        qDebug() << m_characteristics_number;
        for (int i = 0; i < m_node_characteristics->count(); i += step) {
            Node* new_node = new Node();
            for (int j = 0; j < step; ++j) {
                QLayoutItem* item = m_node_characteristics->itemAt(i + j);
                if (!item) {
                    continue;
                }

                QWidget* widget = item->widget();
                if (!widget) {
                    continue;
                }

                QLineEdit* field = qobject_cast<QLineEdit*>(widget);
                QString data;
                if (field) {
                    data = field->text();
                    if (data.isEmpty()) {
                        std::string error_message = "field: " + std::to_string(i + j) + " is empty!";
                        throw std::runtime_error(error_message);
                    }
                }
                switch(j) {
                    case 0:
                    {
                        new_node->name = data.toStdString();
                        break;
                    }
                    case 1:
                    {
                        new_node->x = data.toDouble();
                        break;
                    }
                    case 2:
                    {
                        new_node->y = data.toDouble();
                        break;
                    }
                }
            }
            if (new_node->name != "")
                m_graphes.push_back(new_node);
        }

        for (auto begin = m_graphes.begin(); begin != m_graphes.end(); ++begin) {
            qDebug() << "info" << QString::fromStdString((*begin)->name) << " " << (*begin)->x << " " << (*begin)->y << "\n";
        }

        while (QLayoutItem* item = m_node_characteristics->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->hide();
                delete widget;
            }
            delete item;
        }
    }
    catch(const std::exception& e) {
        qDebug() << e.what();
    }
}

void MainWindow::SetConnections() {
    CheckReadFields();

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
