#include "dbSNP.h"

uint32_t Position::num = 0;

int main (int argc, char** argv) {
	readConfig("files/config");

	if (argc > 1) {
		if (readOptions(argc, argv) == -1)
			return -1;
	} else
		cout << "~~~Default Mode~~~" << endl << endl;

	if (!checkDependencies())
		return -1;

	map<string, pair<Position, string>> rsMap;
	for (int i=0; i<geneName.size(); i++)
		geneTask(rsMap, i);

//	printConfiguration();

//	string fileName = "../../scratch/dbSNP/AllSNPs142.txt";
//	string chrFileName;
//	string geneFileName;
//	makeNewFiles(fileName, chrFileName, geneFileName);

	// Make chromosome and gene specific files
//	makeFile(fileName, chrFileName, 'c');
//	makeFile(chrFileName, geneFileName, 'g');

//	map<string, pair<Position, string>> rsMap;
//	fillMap(rsMap, geneFileName);

	writeMap(rsMap, "files/rsMap_sav.tsv");

	cout << endl << "./dbSNPMod finished" << endl;
}

void geneTask(map<string, pair<Position, string>>& rsMap, const int& num) {
	printConfiguration(num);

	string fileName = "../../scratch/dbSNP/AllSNPs142.txt";
	string chrFileName;
	string geneFileName;
	makeNewFiles(fileName, chrFileName, geneFileName, num);

	// Make chromosome and gene specific files
	makeFile(fileName, chrFileName, 'c', num);
	makeFile(chrFileName, geneFileName, 'g', num);

	fillMap(rsMap, geneFileName);
}
