import sys
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QVBoxLayout, QLineEdit, QHBoxLayout, QPushButton, QTableWidget, QTableWidgetItem
from PyQt5.QtCore import Qt, QTimer

ROWS = 100

class ViolationReportGUI(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("STA Path Violation Report")
        self.setGeometry(100, 100, 800, 600)

        self.report_lines = self.read_violations()
        self.filtered_lines = self.report_lines[:]
        self.total_rows = len(self.filtered_lines)

        self.current_page = 0
        self.total_pages = max(1, self.total_rows // ROWS_PER_PAGE)
        
        layout = QVBoxLayout()

        self.filter_input = QLineEdit(self)
        self.filter_input.setPlaceholderText("Filter by path name")
        self.filter_input.textChanged.connect(self.filter_report)
        layout.addWidget(self.filter_input)

        self.table_widget = QTableWidget(self)
        layout.addWidget(self.table_widget)

        nav_layout = QHBoxLayout()
        self.prev_button = QPushButton("Previous", self)
        self.next_button = QPushButton("Next", self)
        self.prev_button.clicked.connect(self.prev_page)
        self.next_button.clicked.connect(self.next_page)
        nav_layout.addWidget(self.prev_button)
        nav_layout.addWidget(self.next_button)
        layout.addLayout(nav_layout)
        self.update_table()
        self.setLayout(layout)

    def read_violations(self):
        with open("../reports/violations.txt", "r") as file:
            return file.readlines()

    def filter_report(self):
        query = self.filter_input.text().lower()
        self.filtered_lines = [line for line in self.report_lines if query in line.lower()]
        self.total_rows = len(self.filtered_lines)
        self.current_page = 0
        self.update_table()

    def update_table(self):
        self.table_widget.clear()
        start = self.current_page * ROWS
        end = min(start + ROWS, self.total_rows)

        self.table_widget.setRowCount(end - start)
        self.table_widget.setColumnCount(1)
        self.table_widget.setHorizontalHeaderLabels(["Violation Details"])

        for i, line in enumerate(self.filtered_lines[start:end]):
            item = QTableWidgetItem(line.strip())
            self.table_widget.setItem(i, 0, item)

        self.update_navigation_buttons()

    def update_navigation_buttons(self):
        self.prev_button.setEnabled(self.current_page > 0)
        self.next_button.setEnabled(self.current_page < self.total_pages - 1)

    def next_page(self):
        if self.current_page < self.total_pages - 1:
            self.current_page += 1
            self.update_table()

    def prev_page(self):
        if self.current_page > 0:
            self.current_page -= 1
            self.update_table()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = ViolationReportGUI()
    window.show()
    sys.exit(app.exec_())
