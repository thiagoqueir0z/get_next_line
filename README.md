## 💾 Project: get_next_line

### Overview

`get_next_line` is a core C programming project focused on developing an efficient and robust function capable of **reading a line of text from a file descriptor**, regardless of the file type or size.

This function must handle arbitrary file sizes, static variables for internal state management, and ensure optimal performance by reading the file in chunks defined by the `BUFFER_SIZE`. It is designed to work iteratively, returning one line per call.

---

### 🛠️ Key Skills Demonstrated

* **File Descriptor Management:**
    * Handling **file descriptors (FDs)** as input sources, including standard input (`STDIN`), files, and network sockets.
* **Dynamic Memory Allocation:**
    * Advanced use of `malloc()`, `free()`, and **static variables** to manage memory buffers that persist between function calls, ensuring the successful storage of unread data.
* **Buffer and Chunk Management:**
    * Implementing efficient reading strategies using a predefined `BUFFER_SIZE` to minimize system calls while retrieving data accurately.
* **String and Memory Manipulation:**
    * Mastery of custom or library functions (like those from `libft`) to join, search, and extract substrings within large blocks of dynamically allocated memory.
* **Error Handling:**
    * Robust handling of edge cases, including empty files, invalid file descriptors, read errors, and end-of-file (EOF) conditions.

---

### 🚀 Usage and Configuration

| Component | Configuration | Note |
| :--- | :--- | :--- |
| **Function Prototype** | `char *get_next_line(int fd)` | Returns the line read, or `NULL` on error/EOF. |
| **Input Source** | Any valid File Descriptor | Typically a text file, but can be `STDIN` (0). |
| **Internal Buffer Size** | `BUFFER_SIZE` macro | Defined during compilation; impacts reading performance. |
| **Dependencies** | Must be linked with `libft` | Uses custom memory/string functions for internal handling. |

#### Example Usage

To read lines from a file named `text.txt`:

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int main()
{
    int fd = open("text.txt", O_RDONLY);
    char *line;

    if (fd < 0) return 1;

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line); // Free the memory allocated by GNL
    }

    close(fd);
    return 0;
}
