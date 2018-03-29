#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

int i;
int main(int argc, char *argv[]) {
	if (argc != 2) {
        std::cerr <<"Format Error!"<<std::endl;
		std::cerr << "Format: " << argv[0] << " <bin_file_name>" << std::endl;
		return 1;
	}

	const std::string file = argv[1];

	std::fstream input(file, std::ios_base::in | std::ios_base::binary);
	if (!input.is_open()) {
		std::cerr << "Input file Error" << std::endl;
		return 1;
	}

	std::string content((std::istreambuf_iterator<char>(input)), (std::istreambuf_iterator<char>()));
	if (content.length() == 0) {
		std::cerr << "Input file is empty" << std::endl;
		return 1;
	}

	std::fstream headerFile("ecu_iod_ble_binFile.c", std::ios_base::out);
	if (!headerFile.is_open()) {
		std::cerr << "C file ist not writable" << std::endl;
		return 1;
	}


	headerFile << "/* This C file is auto generated. */" << std::endl<<std::endl;
	headerFile << "#include \"ecu_iod_ble_sbllib.h\"" <<std::endl;
	headerFile << "#include \"ecu_iod_ble_serial.h\"" <<std::endl;
	headerFile << "#include \"ecu_iod_ble_binFile.h\""<<std::endl<<std::endl;



	headerFile << "const uint8_t fileBuffer[FILE_BUFFER_SIZE]={";
	{
		i = 0;
		for (auto it = content.begin(); it != content.end(); it++, i++) {
			if (i % 16 == 0)
				headerFile << std::endl << "\t";

			headerFile << "0x" << std::hex <<std::setfill('0') << std::setw(2) << std::uppercase << (((unsigned int)(*it)) & 0xFF);
			if (std::next(it) != content.end())
				headerFile << ", ";
		}
	}
	headerFile << std::endl << "};" << std::endl;
	std::cout<<"Convert completed."<<std::endl;
	std::cout<<"Total: "<<i<<" bytes."<<std::endl;
	return 0;
}
