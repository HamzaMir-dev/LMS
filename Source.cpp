#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

const int MAX_BOOKS = 100;
const int MAX_USERS = 100;
const int MAX_STRING_LENGTH = 100;

int bookCount = 0;
int userCount = 0;

int* bookIds = new int[MAX_BOOKS];
char* bookTitles = new char[MAX_BOOKS * MAX_STRING_LENGTH];
char* bookAuthors = new char[MAX_BOOKS * MAX_STRING_LENGTH];
bool* bookIssued = new bool[MAX_BOOKS];

int* userIds = new int[MAX_USERS];
char* userNames = new char[MAX_USERS * MAX_STRING_LENGTH];
int* userIssuedBookIds = new int[MAX_USERS];

void saveData() {
	ofstream outFile("library_data.txt");
	outFile << bookCount << endl;
	for (int i = 0; i < bookCount; i++) {
		outFile << bookIds[i] << endl;
		outFile << &bookTitles[i * MAX_STRING_LENGTH] << endl;
		outFile << &bookAuthors[i * MAX_STRING_LENGTH] << endl;
		outFile << bookIssued[i] << endl;
	}
	outFile << userCount << endl;
	for (int i = 0; i < userCount; i++) {
		outFile << userIds[i] << endl;
		outFile << &userNames[i * MAX_STRING_LENGTH] << endl;
		outFile << userIssuedBookIds[i] << endl;
	}
	outFile.close();
}

void loadData() {
	ifstream inFile("library_data.txt");
	if (!inFile) {
		return;
	}

	inFile >> bookCount;
	inFile.ignore();
	for (int i = 0; i < bookCount; i++) {
		inFile >> bookIds[i];
		inFile.ignore();
		inFile.getline(&bookTitles[i * MAX_STRING_LENGTH], MAX_STRING_LENGTH);
		inFile.getline(&bookAuthors[i * MAX_STRING_LENGTH], MAX_STRING_LENGTH);
		inFile >> bookIssued[i];
		inFile.ignore();
	}
	inFile >> userCount;
	inFile.ignore();
	for (int i = 0; i < userCount; i++) {
		inFile >> userIds[i];
		inFile.ignore();
		inFile.getline(&userNames[i * MAX_STRING_LENGTH], MAX_STRING_LENGTH);
		inFile >> userIssuedBookIds[i];
		inFile.ignore();
	}
	inFile.close();
}

bool isBookAdded(int id) {
	for (int i = 0; i < bookCount; i++) {
		if (bookIds[i] == id) {
			return true;
		}
	}
	return false;
}

bool isBookIssued(int id) {
	for (int i = 0; i < bookCount; i++) {
		if (bookIds[i] == id && bookIssued[i]) {
			return true;
		}
	}
	return false;
}

void addBook() {
	if (bookCount >= MAX_BOOKS) {
		cout << "Library is full, cannot add more books." << endl;
		return;
	}
	int id;
	char title[MAX_STRING_LENGTH];
	char author[MAX_STRING_LENGTH];

	cout << "Enter book ID: ";
	cin >> id;
	cin.ignore();

	if (isBookAdded(id)) {
		cout << "Book with ID " << id << " already exists." << endl;
		return;
	}

	cout << "Enter book title: ";
	cin.getline(title, MAX_STRING_LENGTH);
	cout << "Enter book author: ";
	cin.getline(author, MAX_STRING_LENGTH);

	bookIds[bookCount] = id;
	strcpy_s(&bookTitles[bookCount * MAX_STRING_LENGTH], MAX_STRING_LENGTH, title);
	strcpy_s(&bookAuthors[bookCount * MAX_STRING_LENGTH], MAX_STRING_LENGTH, author);
	bookIssued[bookCount] = false;

	bookCount++;
	saveData();
}

void registerUser() {
	if (userCount >= MAX_USERS) {
		cout << "User limit reached, cannot register more users." << endl;
		return;
	}
	int id;
	char name[MAX_STRING_LENGTH];

	cout << "Enter user ID: ";
	cin >> id;
	cin.ignore();

	for (int i = 0; i < userCount; i++) {
		if (userIds[i] == id) {
			cout << "User with ID " << id << " already exists." << endl;
			return;
		}
	}

	cout << "Enter user name: ";
	cin.getline(name, MAX_STRING_LENGTH);

	userIds[userCount] = id;
	strcpy_s(&userNames[userCount * MAX_STRING_LENGTH], MAX_STRING_LENGTH, name);
	userIssuedBookIds[userCount] = -1;

	userCount++;
	saveData();
}

void issueBook() {
	int userId, bookId;
	cout << "Enter user ID: ";
	cin >> userId;
	cout << "Enter book ID: ";
	cin >> bookId;

	int userIndex = -1;
	for (int i = 0; i < userCount; i++) {
		if (userIds[i] == userId) {
			userIndex = i;
			break;
		}
	}

	if (userIndex == -1) {
		cout << "User not found." << endl;
		return;
	}

	if (userIssuedBookIds[userIndex] != -1) {
		cout << "User has already issued a book." << endl;
		return;
	}

	int bookIndex = -1;
	for (int i = 0; i < bookCount; i++) {
		if (bookIds[i] == bookId) {
			bookIndex = i;
			break;
		}
	}

	if (bookIndex == -1) {
		cout << "Book not found." << endl;
		return;
	}

	if (bookIssued[bookIndex]) {
		cout << "Book is already issued." << endl;
		return;
	}

	bookIssued[bookIndex] = true;
	userIssuedBookIds[userIndex] = bookId;

	cout << "Book issued successfully." << endl;
	saveData();
}

void returnBook() {
	int userId;
	cout << "Enter user ID: ";
	cin >> userId;

	int userIndex = -1;
	for (int i = 0; i < userCount; i++) {
		if (userIds[i] == userId) {
			userIndex = i;
			break;
		}
	}

	if (userIndex == -1) {
		cout << "User not found." << endl;
		return;
	}

	int bookId = userIssuedBookIds[userIndex];
	if (bookId == -1) {
		cout << "No book to return." << endl;
		return;
	}

	int bookIndex = -1;
	for (int i = 0; i < bookCount; i++) {
		if (bookIds[i] == bookId) {
			bookIndex = i;
			break;
		}
	}

	if (bookIndex == -1) {
		cout << "Book not found." << endl;
		return;
	}

	if (!bookIssued[bookIndex]) {
		cout << "Book is not issued." << endl;
		return;
	}

	bookIssued[bookIndex] = false;
	userIssuedBookIds[userIndex] = -1;

	cout << "Book returned successfully." << endl;
	saveData();
}

void exitProgram() {
	delete[] bookIds;
	delete[] bookTitles;
	delete[] bookAuthors;
	delete[] bookIssued;
	delete[] userIds;
	delete[] userNames;
	delete[] userIssuedBookIds;

	cout << "Exiting program." << endl;
	exit(0);
}

int main() {
	loadData();

	while (true) {
		cout << "1. Add Book" << endl;
		cout << "2. Register User" << endl;
		cout << "3. Issue Book" << endl;
		cout << "4. Return Book" << endl;
		cout << "5. Exit" << endl;

		int choice;
		cout << "Enter your choice: ";
		cin >> choice;

		switch (choice) {
		case 1:
			addBook();
			break;
		case 2:
			registerUser();
			break;
		case 3:
			issueBook();
			break;
		case 4:
			returnBook();
			break;
		case 5:
			exitProgram();
			break;
		default:
			cout << "Invalid choice. Please try again." << endl;
		}
	}
	return 0;
}
