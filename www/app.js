const BOOK_CATALOG = [
    {
        id: 1,
        title: "The Art of Algorithms",
        author: "Mira Hayden",
        category: "Computer",
        status: "available",
        description: "A beginner-friendly guide to algorithms and data structures for campus library users."
    },
    {
        id: 2,
        title: "Campus History: Then and Now",
        author: "Leo Marshall",
        category: "History",
        status: "borrowed",
        description: "A photo-rich tour of campus history, buildings, and student life through the years."
    },
    {
        id: 3,
        title: "Modern Chemistry Lab",
        author: "Anita Costa",
        category: "Science",
        status: "available",
        description: "A practical lab manual for science students, with experiments and safety tips."
    },
    {
        id: 4,
        title: "Business Strategy Fundamentals",
        author: "Ibrahim Khan",
        category: "T.L.E.",
        status: "available",
        description: "Use this book to learn campus entrepreneurship, teamwork, and project planning."
    },
    {
        id: 5,
        title: "Graphic Design Essentials",
        author: "Nia Chen",
        category: "Arts",
        status: "borrowed",
        description: "A creative guide for students working on design, posters, and digital projects."
    },
    {
        id: 6,
        title: "Calculus in Context",
        author: "Samuel Park",
        category: "Mathematics",
        status: "available",
        description: "A clear explanation of calculus concepts with real campus examples and practice problems."
    },
    {
        id: 7,
        title: "English Literature Classics",
        author: "Ava Turner",
        category: "Literature",
        status: "available",
        description: "A curated anthology of modern and classic English literature for students."
    },
    {
        id: 8,
        title: "Physics for Everyone",
        author: "Diego Morales",
        category: "Science",
        status: "available",
        description: "An accessible introduction to physics with campus examples and simple experiments."
    },
    {
        id: 9,
        title: "World History Overview",
        author: "Priya Singh",
        category: "History",
        status: "borrowed",
        description: "A broad review of world history events, leaders, and cultural milestones."
    },
    {
        id: 10,
        title: "Statistics Made Simple",
        author: "Ethan Cole",
        category: "Mathematics",
        status: "available",
        description: "A user-friendly statistics textbook with practical examples and exercises."
    },
    {
        id: 11,
        title: "Algebra Essentials",
        author: "Nina Reyes",
        category: "Mathematics",
        status: "available",
        description: "A clear introduction to algebra concepts for campus and classroom mathematics."
    },
    {
        id: 12,
        title: "Geometry for Students",
        author: "Marlon Tan",
        category: "Mathematics",
        status: "borrowed",
        description: "A visual guide to geometry with practice problems and easy explanations."
    },
    {
        id: 13,
        title: "Trigonometry in Practice",
        author: "Sara Lim",
        category: "Mathematics",
        status: "available",
        description: "A practical trigonometry workbook for high school and college learners."
    },
    {
        id: 14,
        title: "English Grammar Today",
        author: "Keith Andrews",
        category: "English",
        status: "available",
        description: "An up-to-date grammar guide with examples and exercises for English learners."
    },
    {
        id: 15,
        title: "Academic Writing Skills",
        author: "Liza Cortez",
        category: "English",
        status: "borrowed",
        description: "A practical book on essay writing, reports, and academic communication."
    },
    {
        id: 16,
        title: "Reading Comprehension Strategies",
        author: "Jacob Rivera",
        category: "English",
        status: "available",
        description: "A student-friendly guide to improving reading understanding and study skills."
    },
    {
        id: 17,
        title: "Foundations of Physics",
        author: "Mia Gonzalez",
        category: "Science",
        status: "available",
        description: "A foundation book covering key concepts in physics for science students."
    },
    {
        id: 18,
        title: "Biology: Life Systems",
        author: "Hannah Cruz",
        category: "Science",
        status: "borrowed",
        description: "A biology textbook focused on ecosystems, cells, and living systems."
    },
    {
        id: 19,
        title: "Earth Science Explorer",
        author: "Derek Santos",
        category: "Science",
        status: "available",
        description: "A hands-on earth science guide for students learning geology and weather."
    },
    {
        id: 20,
        title: "Philippine History Stories",
        author: "Anna dela Cruz",
        category: "History",
        status: "available",
        description: "A narrative history of the Philippines from pre-colonial times to modern days."
    },
    {
        id: 21,
        title: "World War II and Beyond",
        author: "Oliver Santos",
        category: "History",
        status: "borrowed",
        description: "An overview of 20th century conflicts and their impact on world history."
    },
    {
        id: 22,
        title: "Historical Inquiry",
        author: "Eleanor Reyes",
        category: "History",
        status: "available",
        description: "A critical thinking book for analyzing sources, events, and historical evidence."
    },
    {
        id: 23,
        title: "Music Theory Basics",
        author: "Noah Silva",
        category: "Music",
        status: "available",
        description: "An introductory music theory book for students learning notation and harmony."
    },
    {
        id: 24,
        title: "The Sound of Instruments",
        author: "Maya Tan",
        category: "Music",
        status: "borrowed",
        description: "A guide to musical instruments, styles, and performance fundamentals."
    },
    {
        id: 25,
        title: "Listening to Music",
        author: "Rafael Cruz",
        category: "Music",
        status: "available",
        description: "A student-friendly introduction to music appreciation and analysis."
    },
    {
        id: 26,
        title: "Visual Arts Workshop",
        author: "Gina Palacios",
        category: "Arts",
        status: "available",
        description: "A practical arts manual for drawing, painting, and creative projects."
    },
    {
        id: 27,
        title: "Design, Color, and Form",
        author: "Paulo Reyes",
        category: "Arts",
        status: "borrowed",
        description: "An arts guide focused on design principles, color, and visual composition."
    },
    {
        id: 28,
        title: "Creative Arts Expressions",
        author: "Aria Luna",
        category: "Arts",
        status: "available",
        description: "A book for students exploring modern art, crafts, and personal expression."
    },
    {
        id: 29,
        title: "P.E. Fitness and Sport",
        author: "Jordan Castillo",
        category: "P.E.",
        status: "available",
        description: "A physical education guide covering fitness, sports, and active living."
    },
    {
        id: 30,
        title: "Team Sports Fundamentals",
        author: "Kim Santos",
        category: "P.E.",
        status: "borrowed",
        description: "An introductory textbook for team sports rules, strategy, and training."
    },
    {
        id: 31,
        title: "Active Lifestyle Guide",
        author: "Ella Morales",
        category: "P.E.",
        status: "available",
        description: "A student-focused guide on exercise routines and staying physically active."
    },
    {
        id: 32,
        title: "Health and Wellness",
        author: "Sophia Lim",
        category: "Health",
        status: "available",
        description: "A health textbook covering nutrition, mental wellness, and healthy habits."
    },
    {
        id: 33,
        title: "Teen Health Choices",
        author: "Mia Rodriguez",
        category: "Health",
        status: "borrowed",
        description: "A young adult health guide on puberty, safety, and emotional wellbeing."
    },
    {
        id: 34,
        title: "Personal Health Essentials",
        author: "Carlos Fernandez",
        category: "Health",
        status: "available",
        description: "A basic health handbook for students learning everyday wellness and care."
    },
    {
        id: 35,
        title: "Philippine Literature Today",
        author: "Reina Sampaguita",
        category: "Literature",
        status: "available",
        description: "A collection of modern Filipino literary works and critical essays."
    },
    {
        id: 36,
        title: "Global Short Stories",
        author: "Marcus Liu",
        category: "Literature",
        status: "borrowed",
        description: "A diverse anthology of short stories from contemporary world writers."
    },
    {
        id: 37,
        title: "Modern Poetry Selections",
        author: "Alina Santos",
        category: "Literature",
        status: "available",
        description: "A poetry reader highlighting contemporary voices and themes."
    },
    {
        id: 38,
        title: "T.L.E. Home Skills",
        author: "Rosanna Vega",
        category: "T.L.E.",
        status: "available",
        description: "A practical manual for home economics skills, cooking, and life management."
    },
    {
        id: 39,
        title: "Entrepreneurship Basics",
        author: "Mark Alvarez",
        category: "T.L.E.",
        status: "borrowed",
        description: "A beginner's guide to small business planning, budgeting, and project design."
    },
    {
        id: 40,
        title: "Technical Drafting Essentials",
        author: "Yvonne Lim",
        category: "T.L.E.",
        status: "available",
        description: "A student-focused introduction to drafting, tools, and technical drawing."
    },
    {
        id: 41,
        title: "Computer Basics",
        author: "Ethan Reed",
        category: "Computer",
        status: "available",
        description: "A basic computer textbook covering hardware, software, and digital literacy."
    },
    {
        id: 42,
        title: "Programming Fundamentals",
        author: "Lina Torres",
        category: "Computer",
        status: "borrowed",
        description: "An introductory programming guide for beginners learning code logic and syntax."
    },
    {
        id: 43,
        title: "Web Design Principles",
        author: "Noel Reyes",
        category: "Computer",
        status: "available",
        description: "A student guide to building websites, HTML, CSS, and user-friendly layouts."
    },
    {
        id: 44,
        title: "Values and Ethics",
        author: "Maria Santos",
        category: "ESP/Values",
        status: "available",
        description: "A guide to values formation, ethical behavior, and personal responsibility."
    },
    {
        id: 45,
        title: "Empathy and Respect",
        author: "Jonah Alvarez",
        category: "ESP/Values",
        status: "borrowed",
        description: "A student workbook on empathy, respect, and positive relationships."
    },
    {
        id: 46,
        title: "Character Building",
        author: "Celeste Cruz",
        category: "ESP/Values",
        status: "available",
        description: "A practical guide to building good values and strong character habits."
    },
    {
        id: 47,
        title: "Filipino Language and Culture",
        author: "Leah Navarro",
        category: "Filipino",
        status: "available",
        description: "A textbook on Filipino language, culture, and identity for students."
    },
    {
        id: 48,
        title: "Mga Kuwento ng Bayan",
        author: "Ricardo Villanueva",
        category: "Filipino",
        status: "borrowed",
        description: "A collection of Filipino short stories and folk tales."
    },
    {
        id: 49,
        title: "Pagbasa at Pagsulat Tungo sa Pananaliksik",
        author: "Angela Castro",
        category: "Filipino",
        status: "available",
        description: "A guide to Filipino reading, writing, and research skills for students."
    }
];

const STORAGE_KEY = "campusLibraryBooks";
const ALLOWED_CATEGORIES = [
    "Mathematics",
    "English",
    "Science",
    "History",
    "Music",
    "Arts",
    "P.E.",
    "Health",
    "Literature",
    "T.L.E.",
    "Computer",
    "ESP/Values",
    "Filipino"
];

function loadStoredBooks() {
    try {
        const saved = localStorage.getItem(STORAGE_KEY);
        return saved ? JSON.parse(saved) : null;
    } catch (error) {
        return null;
    }
}

function saveStoredBooks(books) {
    try {
        localStorage.setItem(STORAGE_KEY, JSON.stringify(books));
    } catch (error) {
        console.warn("Unable to save library books.", error);
    }
}

const libraryBooks = loadStoredBooks() || BOOK_CATALOG.slice();

const ADMIN_NAME = "Campus Librarian";

const ADMIN_STATE = {
    books: libraryBooks,
    audit: [
        {
            id: 1,
            timestamp: new Date().toLocaleString(),
            action: "Admin page loaded"
        },
        {
            id: 2,
            timestamp: new Date().toLocaleString(),
            action: "Reviewed current inventory"
        }
    ]
};

const selectedBookIds = new Set();
let ADMIN_ACCOUNTS = [];
let adminCategoryFilter = "";
let currentAdminAction = null;

function populateCategoryFilter() {
    const filter = document.getElementById("admin-category-filter");
    if (!filter) return;

    filter.innerHTML = `
        <option value="">All categories</option>
        ${ALLOWED_CATEGORIES.map((category) => `<option value="${category}">${category}</option>`).join("")}
    `;
    filter.value = adminCategoryFilter;
}

function handleCategoryFilterChange(event) {
    adminCategoryFilter = event.target.value;
    renderBookInventory();
}

function isUserBrowsePage() {
    return document.body.dataset.page === "browse-user";
}

function renderBookCard(book) {
    const isReserved = Boolean(book.reservedBy);
    const effectiveStatus = isReserved ? "borrowed" : book.status;
    const badgeClass = effectiveStatus === "available" ? "green" : "red";
    const badgeLabel = effectiveStatus === "available" ? "Available" : isReserved ? "Reserved" : "Borrowed";
    const statusClass = effectiveStatus === "borrowed" ? "borrowed-book" : "";

    let actionButton = "";
    if (isUserBrowsePage() && effectiveStatus === "available") {
        actionButton = `<button class="button reserve-button" type="button" data-book-id="${book.id}">Reserve</button>`;
    }

    const reservationInfo = isReserved
        ? `<p class="book-note"><strong>Reserved by:</strong> ${book.reservedBy} <br /><strong>Until:</strong> ${book.reservedUntil}</p>`
        : "";

    return `
        <article class="card book-card ${statusClass}">
            <h3>${book.title}</h3>
            <p><strong>Author:</strong> ${book.author}</p>
            <p><strong>Category:</strong> ${book.category}</p>
            <p>${book.description}</p>
            ${reservationInfo}
            <div class="book-actions">
                <div class="badge ${badgeClass}">${badgeLabel}</div>
                ${actionButton}
            </div>
        </article>
    `;
}

function applyReservationData(reservations) {
    const reservationMap = new Map(reservations.map((reservation) => [String(reservation.bookId), reservation]));

    libraryBooks.forEach((book) => {
        delete book.reservedBy;
        delete book.reservedUntil;
        delete book.reservedExpiresAt;

        const reservation = reservationMap.get(String(book.id));
        if (reservation) {
            book.reservedBy = reservation.username;
            book.reservedUntil = reservation.reservedUntil;
            book.reservedExpiresAt = reservation.expiresAt;
        }
    });
}

function fetchReservationData() {
    return fetch("/reservations")
        .then((response) => {
            if (!response.ok) {
                throw new Error("Failed to load reservation data.");
            }
            return response.json();
        })
        .catch((error) => {
            console.warn(error);
            return [];
        });
}

function refreshReservationState() {
    return fetchReservationData().then((reservations) => {
        applyReservationData(reservations);
        if (isUserBrowsePage()) {
            renderBookCatalog(libraryBooks);
        }
        if (document.body.dataset.page === "admin") {
            renderBookInventory();
        }
    });
}

function renderBookCatalog(books) {
    const grid = document.getElementById("book-grid");
    if (!grid) {
        return;
    }

    if (books.length === 0) {
        grid.innerHTML = `
            <article class="card">
                <h3>No results found</h3>
                <p>Try a different keyword or status filter to find more books.</p>
            </article>
        `;
        return;
    }

    grid.innerHTML = books.map(renderBookCard).join("");
}

function filterBooks(query, status) {
    const normalizedQuery = query.trim().toLowerCase();

    return libraryBooks.filter((book) => {
        const effectiveStatus = book.reservedBy ? "borrowed" : book.status;
        const matchesStatus = status ? effectiveStatus === status : true;
        const matchesQuery = normalizedQuery
            ? [book.title, book.author, book.category]
                  .join(" ")
                  .toLowerCase()
                  .includes(normalizedQuery)
            : true;

        return matchesStatus && matchesQuery;
    });
}

function handleSearch(event) {
    event.preventDefault();

    const queryInput = document.getElementById("search-input");
    const statusInput = document.getElementById("status-filter");

    const query = queryInput ? queryInput.value : "";
    const status = statusInput ? statusInput.value : "";

    const filteredBooks = filterBooks(query, status);
    renderBookCatalog(filteredBooks);
}

function handleReserveButtonClick(button) {
    const bookId = button.dataset.bookId;
    if (!bookId) {
        alert("Unable to read book ID for reservation.");
        return;
    }

    fetch("/reserve", {
        method: "POST",
        headers: {
            "Content-Type": "application/x-www-form-urlencoded"
        },
        body: `book=${encodeURIComponent(bookId)}`
    })
        .then((response) => {
            if (response.ok) {
                return response.text();
            }
            throw new Error("Reservation request failed.");
        })
        .then(() => {
            alert("Reservation successful. Please collect the book within 5 hours.");
            refreshReservationState();
        })
        .catch((error) => {
            console.error(error);
            alert("Unable to reserve the book. Please try again.");
        });
}

function handleBrowseCardClick(event) {
    const reserveBtn = event.target.closest(".reserve-button");
    if (reserveBtn) {
        handleReserveButtonClick(reserveBtn);
        return;
    }

    const card = event.target.closest(".book-card");
    if (!card) return;

    const grid = document.getElementById("book-grid");
    if (!grid) return;

    const isActive = card.classList.contains("active");
    grid.querySelectorAll(".book-card.active").forEach((item) => item.classList.remove("active"));

    if (!isActive) {
        card.classList.add("active");
    }
}

function initBrowsePage() {
    const form = document.getElementById("browse-search-form");
    if (form) {
        form.addEventListener("submit", handleSearch);
    }

    const grid = document.getElementById("book-grid");
    if (grid) {
        grid.addEventListener("click", handleBrowseCardClick);
    }

    refreshReservationState();
}

function getSummaryCards() {
    const totalBooks = ADMIN_STATE.books.length;
    const availableBooks = ADMIN_STATE.books.filter((book) => book.status === "available" && !book.reservedBy).length;
    const borrowedBooks = ADMIN_STATE.books.filter((book) => book.status === "borrowed" || book.reservedBy).length;
    const recentActions = ADMIN_STATE.audit.length;

    return [
        { label: "Total Books", value: totalBooks, badge: "green" },
        { label: "Available", value: availableBooks, badge: "green" },
        { label: "Borrowed", value: borrowedBooks, badge: "red" },
        { label: "Audit Entries", value: recentActions, badge: "yellow" }
    ];
}

function renderSummaryCards() {
    const container = document.getElementById("summary-cards");
    if (!container) return;

    container.innerHTML = getSummaryCards()
        .map(
            (card) => `
                <article class="card">
                    <h3>${card.label}</h3>
                    <div class="badge ${card.badge}">${card.value}</div>
                </article>
            `
        )
        .join("");
}

function getSelectedBookIdsFromDom() {
    return Array.from(document.querySelectorAll(".inventory-checkbox-input:checked"), (input) => Number(input.dataset.bookId));
}

function renderBookInventory() {
    const container = document.getElementById("book-inventory");
    if (!container) return;

    const visibleBooks = adminCategoryFilter
        ? ADMIN_STATE.books.filter((book) => book.category.toLowerCase() === adminCategoryFilter.toLowerCase())
        : ADMIN_STATE.books;

    if (visibleBooks.length === 0) {
        container.innerHTML = `
            <article class="card">
                <h3>No books in inventory</h3>
                <p>Add a new book using the form above.</p>
            </article>
        `;
        return;
    }

    container.innerHTML = visibleBooks
        .map((book) => {
            const isChecked = selectedBookIds.has(book.id) ? "checked" : "";
            const effectiveStatus = book.reservedBy ? "borrowed" : book.status;
            const statusClass = effectiveStatus === "borrowed" ? "borrowed-book" : "available-book";
            const selectedClass = selectedBookIds.has(book.id) ? "selected-book" : "";
            const reservationInfo = book.reservedBy
                ? `<p class="book-note"><strong>Reserved by:</strong> ${book.reservedBy}<br /><strong>Until:</strong> ${book.reservedUntil}</p>`
                : "";

            return `
                <article class="card book-card ${statusClass} ${selectedClass}">
                    <label class="inventory-checkbox">
                        <input type="checkbox" class="inventory-checkbox-input" data-book-id="${book.id}" ${isChecked} />
                        Select
                    </label>
                    <h3>${book.title}</h3>
                    <p><strong>Author:</strong> ${book.author}</p>
                    <p><strong>Category:</strong> ${book.category}</p>
                    ${reservationInfo}
                    <div class="badge ${effectiveStatus === "available" ? "green" : "red"}">${effectiveStatus === "available" ? "Available" : "Borrowed"}</div>
                </article>
            `;
        })
        .join("");

    updateSelectAllButton();
}

function formatAdminDetailCard(detail) {
    return `
        <article class="card">
            <h3>${detail.bookTitle}</h3>
            <p><strong>Username:</strong> ${detail.username}</p>
            <p><strong>Name:</strong> ${detail.fullName}</p>
            <p><strong>Email:</strong> ${detail.email}</p>
            <p><strong>Reserved until:</strong> ${detail.reservedUntil}</p>
            <p><strong>Return by:</strong> ${detail.returnBy}</p>
            <p><strong>Status:</strong> ${detail.status}</p>
        </article>
    `;
}

function setAdminDetailModalVisible(visible) {
    const modal = document.getElementById("admin-detail-modal");
    if (!modal) return;
    modal.classList.toggle("hidden", !visible);
}

function showAdminDetailModal(details) {
    const body = document.getElementById("admin-detail-body");
    if (!body) return;
    body.innerHTML = details.length
        ? details.map(formatAdminDetailCard).join("")
        : `<article class="card"><h3>No borrower details found.</h3><p>Selected book(s) have no active reservation record.</p></article>`;
    setAdminDetailModalVisible(true);
}

function hideAdminDetailModal() {
    setAdminDetailModalVisible(false);
}

function fetchSelectedBorrowerDetails() {
    const selectedIds = getSelectedBookIdsFromDom();
    if (selectedIds.length === 0) {
        showAdminMessage("Select at least one book to view borrower details.", "red");
        return Promise.reject("no selection");
    }

    const bookIdsParam = selectedIds.join(",");
    return fetch(`/admin/book-details?bookIds=${encodeURIComponent(bookIdsParam)}`)
        .then((response) => {
            if (!response.ok) {
                throw new Error("Unable to load borrower details.");
            }
            return response.json();
        })
        .catch(() => {
            const fallbackDetails = selectedIds.map((bookId) => {
                const book = ADMIN_STATE.books.find((item) => item.id === bookId);
                return {
                    bookTitle: book ? book.title : `Book ${bookId}`,
                    username: "Unknown",
                    fullName: "Unknown borrower",
                    email: "N/A",
                    reservedUntil: "N/A",
                    returnBy: "N/A",
                    status: book ? book.status : "unknown"
                };
            });
            return Promise.resolve(fallbackDetails);
        });
}

function openAdminBorrowerDetails() {
    currentAdminAction = "available";
    const heading = document.getElementById("admin-detail-heading");
    const confirmButton = document.getElementById("admin-detail-confirm");
    if (heading) {
        heading.textContent = "Borrower Details";
    }
    if (confirmButton) {
        confirmButton.textContent = "Mark Available";
    }

    fetchSelectedBorrowerDetails()
        .then((details) => {
            showAdminDetailModal(details);
        })
        .catch((error) => {
            if (error !== "no selection") {
                console.error(error);
                showAdminMessage("Unable to load borrower details.", "red");
            }
        });
}

function getReturnDateBounds() {
    const today = new Date();
    const maxDay = new Date(today);
    maxDay.setDate(maxDay.getDate() + 7);

    const formatDate = (date) => date.toISOString().slice(0, 10);
    return {
        min: formatDate(today),
        max: formatDate(maxDay)
    };
}

function openAdminBorrowedPrompt() {
    const selectedIds = getSelectedBookIdsFromDom();
    if (selectedIds.length === 0) {
        showAdminMessage("Select at least one book to borrow.", "red");
        return;
    }

    currentAdminAction = "borrowed";
    const heading = document.getElementById("admin-detail-heading");
    const confirmButton = document.getElementById("admin-detail-confirm");
    const body = document.getElementById("admin-detail-body");
    if (heading) {
        heading.textContent = "Borrow Book(s)";
    }
    if (confirmButton) {
        confirmButton.textContent = "Mark Borrowed";
    }
    if (body) {
        const selectedBooks = selectedIds
            .map((bookId) => {
                const book = ADMIN_STATE.books.find((item) => item.id === bookId);
                return book ? `<li>${book.title}</li>` : `<li>Book ${bookId}</li>`;
            })
            .join("");
        const bounds = getReturnDateBounds();
        body.innerHTML = `
            <article class="card">
                <h3>Borrower Information</h3>
                <div class="modal-form-grid">
                    <label>
                        Borrower account
                        <select id="admin-detail-account"></select>
                    </label>
                    <label>
                        Username
                        <input id="admin-detail-username" type="text" placeholder="Username" />
                    </label>
                    <label>
                        Full Name
                        <input id="admin-detail-fullname" type="text" placeholder="Full Name" />
                    </label>
                    <label>
                        Email
                        <input id="admin-detail-email" type="email" placeholder="Email address" />
                    </label>
                    <label>
                        Year
                        <input id="admin-detail-year" type="text" placeholder="Year" />
                    </label>
                    <label>
                        Section
                        <input id="admin-detail-section" type="text" placeholder="Section" />
                    </label>
                    <label>
                        Return Date
                        <input id="admin-detail-return-date" type="date" min="${bounds.min}" max="${bounds.max}" value="${bounds.min}" />
                    </label>
                </div>
                <h4>Selected books</h4>
                <ul>${selectedBooks}</ul>
                <p class="modal-help">Return date must be within 7 days from today (${bounds.min} to ${bounds.max}).</p>
            </article>
        `;

        fetchRegisteredAccounts()
            .then(() => {
                populateBorrowerAccountSelect();
                const accountSelect = document.getElementById("admin-detail-account");
                if (accountSelect) {
                    accountSelect.addEventListener("change", (event) => {
                        fillBorrowerFieldsFromAccount(event.target.value);
                    });
                }
            })
            .catch((error) => {
                console.error(error);
                showAdminMessage("Unable to load registered borrower accounts.", "red");
            });
    }
    setAdminDetailModalVisible(true);
}

function verifyRegisteredUser(username) {
    return fetch(`/account-exists?username=${encodeURIComponent(username)}`)
        .then((response) => {
            if (!response.ok) {
                throw new Error("Unable to verify username.");
            }
            return response.json();
        })
        .then((data) => data.exists === true);
}

function fetchRegisteredAccounts() {
    return fetch("/accounts")
        .then((response) => {
            if (!response.ok) {
                throw new Error("Unable to load registered accounts.");
            }
            return response.json();
        })
        .then((data) => {
            if (!Array.isArray(data)) {
                throw new Error("Invalid account data received.");
            }
            ADMIN_ACCOUNTS = data;
            return ADMIN_ACCOUNTS;
        });
}

function populateBorrowerAccountSelect() {
    const accountSelect = document.getElementById("admin-detail-account");
    if (!accountSelect) return;
    accountSelect.innerHTML = `
        <option value="">Select borrower account</option>
        ${ADMIN_ACCOUNTS.map((account) => `<option value="${account.username}">${account.username} - ${account.fullName || account.username}</option>`).join("")}
    `;
}

function fillBorrowerFieldsFromAccount(username) {
    const account = ADMIN_ACCOUNTS.find((item) => item.username === username);
    if (!account) return;
    const usernameInput = document.getElementById("admin-detail-username");
    const fullNameInput = document.getElementById("admin-detail-fullname");
    const emailInput = document.getElementById("admin-detail-email");
    const yearInput = document.getElementById("admin-detail-year");
    const sectionInput = document.getElementById("admin-detail-section");

    if (usernameInput) usernameInput.value = account.username;
    if (fullNameInput) fullNameInput.value = account.fullName || "";
    if (emailInput) emailInput.value = account.email || "";
    if (yearInput) yearInput.value = account.year || "";
    if (sectionInput) sectionInput.value = account.section || "";
}

function updateSelectedBooksStatus(newStatus) {
    const selectedIds = getSelectedBookIdsFromDom();
    if (selectedIds.length === 0) {
        showAdminMessage("Select at least one book to update.", "red");
        return;
    }

    const updatedTitles = [];

    ADMIN_STATE.books = ADMIN_STATE.books.map((book) => {
        if (selectedIds.includes(book.id) && book.status !== newStatus) {
            updatedTitles.push(book.title);
            return { ...book, status: newStatus };
        }
        return book;
    });

    if (updatedTitles.length === 0) {
        showAdminMessage("Selected books are already set to this status.", "yellow");
        return;
    }

    saveStoredBooks(ADMIN_STATE.books);

    ADMIN_STATE.audit.unshift({
        id: ADMIN_STATE.audit.length + 1,
        timestamp: new Date().toLocaleString(),
        action: `Updated status for ${updatedTitles.length} selected book(s) to ${newStatus}`
    });

    renderSummaryCards();
    renderBookInventory();
    renderAuditLog();
    showAdminMessage(`Updated ${updatedTitles.length} book(s).`, "green");
}

function updateSelectAllButton() {
    const selectAllButton = document.getElementById("select-all-books");
    if (!selectAllButton) return;

    const checkboxes = document.querySelectorAll(".inventory-checkbox-input");
    const checkedBoxes = document.querySelectorAll(".inventory-checkbox-input:checked");

    if (checkboxes.length > 0 && checkboxes.length === checkedBoxes.length) {
        selectAllButton.textContent = "Unselect All";
    } else {
        selectAllButton.textContent = "Select All";
    }
}

function selectAllInventoryBooks() {
    const selectAllButton = document.getElementById("select-all-books");
    const checkboxes = document.querySelectorAll(".inventory-checkbox-input");
    const allSelected = checkboxes.length > 0 && Array.from(checkboxes).every((checkbox) => checkbox.checked);

    if (allSelected) {
        checkboxes.forEach((checkbox) => {
            checkbox.checked = false;
            selectedBookIds.delete(Number(checkbox.dataset.bookId));
        });
    } else {
        checkboxes.forEach((checkbox) => {
            checkbox.checked = true;
            selectedBookIds.add(Number(checkbox.dataset.bookId));
        });
    }

    if (selectAllButton) {
        selectAllButton.textContent = allSelected ? "Select All" : "Unselect All";
    }

    renderBookInventory();
    updateSelectAllButton();
}

function handleInventorySelection(event) {
    const checkbox = event.target.closest(".inventory-checkbox-input");
    if (!checkbox) return;

    const bookId = Number(checkbox.dataset.bookId);
    if (checkbox.checked) {
        selectedBookIds.add(bookId);
    } else {
        selectedBookIds.delete(bookId);
    }

    updateSelectAllButton();
}

function renderAuditLog() {
    const container = document.getElementById("audit-log");
    if (!container) return;

    container.innerHTML = ADMIN_STATE.audit
        .map(
            (entry) => `
                <article class="audit-card">
                    <h3>${entry.action}</h3>
                    <p>${entry.timestamp}</p>
                </article>
            `
        )
        .join("");
}

function updateBookStatus(bookId, newStatus) {
    const bookIndex = ADMIN_STATE.books.findIndex((book) => book.id === bookId);
    if (bookIndex === -1) return;

    const book = ADMIN_STATE.books[bookIndex];
    if (book.status === newStatus) return;

    book.status = newStatus;
    saveStoredBooks(ADMIN_STATE.books);

    ADMIN_STATE.audit.unshift({
        id: ADMIN_STATE.audit.length + 1,
        timestamp: new Date().toLocaleString(),
        action: `Updated status for ${book.title} to ${newStatus}`
    });

    renderSummaryCards();
    renderBookInventory();
    renderAuditLog();
    showAdminMessage(`Status updated for ${book.title}.`, "green");
}

function handleInventoryClick(event) {
    const button = event.target.closest(".status-toggle-button");
    if (button) {
        const bookId = Number(button.dataset.bookId);
        const newStatus = button.dataset.newStatus;
        updateBookStatus(bookId, newStatus);
        return;
    }

    const card = event.target.closest(".book-card");
    if (!card) return;
    if (event.target.closest(".inventory-checkbox")) return;

    const checkbox = card.querySelector(".inventory-checkbox-input");
    if (!checkbox) return;

    checkbox.checked = !checkbox.checked;
    const bookId = Number(checkbox.dataset.bookId);
    if (checkbox.checked) {
        selectedBookIds.add(bookId);
    } else {
        selectedBookIds.delete(bookId);
    }

    updateSelectAllButton();
    renderBookInventory();
}

function setAdminName() {
    const nameElement = document.getElementById("admin-name");
    if (nameElement) {
        nameElement.textContent = ADMIN_NAME;
    }
}

function showAdminMessage(message, type = "") {
    const alert = document.getElementById("admin-alert");
    if (!alert) return;
    alert.textContent = message;
    alert.className = type ? `alert ${type}` : "alert";
}

function addBook(event) {
    event.preventDefault();

    const titleInput = document.getElementById("book-title");
    const authorInput = document.getElementById("book-author");
    const categoryInput = document.getElementById("book-category");

    if (!titleInput || !authorInput || !categoryInput) return;

    const title = titleInput.value.trim();
    const author = authorInput.value.trim();
    const category = categoryInput.value.trim();

    if (!title || !author || !category) {
        showAdminMessage("Please complete all fields before adding the book.", "red");
        return;
    }

    const newBook = {
        id: Date.now(),
        title,
        author,
        category,
        status: "available",
        description: "Newly added book available in the catalog."
    };

    ADMIN_STATE.books.unshift(newBook);
    saveStoredBooks(ADMIN_STATE.books);

    ADMIN_STATE.audit.unshift({
        id: ADMIN_STATE.audit.length + 1,
        timestamp: new Date().toLocaleString(),
        action: `Added new book: ${title}`
    });

    titleInput.value = "";
    authorInput.value = "";
    categoryInput.value = "";

    showAdminMessage(`Book added: ${title}`, "green");
    renderSummaryCards();
    populateCategoryFilter();
    renderBookInventory();
    renderAuditLog();
}

function initializeAdminDashboard() {
    const form = document.getElementById("admin-add-book-form");
    if (form) {
        form.addEventListener("submit", addBook);
    }

    const inventoryContainer = document.getElementById("book-inventory");
    if (inventoryContainer) {
        inventoryContainer.addEventListener("click", handleInventoryClick);
        inventoryContainer.addEventListener("change", handleInventorySelection);
    }

    const selectAllButton = document.getElementById("select-all-books");
    const markAvailableButton = document.getElementById("mark-selected-available");
    const markBorrowedButton = document.getElementById("mark-selected-borrowed");

    if (selectAllButton) {
        selectAllButton.addEventListener("click", selectAllInventoryBooks);
    }
    if (markAvailableButton) {
        markAvailableButton.addEventListener("click", openAdminBorrowerDetails);
    }
    if (markBorrowedButton) {
        markBorrowedButton.addEventListener("click", openAdminBorrowedPrompt);
    }

    const modalCloseButton = document.getElementById("admin-detail-close");
    const modalCancelButton = document.getElementById("admin-detail-cancel");
    const modalConfirmButton = document.getElementById("admin-detail-confirm");
    if (modalCloseButton) {
        modalCloseButton.addEventListener("click", hideAdminDetailModal);
    }
    if (modalCancelButton) {
        modalCancelButton.addEventListener("click", hideAdminDetailModal);
    }
    if (modalConfirmButton) {
        modalConfirmButton.addEventListener("click", () => {
            if (currentAdminAction === "borrowed") {
                const usernameInput = document.getElementById("admin-detail-username");
                const fullNameInput = document.getElementById("admin-detail-fullname");
                const emailInput = document.getElementById("admin-detail-email");
                const yearInput = document.getElementById("admin-detail-year");
                const sectionInput = document.getElementById("admin-detail-section");
                const returnDateInput = document.getElementById("admin-detail-return-date");

                const username = usernameInput ? usernameInput.value.trim() : "";
                const fullName = fullNameInput ? fullNameInput.value.trim() : "";
                const email = emailInput ? emailInput.value.trim() : "";
                const year = yearInput ? yearInput.value.trim() : "";
                const section = sectionInput ? sectionInput.value.trim() : "";
                const returnDate = returnDateInput ? returnDateInput.value : "";

                if (!username || !fullName || !email || !year || !section || !returnDate) {
                    showAdminMessage("Complete all borrower fields before confirming.", "red");
                    return;
                }

                const bounds = getReturnDateBounds();
                if (returnDate < bounds.min || returnDate > bounds.max) {
                    showAdminMessage(`Return date must be between ${bounds.min} and ${bounds.max}.`, "red");
                    return;
                }

                verifyRegisteredUser(username)
                    .then((exists) => {
                        if (!exists) {
                            showAdminMessage("This username is not registered. Borrowers must have an account.", "red");
                            return;
                        }
                        updateSelectedBooksStatus("borrowed");
                        hideAdminDetailModal();
                        showAdminMessage(`Marked selected book(s) borrowed by ${username}.`, "green");
                    })
                    .catch((error) => {
                        console.error(error);
                        showAdminMessage("Unable to verify user registration. Try again.", "red");
                    });
            } else {
                updateSelectedBooksStatus("available");
                hideAdminDetailModal();
            }
        });
    }

    const categoryFilter = document.getElementById("admin-category-filter");
    if (categoryFilter) {
        categoryFilter.addEventListener("change", handleCategoryFilterChange);
    }

    setAdminName();
    populateCategoryFilter();
    refreshReservationState().then(() => {
        renderSummaryCards();
        renderBookInventory();
        renderAuditLog();
        showAdminMessage("Ready to manage inventory.", "green");
    });
}

function initializeApp() {
    const page = document.body.dataset.page;

    if (page === "browse") {
        initBrowsePage();
    }

    if (page === "browse-user") {
        initBrowsePage();
    }

    if (page === "admin") {
        initializeAdminDashboard();
    }
}

window.addEventListener("DOMContentLoaded", initializeApp);
