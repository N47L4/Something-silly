const baseUrl = "http://localhost:8080";

function setMessage(selector, message, type = "error") {
    const element = document.querySelector(selector);
    if (!element) return;
    element.textContent = message;
    element.style.color = type === "success" ? "#166534" : "#b91c1c";
}

async function apiFetch(path, method = "GET", payload = null) {
    const options = { method, headers: { "Content-Type": "application/json" } };
    if (payload) options.body = JSON.stringify(payload);
    const response = await fetch(baseUrl + path, options);
    return response.json();
}

function getUserState() {
    return JSON.parse(localStorage.getItem("inventoryUser") || "null");
}

function requireAuth(expectedRole) {
    const user = getUserState();
    if (!user || (expectedRole && user.role !== expectedRole)) {
        window.location.href = "/login.html";
        return null;
    }
    return user;
}

function logout() {
    localStorage.removeItem("inventoryUser");
    window.location.href = "/login.html";
}

async function handleLogin() {
    const form = document.getElementById("loginForm");
    const message = document.getElementById("message");
    if (!form) return;
    form.addEventListener("submit", async (event) => {
        event.preventDefault();
        setMessage("#message", "Signing in...", "success");
        const data = {
            username: form.username.value.trim(),
            password: form.password.value.trim(),
        };
        const result = await apiFetch("/api/login", "POST", data);
        if (result.success) {
            localStorage.setItem("inventoryUser", JSON.stringify(result));
            if (result.role === "admin") {
                window.location.href = "/admin.html";
            } else {
                window.location.href = "/user.html";
            }
        } else {
            setMessage("#message", result.message || "Login failed.");
        }
    });
}

async function handleRegister() {
    const form = document.getElementById("registerForm");
    if (!form) return;
    form.addEventListener("submit", async (event) => {
        event.preventDefault();
        setMessage("#message", "Creating account...", "success");
        const data = {
            username: form.username.value.trim(),
            email: form.email.value.trim(),
            password: form.password.value.trim(),
        };
        const result = await apiFetch("/api/register", "POST", data);
        if (result.success) {
            setMessage("#message", result.message, "success");
            setTimeout(() => window.location.href = "/login.html", 1400);
        } else {
            setMessage("#message", result.message || "Registration failed.");
        }
    });
}

async function loadAdminDashboard() {
    const admin = requireAuth("admin");
    if (!admin) return;
    document.getElementById("currentAdmin").textContent = `Admin: ${admin.username}`;
    document.getElementById("logoutButton").addEventListener("click", logout);
    const summaryGrid = document.getElementById("summaryGrid");
    const bookList = document.getElementById("bookList");
    const auditList = document.getElementById("auditLog");
    const result = await apiFetch("/api/books");
    const books = Array.isArray(result) ? result : [];
    const total = books.length;
    const available = books.filter((book) => book.status === "available").length;
    const borrowed = books.filter((book) => book.status === "borrowed").length;
    summaryGrid.innerHTML = `
        <div class="card"><h3>Total Books</h3><p>${total}</p></div>
        <div class="card"><h3>Available</h3><p>${available}</p></div>
        <div class="card"><h3>Borrowed</h3><p>${borrowed}</p></div>
    `;
    bookList.innerHTML = books.map((book) => `
        <article class="card">
            <h3>${book.title}</h3>
            <p><strong>Author:</strong> ${book.author}</p>
            <p><strong>Category:</strong> ${book.category}</p>
            <p><strong>Status:</strong> <span class="badge ${book.status === 'available' ? 'green' : 'red'}">${book.status}</span></p>
            <p><strong>Due:</strong> ${book.dueDate || 'n/a'}</p>
        </article>
    `).join("");
    const auditResponse = await apiFetch(`/api/audit?adminId=${admin.userId}`);
    if (Array.isArray(auditResponse)) {
        auditList.innerHTML = auditResponse.slice(-10).reverse().map((audit) => `
            <article class="audit-card">
                <h3>${audit.action}</h3>
                <p>${audit.details}</p>
                <p><em>${audit.actor} • ${audit.timestamp}</em></p>
            </article>
        `).join("");
    } else {
        auditList.textContent = "Unable to load audit log.";
    }
    const form = document.getElementById("addBookForm");
    form.addEventListener("submit", async (evt) => {
        evt.preventDefault();
        const payload = {
            adminId: admin.userId,
            title: form.title.value.trim(),
            author: form.author.value.trim(),
            category: form.category.value.trim(),
        };
        const response = await apiFetch("/api/admin/addbook", "POST", payload);
        if (response.success) {
            setMessage("#adminMessage", response.message, "success");
            form.reset();
            loadAdminDashboard();
        } else {
            setMessage("#adminMessage", response.message || "Unable to add book.");
        }
    });
}

function renderSlot(book, userId) {
    const available = book.status === "available";
    const isMine = book.borrowerId === userId;
    return available ? "green" : isMine ? "yellow" : "red";
}

async function loadUserPage() {
    const user = requireAuth();
    if (!user) return;
    document.getElementById("welcomeUser").textContent = `${user.username}`;
    document.getElementById("logoutButton").addEventListener("click", logout);
    await refreshUserSummary(user.userId);
    await refreshSearchResults();
    await refreshBorrowedBooks(user.userId);
    await refreshNotifications(user.userId);
    const searchForm = document.getElementById("searchForm");
    searchForm.addEventListener("submit", async (evt) => {
        evt.preventDefault();
        await refreshSearchResults();
    });
}

async function loadBrowsePage() {
    const searchForm = document.getElementById("browseSearch");
    const browseResults = document.getElementById("browseResults");
    async function refreshBrowse() {
        const query = document.querySelector("#browseSearch [name='query']").value.trim();
        const status = document.querySelector("#browseSearch [name='status']").value;
        const params = new URLSearchParams();
        if (query) params.set("query", query);
        if (status) params.set("status", status);
        const result = await apiFetch(`/api/books?${params.toString()}`);
        if (!Array.isArray(result)) {
            browseResults.textContent = "Unable to load books.";
            return;
        }
        browseResults.innerHTML = result.map((book) => `
            <article class="card">
                <h3>${book.title}</h3>
                <p><strong>Author:</strong> ${book.author}</p>
                <p><strong>Category:</strong> ${book.category}</p>
                <p><span class="badge ${book.status === 'available' ? 'green' : 'red'}">${book.status}</span></p>
                <p><strong>Due Date:</strong> ${book.dueDate || "n/a"}</p>
            </article>
        `).join("");
    }
    await refreshBrowse();
    searchForm.addEventListener("submit", async (evt) => {
        evt.preventDefault();
        await refreshBrowse();
    });
}

async function refreshUserSummary(userId) {
    const response = await apiFetch(`/api/user?userId=${userId}`);
    if (!response.success) return;
    document.getElementById("creditScore").textContent = response.creditScore;
    document.getElementById("overdueCount").textContent = response.overdueCount;
    document.getElementById("accountStatus").textContent = response.blocked ? "Restricted" : "Good Standing";
}

async function refreshSearchResults() {
    const query = document.querySelector("[name='query']").value.trim();
    const status = document.querySelector("[name='status']").value;
    const params = new URLSearchParams();
    if (query) params.set("query", query);
    if (status) params.set("status", status);
    const url = `/api/books?${params.toString()}`;
    const result = await apiFetch(url);
    const container = document.getElementById("searchResults");
    if (!Array.isArray(result)) {
        container.textContent = "Unable to load books.";
        return;
    }
    const user = getUserState();
    container.innerHTML = result.map((book) => `
        <article class="card">
            <h3>${book.title}</h3>
            <p><strong>Author:</strong> ${book.author}</p>
            <p><strong>Category:</strong> ${book.category}</p>
            <p><span class="badge ${renderSlot(book, user.userId)}">${book.status}</span></p>
            <p><strong>Due Date:</strong> ${book.dueDate || "n/a"}</p>
            <button ${book.status !== 'available' ? 'disabled' : ''} onclick="borrowBook(${book.id})">${book.status === 'available' ? 'Reserve' : 'Unavailable'}</button>
        </article>
    `).join("");
}

async function refreshBorrowedBooks(userId) {
    const result = await apiFetch("/api/books?status=borrowed");
    const container = document.getElementById("borrowedBooks");
    if (!Array.isArray(result)) {
        container.textContent = "Unable to load borrowed books.";
        return;
    }
    const userBooks = result.filter((book) => book.borrowerId === userId);
    if (!userBooks.length) {
        container.innerHTML = `<article class="card"><p>You have no borrowed books right now.</p></article>`;
        return;
    }
    container.innerHTML = userBooks.map((book) => `
        <article class="card">
            <h3>${book.title}</h3>
            <p><strong>Due:</strong> ${book.dueDate}</p>
            <p><span class="badge yellow">Your Item</span></p>
            <button onclick="returnBook(${book.id})">Return Book</button>
        </article>
    `).join("");
}

async function refreshNotifications(userId) {
    const notes = await apiFetch(`/api/user/notifications?userId=${userId}`);
    const container = document.getElementById("notifications");
    if (!Array.isArray(notes)) {
        container.textContent = "Unable to load notifications.";
        return;
    }
    if (!notes.length) {
        container.innerHTML = `<article class="notification-card"><p>No alerts at the moment.</p></article>`;
        return;
    }
    container.innerHTML = notes.slice().reverse().map((note) => `
        <article class="notification-card">
            <h3>${note.message}</h3>
            <p>${note.timestamp}</p>
        </article>
    `).join("");
}

async function borrowBook(bookId) {
    const user = getUserState();
    if (!user) return;
    const response = await apiFetch("/api/borrow", "POST", { userId: user.userId, bookId });
    if (response.success) {
        await refreshSearchResults();
        await refreshBorrowedBooks(user.userId);
        await refreshUserSummary(user.userId);
        await refreshNotifications(user.userId);
        alert(response.message);
    } else {
        alert(response.message || "Unable to reserve book.");
    }
}

async function returnBook(bookId) {
    const user = getUserState();
    if (!user) return;
    const response = await apiFetch("/api/return", "POST", { userId: user.userId, bookId });
    if (response.success) {
        await refreshSearchResults();
        await refreshBorrowedBooks(user.userId);
        await refreshUserSummary(user.userId);
        await refreshNotifications(user.userId);
        alert(response.message);
    } else {
        alert(response.message || "Unable to return book.");
    }
}

function initializePage() {
    const page = document.body.dataset.page;
    if (page === "login") handleLogin();
    if (page === "register") handleRegister();
    if (page === "admin") loadAdminDashboard();
    if (page === "user") loadUserPage();
    if (page === "browse") loadBrowsePage();
}

window.addEventListener("DOMContentLoaded", initializePage);
window.borrowBook = borrowBook;
window.returnBook = returnBook;
