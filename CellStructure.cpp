//
//
//                              CellStructure.cpp
//
//



#include <CellStructure.h>
#include <intermediate.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "Tokenize.h"
#include <fstream>

const char * stringSeperator = ":";
const char * lineSeperator = ";";

bool Cell::IsAString() const
{
    return IsString;
}

bool Cell::IsANumber() const
{
    return IsNumber;
}

bool Cell::IsEmpty() const
{
    return !IsString && !IsNumber;    
}

Cell::Cell(const std::string& input)
: IsString(true), IsNumber(false), ValueAsString(input), ValueAsNumeric(0.0)
{

}
    
Cell::Cell(double Number)
: IsString(false), IsNumber(true), ValueAsNumeric(Number)
{
    
}


Cell::Cell() : IsString(false), IsNumber(false),ValueAsNumeric(0.0)
{

}

const std::string& Cell::StringValue() const
{
    return ValueAsString;
}

std::string Cell::StringValueLowerCase() const
{
    std::string tmp(ValueAsString);

    std::transform(tmp.begin(),tmp.end(),tmp.begin(),tolower);

    return tmp;
}
  

double Cell::NumericValue() const
{
    return ValueAsNumeric;
}

std::string& operator<<(std::string& output, const Cell& thisCell)
{
	if(thisCell.IsANumber())
	{
		std::stringstream s; 		

		s << std::setprecision(17) << thisCell.NumericValue();
		output = s.str();
	}
	else if(thisCell.IsAString())
		output = thisCell.StringValue();

	return output;
}

Cell & operator>>(const std::string& input, Cell & thisCell)
{
	if(isNumeric(input.c_str())) {
		thisCell = Cell(atof(input.c_str()));
	} else {
		thisCell = Cell(input);
	}
	return thisCell;
}

CellStructure::CellStructure()
: Rows(0), Columns(0)
{
}


CellStructure::CellStructure(unsigned long rows, unsigned long columns)
: Rows(rows), Columns(columns)
{
    VecMJ<Cell> model(columns);
    Cells = VecMJ<VecMJ<Cell> >(rows,model);
}


CellStructure::CellStructure(const std::string& pathToXMLFile)
{
	//call init from XML file
	//check if result true
	//else raise error

	initFromXMLFile(pathToXMLFile);
}

void
CellStructure::initFromXMLFile(const std::string& pathToXMLFile)
{
	if (pathToXMLFile == "")
		throw "error initiating CellStructure from  pathToXMLFile empty: " + pathToXMLFile;

	try {
		std::ifstream fileStream;
		fileStream.open(pathToXMLFile);
		std::vector<std::string> theObjectInStr;

		if (fileStream.is_open() == false)
			throw("couldn't open file: " + pathToXMLFile);

		while (!fileStream.eof())
		{
			std::string line;
			fileStream >> line;
			theObjectInStr.push_back(line);
		}
		//we need not to deserialize directly with a cellstructure, but on an intermediate input. We dont do this: //(*this) >> theObjectInStr; 
		//IntermediateInput<MJArray, MJMatrix> theII;
		//theII >> theObjectInStr;
		//(*this) = ConvertToCellStructure(theII);
	    (*this) >> theObjectInStr;		fileStream.close();
	}
	catch (const char* s) {
		throw(s);
	}
	catch (std::string s) {
		throw(s);
	}
	catch (...) {
		throw("error initiating CellStructure from  pathToXMLFile: "+ pathToXMLFile);
	}
}


const Cell& CellStructure::operator()(unsigned long i, unsigned long j) const
{
    if (i>= Rows || j >= Columns)
        throw("attempt to access cell outside range");
    
    return Cells[i][j];
}

Cell& CellStructure::operator()(unsigned long i, unsigned long j)
{
    if (i>= Rows || j >= Columns)
        throw("attempt to access cell outside range");
    
    return Cells[i][j];
}

unsigned long CellStructure::RowsInStructure() const
{
    return Rows;
}

unsigned long CellStructure::ColumnsInStructure() const
{
    return Columns;
}

void CellStructure::operator>>(const std::vector<std::string>& input)
{
	unsigned long NumberOfRows = static_cast<unsigned long>(input.size());
	unsigned long NumberOfCols;

	VecMJ<VecMJ<Cell> > theCSCells;

	for(unsigned long row = 0; row < NumberOfRows; row++) {
		
		std::vector<std::string> theRowOfStr;

		Tokenize(input[row],theRowOfStr,',');

		 NumberOfCols = static_cast<unsigned long>(theRowOfStr.size());

		VecMJ<Cell> theRowOfCells(NumberOfCols);

		for(unsigned long col = 0; col < NumberOfCols; col++)
		{
			theRowOfStr[col] >> theRowOfCells[col] ;
		}

		theCSCells.push_back(theRowOfCells);
	}

	Cells = theCSCells;
	Rows = NumberOfRows;
	Columns = NumberOfCols;

}

//std::vector<std::string>& operator<<(std::vector<std::string>& output, const CellStructure& thisCellStructure)
//{
//	std::vector<std::string> theMatrixOfStr;
//
//	for(unsigned long row = 0; row < thisCellStructure.Rows; row++)
//	{
//		std::stringstream theRowOfStr;
//
//		for(unsigned long col = 0; col < thisCellStructure.Columns; col++)
//		{
//			std::string theCellStr;
//			theCellStr << thisCellStructure(row,col);
//			
//			theRowOfStr << theCellStr;
//
//			if(col < thisCellStructure.Columns-1)
//				theRowOfStr << ',';
//
//		}
//		theMatrixOfStr.push_back(theRowOfStr.str());
//	}	
//	output = theMatrixOfStr;
//
//	return output;
//}

void CellStructure::operator<<(std::vector<std::string>& output) const
{
	std::vector<std::string> theMatrixOfStr;

	for (unsigned long row = 0; row < Rows; row++)
	{
		std::stringstream theRowOfStr;

		for (unsigned long col = 0; col < Columns; col++)
		{
			std::string theCellStr;
			theCellStr << operator()(row, col);

			theRowOfStr << theCellStr;

			if (col < Columns - 1)
				theRowOfStr << ',';

		}
		theMatrixOfStr.push_back(theRowOfStr.str());
	}
	output = theMatrixOfStr;

}
// Auxiliary ======================
CellStructure MergeCellStructures(const CellStructure& Top, const CellStructure& Bottom)
{
    unsigned long r = Top.ColumnsInStructure() > Bottom.ColumnsInStructure() ? Top.ColumnsInStructure()
                                                                             : Bottom.ColumnsInStructure();
    CellStructure result(Top.RowsInStructure()+Bottom.RowsInStructure(), r);


    unsigned long i=0; 
    for (; i < Top.RowsInStructure(); i++)
        for (unsigned long j=0; j < Top.ColumnsInStructure(); j++)
            result(i,j) = Top(i,j);

 
    for (unsigned long l=0; l < Bottom.RowsInStructure(); l++, i++)
        for (unsigned long j=0; j < Bottom.ColumnsInStructure(); j++)
            result(i,j) = Bottom(l,j);

    return result;
}

bool isNumeric( const char* pszInput){	
	std::istringstream iss( pszInput ); 	
	double dTestSink;		
	iss >> dTestSink;	
	if ( ! iss )		
		return false; 	// was all the input successfully consumed/converted?	
	return ( iss.rdbuf()->in_avail() == 0 );
}
