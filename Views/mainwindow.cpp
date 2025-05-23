#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <unordered_map>
#include <QtWidgets>
#include <exception>

#include "../Core/node.hpp"
#include "../Core/edge.hpp"
#include "../Core/graph.hpp"

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

void MainWindow::RecordNodesCharacteristics() {
    CheckReadNumberNodesCharacterisitics();

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setMaximumHeight(300);
    centralWidget->setMaximumWidth(300);

    QGridLayout* grid = new QGridLayout(centralWidget);

    for (size_t row = 1; row <= m_node_number; ++row) {
        for (size_t col = 0; col < m_characteristics_number; ++col) {
            QLineEdit* cell = new QLineEdit();
            grid->addWidget(cell, row, col, Qt::AlignLeft | Qt::AlignTop);
        }
    }

    QLabel* name = new QLabel("Name");
    QLabel* x = new QLabel("X");
    QLabel* y = new QLabel("Y");

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

void MainWindow::SetConnections() {
    CheckReadFields();

    QGridLayout* grid = new QGridLayout();

    int col = 1;
    for (auto begin = m_nodes.begin(); begin != m_nodes.end(); ++begin, ++col) {
        auto name = QString::fromStdString((*begin)->name);
        QLabel* graph_name = new QLabel(name);
        grid->addWidget(graph_name, 0, col);
    }

    int row = 1;
    for (auto begin = m_nodes.begin(); begin != m_nodes.end(); ++begin, ++row) {
        auto name = QString::fromStdString((*begin)->name);
        QLabel* graph_name = new QLabel(name);
        grid->addWidget(graph_name, row, 0, Qt::AlignCenter);
    }

    const size_t number_of_graphs = m_nodes.size();
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

    QPushButton* enter = new QPushButton("Enter");
    enter->setFixedSize(50, 39);
    grid->addWidget(enter);

    QWidget* central_widget = new QWidget();
    central_widget->setMaximumHeight(300);
    central_widget->setMaximumWidth(300);

    central_widget->setLayout(grid);

    m_node_connections = grid;
    this->setCentralWidget(central_widget);

    connect(enter, &QPushButton::clicked, this, &MainWindow::ReadConnections);
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

void MainWindow::CheckReadFields() {
    try {
        if (!m_node_characteristics) {
            throw std::runtime_error("Characteristics does not exist.");
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
                m_nodes.push_back(new_node);
        }

        for (auto begin = m_nodes.begin(); begin != m_nodes.end(); ++begin) {
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

void MainWindow::ReadConnections() {
    try {
        if (!m_node_connections) {
            throw std::runtime_error("Characteristics does not exist.");
        }
        int count = -1;
        for (size_t i = 0; i < m_node_connections->count(); ++i) {
            QLayoutItem* item = m_node_connections->itemAt(i);
            if (!item) {
                continue;
            }
            QWidget* widget = item->widget();
            if (!widget) {
                continue;
            }

            QPushButton* toggle = qobject_cast<QPushButton*>(widget);
            if (toggle) {
                ++count;

                QString data = toggle->text();

                if (data == "1") {

                    int i_index = count / m_node_number;
                    int j_index = count % m_node_number;

                    qDebug() << count << "index name: " << i_index << "\t" << j_index;

                    qDebug() << "node name: " << QString::fromStdString(m_nodes[i_index]->name) << "\t" << QString::fromStdString( m_nodes[j_index]->name);

                    if (m_nodes[i_index] && m_nodes[j_index]) {

                    auto cost = std::sqrt(std::pow(m_nodes[i_index]->x - m_nodes[j_index]->x, 2) +
                                          std::pow(m_nodes[i_index]->y - m_nodes[j_index]->y, 2));

                    Edge* edge = new Edge(cost, m_nodes[i_index], m_nodes[j_index]);

                    m_edges.push_back(edge);
                    }
                }
            }
        }

        std::unordered_map<Node*, std::list<Edge*>> table;

        for (size_t i = 0; i < m_edges.size() - 1; ++i) {
            std::list<Edge*> edges;
            edges.push_back(m_edges[i]);
            for (size_t j = i + 1; j < m_edges.size(); ++j) {
                if (m_edges[i]->nodes.first == m_edges[j]->nodes.first) {
                    edges.push_back(m_edges[j]);
                }
            }
            table.emplace(m_edges[i]->nodes.first, edges);
        }
        m_graph = Graph::GetInstance(table);

        while (QLayoutItem* item = m_node_connections->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->hide();
                delete widget;
            }
            delete item;
        }
        update();
        FindShortestPath();
    } catch (const std::exception& e) {
        qDebug() << e.what();
    }
}

void MainWindow::FindShortestPath() {
    QGridLayout* grid = new QGridLayout;

    QLabel* starting_node_label = new QLabel("Start");
    QListWidget* starting_node_field = new QListWidget();
    for (auto node : m_nodes) {
        starting_node_field->addItem(QString::fromStdString(node->name));
    }
    starting_node_field->setSelectionMode(QAbstractItemView::SingleSelection);

    QLabel* final_node_label = new QLabel("End");
    QListWidget* final_node_field = new QListWidget();
    for (auto node : m_nodes) {
        final_node_field->addItem(QString::fromStdString(node->name));
    }
    final_node_field->setSelectionMode(QAbstractItemView::SingleSelection);

    grid->addWidget(starting_node_label, 0, 0);
    grid->addWidget(final_node_label, 0, 1);
    grid->addWidget(starting_node_field, 1, 0);
    grid->addWidget(final_node_field, 1, 1);

    QPushButton* enter = new QPushButton("Enter");

    grid->addWidget(enter);

    QWidget* central_widget = new QWidget();
    central_widget->setLayout(grid);
    this->setCentralWidget(central_widget);

    connect(enter, &QPushButton::clicked, [starting_node_field, final_node_field, this, grid]() {
        QListWidgetItem* selected_starting_node = starting_node_field->currentItem();
        QListWidgetItem* selected_final_node = final_node_field->currentItem();

        for (auto node : m_nodes) {
            if (selected_starting_node->text() == QString::fromStdString(node->name)) {
                m_starting_node = node;
            }
            else if (selected_final_node->text() == QString::fromStdString(node->name)) {
                m_final_node = node;
            }
        }

        while (QLayoutItem* item = grid->takeAt(0)) {
            if (QWidget* widget = item->widget()) {
                widget->hide();
                delete widget;
            }
            delete item;
        }

        qDebug() << "starting node" << QString::fromStdString(m_starting_node->name);
        qDebug() << "final node" << QString::fromStdString(m_final_node->name);

        m_isAllowedDraw = true;
        update();
    });
}

void MainWindow::paintEvent(QPaintEvent* event) {
    if (m_isAllowedDraw) {
        QPainter painter(this);
        QPen pen(Qt::red);
        painter.setPen(pen);

        for (const auto& [key, value] : m_graph->GetTable()) {
            painter.setPen(Qt::red);

            QString str = QString::fromStdString(key->name);
            painter.drawText(key->x + 20, key->y, str);

            painter.drawEllipse(QPoint(key->x, key->y), 5, 5);

            painter.setPen(Qt::blue);

            for (auto i = value.begin(); i != value.end(); ++i) {
                QPoint point1((*i)->nodes.first->x, (*i)->nodes.first->y);
                QPoint point2((*i)->nodes.second->x, (*i)->nodes.second->y);
                painter.drawLine(point1, point2);
            }
        }

        std::list<Node*> shortest_path = m_graph->DijkstraAlgorithm(m_starting_node, m_final_node, m_graph);

        QPen path_pen(Qt::green);
        painter.setPen(path_pen);

        auto current = shortest_path.begin();
        auto next = current;
        ++next;

        while (next != shortest_path.end()) {
            Node* from = *current;
            Node* to = *next;

            QPoint p1(from->x, from->y);
            QPoint p2(to->x, to->y);
            QLine segment(p1, p2);
            painter.drawLine(segment);

            ++current;
            ++next;
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
