/********************************************************************
*                                                                   *
*                                                                   *
*	Author: vparadas                                            *
*	Comments: vparadas@cc.uoi.gr                                *
*		  evangelos.paradas@cern.ch                         *
*                                                                   *
*	Description: This program reads a txt file                  *
*		     and converts two columns to rows.              *
*		     To execute it you need to                      *
*		     give three arguments: a) path to txt file      *
*					   b) first column          *
*					   c) second column         *
*     Results are going to be written to "linearized.txt"           *
********************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

int main(int argc, char* argv[])
{
	ifstream inf(argv[1]);
	string date = argv[1];
	
	ofstream outfs[3];
	outfs[0].open("linearized_1.txt", ios::out | ios::app);
	outfs[1].open("linearized_2.txt", ios::out | ios::app);
	outfs[2].open("linearized_3.txt", ios::out | ios::app);

	
	for(int i=0; i<3; i++)
		outfs[i] << argv[2] << "\t" << argv[3] << "\t" << argv[4] << "\t";

	int col_a = atoi( argv[5] );
	int col_b = atoi( argv[6] );

	char line[256];
	vector<float> col[3];
	
	char word[]="PRES";
        bool check = false;
        int pos[4];
        do{
           inf.getline(line,256,'\n');
           string temp = line;
           for(int i=0; i<4; i++)
                pos[i] = temp.find(word[i]);
           check = true;
           for(int i=3; i>0; i--)
           {
                if( fabs(pos[i] - pos[i-1]) != 1 )
                        check = false;
           }
        }while( !(check) );
	

	int col_i(1);

	float temp;
	do{
		inf >> temp;
		if ( col_i == 1 )
			col[0].push_back( temp );
		else if ( col_i == col_a )
			col[1].push_back( temp );
		else if ( col_i == col_b )
			col[2].push_back( temp );
		
		col_i++;
		if ( col_i > 16 )
			col_i = 1;
		
	}while(!inf.eof());
	
	
	for(int i=1; i<3; i++)
	{
		for(unsigned int j=0; j<col[i].size(); j++)
			outfs[ (int) col[0].at(j) - 1 ] << col[i].at(j) << "\t";
	}
	for(int i=0; i<3; i++)
	{ 
		outfs[i] << endl;
		outfs[i].close();
	}

	


return 0;
}
