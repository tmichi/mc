#include "mc.hpp"
#include <iostream>
#include <fstream>
int main ( void ) {
	// size 
	int sx = 10; 
	int sy = 10;
	int sz = 10;
	// center 
	int cx = 5; 
	int cy = 5;
	int cz = 5;
	// create volume data 
	std::vector<std::vector<std::vector<float> > > volume;
	for( int z = 0 ; z < sz ; ++z ) {
		volume.push_back( std::vector<std::vector<float> >() );
		for( int y = 0 ; y < sy ; ++y ) {
			volume[z].push_back( std::vector<float>() );
			for( int x = 0 ; x < sx ; ++x ) {
				float value = std::sqrt( ( x - cx ) * ( x - cx ) + ( y - cy ) * ( y - cy ) + ( z - cz ) * ( z - cz ) );
				volume[z][y].push_back( value );
			}
		}
	}
	
	// build mesh
	double isovalue = 3.3;
	std::vector<Point3f> pnts;
	for( int z = 0 ; z < sz - 1 ; ++z ) {
		for( int y = 0 ; y < sy - 1 ; ++y ) {
			for( int x = 0 ; x < sx -1 ; ++x ) {
				std::vector<std::pair<Point3f, double> > cell;
				for( int dz = 0 ; dz < 2; ++dz ) {
					for( int dy = 0 ; dy < 2 ; ++dy ) {
						for( int dx = 0 ; dx < 2 ; ++dx ) {
							Point3f p;
							p.x  = x + dx;
							p.y  = y + dy;
							p.z  = z + dz;
							double value = volume[p.z][p.y][p.x];
							cell.push_back ( std::make_pair( p, value) );
						}
					}
				}							
				polygonize(cell, isovalue, pnts ) ;
			}
		}
	}
	
	// create stl
	std::ofstream fout("sphere.stl", std::ios::binary);
	char ch[80];
	fout.write ( ch, 80 );
	unsigned int nf = pnts.size() / 3;
	fout.write ( (char*)(&nf), 4 );
	for( size_t i = 0 ; i < pnts.size() ; i += 3) {

		Point3f v0, v1, n;
		v0.x = pnts[i+1].x - pnts[i].x;
		v0.y = pnts[i+1].y - pnts[i].y;
		v0.z = pnts[i+1].z - pnts[i].z;
		v1.x = pnts[i+2].x - pnts[i].x;
		v1.y = pnts[i+2].y - pnts[i].y;
		v1.z = pnts[i+2].z - pnts[i].z;
		
		n.x = v0.y * v1.z - v0.z * v1.y;
		n.y = v0.z * v1.x - v0.x * v1.z;
		n.z = v0.x * v1.y - v0.y * v1.x;
		float l = std::sqrt( n.x * n.x + n.y * n.y + n.z * n.z );
		n.x /= l;
		n.y /= l;
		n.z /= l;
		
		fout.write((char*)(&(n.x)), 4 );
		fout.write((char*)(&(n.y)), 4 );
		fout.write((char*)(&(n.z)), 4 );

		fout.write((char*)(&(pnts[i].x)), 4 );
		fout.write((char*)(&(pnts[i].y)), 4 );
		fout.write((char*)(&(pnts[i].z)), 4 );

		fout.write((char*)(&(pnts[i+1].x)), 4 );
		fout.write((char*)(&(pnts[i+1].y)), 4 );
		fout.write((char*)(&(pnts[i+1].z)), 4 );

		fout.write((char*)(&(pnts[i+2].x)), 4 );
		fout.write((char*)(&(pnts[i+2].y)), 4 );
		fout.write((char*)(&(pnts[i+2].z)), 4 );

		unsigned short s = 0x0000;
		fout.write ( ( char* )( &s ), 2 );
	}
	std::cerr<<nf<<"triangles were saved to sphere.stl."<<std::endl;
	return 0;
}
