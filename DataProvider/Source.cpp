#include<string>
#include<vector>
#include<iostream>

#define TAPE_COUNT 6
using namespace std;

enum class  EShiftNum {
	I = 1,
	II,
	III,
	IV
};


enum class  EPost {
	WORKER,
	SHIFT_LEADER,
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
	Employee(size_t id, string fullName, EPost post, EStatus status) : id(id), fullName(fullName), post(post), status(status) {

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
	ProdTape() {
		id = 255;
		name = "empty";
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

	vector<Employee>& GetWorkers() {
		return workers;
	}

	size_t CountWorkers() {
		return workers.size();
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
	Shift(QDate date, EShiftNum shiftNum, size_t shiftLeader) : date(date), shiftNum(shiftNum), idShiftLeader(shiftLeader) {

	}

	void SetTapes(ProdTape* newTapes) {
		for (size_t i = 0; i != 6; ++i) {
			tapes[i] = newTapes[i];
		}
	}

	void SetTape(ProdTape& newTape) {
		tapes[newTape.GetId()] = newTape;
	}

	//void AddTape() {
		//фигня какая-то, зачем нам этот метод?!
	//}

	const ProdTape* GetTapes() {
		return tapes;
	}

	ProdTape& GetTape(size_t id) {
		return tapes[id];
	}

private:
	size_t idShiftLeader;
	ProdTape tapes[TAPE_COUNT];
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

	vector<Employee> GetFreeWorkers(QDate date, EShiftNum shiftNum) {//todo

		vector<Employee> freeWorkers;

		for (int j = 0; j != 10; ++j)
			freeWorkers.push_back(Employee(j, std::to_string(j) + "_worker", EPost::WORKER, EStatus::FREE));

		return freeWorkers;
	}

	Shift GetShift(QDate date, EShiftNum shiftNum) {//todo
		Employee leader(239, "Vasia Pupkin", EPost::SHIFT_LEADER, EStatus::ONPLACE);

		Shift shift(date, shiftNum, leader.GetId());

		for (int i = 0; i != TAPE_COUNT; ++i) {

			ProdTape tape(i, std::to_string(i));

			for (int j = 0; j != 30; ++j)
				tape.AddWorker(Employee(j, std::to_string(j) + "_worker", EPost::WORKER, EStatus::ONPLACE));
			shift.SetTape(tape);
		}
		return shift;
	}

private:
	TestStorageSingleton() : StorageSingleton() {
	}
};


//class DataStorageSingleton : StorageSingleton {
//public:
//	DataStorageSingleton(DataStorageSingleton& other) = delete;
//	void operator=(const DataStorageSingleton&) = delete;
//	static void Init();
//	
//	vector<Employee> GetFreeWorkers(QDate date, EShiftNum shiftNum) {
//		vector<Employee> freeWorkers;
//		return freeWorkers;
//	}
//
//	Shift& GetShift(QDate date, EShiftNum shiftNum) {
//		Shift shift(date, shiftNum,);
//		return shift;
//	}
//private:
//	DataStorageSingleton() : StorageSingleton() {
//	}
//};

void TestStorageSingleton::Init() {
	if (pStorageSingleton_s == nullptr) {
		pStorageSingleton_s = new TestStorageSingleton();
	}
	else {
		//ASSERT
		std::cerr << "Double Init error, last Init was for TestStorageSingleton";
	}
}

//void DataStorageSingleton::Init() {
//	if (pStorageSingleton_s == nullptr) {
//		pStorageSingleton_s = new DataStorageSingleton();
//	}
//	else {
//		//ASSERT
//		std::cerr << "Double Init error, last Init was for DataStorageSingleton";
//	}
//}

int main(void) {
	TestStorageSingleton::Init();
	//DataStorageSingleton::Init();				   
	QDate date;

	Shift shift = StorageSingleton::GetInstance()->GetShift(date, EShiftNum::I);

	for (int i = 0; i != TAPE_COUNT; i++) {
		cout << "Tape " << i << endl;
		for (int j = 0; j < shift.GetTape(i).CountWorkers(); j++) {
			cout << "\t" << (shift.GetTape(i).GetWorkers()[j]).GetFullName() << endl;
		}
	}

	vector<Employee> freeWorkers = StorageSingleton::GetInstance()->GetFreeWorkers(date, EShiftNum::I);

	cout << "\n" << "Free workers" << endl;
	for (Employee freeWorker : freeWorkers) {
		cout << "\t" << freeWorker.GetFullName() << endl;
	}

	StorageSingleton::GetInstance()->Free();
	return 0;
}