# MindEase Flowchart and UML

This document reflects the current active Qt/C++ application.

## Active Project Structure

```text
MindEase/
|- app/
|  |- main.cpp
|  |- mainwindow.h
|  `- mainwindow.cpp
|- core/
|  |- screen.h / screen.cpp
|  `- lucideicons.h / lucideicons.cpp
|- screens/
|  |- home.h / home.cpp
|  |- recommendations.h / .cpp
|  |- toolkit.h / toolkit.cpp
|  `- journal.h / journal.cpp
|- models/
|  |- journalentry.h
|  `- journalentry.cpp
`- storage/
   |- journalstorage.h
   `- journalstorage.cpp
```

## Application Flowchart

```mermaid
flowchart TD
    A([Start MindEase]) --> B["app/main.cpp creates QApplication"]
    B --> C["Create MainWindow"]
    C --> D["Create central widget and main layout"]
    D --> E["Instantiate screens as Screen*"]
    E --> F["Add screens to QStackedWidget"]
    F --> G["Build navigation bar"]
    G --> H["Show Home"]
    H --> I{"User action"}

    I -->|BMCC Resources| J["Recommendations.onActivated()"]
    J --> J1["Browse or search resource cards"]

    I -->|Mental Health Toolkit| K["Toolkit.onActivated()"]
    K --> K1["Open or close wellness folders"]

    I -->|My Journal| L["Journal.onActivated()"]
    L --> L1["Load saved local entries"]

```

## Journal Flowchart

```mermaid
flowchart TD
    A([Open Journal]) --> B["Journal.onActivated()"]
    B --> C["JournalStorage.loadEntries()"]
    C --> D["Populate saved entries list"]
    D --> E{"User action"}

    E -->|Save| F["Create JournalEntry"]
    F --> G["JournalStorage.saveEntry(entry)"]
    G --> H["Write local .txt file"]
    H --> C

    E -->|Select entry| I["JournalStorage.loadEntry(path)"]
    I --> J["Show preview"]

    E -->|Delete entry| K["Confirm delete"]
    K --> L["JournalStorage.deleteEntry(path)"]
    L --> C
```

## Class Diagram

```mermaid
classDiagram
    class Screen {
        <<abstract>>
        +QString title() const
        +QString screenId() const
        +void onActivated()
    }

    class MainWindow
    class Home
    class Recommendations
    class Toolkit
    class Journal
    class JournalStorage
    class JournalEntry

    Screen <|-- Home
    Screen <|-- Recommendations
    Screen <|-- Toolkit
    Screen <|-- Journal
    MainWindow o-- Screen
    Journal o-- JournalStorage
    JournalStorage ..> JournalEntry
```
