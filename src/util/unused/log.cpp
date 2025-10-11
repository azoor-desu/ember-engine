/// \file
/// <pre>
/// <para>    <b>File:</b>    Logger.cpp</para>
/// <para>    <b>Authors:</b>    Loo Jun Yi (70%)</para>
/// <para>						Jan Tiong Shu Ling (30%)</para>
/// <para>    <b>Emails:</b>    tiong.j@digipen.edu</para>
/// <para>					    l.junyi@digipen.edu</para>
/// <para>    <b>Date:</b>    07/09/2022</para>
/// </pre>
/// <summary>
/// This file helps to handle the backend of logging, mainly handling the logging file.
/// Rest of logging/assertion logic can be found in Logger.h
/// \n All content (c) 2022 DigiPen Institute of Technology Singapore. All rights reserved.
/// </summary>


#include <fstream>
#include <iostream>
#include <string>
#include "Tools/Logger.h"

#include <filesystem> // to create folder if there is none.
#include <windows.h> // MAKE SURE TO INCLUDE Logger.h AT THE BOTTOM MOST OF ALL CPP FILES due to this guy.
#include <shlobj.h>
#include "Utils/WstringHelper.h"
const std::string SAVE_FOLDER_NAME = "LunaEngineData";
std::string path_str;
namespace LE {
	// Gets the path to the folder to save stuff in.
	// If the folder is not created, it will gurantee the folder creation.
	// else, it will error.
	const std::string& GetSaveFolderPath() {
		if (path_str.length() > 0) return path_str;

		PWSTR path_ws;
		if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &path_ws) != S_OK) {
			CoTaskMemFree(path_ws);
			LE_ASSERT_MSG(false, "Unable to fetch AppData folder!");
			return path_str;
		}

		path_str = WstrHelper::WstrToStr(path_ws);
		CoTaskMemFree(path_ws);

		// Append folder name.
		path_str += '/';
		path_str += SAVE_FOLDER_NAME;
		path_str += '/';

		if (!std::filesystem::exists(path_str) && !std::filesystem::create_directory(path_str)) {
			LE_ASSERT_MSG(false, "Unable to create folder in: %s", path_str.c_str());
		}
		return path_str;
	}
}

std::ofstream log_file; // file to log to.
std::stringstream logData; //stream to log data too for the debug window
int totalRows = 0; // number of rows in the stringstream

namespace Debug {

		//-------------------------------------------------------------------//
		//							Logging System							 //
		//-------------------------------------------------------------------//

	// to convert enum into string.
	const char* levelStrings[6] = {"[FATAL]: ", "[ERROR]: ", "[WARN]:  ", "[INFO]:  ", "[DEBUG]: ", "[TRACE]: "};

	// Tries to get/create a log_file. Will do nothing if log_file is already opened.
	// Runs one time before logging anything.
	inline void OpenLogFile() {
		if (log_file.is_open()) return;

		// Log file is not open, create one.
		std::string filepath = LE::GetSaveFolderPath();
		filepath += "/LunaEngine.log";

		log_file.open(filepath, std::ios::app);
		//note that the time provided is UTC
		//log_file << "[New Log Created: "  << std::chrono::system_clock::now() << "]" << std::endl;
		log_file << "[New Log Created]" << std::endl;
		if (!log_file) {
			std::cout << "[ERROR] Log file unable to be created! THis error is not logged!";
		}
	}

	void ExitLogger() {
		if (log_file.is_open()) {
			log_file << "[End Log]" << std::endl << std::endl;
			log_file.close();
		}
	}

	/// <summary>
	/// Prints out the level of the error, and the corresponding description to the log file
	/// NOTE: only %d, %c, %f, %lf, %s are currently supported. 
	/// NOTE: Passing of %s should be a const * char
	/// </summary>
	/// <param name="level">the severity of the error, with there being 6 levels</param>
	/// <param name="message">a description about the error</param>
	void _LogOutput(LogLevel level, const char* message, ...) {

		OpenLogFile();

		logData << levelStrings[static_cast<int>(level)];
		log_file << levelStrings[static_cast<int>(level)];
		std::cout << levelStrings[static_cast<int>(level)];

		va_list argp;
		va_start(argp, message);

		//reads the message provided and parses through it, acting like a knockoff scanf
		for (int i = 0; message[i] != '\0'; i++) {
			if (message[i] == '%') {
				if (message[i + 1] == '%') { //checks for "%%" and skips it if that is the case
					log_file << message[i];
					logData << message[i];
					std::cout << message[i];
				}
				else {
					switch (message[i + 1]) {
						case 'd':
						{ //wrapped in {} to prevent errors
							int variable = va_arg(argp, int);
							log_file << variable;
							std::cout << variable;
							break;
						}
						case 'c':
						{
							int variable = va_arg(argp, int);
							log_file << static_cast<char>(variable);
							std::cout << static_cast<char>(variable);
							break;
						}
						case 'f':
						{
							double variable = va_arg(argp, double); //argument was promoted from float to double
							log_file << variable;
							std::cout << variable;
							break;
						}
						case 'l':
						{
							if (message[i + 2] == 'f') { //checking for %lf which is a double
								double variable = va_arg(argp, double);
								log_file << variable;
								std::cout << variable;
								i++;
								break;
							}
							else {
								std::cout << "(command unavailable)";
								i--;
							}
						}
						case 's':
						{
							char* variable = va_arg(argp, char*);
							log_file << variable;
							std::cout << variable;
							break;
						}
						default:
						{
							std::cout << "(command unavailable)";
							i--;
						}
					}
					i++; //increment to skip the variable since it has just been covered
				}
			}
			else {
				log_file << message[i];
				logData << message[i];
				std::cout << message[i];
			}
		}

		va_end(argp);

		log_file << std::endl;
		logData << std::endl;
		std::cout << std::endl;

		totalRows++;

		_EditorLogLimiter();

		//instead of using macros __FILE__ and __LINE__ can research source location && lamda functions
	}

	/// <summary>
	/// Prints out the level of the error, and the corresponding description to the log file
	/// NOTE: only %d, %c, %f, %lf, %s are currently supported. 
	/// NOTE: Passing of %s should be a const * char
	/// </summary>
	/// <param name="expression">the expression being checked</param>
	/// <param name="level">the severity of the error, with there being 6 levels</param>
	/// <param name="message">a description about the error</param>
	void _LogOutput(const char* expression,
								 LogLevel level,
								 const char* message, ...) {

		OpenLogFile();

		bool outputRequired = true; //only needed because low tier warnings will not be turned off for this macro
		if (level == LogLevel::LOG_WARN && _LOG_WARN_ENABLED == 0) {
			outputRequired = false;
		}
		else if (level == LogLevel::LOG_INFO && _LOG_INFO_ENABLED == 0) {
			outputRequired = false;
		}
		else if (level == LogLevel::LOG_DEBUG && _LOG_DEBUG_ENABLED == 0) {
			outputRequired = false;
		}
		else if (level == LogLevel::LOG_TRACE && _LOG_TRACE_ENABLED == 0) {
			outputRequired = false;
		}

		if (outputRequired == true) {

			logData << levelStrings[static_cast<int>(level)];
			log_file << levelStrings[static_cast<int>(level)] << "(Expression failure: " << expression << ") ";
			std::cout << levelStrings[static_cast<int>(level)];

			va_list argp;
			va_start(argp, message);

			//reads the message provided and parses through it, acting like a knockoff scanf
			for (int i = 0; message[i] != '\0'; i++) {
				if (message[i] == '%') {
					if (message[i + 1] == '%') { //checks for "%%" and skips it if that is the case
						log_file << message[i];
						logData << message[i];
						std::cout << message[i];
					}
					else {
						switch (message[i + 1]) {
							case 'd':
							{ //wrapped in {} to prevent errors
								int variable = va_arg(argp, int);
								log_file << variable;
								std::cout << variable;
								break;
							}
							case 'c':
							{
								int variable = va_arg(argp, int);
								log_file << static_cast<char>(variable);
								std::cout << static_cast<char>(variable);
								break;
							}
							case 'f':
							{
								double variable = va_arg(argp, double); //argument was promoted from float to double
								log_file << variable;
								std::cout << variable;
								break;
							}
							case 'l':
							{
								if (message[i + 2] == 'f') { //checking for %lf which is a double
									double variable = va_arg(argp, double);
									log_file << variable;
									std::cout << variable;
									i++;
									break;
								}
								else {
									std::cout << "(command unavailable)";
									i--;
								}
							}
							case 's':
							{
								char* variable = va_arg(argp, char*);
								log_file << variable;
								std::cout << variable;
								break;
							}
							default:
							{
								std::cout << "(command unavailable)";
								i--;
							}
						}
						i++; //increment to skip the variable since it has just been covered
					}
				}
				else {
					log_file << message[i];
					logData << message[i];
					std::cout << message[i];
				}
			}

			va_end(argp);

			log_file << std::endl;
			logData << std::endl;
			std::cout << std::endl;

			totalRows++;

			
		}

		_EditorLogLimiter();
	}

	void _EditorLogLimiter() {
		if (totalRows > 45) {
			std::string temp = logData.str();
			size_t pos = temp.find('\n');
			temp.erase(0, pos + 1);
			logData = std::stringstream();
			logData << temp;
			totalRows--;
		}
	}

		
		//-------------------------------------------------------------------//
		//						Assertion System							 //
		//-------------------------------------------------------------------//

#ifndef NDEBUG
	/// <summary>
	/// Used when an assertion fails to output to the log
	/// </summary>
	/// <param name="expression">the expression that was checked</param>
	/// <param name="message">any message that the programmer can choose to pass</param>
	/// <param name="file">the file where the assert failed</param>
	/// <param name="line">the line where the assert failed</param>
	void _ReportAssertionFailure(const char* expression,
											 const char* message,
											 const char* file,
											 int line) {

		_LogOutput(LogLevel::LOG_FATAL, "Assertion Failure: (%s), message: '%s', in file: %s, line: %d\n", expression, message, file, line);
	}
#endif

}
