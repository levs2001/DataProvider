#include<string>
#include<vector>
#include<iostream>

using namespace std;

enum class  EShiftNum {
	I = 1,
	II,
	III,
	IV
};


enum class  EPost {
	WORKER,
	SHIFT_SUPERVISOR,
	PRODUCTION_MANAGER
};

enum class  EStatus {
	ONPLACE,
	NOTONPLACE, 
	FREE
};

//Instead QDate
struct QDate {

};

class Employee
{
public:
	Employee(size_t id, string fullName, EPost post) : id(id), fullName(fullName), post(post) {

	}

	bool operator==(const Employee& wComp) {
		if (wComp.id == id && wComp.fullName == fullName && wComp.post == post)
			return true;
		return false;
	}

	const size_t GetId() {
		return id;
	}

	const string& GetFullName() {
		return fullName;
	}

	const EPost  GetPost() {
		return post;
	}

private:
	size_t id;
	string fullName;
	EPost post;
	EStatus status;
};


class ProdTape {
public:
	ProdTape(size_t id, string name) : id(id), name(name) {

	}

	void AddWorker(const Employee& worker) {
		workers.push_back(worker);
		//Need to remove from freeWorkers, but on the higher lvl
	}

	bool RemoveWorker(Employee& worker) {
		//HACK: Maybe we should write special func remove for vector, because it can be used in many cases
		auto it = find(workers.begin(), workers.end(), worker);

		if (it != workers.end()) {
			workers.erase(it);
			return true;
		}

		return false;
	}

	const size_t GetId() {
		return id;
	}

	const string& GetName() {
		return name;
	}

	const vector<Employee>& GetWorkers() {
		return workers;
	}

	size_t CountWorkers() {

	}
private:
	size_t id;
	string name;
	vector<Employee> workers;
};


//class FreeWorkers {
//public:
//	void Add(Employee worker) {
//		workers.push_back(worker);
//	}
//
//	bool RemoveWorker(Employee& worker) {
//		//HACK: Maybe we should write special func remove for vector, because it can be used in many cases
//		auto it = find(workers.begin(), workers.end(), worker);
//
//		if (it != workers.end()) {
//			workers.erase(it);
//			return true;
//		}
//
//		return false;
//	}
//
//	size_t CountWorkers() {
//
//	}
//
//protected:
//	vector<Employee> workers;
//};
//
//
//class DataFreeWorkers : FreeWorkers {
//public:
//	DataFreeWorkers(QDate date, EShiftNum shift) {
//		//HACK: Fills mass workers on current shift and date from DB
//	}
//};
//
//
//class TestFreeWorkers : FreeWorkers {
//	TestFreeWorkers(QDate date, EShiftNum shift) {
//		//HACK: Fills mass workers by test values
//	}
//};

class Shift {
public:
	Shift(QDate date, EShiftNum shiftNum) : date(date), shiftNum(shiftNum) {

	}
	
	void SetTapes(vector<ProdTape> newTapes) {

	}

	void SetTape(ProdTape newTape) {

	}

	void AddTape() {

	}

	const vector<ProdTape> GetTapes() {

	}

	const ProdTape GetTape(size_t id) {

	}

private:
	size_t idShiftLeader;
	vector<ProdTape> tapes;
	QDate date;
	EShiftNum shiftNum;
};

class StorageSingleton {
public:
	StorageSingleton(StorageSingleton& other) = delete;
	void operator=(const StorageSingleton&) = delete;

	virtual vector<Employee> GetFreeWorkers(QDate date, EShiftNum shiftNum) = 0;

	virtual Shift GetShift(QDate date, EShiftNum shiftNum) = 0;

	static StorageSingleton* GetInstance() {
		return pStorageSingleton_s;
	}

	static void Free() {
		delete pStorageSingleton_s;
	}
protected:
	StorageSingleton() {

	}
	//Static objects will be deleted automatically in the end of the programm
	static StorageSingleton* pStorageSingleton_s;
};

StorageSingleton* StorageSingleton::pStorageSingleton_s = nullptr;


class TestStorageSingleton : StorageSingleton {
public:
	TestStorageSingleton(TestStorageSingleton& other) = delete;
	void operator=(const TestStorageSingleton&) = delete;
	static void Init();
	
	vector<Employee> GetFreeWorkers(QDate date, EShiftNum shiftNum) {
		vector<Employee> freeWorkers;
		return freeWorkers;
	}

	Shift GetShift(QDate date, EShiftNum shiftNum) {
		Shift shift(date, shiftNum);
		return shift;
	}

private:
	TestStorageSingleton() : StorageSingleton(){
	}
};


class DataStorageSingleton : StorageSingleton {
public:
	DataStorageSingleton(DataStorageSingleton& other) = delete;
	void operator=(const DataStorageSingleton&) = delete;
	static void Init();
	
	vector<Employee> GetFreeWorkers(QDate date, EShiftNum shiftNum) {
		vector<Employee> freeWorkers;
		return freeWorkers;
	}

	Shift GetShift(QDate date, EShiftNum shiftNum) {
		Shift shift(date, shiftNum);
		return shift;
	}
private:
	DataStorageSingleton() : StorageSingleton() {
	}
};



void TestStorageSingleton::Init() {
	if (pStorageSingleton_s == nullptr) {
		pStorageSingleton_s = new TestStorageSingleton();
	}
	else {
		//ASSERT
		std::cerr << "Double Init error, last Init was for TestStorageSingleton";
	}
}

void DataStorageSingleton::Init() {
	if (pStorageSingleton_s == nullptr) {
		pStorageSingleton_s = new DataStorageSingleton();
	}
	else {
		//ASSERT
		std::cerr << "Double Init error, last Init was for DataStorageSingleton";
	}
}

int main(void) {
	TestStorageSingleton::Init();
	//DataStorageSingleton::Init();
	QDate date;
	StorageSingleton::GetInstance()->GetShift(date, EShiftNum::I);
	StorageSingleton::GetInstance()->Free();
	return 0;
}