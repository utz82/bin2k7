#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char *argv[]){

	cout << "Z80 BIN TO HECTOR K7 CONVERTER\n";
	
	string infile, outfile;
	long dest = 0;
	long pc = 0;
	long stack = 0;
	bool verbose = false;
	bool finfile = false;
	bool fdest = false;
	
	for (int i = 1; i < argc; i++) {
	
		
		string arg = argv[i];
		if (arg == "-v") verbose = true;
		if (arg == "-i" && i < argc-1) {
			infile = argv[i+1];
			outfile = infile;
			finfile = true;
		}
		if (arg == "-d" && i < argc-1) {
			dest = stol(argv[i+1], NULL, 16);
			fdest = true;
		}
		if (arg == "-p" && i < argc-1) pc = stol(argv[i+1], NULL, 16);
		if (arg == "-s" && i < argc-1) stack = stol(argv[i+1], NULL, 16);
		if (arg == "-o" && i < argc-1) outfile = argv[i+1];
	}
	
	if (!finfile | !fdest) {
		cout << "Usage: bin2k7 -i <infile> -d <destination> [-p <program counter> -s <stack> -o <outfile>]\nAll values are to be given in hex.\n";
		return -1;
	}
	
	if (verbose) cout << "dest: " << dest << "\tpc: " << pc << "\tstack: " << stack << endl;
	
	
	ifstream BINFILE;
	BINFILE.open (infile.data(), ios::in | ios::binary);
	
	if (!BINFILE.is_open()) {
	
		cout << "Error: Could not open " << infile << endl;
		return -1;	
	}

	
	//read binfile into array
	BINFILE.seekg(0,ios_base::end);
	long filesize = BINFILE.tellg();
	
	char *bindata = new char[filesize];
	
	BINFILE.seekg(0, ios::beg);
	BINFILE.read(bindata, filesize);
	
	BINFILE.close();
	
	outfile += ".k7";
	ofstream CASS;
	CASS.open (outfile.data(), ios::out | ios::binary | ios::trunc);
	if (!CASS.is_open()) {
		cout << "Error: Could not create " << outfile << " - need to set write permission?\n";
		delete[] bindata;
		return -1;
	}
	
	
	//write program block
	const char ff = 0xff;
	char destlo = static_cast<char>(dest & 0xff);
	char desthi = static_cast<char>((dest/256) & 0xff);
	char lenlo = static_cast<char>(filesize & 0xff);
	char lenhi = static_cast<char>((filesize/256) & 0xff);

	CASS << '\5' << destlo << desthi << lenlo << lenhi << ff;
	long lmod = static_cast<long>(filesize % 256);
	long nblocks = static_cast<long>(filesize/256);
	if (verbose) cout << "writing " << nblocks << " 256b blocks and one " << +lmod << "b block\n";
	long fp = 0;

	for (long block = 0; block < nblocks; block++) {
		CASS << '\0';
		for (int i = 0; i < 256; i++) {

			CASS << bindata[fp];
			fp++;
		}
	}

	CASS << static_cast<unsigned char>(lmod);
	for (long j = 0; j < lmod; j++) {

		CASS << bindata[fp];
		fp++;
	}

	
	//write terminating block
	const char fd = 0xfd;
	char pclo = static_cast<char>(pc & 0xff);
	char pchi = static_cast<char>((pc/256) & 0xff);
	char stacklo = static_cast<char>(stack & 0xff);
	char stackhi = static_cast<char>((stack/256) & 0xff);
	
	CASS << '\5' << stacklo << stackhi << pclo << pchi << fd;
	CASS.close();
	delete[] bindata;
	return 0;
}