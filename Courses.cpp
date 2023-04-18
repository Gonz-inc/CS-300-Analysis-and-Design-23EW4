////============================================================================
// Name        : Courses.cpp
// Author      : Gerardo Gonzalez
// Version     : 1.0
// Description : Course minipulation for ABC University. 
//============================================================================

#include <algorithm>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <fstream>
#include <time.h>
#include "CSVparser.hpp"

using namespace std;
/*
Global definitions
*/

// forward declarations as seen from the previous BTS project.
double strToDouble(string str, char ch);

// Initializing the structs. 
struct Course {
	string courseID;
	string courseTitle;
	vector<string> preReq;

	Course() {
	}
};

// Attempting to create a BST for course data. sets the left and right leaves to nullptr.
struct Node {
	Course course;
	Node* left;
	Node* right;

	Node() {
		left = nullptr;
		right = nullptr;
	}
	Node(Course aCourse) : Node() {
		this->course = aCourse;
	}
};

// Class Initialization 
class  courseBinarySearchTree {
private:
	Node* root;
	void addNode(Node* node, Course course);
	void printSampleSchedule(Node* node);
	void printCourseInformation(Node* node, string courseID);

public:
	courseBinarySearchTree();
	virtual ~courseBinarySearchTree();
	void deleteRecurse(Node* node);
	void Insert(Course course);
	int numPrerequisiteCourses(Course course);
	void PrintSampleSchedule();
	void PrintCourseInfromation(string courseID);
};

// BST construct 
courseBinarySearchTree::courseBinarySearchTree() {
	root = nullptr;
}

// Destructor used for deleting recurtions. 
courseBinarySearchTree::~courseBinarySearchTree() {
	deleteRecurse(root);
}

// deleting method for deleteRecurse
void courseBinarySearchTree::deleteRecurse(Node* node) {
	if (node) {
		deleteRecurse(node->left);
		deleteRecurse(node->right);
		delete node;
	}
}

// This is method is used to insert nodes in the Tree.
void courseBinarySearchTree::Insert(Course course) {
	// If else loop to insert node appropriately  
	if (root == nullptr) {
		root = new Node(course);
	}
	else {
		this->addNode(root, course);
	}
}

// Counts the number of prerequisite coures and returns the number of course. This outputs it with course Number. 
int courseBinarySearchTree::numPrerequisiteCourses(Course course) {
	int count = 0;
	for (int i = 0; i < course.preReq.size(); i++) {
		if (course.preReq.at(i).length() > 0) {
			count++;
		}
		return count;
	}
}

// Print Smaple Schedule constructor calls another functions.
void courseBinarySearchTree::PrintSampleSchedule() {
	this->printSampleSchedule(root);
}

// Passes course attributes to functions. 
void courseBinarySearchTree::PrintCourseInfromation(string courseID) {
	this->printCourseInformation(root, courseID);
}



// Reason for this functions is to pring the sample schedule
void courseBinarySearchTree::printSampleSchedule(Node* node) {
	if (node != nullptr) {

		printSampleSchedule(node->left);
		cout << node->course.courseID << " " << node->course.courseTitle << endl;
		printSampleSchedule(node->right);
	}
	return;
}

// Reason for this function is to insert the courses in the correct nodes.
void courseBinarySearchTree::addNode(Node* node, Course course) {
	//This tries to sort the course based on there course number pr course ID.
	if (node->course.courseID.compare(course.courseID) > 0) {
		if (node->left == nullptr) {
			node->left == new Node(course);
		}
		else {
			this->addNode(node->left, course);
		}
	}
	else {
		if (node->right == nullptr) {
			node->right = new Node(course);
		}
		else {
			this->addNode(node->right, course);
		}
	}
}

// Prints the Courses and also checks the number of prerequisites is needed if any.
void  courseBinarySearchTree::printCourseInformation(Node* current, string courseID) {

	while (current != nullptr) {
		if (current->course.courseID.compare(courseID) == 0) {
			cout << current->course.courseID << ", " << current->course.courseTitle << endl;
			unsigned int sizeOfPrereq = numPrerequisiteCourses(current->course);
			cout << "Prerequisites: " << endl;
			unsigned int i = 0;
			for (i = 0; i < sizeOfPrereq; i++) {
				cout << current->course.preReq.at(i);
				if (i != sizeOfPrereq - 1) {
					cout << ", ";
				}
			}
			if (i == 0) {
				cout << "No prerequisites.\n" << endl;
				return;
			}
		}
		// moves down to see if there is any prerequistes in leafs/nodes.
		else if (courseID.compare(current->course.courseID) < 0) {
			current = current->left;
		}
		else {
			current = current->right;
		}
	}
	cout << "Course " << courseID << " not found." << endl;
}

// This attemp to load the the txt files and pushback them into a vector.
// Note that there is code commented out as I tried to trouble shoot and get this function to work properly. 
// FIXME: The file copies and inputs multiple copies of each course. 
void loadCourses(string csvPath, courseBinarySearchTree* cBST) {
	try {
		ifstream courseFile(csvPath);
		if (courseFile.is_open()) {
			while (!courseFile.eof()) {
				//cout << "Loading CSV file " << csvPath << endl;
				vector<string> someData;
				string courseData;
				Course course;

				getline(courseFile, courseData);

				//cout << courseData << endl;
				while (courseData.length() > 0) {
					unsigned int signifier = courseData.find(',');
					if (signifier < 100) {

						someData.push_back(courseData.substr(0, signifier));
						courseData.erase(0, signifier + 1);
						//cout << "Task: " << signifier << endl;
						//cout << signifier<< endl;

					}
					else {
						someData.push_back(courseData.substr(0, courseData.length()));
						courseData = "";

					}
				}

				for (int i = 2; i < someData.size(); i++) {
					course.preReq.push_back(someData[i]);
					course.courseID = someData[0];
					course.courseTitle = someData[1];


				}
				//for (string c : someData) {
					//cout << c << endl;
					//cout << "0 : " << someData[0];
					//cout << " 1 : " << someData[1] << endl;
					//cout << "";

				//}
				cBST->Insert(course);
			}
			courseFile.close();
			return;
		}

	}

	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}

// The main method inspired from past code projects in this course. 
int main(int argc, char* argv[]) {
	string csvPath, courseID;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		break;
	case 3:
		csvPath = argv[1];
		courseID = argv[2];
		break;
	default:
		csvPath = "ABCU_Advising_Program_Input.txt";
		break;
	}

	courseBinarySearchTree* cBinarySearchTree;
	cBinarySearchTree = new courseBinarySearchTree();
	Course course;

	cout << "\nWelcome to the course planner!\n" << endl;

	string decision = "0";
	int userDecision = decision[0] - '0';
	while (userDecision != 9) {
		cout << " 1. Load Data Structure" << endl;
		cout << " 2. Print Course List" << endl;
		cout << " 3. Print Course" << endl;
		cout << " 9. Exit" << endl;
		cout << "\nWhat would you like to do? ";
		cin >> userDecision;

		switch (userDecision) {
		case 1:
			loadCourses(csvPath, cBinarySearchTree);
			break;

		case 2:
			if (course.courseID.empty()) {
				cout << "Here is a sample schedule:\n" << endl;
				cBinarySearchTree->PrintSampleSchedule();
				cout << endl;
			}
			else {
				cout << "\nPlease load courses\n" << endl;
			}
			break;
		case 3:
			if (cBinarySearchTree != nullptr) {
				if (courseID.length() == 0) {
					cout << "What course do you want to know about?" << endl;
					cin >> courseID;
					for (auto& userDecision : courseID) userDecision = toupper(userDecision); {
						cBinarySearchTree->PrintCourseInfromation(courseID);
						cout << endl;
					}
				}
				else {
				}
					courseID = "";
			}
			break;
		case 4:
			if (userDecision == 9) {
				cout << "\nThank you for using the course planner!" << endl;
			}
			break;
		}
	}
	cout << "\nThank you for using the course planner!" << endl;
}