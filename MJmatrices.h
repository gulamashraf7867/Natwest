//
//
//                          MJmatrices.h
//
//
/*
    a simple matrix class
    it has been kept so that its interface is minimal to facilitate interchange
    with other matrix classes


  */
#ifndef MJMATRICES_H
#define MJMATRICES_H


#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#include <algorithm>
#define NUMBER_OF_MATRIX_ROWS_TO_VIEW 100

#ifdef _BOOST_SERIALIZATION
#include <boost/serialization/split_member.hpp>
#endif

class MJMatrix
{
public:

    explicit MJMatrix(unsigned long Rows_=0, unsigned long Cols_=0);
    MJMatrix(const MJMatrix& original);

    MJMatrix& operator=(const MJMatrix& original);
    
    ~MJMatrix();
    
    inline const double& operator()(unsigned long i, unsigned long j) const;
    inline double& operator()(unsigned long i, unsigned long j);

    inline double* operator[](unsigned long i);
    inline const double* const operator[](unsigned long i) const;

    inline unsigned long rows() const;
    inline unsigned long columns() const;

    inline MJMatrix& operator+=(const MJMatrix& addend);
	inline MJMatrix& operator-=(const MJMatrix& addend);

    MJMatrix& resize(unsigned long rows, unsigned long columns);

	MJMatrix* clone() const;

#if _MSC_VER >= 1600 || (!defined _MSC_VER)
	// Move semantics unless we're still using vc7
	MJMatrix(MJMatrix&& temp_matrix);
	MJMatrix& operator=(MJMatrix&& temp_matrix);
#endif

	void swap(MJMatrix& other);
 
private:
    unsigned long Rows;
    unsigned long Columns;
    double** RowStarts;
    double* Start;

    void Create();
    void Create(unsigned long rows, unsigned long cols);
#ifdef _DEBUG
	typedef union { double _v32[32]; double _v120[120]; double _v360[360]; double _v720[720]; } DoubleDebugView;
	mutable DoubleDebugView *MatrixView[NUMBER_OF_MATRIX_ROWS_TO_VIEW];
	void UpdateViews()const;
#endif

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
				ar & RowStarts[row][col];
			}
		}
	}

	template<class Archive>
	void load(Archive & ar, const unsigned int version) {
		ar & Rows & Columns;
		Create();
		for(unsigned long row = 0; row < Rows; row++)
		{
			for(unsigned long col = 0; col < Columns; col++)
			{
				ar & RowStarts[row][col];
			}
		}
	}

#endif


};

inline const double& MJMatrix::operator()(unsigned long i, unsigned long j) const
{
#ifdef _DEBUG
if (i >= Rows || j >= Columns)
    throw("index out of bounds");
#endif
    return RowStarts[i][j];
}

inline double& MJMatrix::operator()(unsigned long i, unsigned long j)
{
#ifdef _DEBUG
if (i >= Rows || j >= Columns)
    throw("index out of bounds");
#endif

    return RowStarts[i][j];
}

inline const double* const MJMatrix::operator[](unsigned long i) const
{
#ifdef _DEBUG
if (i >= Rows )
    throw("index out of bounds");
#endif

    return RowStarts[i];
}



inline double*  MJMatrix::operator[](unsigned long i)
{
#ifdef _DEBUG
if (i >= Rows )
    throw("index out of bounds");
#endif

    return RowStarts[i];
}

inline unsigned long MJMatrix::rows() const
{
    return Rows;
}

inline unsigned long MJMatrix::columns() const
{
    return Columns;
}   


inline MJMatrix& MJMatrix::operator+=(const MJMatrix& addend)
{
#ifdef _DEBUG
    if (addend.rows() != rows() || addend.columns() != columns())
        throw("bad addition");
#endif

    double* i = Start;
    double *j = addend.Start;
    double* end = Start+Rows*Columns;
    while (i != end)
    {
        *i += *j;
        ++i;
        ++j;
    }

    return *this;

}

inline MJMatrix& MJMatrix::operator-=(const MJMatrix& addend)
{
#ifdef _DEBUG
    if (addend.rows() != rows() || addend.columns() != columns())
        throw("bad addition");
#endif

    double* i = Start;
    double *j = addend.Start;
    double* end = Start+Rows*Columns;
    while (i != end)
    {
        *i -= *j;
        ++i;
        ++j;
    }

    return *this;

}

inline void MJMatrix::swap(MJMatrix& other)
{
	using std::swap;

	swap(Rows,      other.Rows);
	swap(Columns,   other.Columns);
	swap(RowStarts, other.RowStarts);
	swap(Start,     other.Start);
	
#ifdef _DEBUG
	UpdateViews();
	other.UpdateViews();
#endif
}

inline void swap(MJMatrix& first, MJMatrix& second)
{
	first.swap(second);
}

#if _MSC_VER >= 1600 || (!defined _MSC_VER)
inline MJMatrix::MJMatrix(MJMatrix&& temp_matrix)
{
	Rows      = temp_matrix.Rows;
	Columns   = temp_matrix.Columns;
	RowStarts = temp_matrix.RowStarts;
	Start     = temp_matrix.Start;

	// make temp_matrix a valid 0x0 MJMatrix
	temp_matrix.Rows      = 0;
	temp_matrix.Columns   = 0;
	temp_matrix.RowStarts = 0;
	temp_matrix.Start     = 0;

#ifdef _DEBUG
	UpdateViews();
	temp_matrix.UpdateViews();
#endif
}

inline MJMatrix& MJMatrix::operator=(MJMatrix&& temp_matrix)
{
	MJMatrix my_temp(std::move(temp_matrix));
	swap(my_temp);

	return *this;
}
#endif

#endif // MJMATRICES_H
