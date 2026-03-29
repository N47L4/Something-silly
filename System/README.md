# Inventory Management System

A simple inventory / library management backend in C++ with an HTML/JavaScript frontend.

## Features

- Login and account creation
- Admin book management dashboard
- Book search, reservation, borrow, and return flows
- Credit score deductions and restrictions on overdue or low-credit users
- Overdue notification triggers simulated via logs and frontend alerts
- Responsive UI with slot badges (green/red/yellow)
- Data persistence in `storage.txt`
- Audit log and notification history

## Build & Run

1. Open a terminal in this folder.
2. Build the backend:
   - With `g++`:
     ```batch
     g++ main.cpp -o inventory.exe -std=c++17 -lws2_32
     ```
   - With MSVC Developer Command Prompt:
     ```batch
     cl /EHsc main.cpp ws2_32.lib
     ```
3. Run the server:
   ```batch
   inventory.exe
   ```
4. Open `http://localhost:8080/` in your browser.

## Default Admin

- Username: `admin`
- Password: `admin123`

## Pages

- `/login.html` — user/admin login
- `/register.html` — create a new student account
- `/admin.html` — admin-only book management
- `/user.html` — borrower account, search, and actions

## Notes

- The backend listens on port `8080`.
- If the request host is different, ensure the browser allows local requests.
- Notifications are appended to `notifications.log` and shown in the frontend.

## Files

- `main.cpp` — C++ server backend and data manager
- `www/` — frontend HTML, CSS, and JavaScript
- `storage.txt` — generated persistent storage file
- `notifications.log` — generated notification audit log
