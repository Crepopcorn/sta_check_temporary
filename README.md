# STA Path Violation Checker

This project provides a temporary solution for checking path violations in STA (Static Timing Analysis) reports. This is not the final version due to confidentiality.

## Overview

The STA Path Violation Checker automates analysis of STA reports, efficiently identifying paths that violate timing constraints. Capable of handling reports with hundreds to million of violated paths.

## Components

1. **`main.c`**:
   - Generates random simulated path data for testing.

2. **`check_sta_violations.pl`**:
   - Parses large STA reports to identify path violations.
   - Uses parallel processing for improved performance.

3. **`gui_result.py`**:
   - User-friendly interface to view and filter violation results.
   - Supports pagination and search features.

4. **`run_analysis.sh`**:
   - Orchestrates execution of analysis pipeline.
   - Provides robust logging and error handling.
  
5. **`check.c`**:
   - Check any other violations in STA and show the result.

## Features

- **Scalability**
- **Parallel Processing**
- **Interactive GUI**
- **Comprehensive Logging**

## Requirements

- **Dependencies**:
  - Perl with `Parallel::ForkManager` module
  - Python 3 with `PyQt5`
  - C compiler (e.g., `gcc`)

## Usage

1. **Set Up**:
   - Clone the repository:
     ```bash
     git clone https://github.com/crepopcorn/sta_check_temporary.git
     cd sta_check_temporary
     ```

2. **Compile the C Program** (if applicable):
   ```bash
   gcc src/main.c -o bin/generate_sta_report
   ```

3. **Run the Analysis**:
   ```bash
   bash bin/run_analysis.sh
   ```

4. **View Results**:
   - Results displayed in PyQt GUI after running analysis.
  
## Disclaimer

This is a temporary implementation for STA report path violation checks. Specific details and functionalities may be restricted due to confidentiality.
