from PyQt5.QtWidgets import QWidget
from PyQt5.QtChart import QChartView, QChart, QStackedBarSeries, QBarSet, QBarCategoryAxis, QValueAxis
from const import *
from PyQt5.QtGui import QPainter, QBrush, QColor
from PyQt5.QtCore import Qt

DARK_BROWN = QColor(13, 8, 3)
YELLOW  = QColor(213, 219, 35)

RESET_AXIS_Y = 1

class Chart(QWidget):
    def __init__(self):
        super().__init__()
        self.axisY = QValueAxis()
        self.chart = self.create_chart()
        self.reset()

    def create_chart(self):
        self.series = QStackedBarSeries()
        self.series.append(QBarSet("Anillo"))
        self.series.append(QBarSet("Rumba"))

        bars = self.series.barSets()
        for bar in bars:
            if (bar.label() == "Anillo"):
                bar.setBrush(QBrush(YELLOW))
            
            elif (bar.label() == "Rumba"):
                bar.setBrush(QBrush(DARK_BROWN))

        chart = QChart()
        chart.legend().hide()
        chart.addSeries(self.series)
        chart.createDefaultAxes()
        chart.setAnimationOptions(QChart.SeriesAnimations)
        chart.setTitle("Galletas contadas")

        #chart.legend().setVisible(True)
        chart.legend().setAlignment(Qt.AlignBottom)

        cookies = ["Anillos", "Rumbas"]
        axisX = QBarCategoryAxis()
        axisX.append(cookies)

        self.axisY.setMin(0)
        self.axisY.setMax(RESET_AXIS_Y)
        chart.setAxisX(axisX, self.series)
        chart.setAxisY(self.axisY, self.series)

        chartview = QChartView(chart)
        chartview.setRenderHint(QPainter.Antialiasing)

        return chartview

    def update_bar(self, label:str):
        i = 0
        for bar in self.series.barSets():
            if (bar.label() == label):
                bar.replace(i, bar.at(i)+1)
                if (bar.at(i) >= self.axisY.max()):
                    self.axisY.setMax(self.axisY.max() + 1)
                break
            i = i + 1

    def reset(self):
        self.axisY.setMax(RESET_AXIS_Y)
        for bar in self.series.barSets():
            for i in range(0, len(self.series.barSets())):
                bar.insert(i,0)
