Main Focus: Implement the actual processes of borrowing, reserving, and tracking books.
(3 days from now it should be already running at least the basic)

Tasks:
- Login & Account Handling
  Validate login credentials against database.
  Connect “beep ID” scanning to student accounts.
- Reservation & Lending Logic
  Functions for borrowing, returning, and reserving books.
  Update book status (available → borrowed → returned).
- Credit Score Mechanism
  Deduct points for overdue returns.
  Restrict borrowing if score falls below threshold.
- Notification Trigger
  Logic to check due dates daily.
  Trigger SMS/Gmail notification system (integrate with APIs later).
- Integration
  Connect frontend forms (HTML) with backend functions.
  Ensure smooth data flow between UI and database
