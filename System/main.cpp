#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <filesystem>
#include <thread>
#include <mutex>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <regex>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;
using namespace std::chrono;

const string STORAGE_FILE = "storage.txt";
const string NOTIFICATION_LOG = "notifications.log";
const string WWW_ROOT = "www";
mutex storeMutex;

string trim(const string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    auto end = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

vector<string> split(const string& s, char delim) {
    vector<string> parts;
    string current;
    for (char c : s) {
        if (c == delim) {
            parts.push_back(current);
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    parts.push_back(current);
    return parts;
}

string nowIso() {
    auto now = system_clock::now();
    time_t t = system_clock::to_time_t(now);
    tm local;
    localtime_s(&local, &t);
    ostringstream ss;
    ss << put_time(&local, "%Y-%m-%d");
    return ss.str();
}

string addDays(const string& date, int days) {
    tm tmDate{};
    istringstream ss(date);
    ss >> get_time(&tmDate, "%Y-%m-%d");
    if (ss.fail()) return date;
    time_t timeVal = mktime(&tmDate);
    if (timeVal == -1) return date;
    timeVal += days * 24 * 60 * 60;
    localtime_s(&tmDate, &timeVal);
    ostringstream out;
    out << put_time(&tmDate, "%Y-%m-%d");
    return out.str();
}

int compareDates(const string& a, const string& b) {
    if (a == b) return 0;
    return a < b ? -1 : 1;
}

bool isPastDate(const string& date) {
    string today = nowIso();
    return !date.empty() && compareDates(date, today) < 0;
}

struct Notification {
    int id;
    int userId;
    string message;
    string timestamp;
};

struct User {
    int id;
    string username;
    string password;
    string email;
    string role;
    int creditScore;
    int overdueCount;
    bool blocked;
};

struct Book {
    int id;
    string title;
    string author;
    string category;
    string status;
    int borrowerId;
    string dueDate;
    bool overduePenaltyApplied;
};

struct Reservation {
    int id;
    int userId;
    int bookId;
    string reservedAt;
    string status;
};

struct Audit {
    int id;
    string actor;
    string action;
    string timestamp;
    string details;
};

struct DataStore {
    vector<User> users;
    vector<Book> books;
    vector<Reservation> reservations;
    vector<Audit> audits;
    vector<Notification> notifications;
    int nextUserId = 1;
    int nextBookId = 1;
    int nextReservationId = 1;
    int nextAuditId = 1;
    int nextNotificationId = 1;

    void initDefault() {
        users.clear();
        books.clear();
        reservations.clear();
        audits.clear();
        notifications.clear();
        nextUserId = 1;
        nextBookId = 1;
        nextReservationId = 1;
        nextAuditId = 1;
        nextNotificationId = 1;

        users.push_back({nextUserId++, "admin", "admin123", "admin@inventory.local", "admin", 100, 0, false});
        users.push_back({nextUserId++, "student1", "pass123", "student1@school.local", "student", 95, 0, false});

        books.push_back({nextBookId++, "Effective C++", "Scott Meyers", "Programming", "available", 0, "", false});
        books.push_back({nextBookId++, "Design Patterns", "Erich Gamma", "Software", "available", 0, "", false});
        books.push_back({nextBookId++, "Algorithms Unlocked", "Thomas H. Cormen", "Math", "available", 0, "", false});

        audits.push_back({nextAuditId++, "system", "initialized default database", nowIso(), "created default admin and sample books"});
    }

    void load() {
        lock_guard<mutex> lock(storeMutex);
        if (!filesystem::exists(STORAGE_FILE)) {
            initDefault();
            save();
            return;
        }

        ifstream file(STORAGE_FILE);
        if (!file.is_open()) {
            initDefault();
            save();
            return;
        }

        string line;
        string section;
        while (getline(file, line)) {
            line = trim(line);
            if (line.empty()) continue;
            if (line.rfind("#", 0) == 0) {
                section = line;
                continue;
            }
            auto parts = split(line, '|');
            if (section == "#USERS") {
                if (parts.size() >= 8) {
                    users.push_back({
                        stoi(parts[0]), parts[1], parts[2], parts[3], parts[4],
                        stoi(parts[5]), stoi(parts[6]), parts[7] == "1"
                    });
                }
            } else if (section == "#BOOKS") {
                if (parts.size() >= 8) {
                    books.push_back({
                        stoi(parts[0]), parts[1], parts[2], parts[3], parts[4],
                        stoi(parts[5]), parts[6], parts[7] == "1"
                    });
                }
            } else if (section == "#RESERVATIONS") {
                if (parts.size() >= 5) {
                    reservations.push_back({
                        stoi(parts[0]), stoi(parts[1]), stoi(parts[2]), parts[3], parts[4]
                    });
                }
            } else if (section == "#AUDIT") {
                if (parts.size() >= 5) {
                    audits.push_back({
                        stoi(parts[0]), parts[1], parts[2], parts[3], parts[4]});
                }
            } else if (section == "#NOTIFICATIONS") {
                if (parts.size() >= 4) {
                    notifications.push_back({
                        stoi(parts[0]), stoi(parts[1]), parts[2], parts[3]});
                }
            }
        }

        nextUserId = 1;
        nextBookId = 1;
        nextReservationId = 1;
        nextAuditId = 1;
        nextNotificationId = 1;

        for (auto& u : users) nextUserId = max(nextUserId, u.id + 1);
        for (auto& b : books) nextBookId = max(nextBookId, b.id + 1);
        for (auto& r : reservations) nextReservationId = max(nextReservationId, r.id + 1);
        for (auto& a : audits) nextAuditId = max(nextAuditId, a.id + 1);
        for (auto& n : notifications) nextNotificationId = max(nextNotificationId, n.id + 1);

        if (users.empty() || books.empty()) {
            initDefault();
            save();
        }

        reconcileOverdues();
    }

    void save() {
        lock_guard<mutex> lock(storeMutex);
        ofstream file(STORAGE_FILE, ios::trunc);
        if (!file.is_open()) return;
        file << "#USERS\n";
        for (auto& u : users) {
            file << u.id << '|' << u.username << '|' << u.password << '|' << u.email << '|' << u.role << '|' << u.creditScore << '|' << u.overdueCount << '|' << (u.blocked ? "1" : "0") << '\n';
        }
        file << "#BOOKS\n";
        for (auto& b : books) {
            file << b.id << '|' << b.title << '|' << b.author << '|' << b.category << '|' << b.status << '|' << b.borrowerId << '|' << b.dueDate << '|' << (b.overduePenaltyApplied ? "1" : "0") << '\n';
        }
        file << "#RESERVATIONS\n";
        for (auto& r : reservations) {
            file << r.id << '|' << r.userId << '|' << r.bookId << '|' << r.reservedAt << '|' << r.status << '\n';
        }
        file << "#AUDIT\n";
        for (auto& a : audits) {
            file << a.id << '|' << a.actor << '|' << a.action << '|' << a.timestamp << '|' << a.details << '\n';
        }
        file << "#NOTIFICATIONS\n";
        for (auto& n : notifications) {
            file << n.id << '|' << n.userId << '|' << n.message << '|' << n.timestamp << '\n';
        }
    }

    void logAudit(const string& actor, const string& action, const string& details) {
        audits.push_back({nextAuditId++, actor, action, nowIso(), details});
        save();
    }

    void addNotification(int userId, const string& message) {
        string timestamp = nowIso();
        string entry = timestamp + " | user=" + to_string(userId) + " | " + message;
        ofstream log(NOTIFICATION_LOG, ios::app);
        if (log.is_open()) {
            log << entry << "\n";
        }
        notifications.push_back({nextNotificationId++, userId, message, timestamp});
        save();
    }

    void reconcileOverdues() {
        string today = nowIso();
        for (auto& book : books) {
            if (book.status == "borrowed" && !book.dueDate.empty() && compareDates(book.dueDate, today) < 0 && !book.overduePenaltyApplied) {
                auto user = findUserById(book.borrowerId);
                if (user) {
                    user->creditScore = max(0, user->creditScore - 10);
                    user->overdueCount++;
                    if (user->creditScore < 30 || user->overdueCount >= 3) user->blocked = true;
                    addNotification(user->id, "Overdue alert: book '" + book.title + "' is late. Credit deducted.");
                    logAudit(user->username, "overdue penalty", "Book " + book.title + " overdue and penalty applied.");
                }
                book.overduePenaltyApplied = true;
            }
        }
        save();
    }

    User* findUserByUsername(const string& username) {
        for (auto& u : users) if (u.username == username) return &u;
        return nullptr;
    }

    User* findUserById(int id) {
        for (auto& u : users) if (u.id == id) return &u;
        return nullptr;
    }

    Book* findBookById(int id) {
        for (auto& b : books) if (b.id == id) return &b;
        return nullptr;
    }

    vector<Book> searchBooks(const map<string, string>& filters) {
        vector<Book> result;
        for (auto& book : books) {
            bool keep = true;
            for (auto& [key, value] : filters) {
                if (value.empty()) continue;
                string lowerValue = value;
                transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(), ::tolower);
                if (key == "query") {
                    string haystack = book.title + " " + book.author + " " + book.category;
                    string lowerHay = haystack;
                    transform(lowerHay.begin(), lowerHay.end(), lowerHay.begin(), ::tolower);
                    if (lowerHay.find(lowerValue) == string::npos) keep = false;
                } else if (key == "category") {
                    string lowerField = book.category;
                    transform(lowerField.begin(), lowerField.end(), lowerField.begin(), ::tolower);
                    if (lowerField.find(lowerValue) == string::npos) keep = false;
                } else if (key == "status") {
                    string lowerField = book.status;
                    transform(lowerField.begin(), lowerField.end(), lowerField.begin(), ::tolower);
                    if (lowerField != lowerValue) keep = false;
                }
                if (!keep) break;
            }
            if (keep) result.push_back(book);
        }
        return result;
    }

    bool registerUser(const string& username, const string& password, const string& email, string& error) {
        if (username.empty() || password.empty() || email.empty()) {
            error = "All fields are required.";
            return false;
        }
        if (findUserByUsername(username) != nullptr) {
            error = "Username is already taken.";
            return false;
        }
        users.push_back({nextUserId++, username, password, email, "student", 100, 0, false});
        logAudit(username, "register", "New student account created.");
        save();
        return true;
    }

    User* validateLogin(const string& username, const string& password) {
        User* user = findUserByUsername(username);
        if (user && user->password == password) return user;
        return nullptr;
    }

    bool addBook(const string& title, const string& author, const string& category, int adminId, string& error) {
        if (title.empty() || author.empty() || category.empty()) {
            error = "Book title, author, and category are required.";
            return false;
        }
        books.push_back({nextBookId++, title, author, category, "available", 0, "", false});
        User* admin = findUserById(adminId);
        logAudit(admin ? admin->username : "unknown", "add book", title + " by " + author);
        save();
        return true;
    }

    bool borrowBook(int userId, int bookId, string& error) {
        User* user = findUserById(userId);
        Book* book = findBookById(bookId);
        if (!user || !book) {
            error = "Invalid user or book.";
            return false;
        }
        if (user->blocked) {
            error = "Your account is blocked. Contact admin.";
            return false;
        }
        if (user->creditScore < 50) {
            error = "Credit too low to borrow. Please return overdue items.";
            return false;
        }
        if (book->status != "available") {
            error = "Book is not available.";
            return false;
        }
        book->status = "borrowed";
        book->borrowerId = userId;
        book->dueDate = addDays(nowIso(), 7);
        book->overduePenaltyApplied = false;
        reservations.push_back({nextReservationId++, userId, bookId, nowIso(), "borrowed"});
        logAudit(user->username, "borrow", book->title);
        save();
        return true;
    }

    bool returnBook(int userId, int bookId, string& error) {
        User* user = findUserById(userId);
        Book* book = findBookById(bookId);
        if (!user || !book) {
            error = "Invalid user or book.";
            return false;
        }
        if (book->status != "borrowed" || book->borrowerId != userId) {
            error = "You do not have this book borrowed.";
            return false;
        }
        bool late = isPastDate(book->dueDate);
        if (late) {
            user->creditScore = max(0, user->creditScore - 10);
            user->overdueCount++;
            addNotification(userId, "Return penalty: book '" + book->title + "' returned late.");
        }
        if (user->creditScore < 30 || user->overdueCount >= 3) user->blocked = true;
        book->status = "available";
        book->borrowerId = 0;
        book->dueDate.clear();
        book->overduePenaltyApplied = false;
        logAudit(user->username, "return", book->title + (late ? " (late)" : ""));
        save();
        return true;
    }

    vector<Notification> userNotifications(int userId) {
        vector<Notification> result;
        for (auto& n : notifications) if (n.userId == userId) result.push_back(n);
        return result;
    }
};

DataStore store;

string urlDecode(const string& s) {
    string result;
    result.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '+') {
            result.push_back(' ');
        } else if (s[i] == '%' && i + 2 < s.size()) {
            string hex = s.substr(i + 1, 2);
            char c = (char)strtol(hex.c_str(), nullptr, 16);
            result.push_back(c);
            i += 2;
        } else {
            result.push_back(s[i]);
        }
    }
    return result;
}

map<string, string> parseQueryString(const string& query) {
    map<string, string> values;
    auto pairs = split(query, '&');
    for (auto& pair : pairs) {
        auto kv = split(pair, '=');
        if (!kv[0].empty()) values[urlDecode(kv[0])] = kv.size() > 1 ? urlDecode(kv[1]) : "";
    }
    return values;
}

map<string, string> parseJson(const string& body) {
    map<string, string> result;
    regex pairRegex("\"([^\"]+)\"\\s*:\\s*(\"([^\"]*)\"|([0-9]+))");
    auto begin = sregex_iterator(body.begin(), body.end(), pairRegex);
    auto end = sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        smatch match = *it;
        string key = match[1].str();
        string value = match[3].matched ? match[3].str() : match[4].str();
        result[key] = value;
    }
    return result;
}

string jsonEscape(const string& value) {
    string escaped;
    for (char c : value) {
        switch (c) {
            case '"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped.push_back(c); break;
        }
    }
    return escaped;
}

string makeJson(const map<string, string>& pairs) {
    ostringstream ss;
    ss << "{";
    bool first = true;
    for (auto& [key, value] : pairs) {
        if (!first) ss << ",";
        first = false;
        ss << '"' << jsonEscape(key) << "\":\"" << jsonEscape(value) << '"';
    }
    ss << "}";
    return ss.str();
}

bool sendAll(SOCKET socket, const string& data) {
    const char* buffer = data.c_str();
    int total = 0;
    int length = static_cast<int>(data.size());
    while (total < length) {
        int sent = send(socket, buffer + total, length - total, 0);
        if (sent == SOCKET_ERROR) return false;
        total += sent;
    }
    return true;
}

string loadStaticFile(const string& path, string& contentType) {
    string fullPath = WWW_ROOT + path;
    if (fullPath == WWW_ROOT + "/") fullPath = WWW_ROOT + "/index.html";
    ifstream file(fullPath, ios::binary);
    if (!file.is_open()) return "";
    ostringstream buffer;
    buffer << file.rdbuf();
    string ext;
    auto pos = fullPath.find_last_of('.');
    if (pos != string::npos) ext = fullPath.substr(pos + 1);
    if (ext == "css") contentType = "text/css";
    else if (ext == "js") contentType = "application/javascript";
    else if (ext == "html") contentType = "text/html";
    else if (ext == "png") contentType = "image/png";
    else contentType = "text/plain";
    return buffer.str();
}

string readRequest(SOCKET client, string& method, string& path, map<string, string>& headers, string& body) {
    constexpr int bufferSize = 4096;
    char buffer[bufferSize];
    string request;
    int received;
    while (request.find("\r\n\r\n") == string::npos) {
        received = recv(client, buffer, bufferSize, 0);
        if (received <= 0) return "";
        request.append(buffer, received);
    }
    auto headerEnd = request.find("\r\n\r\n");
    string headerPart = request.substr(0, headerEnd);
    body = request.substr(headerEnd + 4);

    istringstream stream(headerPart);
    stream >> method;
    stream >> path;
    string line;
    getline(stream, line);
    while (getline(stream, line)) {
        if (line.empty() || line == "\r") continue;
        auto colon = line.find(':');
        if (colon == string::npos) continue;
        string name = trim(line.substr(0, colon));
        string value = trim(line.substr(colon + 1));
        headers[name] = value;
    }
    if (headers.count("Content-Length")) {
        int length = stoi(headers["Content-Length"]);
        while ((int)body.size() < length) {
            received = recv(client, buffer, bufferSize, 0);
            if (received <= 0) break;
            body.append(buffer, received);
        }
    }
    return request;
}

void sendResponse(SOCKET client, const string& body, const string& contentType = "application/json", int statusCode = 200, const string& statusText = "OK") {
    ostringstream response;
    response << "HTTP/1.1 " << statusCode << " " << statusText << "\r\n";
    response << "Content-Type: " << contentType << "; charset=UTF-8\r\n";
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "Access-Control-Allow-Origin: *\r\n";
    response << "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n";
    response << "Access-Control-Allow-Headers: Content-Type\r\n";
    response << "\r\n";
    response << body;
    sendAll(client, response.str());
}

void handleApiRequest(SOCKET client, const string& method, const string& rawPath, const map<string, string>& headers, const string& body) {
    string path = rawPath;
    string query;
    auto qpos = rawPath.find('?');
    if (qpos != string::npos) {
        path = rawPath.substr(0, qpos);
        query = rawPath.substr(qpos + 1);
    }
    if (method == "OPTIONS") {
        sendResponse(client, "", "text/plain", 200, "OK");
        return;
    }
    if (path == "/api/status") {
        sendResponse(client, "{\"status\":\"ready\"}");
        return;
    }
    if (path == "/api/login" && method == "POST") {
        auto dataMap = parseJson(body);
        string username = dataMap["username"];
        string password = dataMap["password"];
        User* user = store.validateLogin(username, password);
        if (!user) {
            sendResponse(client, "{\"success\":false,\"message\":\"Invalid credentials.\"}");
            return;
        }
        store.logAudit(user->username, "login", "User signed in.");
        string payload = "{\"success\":true,\"userId\":" + to_string(user->id) + ",\"username\":\"" + jsonEscape(user->username) + "\",\"role\":\"" + jsonEscape(user->role) + "\",\"creditScore\":" + to_string(user->creditScore) + ",\"blocked\":" + string(user->blocked ? "true" : "false") + "}";
        sendResponse(client, payload);
        return;
    }
    if (path == "/api/register" && method == "POST") {
        auto dataMap = parseJson(body);
        string username = dataMap["username"];
        string password = dataMap["password"];
        string email = dataMap["email"];
        string error;
        if (!store.registerUser(username, password, email, error)) {
            sendResponse(client, "{\"success\":false,\"message\":\"" + jsonEscape(error) + "\"}");
            return;
        }
        sendResponse(client, "{\"success\":true,\"message\":\"Account created. Please login.\"}");
        return;
    }
    if (path == "/api/books" && method == "GET") {
        auto filters = parseQueryString(query);
        auto list = store.searchBooks(filters);
        ostringstream result;
        result << "[";
        for (size_t i = 0; i < list.size(); ++i) {
            auto& book = list[i];
            result << "{\"id\":" << book.id << ",\"title\":\"" << jsonEscape(book.title) << "\",\"author\":\"" << jsonEscape(book.author) << "\",\"category\":\"" << jsonEscape(book.category) << "\",\"status\":\"" << jsonEscape(book.status) << "\",\"borrowerId\":" << book.borrowerId << ",\"dueDate\":\"" << jsonEscape(book.dueDate) << "\"}";
            if (i + 1 < list.size()) result << ",";
        }
        result << "]";
        sendResponse(client, result.str());
        return;
    }
    if (path == "/api/user" && method == "GET") {
        auto params = parseQueryString(query);
        int userId = params.count("userId") ? stoi(params["userId"]) : 0;
        User* user = store.findUserById(userId);
        if (!user) {
            sendResponse(client, "{\"success\":false,\"message\":\"User not found.\"}");
            return;
        }
        ostringstream payload;
        payload << "{\"success\":true,\"id\":" << user->id << ",\"username\":\"" << jsonEscape(user->username) << "\",\"email\":\"" << jsonEscape(user->email) << "\",\"role\":\"" << jsonEscape(user->role) << "\",\"creditScore\":" << user->creditScore << ",\"overdueCount\":" << user->overdueCount << ",\"blocked\":" << (user->blocked ? "true" : "false") << "}";
        sendResponse(client, payload.str());
        return;
    }
    if (path == "/api/user/notifications" && method == "GET") {
        auto params = parseQueryString(query);
        int userId = params.count("userId") ? stoi(params["userId"]) : 0;
        vector<Notification> list = store.userNotifications(userId);
        ostringstream result;
        result << "[";
        for (size_t i = 0; i < list.size(); ++i) {
            auto& note = list[i];
            result << "{\"id\":" << note.id << ",\"message\":\"" << jsonEscape(note.message) << "\",\"timestamp\":\"" << jsonEscape(note.timestamp) << "\"}";
            if (i + 1 < list.size()) result << ",";
        }
        result << "]";
        sendResponse(client, result.str());
        return;
    }
    if (path == "/api/admin/addbook" && method == "POST") {
        auto dataMap = parseJson(body);
        int adminId = dataMap.count("adminId") ? stoi(dataMap["adminId"]) : 0;
        string title = dataMap["title"];
        string author = dataMap["author"];
        string category = dataMap["category"];
        string error;
        if (!store.addBook(title, author, category, adminId, error)) {
            sendResponse(client, "{\"success\":false,\"message\":\"" + jsonEscape(error) + "\"}");
            return;
        }
        sendResponse(client, "{\"success\":true,\"message\":\"Book added successfully.\"}");
        return;
    }
    if (path == "/api/borrow" && method == "POST") {
        auto dataMap = parseJson(body);
        int userId = dataMap.count("userId") ? stoi(dataMap["userId"]) : 0;
        int bookId = dataMap.count("bookId") ? stoi(dataMap["bookId"]) : 0;
        string error;
        if (!store.borrowBook(userId, bookId, error)) {
            sendResponse(client, "{\"success\":false,\"message\":\"" + jsonEscape(error) + "\"}");
            return;
        }
        sendResponse(client, "{\"success\":true,\"message\":\"Book reserved successfully.\"}");
        return;
    }
    if (path == "/api/return" && method == "POST") {
        auto dataMap = parseJson(body);
        int userId = dataMap.count("userId") ? stoi(dataMap["userId"]) : 0;
        int bookId = dataMap.count("bookId") ? stoi(dataMap["bookId"]) : 0;
        string error;
        if (!store.returnBook(userId, bookId, error)) {
            sendResponse(client, "{\"success\":false,\"message\":\"" + jsonEscape(error) + "\"}");
            return;
        }
        sendResponse(client, "{\"success\":true,\"message\":\"Book returned successfully.\"}");
        return;
    }
    if (path == "/api/audit" && method == "GET") {
        auto params = parseQueryString(query);
        int adminId = params.count("adminId") ? stoi(params["adminId"]) : 0;
        User* admin = store.findUserById(adminId);
        if (!admin || admin->role != "admin") {
            sendResponse(client, "{\"success\":false,\"message\":\"Unauthorized.\"}");
            return;
        }
        ostringstream result;
        result << "[";
        for (size_t i = 0; i < store.audits.size(); ++i) {
            auto& item = store.audits[i];
            result << "{\"id\":" << item.id << ",\"actor\":\"" << jsonEscape(item.actor) << "\",\"action\":\"" << jsonEscape(item.action) << "\",\"timestamp\":\"" << jsonEscape(item.timestamp) << "\",\"details\":\"" << jsonEscape(item.details) << "\"}";
            if (i + 1 < store.audits.size()) result << ",";
        }
        result << "]";
        sendResponse(client, result.str());
        return;
    }
    sendResponse(client, "{\"success\":false,\"message\":\"Unknown API endpoint.\"}", "application/json", 404, "Not Found");
}

void handleClient(SOCKET client) {
    string method, rawPath, body;
    map<string, string> headers;
    string request = readRequest(client, method, rawPath, headers, body);
    if (request.empty()) {
        closesocket(client);
        return;
    }
    if (rawPath.rfind("/api/", 0) == 0) {
        handleApiRequest(client, method, rawPath, headers, body);
    } else {
        string path = rawPath;
        if (path == "/") path = "/index.html";
        string contentType;
        string data = loadStaticFile(path, contentType);
        if (data.empty()) {
            sendResponse(client, "<h1>404 Not Found</h1>", "text/html", 404, "Not Found");
        } else {
            sendResponse(client, data, contentType, 200, "OK");
        }
    }
    closesocket(client);
}

int main() {
    store.load();

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in service{};
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(8080);

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));

    if (bind(server, (sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
        cerr << "Bind failed." << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed." << endl;
        closesocket(server);
        WSACleanup();
        return 1;
    }

    cout << "Inventory server is running at http://localhost:8080/" << endl;
    cout << "Use admin: admin/admin123 or create a student account." << endl;

    while (true) {
        SOCKET client = accept(server, nullptr, nullptr);
        if (client == INVALID_SOCKET) continue;
        thread(handleClient, client).detach();
    }

    closesocket(server);
    WSACleanup();
    return 0;
}
