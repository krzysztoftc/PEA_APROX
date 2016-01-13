/*
 * Edge.cpp
 *
 *  Created on: 1 maj 2015
 *      Author: krzysztof
 */

#include "Edge.h"

Edge::Edge(){
	this -> begin = 0;
	this -> wage = 0;
	this -> end = 0;
}

Edge::Edge(int begin, int wage, int end){
	this -> begin = begin;
	this -> wage = wage;
	this -> end = end;
}

bool Edge::operator ==(const Edge &e){
	if (this -> begin == e.begin && this -> end == e.end)
		return true;
	return false;
}

Edge::~Edge(){

}
