## 1. `complete/` Folder (Complete Version)

This folder contains the **robust and fully functional** implementations, ready for a "production" environment.

* **Main Focus:** **All operations (CRUD)**.
* **Key Differentiator:** Includes the complex logic for **Deletion (`delete`)**.
* **Complexity Examples:**
    * **AVL:** Handles recursion and rotations to rebalance after deletion.
    * **Red-Black:** Includes the difficult "fixup" cases for deletion (`fixDeleteLeftCase`, `fixDeleteRightCase`).
    * **2-3-4:** Implements node merging (`merge`) and key borrowing (`borrow`) to maintain the minimum number of keys.
* **Ideal Use:** Technical reference and applications that require guaranteed performance in all operations.

---

## 2. `simplified/` Folder (Simplified Version)

This folder contains **didactic** versions of the trees, focused on clarity and learning.

* **Main Focus:** **Insertion (`insert`) and Search (`search`)**.
* **Key Differentiator:** The **Deletion (`delete`) operation is generally omitted** to reduce complexity.
* **Objective:** To allow the reader to understand the tree's **balancing invariant** (rotations, splits, recoloring) without having to deal with the long and complex edge cases of deletion.
* **Ideal Use:** Study, first contact with the data structure, and focusing on the core balancing logic.