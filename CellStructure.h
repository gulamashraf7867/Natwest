//
//  
//                                  CellStructure.h
//
//
/*! \class Cell
this class is intended to encapsulate the notion of cells in a spreadsheet, if you write code to work
off this it will be cleaner and it also allows easy use of mixtures of strings and numbers for both output
and input

  a single cell is represented by a cell object which can be a number, a string or empty

  a matrix of cells is represented by a CellStructure object

  routines to convert to and from XLOPERs can be found in ExcelCellConversions (old code)

  .AsCellStructure can be used in the new code to create a cell structure 
  XlfOper s have a constructor that takes cell structures in (new code)

note the file CellStructure deliberately contains no information about Excel so that routines written off
cell structures could be interfaced with anything

  */
#ifndef CELL_STRUCTURE_H
#define CELL_STRUCTURE_H

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <VecMJ.h>
#include "serealizableInterface.h"

#ifdef _BOOST_SERIALIZATION
#include <boost/serialization/split_member.hpp>
#endif

class Cell
{

public:
    bool IsAString() const;
    bool IsANumber() const;
    bool IsEmpty() const;

    Cell(const std::string&);
    Cell(double Number);

    Cell();

    const std::string& StringValue() const;
    std::string StringValueLowerCase() const;
    double NumericValue() const;
	
	friend std::string& operator<<(std::string& output, const Cell& thisCell);
	friend Cell & operator>>(const std::string& input, Cell & thisCell);

private:

    bool IsString;
    bool IsNumber;
  
    std::string ValueAsString;
    double ValueAsNumeric;

#ifdef _BOOST_SERIALIZATION

	friend class boost::serialization::access;

	BOOST_SERIALIZATION_SPLIT_MEMBER()

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const {
		ar & IsString & IsNumber & ValueAsString & ValueAsNumeric;
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version) {
		ar & IsString & IsNumber & ValueAsString & ValueAsNumeric;
	}

#endif

};

class CellStructure : public serealizableObject
{
public:

	CellStructure();

    CellStructure(unsigned long rows, unsigned long columns);

	explicit CellStructure(const std::string& pathToXMLFile);
    
    const Cell& operator()(unsigned long i, unsigned long j) const;
    Cell& operator()(unsigned long i, unsigned long j);

	//friend CellStructure & operator>>(const std::vector<std::string>& input, CellStructure & thisCellStructure);
	//friend std::vector<std::string>& operator<<(std::vector<std::string>& output, const CellStructure& thisCellStructure);
	void operator<<(std::vector<std::string>& output) const override;
	void operator>>(const std::vector<std::string>& input) override;

    unsigned long RowsInStructure() const;
    unsigned long ColumnsInStructure() const;	

	friend std::string& operator<<(std::string& output, const Cell& thisCell);
	friend Cell & operator>>(const std::string& input, Cell & thisCell);

private:

    VecMJ<VecMJ<Cell> > Cells;
    unsigned long Rows;
    unsigned long Columns;

	void initFromXMLFile(const std::string& pathToXMLFile);

#ifdef _BOOST_SERIALIZATION

	friend class boost::serialization::access;

	BOOST_SERIALIZATION_SPLIT_MEMBER()

	template<class Archive>
	void save(Archive & ar, const unsigned int version) const {
		ar & Rows & Columns;

		for(unsigned long row = 0; row < Rows; row++)
		{
			for(unsigned long col = 0; col < Columns; col++)
			{
				ar & Cells[row][col];
			}
		}
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version) {
		ar & Rows & Columns;

		VecMJ<Cell> model(Columns);
		Cells = VecMJ<VecMJ<Cell> >(Rows,model);

		for(unsigned long row = 0; row < Rows; row++)
		{
			for(unsigned long col = 0; col < Columns; col++)
			{
				ar & Cells[row][col];
			}
		}
	}

#endif

};


CellStructure MergeCellStructures(const CellStructure& Top, const CellStructure& Bottom);

bool isNumeric( const char* pszInput);

#endif
