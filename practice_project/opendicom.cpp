#include "opendicom.h"
#include <iostream>
#include <fstream>
#include <QtWidgets>
using namespace std;

opendicom::opendicom()
{

}

opendicom::~opendicom()
{

}
template<typename T>
std::ostream& write_typed_data(std::ostream& stream, const T& value) {
	return stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
}
template<typename T>
std::istream& read_typed_data(std::istream& stream, T& value) {
	return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}
int opendicom::open_dicom_files()
{
	int length = 0;
	char* buffer;
	//QByteArray* buffer;
	ofstream is("D://sample//data.png", ios::out|ios::binary);

	ifstream openFile("D://sample//FILE9.dcm", ios::binary);

	openFile.seekg(0, ios::end);
	length = openFile.tellg();//get length
	//openFile.seekg(0, ios::beg);
	int image_start_pos = (512 * 512);

	openFile.seekg(-image_start_pos, ios::end);
	// allocate memory:
	buffer = new char[image_start_pos];
	QPixmap m_img;
	
	// read data as a block:
	openFile.read(buffer, image_start_pos);
	string str;
	getline(openFile, str);
	//m_img.(buffer, image_start_pos);
	//is << str;
	is.write((const char*)buffer, image_start_pos);
	
	openFile.close();
	is.close();
	//cout.write(buffer, image_start_pos);
	
	//QImage i;
	delete[] buffer;
	return 0;

}
