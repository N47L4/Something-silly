Main Focus: Build and maintain the database structure for books, users, and transactions.
(I need it beofre March 29, 2026 ends)

Tasks:
- Database Schema Design
  Tables: Users, Books, Reservations, BorrowHistory, CreditScores.
  Relationships: Users ↔ BorrowHistory ↔ Books.
- Data Entry & Validation
  Ensure book details (title, author, slot, summary) are stored correctly.
  Maintain borrower records (name, contact, student ID).
- Availability & Slot Imagery
  Track book status (available/borrowed).
  Provide data for frontend to display green/red slots.
- Overdue & Penalty Records
  Store due dates, overdue flags, and penalties.
  Link overdue status to credit score updates.
- Audit & History
  Keep permanent records of all borrow/return transactions.
  Ensure data integrity for graduation clearance checks.

Data Manager (Admin-Only Access)
- Design database schema (Users, Books, Reservations, BorrowHistory, CreditScores).
- Maintain book details and borrower records (admin-only access).
- Track availability and slot imagery data for frontend display.
- Store overdue and penalty records linked to credit scores.
- Keep audit/history logs for graduation clearance checks.
- Ensure privacy and security: only admin can view/manage all accounts; students can only see their own records.
