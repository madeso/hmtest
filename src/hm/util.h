#pragma once


#include <string>
#include <vector>


std::string
loadFile(const std::string& pFileName);

float
randomSign();

float
randomRealWithoutSign();

float
randomReal();

void
splitString(char delim, const std::string& s, std::vector<std::string>* numbers);

std::string
trim(const std::string& s, const std::string& drop = " ");

float
toReal(const std::string& str);

int
toInt(const std::string& str);