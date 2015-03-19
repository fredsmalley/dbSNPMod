#include "functions.h"

int readOptions(int& argc, char** &argv) {
	int c;

	while(true) {
		static struct option options[] = {
			{"chr_remake", no_argument, &chr_remake, 1},
			{"gene_remake", no_argument, &gene_remake, 1},
			{"gene", required_argument, 0, 'g'},
			{"chromosome", required_argument, 0, 'c'},
			{"gene_start", required_argument, 0, 's'},
			{"gene_end", required_argument, 0, 'e'},
			{0, 0, 0, 0}
		};

		int option_index = 0;
		c = getopt_long(argc, argv, "g:c:s:e:", options, &option_index);

		if (c == -1)
			break;

		switch(c) {
		case 0:
		case '?':
			break;
		case 'g':
			geneName.push_back(optarg);
			break;
		case 'c':
			chromosome.push_back(optarg);
			break;
		case 's':
			geneStart.push_back(atoi(optarg));
			break;
		case 'e':
			geneEnd.push_back(atoi(optarg));
			break;
		default:
			return -1;
		}
	}

	for (int i=0; i<geneName.size(); i++) {
		if (chr_remake || gene_remake)
			cout << endl;
		if (chr_remake)
			cout << "!!!!!Forcing remake of " << chromosome[i] << " specific file!!!!!" << endl;
		if (gene_remake)
			cout << "!!!!!Forcing remake of " << geneName[i] << " specific file!!!!!" << endl;
		cout << endl;
	}
}

void readConfig(const string& fileName) {
	struct stat buffer;
	string value;
	string parameter;

	fstream file;
	if (stat(fileName.c_str(), &buffer) == 0) {
		file.open(fileName, fstream::in);

		while(getline(file, value)) {
			if (value.length() == 0 || value[0] == '#')
				continue;

			parameter = value.substr(0, value.find(":"));
			while(isspace(parameter[parameter.size() - 1]))
				parameter = parameter.substr(0, parameter.size() - 1);

			value = value.substr(value.find(":") + 1);
			while(isspace(value[0]))
				value = value.substr(1);

			if (parameter.compare("gene") == 0)
				geneName.push_back(value);
			else if (parameter.compare("chromosome") == 0)
				chromosome.push_back(value);
			else if (parameter.compare("start") == 0)
				geneStart.push_back(stoi(value));
			else if (parameter.compare("end") == 0)
				geneEnd.push_back(stoi(value));
			else {
				cout << "Unknown parameter " << parameter << endl;
				break;
			}
		}
		file.close();
	} else
		cout << fileName << " does not exist." << endl;
}

bool checkDependencies() {
	// check for uneveness
	int numberOfGenes = geneName.size();
	if (numberOfGenes != chromosome.size() ||
			numberOfGenes != geneStart.size() ||
			numberOfGenes != geneEnd.size()) {
		cout << "Unable to match up Gene information correctly" << endl
			<< "Make sure to have all groups in config filled" << endl;
		return false;
	}

	bool ready = true;
	if (geneName.size() == 0) {
		cout << "Set name of gene with:\n"
			<< "1: Option `--gene' or `-g'\n"
			<< "2: Config entry `gene : {value}'\n";
		ready = false;
	}
	if (chromosome.size() == 0) {
		cout << "Set chromosome with:\n"
			<< "1: Option `--chromosome' or `-c'\n"
			<< "2: Config entry `chromosome : {value}'\n";
		ready = false;
	}
	if (geneStart.size() == 0) {
		cout << "Set the start of the gene with:\n"
			<< "1: Option `--gene_start` or `-s'\n"
			<< "2: Config entry `start : {value}'\n";
		ready = false;
	}
	if (geneEnd.size() == 0) {
		cout << "Set the end of the gene with:\n"
			<< "1: Option `--gene_end' or `-e'\n"
			<< "2: Config entry `end : {value}'\n";
		ready = false;
	}

	return ready;
}

void printConfiguration(const int& num) {
	cout << geneName[num] << ":\tchromosome = " << chromosome[num] << endl;
	cout << "\tgene start = " << geneStart[num] << ", "
		<< "gene end = " << geneEnd[num] << endl << endl;
}

void makeNewFiles(const string& fileName, string& chrFileName, string& geneFileName, const int& num) {
	chrFileName = fileName.substr(fileName.find("All"));
	chrFileName = chrFileName.substr(0, chrFileName.find(".txt"));
	chrFileName = chrFileName.substr(chrFileName.find("SNP"));
	geneFileName  = "files/" + geneName[num] + chrFileName + ".tsv";
	chrFileName = "files/" + chromosome[num] + chrFileName + ".tsv"; 
}

int makeFile(const string& fileName, const string& newFileName, const char& mode, const int& num) {
	int* flag;
	string* section;
	int (*find)(const string&, const string&, const int&);
	switch (mode) {
	case 'c':
		flag = &chr_remake;
		section = &chromosome[num];
		find = &findChr;
		break;
	case 'g':
		flag = &gene_remake;
		section = &geneName[num];
		find = &findGene;
		break;
	default:
		cerr << "!!!Unknown mode " << mode << "!!!" << endl;
		return -1;
	}

	if (exists(newFileName) && !(*flag))
		cout << newFileName << " already exists." << endl;
	else {
		if (!(*flag))
			cout << newFileName << " does not exist, writting..." << endl;

		if (find(fileName, newFileName, num) == 0) {
			cout << "Found " << *section << " in " << fileName
				<< " and saved to " << newFileName << endl;
		} else
			return -1;
	}

	return 0;
}

bool exists(const string& file) {
	struct stat buffer;

	if (stat(file.c_str(), &buffer) == 0)
		return true;
	else
		return false;
}

int findChr(const string& fileName, const string& newFileName, const int& num) {
	string nextChr = "chr" + to_string(stoi(chromosome[num].substr(3)) + 1);

	fstream inFile;
	fstream outFile;

	string line;
	string sline;
	string part;

	inFile.open(fileName, fstream::in);					// open large file for reading	
	if (inFile.is_open()) {
		outFile.open(newFileName, fstream::out);			// open new file for writting
		if (outFile.is_open()) {
			getline(inFile, line);
			outFile << line << endl;				// save titles
			while(getline(inFile, line)) {				// loop for each line in large file
				sline = line.substr(line.find("\t") + 1);	// skip bin
				part = sline.substr(0, sline.find("\t"));	// get chromosome
				if (part.compare(chromosome[num]) == 0)	
					outFile << line << endl;		// save line if chromosome
				else if (part.compare(nextChr) == 0)
					break;					// break out if next chromosome
			}

			outFile.close();
		} else {
			cerr << "Unable to open " << newFileName << endl;
			return 1;
		}
		inFile.close();
	} else {
		cerr << "Unable to open " << fileName << endl;
		return -1;
	}

	return 0;
}

int findGene(const string& fileName, const string& newFileName, const int& num) {
	fstream inFile;
	fstream outFile;

	string line;
	string sline;
	unsigned int begin;
	unsigned int end;

	inFile.open(fileName, fstream::in);
	if (inFile.is_open()) {
		outFile.open(newFileName, fstream::out);
		if (outFile.is_open()) {
			getline(inFile, line);
			outFile << line << endl;					// save titles
			while (getline(inFile, line)) {
				sline = line.substr(line.find("\t") + 1);		// skip bin
				sline = sline.substr(sline.find("\t") + 1);		// skip chromosome
				begin = stoi(sline.substr(0, sline.find("\t")));	// save begin
				sline = sline.substr(sline.find("\t") + 1);
				end = stoi(sline.substr(0, sline.find("\t")));		// save end
				if (begin >= geneStart[num] && end <= geneEnd[num])
					outFile << line << endl;			// write line to outFile
				else
					continue;
			}
		} else {
			cerr << "Unable to open " << newFileName << endl;
			return -1;
		}
	} else {
		cerr << "Unable to open " << fileName << endl;
		return -1;
	}

	return 0;
}

void fillMap(map<string, pair<Position, string>>& rsMap, const string& fileName) {
	unsigned int begin;
	unsigned int end;
	string line;
	string rsID;

	fstream inFile;
	inFile.open(fileName);
	if (inFile.is_open()) {
		getline(inFile, line);					// skip titles
		while (getline(inFile, line)) {
			line = line.substr(line.find("\t")+ 1);		// skip bin
			line = line.substr(line.find("\t") + 1);	// skip chromosome
			begin = stoi(line.substr(0, line.find("\t")));	// save begin
			line = line.substr(line.find("\t") + 1);
			end = stoi(line.substr(0, line.find("\t")));	// save end
			line = line.substr(line.find("\t") + 1);
			rsID = line.substr(0, line.find("\t"));		// save rsID
			line = line.substr(line.find("\t") + 1);

			line = line.substr(line.find("\t") + 1);	// skip score
			line = line.substr(line.find("\t") + 1);	// skip strand
			line = line.substr(0, line.find("\t"));		// save ref

			rsMap.insert(pair<string, pair<Position, string>>(rsID, pair<Position, string>(Position(begin, end, false), line)));
		}
	} else
		cerr << "Unable to open " << fileName << endl;
}

void writeMap(const map<string, pair<Position, string>>& rsMap, const string& fileName) {
	bool toScreen = false;
	fstream outFile;
	if (fileName.length() == 0)
		toScreen = true;
	else {
		outFile.open(fileName, fstream::out);
		if(!outFile.is_open()) {
			cerr << "Unable to open " << fileName << endl;
			return;
		}
	}

	for (map<string, pair<Position, string>>::const_iterator itr = rsMap.begin();
			itr != rsMap.end(); itr++)
		if (toScreen)
			cout << itr->first << ":\t(" << itr->second.first.getBegin() << "," << itr->second.first.getEnd() << ")" << itr->second.second << endl; 
		else
			outFile << itr->first << ":\t(" << itr->second.first.getBegin() << "," << itr->second.first.getEnd() << ")" << itr->second.second << endl;
}
