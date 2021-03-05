#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <algorithm>
#include <sstream>

using namespace std;

void CheckMap()
{
    ifstream inFile1;
    ifstream inFile2;

    inFile1.open("map_out_1.dat"); //my map
    inFile2.open("Map_Yingying_layer1.dat"); //Yingying's map

    map < string , int > My_map;
    map < string , int > Yingying_map;

    string line;
    int Strip;
    string FEE, ALTRO, Channel;
    string tag;
    int nCounts = 0;

    while (inFile1.good() && !inFile1.eof() )
    {
        getline( inFile1, line );
        if ( !line.size() ) continue;// ignore empty lines
        if ( line[0] == 'F' ) continue; //ignore title
        istringstream sstr(line);
        sstr >> FEE >> ALTRO >> Channel >> Strip;
        if (inFile1.eof()) break;
        tag = FEE+" "+ALTRO+" "+Channel;
        My_map[tag] = Strip;
        nCounts++;
    }

    while (inFile2.good() && !inFile2.eof() )
    {
        getline( inFile2, line );
        if ( !line.size() ) continue;// ignore empty lines
        if ( line[0] == 'F' ) continue; //ignore title
        istringstream sstr(line);
        sstr >> FEE >> ALTRO >> Channel >> Strip;
        if (inFile2.eof()) break;
        tag = FEE+" "+ALTRO+" "+Channel;
        Yingying_map[tag] = Strip;
    }

    for ( auto const &ent1 : My_map )
    {
        if ( Yingying_map[ ent1.first ] != ent1.second ) 
        {
            cout << "Map : " << ent1.first << " can not match Yingying's map!!!! Please check it " << endl;
            cout << " YingYing's map : " << Yingying_map[ ent1.first ] << " My map : " << ent1.second << endl;
        }
            
    }
    
}