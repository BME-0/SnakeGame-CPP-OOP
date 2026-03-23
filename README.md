# 🐍 Advanced Snake Game (C++ OOP)

A professional, high-performance console-based Snake game built with **C++**. This project demonstrates advanced programming concepts, including Object-Oriented Design (OOP), efficient data structures, and optimized console rendering.

---

## 🚀 Key Features

### 🏗️ Object-Oriented Design
The game architecture is fully modular, using a clean class-based structure:
* **`ClsSnake`**: Manages snake movement, growth, and body logic.
* **`ClsBoard`**: Handles the game arena, walls, and boundary checks.
* **`ClsGameManager`**: The "brain" of the game that coordinates logic, levels, and scoring.
* **`ClsRenderer`**: Optimized engine for flicker-free console graphics.



### 📈 Dynamic Level System
The challenge evolves as you play:
* **Increasing Difficulty:** Game speed increases with each level.
* **Procedural Obstacles:** New obstacles (`!`) are randomly generated in every level, making each run unique.
* **Target Scores:** Each level has a specific score goal to progress.

### 🧠 Advanced Data Structures
* **`std::deque`**: Used for the snake's body to allow fast $O(1)$ insertions at the head and deletions at the tail.
* **`std::set`**: Used for obstacle management, providing high-performance lookup for collision detection.



### ⚡ Performance Optimized
* **Flicker-Free Rendering:** Uses `std::stringstream` to buffer the entire frame before printing, ensuring smooth visuals.
* **Precise Timing:** Implements `std::chrono` for frame-independent movement logic.

### 🍎 Multiple Food Types
| Food | Symbol | Effect |
| :--- | :---: | :--- |
| **Normal** | `*` | Standard points and growth. |
| **Bonus** | `$` | High score reward, expires over time. |
| **Poison** | `!` | Deducts points—avoid at all costs! |

---

## 🎮 How to Play

1.  **Move:** Use `Arrow Keys` or `WASD`.
2.  **Start:** Press `Enter` from the main menu.
3.  **Pause:** Press `P` anytime during the game.
4.  **Restart:** Press `R` after Game Over.
5.  **Exit:** Press `Esc` to close the game.

---

## 🛠️ Technical Requirements
* **Language:** C++17 or higher.
* **Compiler:** MSVC (Visual Studio 2022 recommended).
* **Platform:** Windows (Uses `<conio.h>` and `<windows.h>`).

---

## 👨‍💻 Developed By
**Bassem Mossad (BME-0)**
*Self-taught C++ Developer focused on writing clean and efficient code.*
