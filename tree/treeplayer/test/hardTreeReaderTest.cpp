#include "TFile.h"
#include "TTreeReader.h"
#include "TInterpreter.h"
#include "TSystem.h"
#include "MyParticle.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include <vector>
#include "A.h"
#include "B.h"

#ifdef __CINT__
#pragma link C++ class TTreeReaderValue<std::vector<MyParticle*>>+;
#pragma link C++ class TTreeReaderValue<MyParticle>+;
#pragma link C++ class TTreeReaderValue<Int_t>+;
#pragma link C++ class TTreeReaderValue<B>+;
#pragma link C++ class TTreeReaderValue<A>+;
#pragma link C++ class TTreeReaderValue<std::vector<B>>+;
#pragma link C++ class TTreeReaderValue<B*>+;
#pragma link C++ class TTreeReaderValue<TClonesArray>+;
#pragma link C++ class TTreeReaderValue<std::vector<B*>>+;
#pragma link C++ class TTreeReaderArray<double>+;
#pragma link C++ class TTreeReaderArray<B>+;
#pragma link C++ class TTreeReaderArray<Int_t>+;
#pragma link C++ class std::vector<B*>+;
#endif

#define TREE_ENTRIES 10
#define LIST_ENTRIES 10

#define NUM_CONSTANT 14
#define MULTIPLIER_B_OBJECT 1
#define MULTIPLIER_VECTOR_B 1
#define MULTIPLIER_VECTOR_B_STAR 3
#define MULTIPLIER_VECTOR_STAR_B 2
#define MULTIPLIER_B_STAR 2
#define MULTIPLIER_B_ARRAY 4
#define MULTIPLIER_B_STAR_ARRAY 5
#define MULTIPLIER_B_CLONES_ARRAY 6

void makeTree(){
	TFile *myFile = new TFile("HardTreeFile.root", "RECREATE");
	TTree *myTree = new TTree("HardTree", "This is hard to read");

	A myObject0 (NUM_CONSTANT);

	myTree->Branch("A0.",	"A", 	&myObject0,		32000, 0);
	myTree->Branch("A1.",	"A", 	&myObject0,		32000, 1);
	myTree->Branch("A99.",	"A", 	&myObject0,	32000, 99);
	myTree->Branch("A101.",	"A", 	&myObject0,	32000, 101);

	// myTree->Branch("vectorB_0.",	&branchForMembers.vectorB,	32000, 0);
	// myTree->Branch("vectorB_99.",	&branchForMembers.vectorB,	32000, 99);

	// myTree->Branch("vectorBStar_0.",	&branchForMembers.vectorBStar,	32000, 0);
	// myTree->Branch("vectorBStar_99.",	&branchForMembers.vectorBStar,	32000, 99);
	// myTree->Branch("vectorBStar_101.",	&branchForMembers.vectorBStar,	32000, 101);

	myTree->Branch("S0_vectorB.",	&myObject0.vectorB,	32000, 0);
	myTree->Branch("S99_vectorB.",	&myObject0.vectorB,	32000, 99);

	myTree->Branch("S0_vectorBStar.",	&myObject0.vectorBStar,	32000, 0);
	myTree->Branch("S99_vectorBStar.",	&myObject0.vectorBStar,	32000, 99);
	//myTree->Branch("vectorBStar_101.",	&branchForMembers.vectorBStar,	32000, 101); // Breaks Fill()


	for (int i = 1; i < TREE_ENTRIES + 1; ++i){
		printf("\nEntry %i\n\n", i);

		// Clear old values
		myObject0.ResetVectorB();

		printf("Setting BObject\n");
		myObject0.BObject.dummy = i;

		for (int j = 0; j < LIST_ENTRIES; ++j){
			// Vector of objects
			B obj (i*j);
			printf("Adding %i to vectorB\n", i*j);
			myObject0.AddToVectorB(obj);

			obj.dummy *= 2;
			// Hangs makeTree
			printf("Adding %i to vectorStarB\n",  obj.dummy);
			myObject0.AddToVectorStarB(obj);

			// Vector of pointers
			printf("Adding %i to vectorBStar\n", i*j*2);
			B *ptr = new B(i*j*3);
			myObject0.AddToVectorBStar(ptr);
		}

		printf("Setting BStar\n");
		B *objPtr = new B(i*2);
		myObject0.SetBStar(objPtr);

		printf("Filling BArray\n");
		B *myArray = new B[12];
		for (int j = 0; j < 12; ++j){
			myArray[j].dummy = i * j * 4;
		}
		myObject0.FillBArray(myArray);
		delete [] myArray;

		printf("Filling BStarArray\n");
		for (int j = 0; j < NUM_CONSTANT; ++j){
			myObject0.BStarArray[j] = i*j*5;
		}

		printf("Filling BClonesArray\n");
		for (int j = 0; j < LIST_ENTRIES; ++j ){
			((B*)myObject0.BClonesArray.New(j))->dummy = i*j*6;
		}

		printf("Filling tree\n");
		myTree->Fill();

		myObject0.ResetVectorStarB();

		myObject0.vectorBStar.clear();

		printf("Entry created\n");
	}
	printf("Tree created\n");

	myFile->Write();
}

void readNum(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "num";

	TTreeReaderValue<Int_t> myNum (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (testValues && *myNum != NUM_CONSTANT) success = false;
		if (printOut) printf("Num: %i\n", *myNum);
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBObject(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BObject";

	TTreeReaderValue<B> myBObject (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (testValues && myBObject->dummy != i * MULTIPLIER_B_OBJECT) success = false;
		if (printOut) printf("Dummy: %i\n", myBObject->dummy);
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBObjectDummy(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BObject.dummy";

	TTreeReaderValue<Int_t> myDummy (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (testValues && *myDummy != i * MULTIPLIER_B_OBJECT) success = false;
		if (printOut) printf("Dummy: %i\n", *myDummy);
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBStar(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BStar";

	TTreeReaderValue<B> myBStar (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (testValues && myBStar->dummy != i * MULTIPLIER_B_STAR) success = false;
		if (printOut) printf("Dummy: %i\n", myBStar->dummy);
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readVectorBValue(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "vectorB";

	TTreeReaderValue<std::vector<B> > myVectorB (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorB dummies:");

		for (int j = 0; j < LIST_ENTRIES; ++j){
			if (testValues && myVectorB->at(j).dummy != i * j * MULTIPLIER_VECTOR_B) success = false;
			if (printOut) printf(" %i", myVectorB->at(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readVectorStarBValue(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "vectorStarB";

	TTreeReaderValue<std::vector<B> > myVectorStarB (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorB dummies(%i):", myVectorStarB->size());

		for (int j = 0; j < LIST_ENTRIES; ++j){
			if (testValues && myVectorStarB->at(j).dummy != i * j * MULTIPLIER_VECTOR_STAR_B) success = false;
			if (printOut) printf(" %i", myVectorStarB->at(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readVectorStarBArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "vectorStarB";

	TTreeReaderArray<B> myVectorStarB (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorB dummies(%i):", myVectorStarB.GetSize());

		for (int j = 0; j < LIST_ENTRIES; ++j){
			if (testValues && myVectorStarB.At(j).dummy != i * j * MULTIPLIER_VECTOR_STAR_B) success = false;
			if (printOut) printf(" %i", myVectorStarB.At(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readVectorBArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "vectorB";

	TTreeReaderArray<B> myVectorB (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorB dummies(%i):", myVectorB.GetSize());

		for (int j = 0; j < LIST_ENTRIES && j < myVectorB.GetSize(); ++j){
			if (testValues && myVectorB.At(j).dummy != i * j * MULTIPLIER_VECTOR_B) success = false;
			if (printOut) printf(" %i", myVectorB.At(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BArray[12]";

	TTreeReaderArray<B> myBArray (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("BArray dummies(%i):", myBArray.GetSize());

		for (int j = 0; j < LIST_ENTRIES; ++j){
			if (testValues && myBArray.At(j).dummy != i * j * MULTIPLIER_B_ARRAY) success = false;
			if (printOut) printf(" %i", myBArray.At(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBStarArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BStarArray";

	TTreeReaderArray<B> myBStarArray (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("BStarArray dummies(%i):", myBStarArray.GetSize());

		for (int j = 0; j < myBStarArray.GetSize(); ++j){
			if (testValues && myBStarArray.At(j).dummy != i * j * MULTIPLIER_B_STAR_ARRAY) success = false;
			if (printOut) printf(" %i", myBStarArray.At(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readVectorBStarValue(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "vectorBStar";

	TTreeReaderValue<std::vector<B*> > myVectorBStar (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorBStar dummies(%i):", myVectorBStar->size());

		for (int j = 0; j < LIST_ENTRIES && j < myVectorBStar->size(); ++j){
			if (testValues && myVectorBStar->at(j)->dummy != i * j * MULTIPLIER_VECTOR_B_STAR) success = false;
			if (printOut) printf(" %i", myVectorBStar->at(j)->dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readVectorBStarArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "vectorBStar";

	TTreeReaderArray<B> myVectorBStar (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorBStar dummies(%i):", myVectorBStar.GetSize());

		for (int j = 0; j < LIST_ENTRIES && myVectorBStar.GetSize(); ++j){
			if (testValues && myVectorBStar.At(j).dummy != i * j * MULTIPLIER_VECTOR_B_STAR) success = false;
			if (printOut) printf(" %i", myVectorBStar.At(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBClonesArrayValue(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BClonesArray";

	TTreeReaderValue<TClonesArray> myBClonesArray (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("BClonesArray dummies(%i):", myBClonesArray->GetSize());

		for (int j = 0; j < LIST_ENTRIES && j < myBClonesArray->GetSize(); ++j){
			if (testValues && ((B*)myBClonesArray->At(j))->dummy != i * j * MULTIPLIER_B_CLONES_ARRAY) success = false;
			if (printOut) printf(" %i", ((B*)myBClonesArray->At(j))->dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBClonesArrayArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BClonesArray";

	TTreeReaderArray<B> myBClonesArray (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("BClonesArray dummies(%i):", myBClonesArray.GetSize());

		for (int j = 0; j < LIST_ENTRIES && j < myBClonesArray.GetSize(); ++j){
			if (testValues && myBClonesArray.At(j).dummy != i * j * MULTIPLIER_B_CLONES_ARRAY) success = false;
			if (printOut) printf(" %i", myBClonesArray.At(j).dummy);
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readVectorBDummyArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "vectorB.dummy";

	TTreeReaderArray<Int_t> myVectorBDummyArray (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorB.dummies(%i):", myVectorBDummyArray.GetSize());

		for (int j = 0; j < LIST_ENTRIES && j < myVectorBDummyArray.GetSize(); ++j){
			if (testValues && myVectorBDummyArray.At(j) != i * j * MULTIPLIER_VECTOR_B) success = false;
			if (printOut) printf(" %i", myVectorBDummyArray.At(j));
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

void readBClonesArrayDummyArray(const char* branchName = "A99.", Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = branchName;
	finalBranchName += "BClonesArray.dummy";

	TTreeReaderArray<Int_t> myBClonesArrayDummy (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("BClonesArray.dummies(%i):", myBClonesArrayDummy.GetSize());

		for (int j = 0; j < LIST_ENTRIES && j < myBClonesArrayDummy.GetSize(); ++j){
			if (testValues && myBClonesArrayDummy.At(j) != i * j * MULTIPLIER_B_CLONES_ARRAY) success = false;
			if (printOut) printf(" %i", myBClonesArrayDummy.At(j));
		}

		if (printOut) printf("\n");
	}
	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}

// void readArrayOfBs(const char *branchName, const char *subBranchPostFix, Bool_t printOut = true, Bool_t testValues = false){
// 	TFile *myFile = TFile::Open("HardTreeFile.root");
// 	TTreeReader myTreeReader ("HardTree");

// 	Int_t multiplier;
// 	if (!strcmp(subBranchPostFix))

// 	TString finalBranchName = branchName;
// 	finalBranchName += subBranchPostFix;

// 	TTreeReaderArray<Int_t> myArrayOfBs (myTreeReader, finalBranchName);

// 	Bool_t success = true;
// 	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
// 		if (printOut) printf("%s(%i):", subBranchPostFix, myArrayOfBs.GetSize());

// 		for (int j = 0; j < LIST_ENTRIES && i < myArrayOfBs.GetSize(); ++j){
// 			if (testValues && myArrayOfBs.At(j) != i * j * MULTIPLIER_B_CLONES_ARRAY) success = false;
// 			if (printOut) printf(" %i", myArrayOfBs.At(j));
// 		}

// 		if (printOut) printf("\n");
// 	}
// 	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
// }

void readAObject(const char* branchName = "A99."){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TTreeReaderValue<A> myAObject (myTreeReader, branchName);

	while (myTreeReader.SetNextEntry()){
		printf("Num: %i ", myAObject->num);
		printf("BDummy: %i ", myAObject->BObject.dummy);
		printf("BStarDummy: %i\n", myAObject->BStar->dummy);
	}
}

void readBranchVectorBValue(const char* splitLevel, Bool_t printOut = true, Bool_t testValues = false){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTreeReader myTreeReader ("HardTree");

	TString finalBranchName = "vectorB_";
	finalBranchName += splitLevel;

	TTreeReaderValue<std::vector<B> > myObject (myTreeReader, finalBranchName);

	Bool_t success = true;
	for (int i = 1; myTreeReader.SetNextEntry(); ++i){
		if (printOut) printf("vectorB dummies:");

		for (int j = 0; j < myObject->size(); ++j){
			if (testValues && myObject->at(j).dummy != i * j * MULTIPLIER_VECTOR_B) success = false;
			if (printOut) printf(" %i", myObject->at(j).dummy);
		}

		if (printOut) printf("\n");
	}

	if (testValues) printf("%s\n", success ? "Success!" : "Failure");
}


void readTree(){
	TFile *myFile = TFile::Open("HardTreeFile.root");
	TTree *myTree = (TTree*)myFile->Get("HardTree");
	myTree->Print();
	
	for (int i = 0; i < 10 && i < myTree->GetEntries(); ++i){
		myTree->Show(i);
	}

	myFile->Close();
}


void testAll(){
	printf("A1: readNum(): ---------------------------- "); readNum(					"A1.", false, true);
	printf("A1: readBObject(): ------------------------ "); readBObject(				"A1.", false, true);
	printf("A1: readBStar(): -------------------------- "); readBStar(					"A1.", false, true);
	printf("A1: readVectorBValue(): ------------------- "); readVectorBValue(			"A1.", false, true);
	printf("A1: readVectorStarBValue(): --------------- "); readVectorStarBValue(		"A1.", false, true);
	printf("A1: readVectorStarBArray(): --------------- "); readVectorStarBArray(		"A1.", false, true);
	printf("A1: readVectorBArray(): ------------------- "); readVectorBArray(			"A1.", false, true);
	printf("A1: readBArray(): ------------------------- "); readBArray(					"A1.", false, true);
	printf("A1: readBStarArray(): --------------------- "); readBStarArray(				"A1.", false, true);
	printf("A1: readVectorBStarValue(): --------------- "); readVectorBStarValue(		"A1.", false, true);
	printf("A1: readVectorBStarArray(): --------------- "); readVectorBStarArray(		"A1.", false, true);
	printf("A1: readBClonesArrayValue(): -------------- "); readBClonesArrayValue(		"A1.", false, true);
	printf("A1: readBClonesArrayArray(): -------------- "); readBClonesArrayArray(		"A1.", false, true);

	printf("A99: readNum(): ---------------------------- "); readNum(					"A99.", false, true);
	printf("A99: readBObject(): ------------------------ "); readBObject(				"A99.", false, true);
	printf("A99: readBObjectDummy(): ------------------- "); readBObjectDummy(			"A99.", false, true);
	printf("A99: readBStar(): -------------------------- "); readBStar(					"A99.", false, true);
	printf("A99: readVectorBValue(): ------------------- "); readVectorBValue(			"A99.", false, true);
	printf("A99: readVectorStarBValue(): --------------- "); readVectorStarBValue(		"A99.", false, true);
	printf("A99: readVectorStarBArray(): --------------- "); readVectorStarBArray(		"A99.", false, true);
	printf("A99: readVectorBArray(): ------------------- "); readVectorBArray(			"A99.", false, true);
	printf("A99: readBArray(): ------------------------- "); readBArray(				"A99.", false, true);
	printf("A99: readBStarArray(): --------------------- "); readBStarArray(			"A99.", false, true);
	printf("A99: readVectorBStarValue(): --------------- "); readVectorBStarValue(		"A99.", false, true);
	printf("A99: readVectorBStarArray(): --------------- "); readVectorBStarArray(		"A99.", false, true);
	printf("A99: readBClonesArrayValue(): -------------- "); readBClonesArrayValue(		"A99.", false, true);
	printf("A99: readBClonesArrayArray(): -------------- "); readBClonesArrayArray(		"A99.", false, true);
	printf("A99: readVectorBDummyArray(): -------------- "); readVectorBDummyArray(		"A99.", false, true);
	printf("A99: readBClonesArrayDummyArray(): --------- "); readBClonesArrayDummyArray("A99.", false, true);
	//printf("readAObject(): ------------------------ "); readAObject();

	printf("A101: readNum(): ---------------------------- "); readNum(						"A101.", false, true);
	printf("A101: readBObject(): ------------------------ "); readBObject(					"A101.", false, true);
	printf("A101: readBStar(): -------------------------- "); readBStar(					"A101.", false, true);
	printf("A101: readVectorBValue(): ------------------- "); readVectorBValue(				"A101.", false, true);
	printf("A101: readVectorStarBValue(): --------------- "); readVectorStarBValue(			"A101.", false, true);
	printf("A101: readVectorStarBArray(): --------------- "); readVectorStarBArray(			"A101.", false, true);
	printf("A101: readVectorBArray(): ------------------- "); readVectorBArray(				"A101.", false, true);
	printf("A101: readBArray(): ------------------------- "); readBArray(					"A101.", false, true);
	printf("A101: readBStarArray(): --------------------- "); readBStarArray(				"A101.", false, true);
	//printf("A101: readVectorBStarValue(): --------------- "); readVectorBStarValue(			"A101.", false, true); // Segfault
	//printf("A101: readVectorBStarArray(): --------------- "); readVectorBStarArray(			"A101.", false, true); // Segfault
	printf("A101: readBClonesArrayValue(): -------------- "); readBClonesArrayValue(		"A101.", false, true);
	printf("A101: readBClonesArrayArray(): -------------- "); readBClonesArrayArray(		"A101.", false, true);

	printf("vectorB_0: readBranchVectorBValue(): -------- "); readVectorBValue(		"S0_", 	false, true);
	// printf("vectorB_0: readBranchVectorBArray(): -------- "); readBranchVectorBArray(		"S0_", 	false, true);
}

void printAll(){
	printf("A1: readNum(): ---------------------------- \n"); readNum(					"A1.", true, true);
	printf("A1: readBObject(): ------------------------ \n"); readBObject(				"A1.", true, true);
	printf("A1: readBStar(): -------------------------- \n"); readBStar(				"A1.", true, true);
	printf("A1: readVectorBValue(): ------------------- \n"); readVectorBValue(			"A1.", true, true);
	printf("A1: readVectorStarBValue(): --------------- \n"); readVectorStarBValue(		"A1.", true, true);
	printf("A1: readVectorStarBArray(): --------------- \n"); readVectorStarBArray(		"A1.", true, true);
	printf("A1: readVectorBArray(): ------------------- \n"); readVectorBArray(			"A1.", true, true);
	printf("A1: readBArray(): ------------------------- \n"); readBArray(				"A1.", true, true);
	printf("A1: readBStarArray(): --------------------- \n"); readBStarArray(			"A1.", true, true);
	printf("A1: readVectorBStarValue(): --------------- \n"); readVectorBStarValue(		"A1.", true, true);
	printf("A1: readVectorBStarArray(): --------------- \n"); readVectorBStarArray(		"A1.", true, true);
	printf("A1: readBClonesArrayValue(): -------------- \n"); readBClonesArrayValue(	"A1.", true, true);
	printf("A1: readBClonesArrayArray(): -------------- \n"); readBClonesArrayArray(	"A1.", true, true);

	printf("A99: readNum(): ---------------------------- \n"); readNum(						"A99.", true, true);
	printf("A99: readBObject(): ------------------------ \n"); readBObject(					"A99.", true, true);
	printf("A99: readBObjectDummy(): ------------------- \n"); readBObjectDummy(			"A99.", true, true);
	printf("A99: readBStar(): -------------------------- \n"); readBStar(					"A99.", true, true);
	printf("A99: readVectorBValue(): ------------------- \n"); readVectorBValue(			"A99.", true, true);
	printf("A99: readVectorStarBValue(): --------------- \n"); readVectorStarBValue(		"A99.", true, true);
	printf("A99: readVectorStarBArray(): --------------- \n"); readVectorStarBArray(		"A99.", true, true);
	printf("A99: readVectorBArray(): ------------------- \n"); readVectorBArray(			"A99.", true, true);
	printf("A99: readBArray(): ------------------------- \n"); readBArray(					"A99.", true, true);
	printf("A99: readBStarArray(): --------------------- \n"); readBStarArray(				"A99.", true, true);
	printf("A99: readVectorBStarValue(): --------------- \n"); readVectorBStarValue(		"A99.", true, true);
	printf("A99: readVectorBStarArray(): --------------- \n"); readVectorBStarArray(		"A99.", true, true);
	printf("A99: readBClonesArrayValue(): -------------- \n"); readBClonesArrayValue(		"A99.", true, true);
	printf("A99: readBClonesArrayArray(): -------------- \n"); readBClonesArrayArray(		"A99.", true, true);
	printf("A99: readVectorBDummyArray(): -------------- \n"); readVectorBDummyArray(		"A99.", true, true);
	printf("A99: readBClonesArrayDummyArray(): --------- \n"); readBClonesArrayDummyArray(	"A99.", true, true);
	//printf("readAObject(): ------------------------ \n"); readAObject();

	printf("A101: readNum(): ---------------------------- \n"); readNum(					"A101.", true, true);
	printf("A101: readBObject(): ------------------------ \n"); readBObject(				"A101.", true, true);
	printf("A101: readBStar(): -------------------------- \n"); readBStar(					"A101.", true, true);
	printf("A101: readVectorBValue(): ------------------- \n"); readVectorBValue(			"A101.", true, true);
	printf("A101: readVectorStarBValue(): --------------- \n"); readVectorStarBValue(		"A101.", true, true);
	printf("A101: readVectorStarBArray(): --------------- \n"); readVectorStarBArray(		"A101.", true, true);
	printf("A101: readVectorBArray(): ------------------- \n"); readVectorBArray(			"A101.", true, true);
	printf("A101: readBArray(): ------------------------- \n"); readBArray(					"A101.", true, true);
	printf("A101: readBStarArray(): --------------------- \n"); readBStarArray(				"A101.", true, true);
	//printf("A101: readVectorBStarValue(): --------------- \n"); readVectorBStarValue(			"A101.", true, true); // Segfault
	//printf("A101: readVectorBStarArray(): --------------- \n"); readVectorBStarArray(			"A101.", true, true); // Segfault
	printf("A101: readBClonesArrayValue(): -------------- \n"); readBClonesArrayValue(		"A101.", true, true);
	printf("A101: readBClonesArrayArray(): -------------- \n"); readBClonesArrayArray(		"A101.", true, true);

	printf("vectorB_0: readBranchVectorBValue(): -------- \n"); readVectorBValue(		"S0_", 	true, true);
	// printf("vectorB_0: readBranchVectorBArray(): -------- \n"); readVectorBArray(		"S0_", 	true, true);
}
