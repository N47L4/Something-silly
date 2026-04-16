#define _WIN32_WINNT 0x0601
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
#include <iomanip>

#pragma comment(lib, "ws2_32.lib")

static const std::string ADMIN_USER = "admin";
static const std::string ADMIN_PASS = "admin123";
static const std::string DEFAULT_USER = "user";
static const std::string DEFAULT_PASS = "password";
static const std::string ACCOUNTS_FILE = "accounts.txt";
static const std::string RESERVATIONS_FILE = "reservations.txt";
static const std::string USER_PROFILES_FILE = "user_profiles.txt";

struct UserProfile {
    std::string creditScore = "900";
    int overdueCount = 0;
    std::string accountStatus = "Good Standing";
    std::string fullName;
    std::string email;
    std::string year;
    std::string section;
    std::vector<std::string> notifications;
};

static std::map<std::string, std::string> userAccounts;
static std::map<std::string, std::string> sessionUsers;
static std::map<std::string, std::pair<std::string, std::time_t>> bookReservations;
static std::map<std::string, UserProfile> userProfiles;
static const std::map<std::string, std::string> BOOK_TITLE_MAP = {
    {"1", "The Art of Algorithms"},
    {"2", "Campus History: Then and Now"},
    {"3", "Modern Chemistry Lab"},
    {"4", "Business Strategy Fundamentals"},
    {"5", "Graphic Design Essentials"},
    {"6", "Calculus in Context"},
    {"7", "English Literature Classics"},
    {"8", "Physics for Everyone"},
    {"9", "World History Overview"},
    {"10", "Statistics Made Simple"},
    {"11", "Algebra Essentials"},
    {"12", "Geometry for Students"},
    {"13", "Trigonometry in Practice"},
    {"14", "English Grammar Today"},
    {"15", "Academic Writing Skills"},
    {"16", "Reading Comprehension Strategies"},
    {"17", "Foundations of Physics"},
    {"18", "Biology: Life Systems"},
    {"19", "Earth Science Explorer"},
    {"20", "Philippine History Stories"},
    {"21", "World War II and Beyond"},
    {"22", "Historical Inquiry"},
    {"23", "Music Theory Basics"},
    {"24", "The Sound of Instruments"},
    {"25", "Listening to Music"},
    {"26", "Visual Arts Workshop"},
    {"27", "Design, Color, and Form"},
    {"28", "Creative Arts Expressions"},
    {"29", "P.E. Fitness and Sport"},
    {"30", "Team Sports Fundamentals"},
    {"31", "Active Lifestyle Guide"},
    {"32", "Health and Wellness"},
    {"33", "Teen Health Choices"},
    {"34", "Personal Health Essentials"},
    {"35", "Philippine Literature Today"},
    {"36", "Global Short Stories"},
    {"37", "Modern Poetry Selections"},
    {"38", "T.L.E. Home Skills"},
    {"39", "Entrepreneurship Basics"},
    {"40", "Technical Drafting Essentials"},
    {"41", "Computer Basics"},
    {"42", "Programming Fundamentals"},
    {"43", "Web Design Principles"},
    {"44", "Values and Ethics"},
    {"45", "Empathy and Respect"},
    {"46", "Character Building"},
    {"47", "Filipino Language and Culture"},
    {"48", "Mga Kuwento ng Bayan"},
    {"49", "Pagbasa at Pagsulat Tungo sa Pananaliksik"}
};

std::string replaceAll(std::string text, const std::string &search, const std::string &replacement);
std::map<std::string, std::string> parseQueryString(const std::string &query);
std::string buildBorrowedBooksHtml(const std::string &username);
std::string renderUserPage(const std::string &username, const std::map<std::string, std::string> &queryParams);
std::string jsonEscape(const std::string &text);
std::string getAccountsJson();
std::vector<std::string> splitString(const std::string &value, char delimiter);
std::string joinNotifications(const std::vector<std::string> &notifications);
std::time_t currentTime();
std::string readFile(const std::string &path);
UserProfile &ensureUserProfile(const std::string &username);

std::string jsonEscape(const std::string &text) {
    std::string result;
    result.reserve(text.size());
    for (char c : text) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    std::ostringstream oss;
                    oss << "\\u" << std::hex << std::uppercase << std::setw(4) << std::setfill('0') << (int)(unsigned char)c;
                    result += oss.str();
                } else {
                    result += c;
                }
        }
    }
    return result;
}

std::string getAccountsJson() {
    std::ostringstream out;
    out << "[";
    bool first = true;
    for (const auto &entry : userAccounts) {
        const std::string &username = entry.first;
        if (username == ADMIN_USER) {
            continue;
        }
        UserProfile &profile = ensureUserProfile(username);
        if (!first) {
            out << ",";
        }
        first = false;
        out << "{"
            << "\"username\":\"" << jsonEscape(username) << "\",";
        out << "\"fullName\":\"" << jsonEscape(profile.fullName) << "\",";
        out << "\"email\":\"" << jsonEscape(profile.email) << "\",";
        out << "\"year\":\"" << jsonEscape(profile.year) << "\",";
        out << "\"section\":\"" << jsonEscape(profile.section) << "\"}";
    }
    out << "]";
    return out.str();
}

std::string trim(const std::string &value) {
    size_t start = 0;
    while (start < value.size() && std::isspace(static_cast<unsigned char>(value[start]))) {
        start++;
    }
    size_t end = value.size();
    while (end > start && std::isspace(static_cast<unsigned char>(value[end - 1]))) {
        end--;
    }
    return value.substr(start, end - start);
}

std::string urlDecode(const std::string &input) {
    std::string result;
    result.reserve(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        if (c == '+') {
            result.push_back(' ');
        } else if (c == '%' && i + 2 < input.size()) {
            std::string hex = input.substr(i + 1, 2);
            char decoded = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
            result.push_back(decoded);
            i += 2;
        } else {
            result.push_back(c);
        }
    }
    return result;
}

std::map<std::string, std::string> parseHeaders(const std::string &text) {
    std::map<std::string, std::string> headers;
    std::istringstream stream(text);
    std::string line;
    while (std::getline(stream, line) && !line.empty()) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string value = line.substr(colon + 1);
            std::transform(key.begin(), key.end(), key.begin(), ::tolower);
            headers[key] = trim(value);
        }
    }
    return headers;
}

std::map<std::string, std::string> parseFormData(const std::string &body) {
    std::map<std::string, std::string> form;
    size_t pos = 0;
    while (pos < body.size()) {
        size_t amp = body.find('&', pos);
        std::string pair = body.substr(pos, amp - pos);
        size_t eq = pair.find('=');
        if (eq != std::string::npos) {
            std::string name = urlDecode(pair.substr(0, eq));
            std::string value = urlDecode(pair.substr(eq + 1));
            form[name] = value;
        }
        if (amp == std::string::npos) {
            break;
        }
        pos = amp + 1;
    }
    return form;
}

std::map<std::string, std::string> parseQueryString(const std::string &query) {
    return parseFormData(query);
}

std::string formatTimeRemaining(std::time_t expiry) {
    std::time_t now = currentTime();
    long seconds = static_cast<long>(expiry - now);
    if (seconds <= 0) {
        return "expired";
    }
    int hours = static_cast<int>(seconds / 3600);
    int minutes = static_cast<int>((seconds % 3600) / 60);
    int secs = static_cast<int>(seconds % 60);
    std::ostringstream out;
    if (hours > 0) {
        out << hours << "h ";
    }
    if (minutes > 0 || hours > 0) {
        out << minutes << "m ";
    }
    out << secs << "s";
    return out.str();
}

std::string buildBorrowedBooksHtml(const std::string &username) {
    std::ostringstream html;
    bool found = false;
    std::time_t now = currentTime();
    for (const auto &entry : bookReservations) {
        if (entry.second.first == username && entry.second.second > now) {
            found = true;
            std::tm *expiryTime = std::localtime(&entry.second.second);
            char buffer[128];
            std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", expiryTime);
            std::string title = entry.first;
            auto it = BOOK_TITLE_MAP.find(entry.first);
            if (it != BOOK_TITLE_MAP.end()) {
                title = it->second;
            }
            html << "<article class=\"card\">"
                 << "<h3>" << title << "</h3>"
                 << "<p>Status: Pending</p>"
                 << "<p>Reserved until " << buffer << "</p>"
                 << "<p>Time remaining: " << formatTimeRemaining(entry.second.second) << "</p>"
                 << "</article>\n";
        }
    }
    if (!found) {
        html << "<article class=\"card\"><h3>No reserved books</h3><p>You have no active reservations.</p></article>\n";
    }
    return html.str();
}

std::string renderUserPage(const std::string &username, const std::map<std::string, std::string> &queryParams) {
    std::string page = readFile("user.html");
    if (page.empty()) {
        return std::string();
    }
    UserProfile &profile = ensureUserProfile(username);
    std::string searchQuery = queryParams.count("query") ? queryParams.at("query") : "";
    std::string status = queryParams.count("status") ? queryParams.at("status") : "";
    std::string statusAll = status.empty() ? " selected" : "";
    std::string statusAvailable = status == "available" ? " selected" : "";
    std::string statusBorrowed = status == "borrowed" ? " selected" : "";

    page = replaceAll(page, "{{MESSAGE}}", "");
    page = replaceAll(page, "{{CREDIT_SCORE}}", profile.creditScore);
    page = replaceAll(page, "{{OVERDUE_COUNT}}", std::to_string(profile.overdueCount));
    page = replaceAll(page, "{{ACCOUNT_STATUS}}", profile.accountStatus);
    page = replaceAll(page, "{{FULL_NAME}}", profile.fullName.empty() ? username : profile.fullName);
    page = replaceAll(page, "{{EMAIL}}", profile.email.empty() ? "N/A" : profile.email);
    page = replaceAll(page, "{{YEAR}}", profile.year.empty() ? "N/A" : profile.year);
    page = replaceAll(page, "{{SECTION}}", profile.section.empty() ? "N/A" : profile.section);
    page = replaceAll(page, "{{SEARCH_QUERY}}", searchQuery);
    page = replaceAll(page, "{{STATUS_ALL}}", statusAll);
    page = replaceAll(page, "{{STATUS_AVAILABLE}}", statusAvailable);
    page = replaceAll(page, "{{STATUS_BORROWED}}", statusBorrowed);
    page = replaceAll(page, "{{SEARCH_RESULTS}}", "");
    page = replaceAll(page, "{{BORROWED_BOOKS}}", buildBorrowedBooksHtml(username));
    page = replaceAll(page, "{{NOTIFICATIONS}}", joinNotifications(profile.notifications));
    return page;
}

std::string getCookie(const std::map<std::string, std::string> &headers, const std::string &name) {
    auto it = headers.find("cookie");
    if (it == headers.end()) {
        return std::string();
    }
    std::string cookieHeader = it->second;
    size_t pos = 0;
    while (pos < cookieHeader.size()) {
        size_t semi = cookieHeader.find(';', pos);
        std::string pair = cookieHeader.substr(pos, semi - pos);
        size_t eq = pair.find('=');
        if (eq != std::string::npos) {
            std::string key = trim(pair.substr(0, eq));
            std::string value = trim(pair.substr(eq + 1));
            if (key == name) {
                return value;
            }
        }
        if (semi == std::string::npos) {
            break;
        }
        pos = semi + 1;
    }
    return std::string();
}

std::time_t currentTime() {
    return std::time(nullptr);
}

void cleanupExpiredReservations() {
    std::time_t now = currentTime();
    for (auto it = bookReservations.begin(); it != bookReservations.end();) {
        if (it->second.second <= now) {
            it = bookReservations.erase(it);
        } else {
            ++it;
        }
    }
}

bool saveBookReservations() {
    std::ofstream file(RESERVATIONS_FILE, std::ios::trunc);
    if (!file) {
        return false;
    }
    for (const auto &entry : bookReservations) {
        file << entry.first << ":" << entry.second.first << ":" << entry.second.second << "\n";
    }
    return true;
}

void loadBookReservations() {
    bookReservations.clear();
    std::ifstream file(RESERVATIONS_FILE);
    if (!file) {
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        size_t firstSep = line.find(':');
        size_t secondSep = line.find(':', firstSep == std::string::npos ? std::string::npos : firstSep + 1);
        if (firstSep == std::string::npos || secondSep == std::string::npos) {
            continue;
        }
        std::string bookId = trim(line.substr(0, firstSep));
        std::string username = trim(line.substr(firstSep + 1, secondSep - firstSep - 1));
        std::time_t expiry = static_cast<std::time_t>(std::stoll(line.substr(secondSep + 1)));
        if (expiry > currentTime()) {
            bookReservations[bookId] = std::make_pair(username, expiry);
        }
    }
}

bool isBookAvailable(const std::string &bookId) {
    cleanupExpiredReservations();
    return bookReservations.find(bookId) == bookReservations.end();
}

bool reserveBook(const std::string &bookId, const std::string &username, int hours = 5) {
    cleanupExpiredReservations();
    auto it = bookReservations.find(bookId);
    std::time_t expiry = currentTime() + static_cast<std::time_t>(hours) * 3600;
    if (it == bookReservations.end()) {
        bookReservations[bookId] = std::make_pair(username, expiry);
        return saveBookReservations();
    }
    if (it->second.first == username) {
        it->second.second = expiry;
        return saveBookReservations();
    }
    return false;
}

std::string formatTimestamp(std::time_t timestamp) {
    std::tm *tmPtr = std::localtime(&timestamp);
    char buffer[128];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tmPtr);
    return std::string(buffer);
}

std::string getReservationsJson() {
    cleanupExpiredReservations();
    std::ostringstream output;
    output << "[";
    bool first = true;
    for (const auto &entry : bookReservations) {
        if (!first) {
            output << ",";
        }
        first = false;
        output << "{"
               << "\"bookId\":\"" << jsonEscape(entry.first) << "\"," 
               << "\"username\":\"" << jsonEscape(entry.second.first) << "\"," 
               << "\"reservedUntil\":\"" << jsonEscape(formatTimestamp(entry.second.second)) << "\"," 
               << "\"expiresAt\":\"" << entry.second.second << "\""
               << "}";
    }
    output << "]";
    return output.str();
}

std::string getAdminBookDetailsJson(const std::vector<std::string> &bookIds) {
    cleanupExpiredReservations();
    std::ostringstream output;
    output << "[";
    bool first = true;
    for (const auto &bookId : bookIds) {
        if (!first) {
            output << ",";
        }
        first = false;
        std::string status = "available";
        std::string username = "N/A";
        std::string reservedUntil = "N/A";
        std::string fullName = "Unknown";
        std::string email = "N/A";
        auto it = bookReservations.find(bookId);
        if (it != bookReservations.end()) {
            status = "reserved";
            username = it->second.first;
            reservedUntil = formatTimestamp(it->second.second);
            UserProfile &profile = ensureUserProfile(username);
            fullName = profile.fullName.empty() ? username : profile.fullName;
            email = profile.email.empty() ? "N/A" : profile.email;
        }
        output << "{"
               << "\"bookId\":\"" << jsonEscape(bookId) << "\"," 
               << "\"username\":\"" << jsonEscape(username) << "\"," 
               << "\"fullName\":\"" << jsonEscape(fullName) << "\"," 
               << "\"email\":\"" << jsonEscape(email) << "\"," 
               << "\"reservedUntil\":\"" << jsonEscape(reservedUntil) << "\"," 
               << "\"status\":\"" << jsonEscape(status) << "\""
               << "}";
    }
    output << "]";
    return output.str();
}

std::string replaceAll(std::string text, const std::string &search, const std::string &replacement) {
    size_t pos = 0;
    while ((pos = text.find(search, pos)) != std::string::npos) {
        text.replace(pos, search.length(), replacement);
        pos += replacement.length();
    }
    return text;
}

std::vector<std::string> splitString(const std::string &value, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(value);
    while (std::getline(stream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::string joinNotifications(const std::vector<std::string> &notifications) {
    std::ostringstream output;
    for (const auto &note : notifications) {
        output << "<div class=\"notification-card\">" << note << "</div>\n";
    }
    return output.str();
}

bool saveUserProfiles() {
    std::ofstream file(USER_PROFILES_FILE, std::ios::trunc);
    if (!file) {
        return false;
    }
    for (const auto &entry : userProfiles) {
        const auto &username = entry.first;
        const auto &profile = entry.second;
        file << username << ":" << profile.creditScore << ":" << profile.overdueCount << ":" << profile.accountStatus << ":" << profile.fullName << ":" << profile.email << ":" << profile.year << ":" << profile.section << ":";
        for (size_t i = 0; i < profile.notifications.size(); ++i) {
            if (i > 0) {
                file << "|";
            }
            file << profile.notifications[i];
        }
        file << "\n";
    }
    return true;
}

void loadUserProfiles() {
    userProfiles.clear();
    std::ifstream file(USER_PROFILES_FILE);
    if (!file) {
        return;
    }
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        std::vector<size_t> separators;
        size_t pos = 0;
        while (pos < line.size() && separators.size() < 8) {
            size_t sep = line.find(':', pos);
            if (sep == std::string::npos) {
                break;
            }
            separators.push_back(sep);
            pos = sep + 1;
        }

        if (separators.size() < 4) {
            continue;
        }

        std::string username = trim(line.substr(0, separators[0]));
        UserProfile profile;
        profile.creditScore = trim(line.substr(separators[0] + 1, separators[1] - separators[0] - 1));
        profile.overdueCount = std::stoi(line.substr(separators[1] + 1, separators[2] - separators[1] - 1));
        profile.accountStatus = trim(line.substr(separators[2] + 1, separators[3] - separators[2] - 1));

        std::string notificationsLine;
        if (separators.size() >= 8) {
            profile.fullName = trim(line.substr(separators[3] + 1, separators[4] - separators[3] - 1));
            profile.email = trim(line.substr(separators[4] + 1, separators[5] - separators[4] - 1));
            profile.year = trim(line.substr(separators[5] + 1, separators[6] - separators[5] - 1));
            profile.section = trim(line.substr(separators[6] + 1, separators[7] - separators[6] - 1));
            notificationsLine = line.substr(separators[7] + 1);
        } else {
            profile.fullName = "";
            profile.email = "";
            profile.year = "";
            profile.section = "";
            notificationsLine = line.substr(separators[3] + 1);
        }

        if (!notificationsLine.empty()) {
            profile.notifications = splitString(notificationsLine, '|');
        }
        userProfiles[username] = profile;
    }
}

UserProfile &ensureUserProfile(const std::string &username) {
    auto it = userProfiles.find(username);
    if (it == userProfiles.end()) {
        UserProfile profile;
        if (username == "userAko") {
            profile.creditScore = "500";
            profile.notifications.push_back("You have received 500 credits in your account.");
        }
        profile.notifications.push_back("Welcome " + username + "! Your personal account is ready.");
        userProfiles[username] = profile;
        saveUserProfiles();
    } else if (username == "userAko" && it->second.creditScore != "500") {
        it->second.creditScore = "500";
        saveUserProfiles();
    }
    return userProfiles[username];
}

UserProfile &createUserProfile(const std::string &username, const std::string &fullName, const std::string &email, const std::string &year, const std::string &section) {
    UserProfile &profile = ensureUserProfile(username);
    profile.fullName = fullName;
    profile.email = email;
    profile.year = year;
    profile.section = section;
    saveUserProfiles();
    return profile;
}

void addUserNotification(const std::string &username, const std::string &message) {
    UserProfile &profile = ensureUserProfile(username);
    profile.notifications.push_back(message);
    saveUserProfiles();
}

std::string getUsernameForSession(const std::string &sessionId) {
    auto it = sessionUsers.find(sessionId);
    if (it == sessionUsers.end()) {
        return std::string();
    }
    return it->second;
}

std::string makeSessionId() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static const char *hexDigits = "0123456789abcdef";
    std::string id;
    id.reserve(32);
    for (int i = 0; i < 32; ++i) {
        id.push_back(hexDigits[gen() % 16]);
    }
    return id;
}

std::string readFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return std::string();
    }
    std::ostringstream content;
    content << file.rdbuf();
    return content.str();
}

std::string getMimeType(const std::string &path) {
    if (path.size() >= 5 && path.compare(path.size() - 5, 5, ".html") == 0) {
        return "text/html; charset=utf-8";
    }
    if (path.size() >= 4 && path.compare(path.size() - 4, 4, ".css") == 0) {
        return "text/css; charset=utf-8";
    }
    return "application/octet-stream";
}

void loadUserAccounts() {
    userAccounts.clear();
    userAccounts[DEFAULT_USER] = DEFAULT_PASS;

    std::ifstream file(ACCOUNTS_FILE);
    if (!file) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        size_t separator = line.find(':');
        if (separator == std::string::npos) {
            continue;
        }

        std::string username = trim(line.substr(0, separator));
        std::string password = line.substr(separator + 1);
        if (username.empty() || username == ADMIN_USER) {
            continue;
        }

        if (userAccounts.count(username) == 0) {
            userAccounts[username] = password;
        }
    }
}

bool saveUserAccount(const std::string &username, const std::string &password) {
    std::ofstream file(ACCOUNTS_FILE, std::ios::app);
    if (!file) {
        return false;
    }
    file << username << ":" << password << "\n";
    userAccounts[username] = password;
    ensureUserProfile(username);
    return true;
}

bool accountExists(const std::string &username) {
    if (username == ADMIN_USER) {
        return true;
    }
    return userAccounts.count(username) > 0;
}

bool validateUserCredentials(const std::string &username, const std::string &password) {
    auto it = userAccounts.find(username);
    return it != userAccounts.end() && it->second == password;
}

void sendResponse(SOCKET client, const std::string &status, const std::map<std::string, std::string> &headers, const std::string &body) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << "\r\n";
    for (const auto &header : headers) {
        response << header.first << ": " << header.second << "\r\n";
    }
    response << "Content-Length: " << body.size() << "\r\n";
    response << "Connection: close\r\n";
    response << "\r\n";
    response << body;
    std::string data = response.str();
    send(client, data.c_str(), static_cast<int>(data.size()), 0);
}

std::string buildRedirect(const std::string &location, const std::string &setCookie = "") {
    std::map<std::string, std::string> headers;
    headers["Location"] = location;
    if (!setCookie.empty()) {
        headers["Set-Cookie"] = setCookie;
    }
    sendResponse(0, "302 Found", headers, "");
    return std::string();
}

bool pathIsValid(const std::string &path) {
    return path.find("..") == std::string::npos;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize WinSock." << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket." << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(8000);

    int yes = 1;
    setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&yes), sizeof(yes));

    if (bind(listenSocket, reinterpret_cast<sockaddr *>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket. Make sure port 8000 is available." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    loadUserAccounts();
    loadUserProfiles();
    loadBookReservations();
    std::cout << "Server is running on http://localhost:8000" << std::endl;
    std::cout << "Open your browser and visit the site." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            continue;
        }

        std::string request;
        char buffer[4096];
        int received;
        while ((received = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
            request.append(buffer, received);
            if (request.find("\r\n\r\n") != std::string::npos) {
                break;
            }
        }

        if (request.empty()) {
            closesocket(clientSocket);
            continue;
        }

        size_t headerEnd = request.find("\r\n\r\n");
        std::string headerText = request.substr(0, headerEnd);
        std::vector<std::string> requestLines;
        std::istringstream headerStream(headerText);
        std::string line;
        while (std::getline(headerStream, line)) {
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }
            requestLines.push_back(line);
        }

        if (requestLines.empty()) {
            closesocket(clientSocket);
            continue;
        }

        std::istringstream requestLine(requestLines[0]);
        std::string method;
        std::string rawPath;
        requestLine >> method >> rawPath;
        std::string path = rawPath;
        std::string queryString;
        size_t queryPos = path.find('?');
        if (queryPos != std::string::npos) {
            queryString = path.substr(queryPos + 1);
            path = path.substr(0, queryPos);
        }

        std::string body;
        std::map<std::string, std::string> headers = parseHeaders(headerText.substr(headerText.find('\n') + 1));

        if (headerEnd != std::string::npos) {
            size_t contentStart = headerEnd + 4;
            if (request.size() > contentStart) {
                body = request.substr(contentStart);
            }
            auto it = headers.find("content-length");
            if (it != headers.end()) {
                int contentLength = std::stoi(it->second);
                while (static_cast<int>(body.size()) < contentLength) {
                    received = recv(clientSocket, buffer, sizeof(buffer), 0);
                    if (received <= 0) {
                        break;
                    }
                    body.append(buffer, received);
                }
                if (static_cast<int>(body.size()) > contentLength) {
                    body.resize(contentLength);
                }
            }
        }

        std::string resourcePath = path;
        if (resourcePath == "/") {
            resourcePath = "index.html";
        } else if (resourcePath == "/register") {
            resourcePath = "register.html";
        }

        std::string requestedFile;
        if (!resourcePath.empty() && resourcePath[0] == '/') {
            requestedFile = resourcePath.substr(1);
        } else {
            requestedFile = resourcePath;
        }

        if (!pathIsValid(requestedFile)) {
            sendResponse(clientSocket, "400 Bad Request", { {"Content-Type", "text/plain"} }, "Invalid path.");
            closesocket(clientSocket);
            continue;
        }

        std::string cookieSession = getCookie(headers, "session");
        bool sessionValid = !cookieSession.empty() && sessionUsers.count(cookieSession) > 0;

        if (method == "GET" && path == "/user.html") {
            if (!sessionValid) {
                sendResponse(clientSocket, "302 Found", { {"Location", "/"} }, "");
                closesocket(clientSocket);
                continue;
            }
            std::map<std::string, std::string> queryParams = parseQueryString(queryString);
            std::string username = getUsernameForSession(cookieSession);
            std::string pageContent = renderUserPage(username, queryParams);
            if (pageContent.empty()) {
                sendResponse(clientSocket, "500 Internal Server Error", { {"Content-Type", "text/plain"} }, "Unable to render user page.");
            } else {
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, pageContent);
            }
            closesocket(clientSocket);
            continue;
        }

        if (method == "GET" && path == "/browse.html" && sessionValid) {
            sendResponse(clientSocket, "302 Found", { {"Location", "/browse-user.html"} }, "");
            closesocket(clientSocket);
            continue;
        }

        if (method == "GET" && path == "/browse-user.html" && !sessionValid) {
            sendResponse(clientSocket, "302 Found", { {"Location", "/browse.html"} }, "");
            closesocket(clientSocket);
            continue;
        }

        if (method == "POST" && path == "/login") {
            auto form = parseFormData(body);
            std::string username = form["username"];
            std::string password = form["password"];

            if (username == ADMIN_USER && password == ADMIN_PASS) {
                std::string sessionId = makeSessionId();
                sessionUsers[sessionId] = username;
                std::ostringstream cookieValue;
                cookieValue << "session=" << sessionId << "; Path=/; HttpOnly";
                std::map<std::string, std::string> headersOut;
                headersOut["Location"] = "admin.html";
                headersOut["Set-Cookie"] = cookieValue.str();
                sendResponse(clientSocket, "302 Found", headersOut, "");
            } else if (validateUserCredentials(username, password)) {
                std::string sessionId = makeSessionId();
                sessionUsers[sessionId] = username;
                std::ostringstream cookieValue;
                cookieValue << "session=" << sessionId << "; Path=/; HttpOnly";
                std::map<std::string, std::string> headersOut;
                headersOut["Location"] = "user.html";
                headersOut["Set-Cookie"] = cookieValue.str();
                sendResponse(clientSocket, "302 Found", headersOut, "");
            } else {
                std::string bodyHtml = "<html><body><h1>Invalid login</h1><p>Username or password is incorrect.</p><p><a href='index.html'>Return to login</a></p></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            }
            closesocket(clientSocket);
            continue;
        }

        if (method == "POST" && path == "/register") {
            auto form = parseFormData(body);
            std::string username = trim(form["username"]);
            std::string password = form["password"];
            std::string email = trim(form["email"]);
            std::string fullName = trim(form["fullName"]);
            std::string year = trim(form["year"]);
            std::string section = trim(form["section"]);

            std::string bodyHtml;
            if (username.empty() || password.empty() || email.empty() || fullName.empty() || year.empty() || section.empty()) {
                bodyHtml = "<html><body><h1>Registration failed</h1><p>Please complete all fields.</p><p><a href='register.html'>Return to registration</a></p></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            } else if (username == ADMIN_USER || username.find(':') != std::string::npos) {
                bodyHtml = "<html><body><h1>Registration failed</h1><p>The chosen username is not allowed.</p><p><a href='register.html'>Return to registration</a></p></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            } else if (accountExists(username)) {
                bodyHtml = "<html><body><h1>Registration failed</h1><p>That username is already taken.</p><p><a href='register.html'>Try another username</a></p></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            } else if (!saveUserAccount(username, password)) {
                bodyHtml = "<html><body><h1>Registration failed</h1><p>Unable to save your account. Please try again.</p><p><a href='register.html'>Return to registration</a></p></body></html>";
                sendResponse(clientSocket, "500 Internal Server Error", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            } else {
                createUserProfile(username, fullName, email, year, section);
                bodyHtml = "<html><head><meta charset='UTF-8' /><meta name='viewport' content='width=device-width, initial-scale=1.0' /><title>Registration Successful</title><style>body{font-family:Arial,sans-serif;background:#0f172a;color:#fff;display:flex;align-items:center;justify-content:center;height:100vh;margin:0;padding:24px} .card{max-width:420px;width:100%;background:#111827;border:1px solid rgba(148,163,184,.18);border-radius:20px;padding:32px;text-align:center} .card h1{margin:0 0 16px;font-size:1.75rem;} .card p{margin:0 0 24px;color:#cbd5e1;} .card a{display:inline-block;padding:12px 24px;border-radius:999px;background:#10b981;color:#fff;text-decoration:none;font-weight:700;}</style></head><body><div class='card'><h1>Success!</h1><p>Your account has been created successfully.</p><a href='login.html'>Continue to login</a></div></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            }
            closesocket(clientSocket);
            continue;
        }

        if (method == "GET" && path == "/accounts") {
            std::string bodyJson = getAccountsJson();
            sendResponse(clientSocket, "200 OK", { {"Content-Type", "application/json; charset=utf-8"} }, bodyJson);
            closesocket(clientSocket);
            continue;
        }

        if (method == "POST" && path == "/reserve") {
            std::string username = getUsernameForSession(cookieSession);
            if (username.empty()) {
                sendResponse(clientSocket, "302 Found", { {"Location", "/"} }, "");
                closesocket(clientSocket);
                continue;
            }
            auto form = parseFormData(body);
            std::string bookId = trim(form["book"]);
            if (bookId.empty()) {
                std::string bodyHtml = "<html><body><h1>Reservation failed</h1><p>No book ID provided.</p><p><a href='user.html'>Return to account</a></p></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
                closesocket(clientSocket);
                continue;
            }
            if (reserveBook(bookId, username, 5)) {
                std::time_t expiry = currentTime() + 5 * 3600;
                std::tm *expiryTime = std::localtime(&expiry);
                char expiryBuf[128];
                std::strftime(expiryBuf, sizeof(expiryBuf), "%Y-%m-%d %H:%M:%S", expiryTime);
                addUserNotification(username, std::string("Reserved book ") + bookId + " until " + expiryBuf + ".");
                std::string bodyHtml = "<html><body><h1>Reservation successful</h1><p>Book '" + bookId + "' is reserved for 5 hours.</p><p><a href='user.html'>Return to account</a></p></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            } else {
                std::string bodyHtml = "<html><body><h1>Reservation failed</h1><p>The book is already reserved by another user.</p><p><a href='user.html'>Return to account</a></p></body></html>";
                sendResponse(clientSocket, "200 OK", { {"Content-Type", "text/html; charset=utf-8"} }, bodyHtml);
            }
            closesocket(clientSocket);
            continue;
        }

        if (method == "GET" && path == "/account-exists") {
            std::map<std::string, std::string> queryParams = parseQueryString(queryString);
            std::string username = trim(queryParams["username"]);
            bool exists = accountExists(username);
            std::string bodyJson = std::string("{\"exists\":") + (exists ? "true" : "false") + "}";
            sendResponse(clientSocket, "200 OK", { {"Content-Type", "application/json; charset=utf-8"} }, bodyJson);
            closesocket(clientSocket);
            continue;
        }

        if (method == "GET" && path == "/reservations") {
            std::string bodyJson = getReservationsJson();
            sendResponse(clientSocket, "200 OK", { {"Content-Type", "application/json; charset=utf-8"} }, bodyJson);
            closesocket(clientSocket);
            continue;
        }

        if (method == "GET" && path == "/admin/book-details") {
            if (!sessionValid || getUsernameForSession(cookieSession) != ADMIN_USER) {
                sendResponse(clientSocket, "403 Forbidden", { {"Content-Type", "application/json; charset=utf-8"} }, "{\"error\":\"Forbidden\"}");
                closesocket(clientSocket);
                continue;
            }
            std::map<std::string, std::string> queryParams = parseQueryString(queryString);
            std::string bookIdsParam = trim(queryParams["bookIds"]);
            std::vector<std::string> bookIds = splitString(bookIdsParam, ',');
            std::string bodyJson = getAdminBookDetailsJson(bookIds);
            sendResponse(clientSocket, "200 OK", { {"Content-Type", "application/json; charset=utf-8"} }, bodyJson);
            closesocket(clientSocket);
            continue;
        }

        if (path == "admin.html" && !sessionValid) {
            sendResponse(clientSocket, "302 Found", { {"Location", "/"} }, "");
            closesocket(clientSocket);
            continue;
        }

        if (method != "GET") {
            sendResponse(clientSocket, "405 Method Not Allowed", { {"Content-Type", "text/plain"} }, "Method not allowed.");
            closesocket(clientSocket);
            continue;
        }

        std::string fileContent = readFile(requestedFile);
        if (fileContent.empty()) {
            sendResponse(clientSocket, "404 Not Found", { {"Content-Type", "text/plain"} }, "File not found.");
            closesocket(clientSocket);
            continue;
        }

        std::string mimeType = getMimeType(requestedFile);
        sendResponse(clientSocket, "200 OK", { {"Content-Type", mimeType} }, fileContent);
        closesocket(clientSocket);
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}
