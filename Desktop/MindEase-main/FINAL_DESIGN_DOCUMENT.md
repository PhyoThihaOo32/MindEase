# MindEase Final Design Document

**Project:** MindEase - BMCC Wellness Companion  
**Course:** CSC211H Advanced Programming Techniques (Honors)  
**Student:** Phyo Thiha Oo  
**Semester:** Spring 2026  
**Platform:** C++17 / Qt 6 desktop application

## 1. Project Summary

MindEase is a calm desktop wellness companion for Borough of Manhattan Community College students. The application helps students find campus support resources, explore simple wellness tools, and keep a private local journal.

MindEase is a student-support and campus-navigation tool. It is not a therapist, doctor, counselor, diagnosis tool, treatment planner, medication guide, or emergency service.

## 2. Problem Statement

Students often need help but may not know which office, page, or resource to start with. BMCC has many support services, but students may feel stressed, overwhelmed, or unsure where to search first.

MindEase reduces that friction by organizing support around student needs instead of office names. A student can choose a topic such as study stress, finance, immigration, wellness, relationships, or career support, then receive curated contact information, links, and next steps.

## 3. Design Goals

- Keep the interface calm, clean, and easy to explore.
- Use simple navigation instead of overwhelming menus.
- Organize support by student needs and subtopics.
- Demonstrate C++ OOP concepts through clean class structure.
- Use file I/O for local journal storage.
- Avoid storing private journal data in a cloud database.
- Provide a polished Zen-inspired visual identity with soft green tones and gentle glow.

## 4. Scope

### Included

- Home screen with clear entry points.
- BMCC resource finder with search for topics and subtopics.
- Mental Health Toolkit with self-care resource categories.
- Journal page with local save, preview, and delete.
- Embedded local PDF resource support.
- Lucide icon-based UI.

### Not Included

- User accounts.
- Cloud database storage.
- Medical diagnosis or treatment planning.
- Emergency response handling by the app itself.
- Replacing professional counseling or emergency services.

## 5. Core Features

### Home

The Home screen introduces MindEase with a simple message and direct buttons to the major sections.

### BMCC Resources

The BMCC Resources page organizes campus help by need. Students can browse cards or search by topic/subtopic. The search algorithm matches both high-level categories and nested resource details.

### Mental Health Toolkit

The toolkit provides low-risk wellness tools and resource links. It includes topics such as academic stress, mindfulness, connection and community, sleep, nutrition, music, identity care, and journaling.

### My Journal

The journal lets students write private reflections. Entries are saved locally as plain text files using file I/O. Users can view past reflections and delete entries they no longer want to keep.

## 6. Object-Oriented Design

- `Screen` is an abstract base class for all pages.
- `Home`, `Recommendations`, `Toolkit`, and `Journal` inherit from `Screen`.
- `MainWindow` owns the screen pages and switches between them through a `QStackedWidget`.
- `JournalStorage` handles file I/O.
- `JournalEntry` represents saved journal data.
- `LucideIcons` centralizes icon rendering.

## 7. File I/O Design

Journal entries are saved as local plain text files. This demonstrates file I/O while keeping the project simple and understandable.

Typical journal flow:

1. User writes a title and body.
2. `Journal` sends the data to `JournalStorage`.
3. `JournalStorage` creates a safe local filename.
4. Entry content is written to a `.txt` file.
5. Past entries are loaded back into the journal list.
6. User can preview or delete entries.

## 8. UI / Visual Design

MindEase uses a calm Zen-inspired visual system:

- Cream and soft green background.
- Rounded white and mint cards.
- Subtle green borders and shadows.
- Minimal navigation.
- Lucide icons.

## 9. Technology Stack

| Layer | Technology |
|---|---|
| Desktop UI | C++17, Qt 6 Widgets |
| Navigation | `QMainWindow`, `QStackedWidget`, custom `Screen` subclasses |
| Local Storage | Qt file I/O, plain text files |
| Icons | Lucide SVG icons |
| Build System | qmake |

## 10. Final Summary

MindEase demonstrates CSC211H course concepts through a complete student-centered application. It uses C++ OOP design, Qt UI development, file I/O, and modular architecture in a personal BMCC Honors Project focused on helping students find support more calmly and efficiently.
