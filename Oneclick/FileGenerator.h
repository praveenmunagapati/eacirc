#ifndef FILEGENERATOR_H
#define FILEGENERATOR_H

#include <iostream>
#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <fstream>

#include "XMLProcessor.h"
#include "ConfigParser.h"
#include "OneclickConstants.h"


class FileGenerator {
private:
	ConfigParser * parser;
public:
	/** Constructor for FileGenerator class. Inits ConfigParser class, then uses parsed
	  * info from XML file to generate config files for EACirc and perl scripts to upload
	  * and download them to BOINC server.
	  * @param path			path to XML config file for Oneclick.
	  */
	FileGenerator(std::string path);

	/** Destructor, deallocates ConfigParser class.
	  */
	~FileGenerator();
private:

	/** Generates XML configs specified in ConfigParser class, PERL script for uploading configs
	  * to BOINC server and script for downloading results.
	  * @throw runtime_error	if files can't be opened or written into
	  */
	void generateFiles();

	/** Opens file, reads it into string, closes file, returns string
	  * @param path				path to file
	  * @return					content of the file
	  * @throws runtime_error	when file can't be opened
	  */
	std::string readFileToString(std::string path);

	/** Opens file, loads string into it, closes it.
	  * Source's content is erased.
	  * @param path				path to file
	  * @param source			string to be saved
	  * @throws runtime_error	when file can't be opened
	  */
	void saveStringToFile(std::string path , std::string * source);

	/** Gets method prototype from string with loaded sample string. 
	  * It will match first occurence of methodName and copies rest 
	  * of the line(until DEFAULT_SCRIPT_LINE_SEPARATOR is found). 
	  * @param source				string with content of sample script
	  * @param methodName			typically one of KEYWORD_METHOD_... constants
	  * @return						prototype of the specified method
	  * @throws runtime_error		if methodName has no occurence in source
	  */
	std::string FileGenerator::getMethodPrototype(std::string source , std::string methodName);

	/** Replaces one string with another in target string.
	  * @param target				string in which changes will be made
	  * @param replace				substring to be replaced
	  * @param instead				with this string
	  * @throw runtime_error		if string replace has no occurence in target string
	  */
	void FileGenerator::replaceInString(std::string * target , std::string replace , std::string instead);

	/** Inserts toInsert into target string at position index.
	  * If position is at same position as methodPrototype, this string
	  * will be erased from target string first.
	  * @param target				target string
	  * @param methodPrototype		typicaly method prototype
	  * @param toInsert				method with completed arguments,
	  *								without keywords
	  * @param position				position where argument toInsert
	  *								will be inserted
	  * @return						new position - at the end of
	  *								inserted string in target string
	  */
	int FileGenerator::insertIntoScript(std::string * target , std::string methodPrototype , std::string toInsert , int position);
};

#endif //FILEGENERATOR_H