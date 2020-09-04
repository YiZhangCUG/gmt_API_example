#include "gmt.h"
#include "iostream"
#include "string"

int main(int argc, char *argv[])
{
	// Initiate a new GMT session
	void *API = GMT_Create_Session("gridding_example", 2U, 0, NULL);

	// Read table file from disk
	struct GMT_DATASET *D = reinterpret_cast<GMT_DATASET*>(
		GMT_Read_Data (API, GMT_IS_DATASET, GMT_IS_FILE, GMT_IS_PLP, 
			GMT_READ_NORMAL, NULL, "table.txt", NULL));

	// load dataset to a virtual file for gridding
	// data type of a GMT virtual file must be GMT_STR16
	char table_file[GMT_STR16] = {""};
	GMT_Open_VirtualFile(API, GMT_IS_DATASET, GMT_IS_PLP, GMT_IN, D, table_file);

	// create a virtual file to hold result
	char grid_file[GMT_STR16] = {""};
	GMT_Open_VirtualFile(API, GMT_IS_GRID, GMT_IS_SURFACE, GMT_OUT, NULL, grid_file);

	// prepare CMD arguments for the gridding module
	std::string table_file_str = table_file;
	std::string grid_file_str = grid_file;
	std::string args_gridding = "-R0/1000/0/1000 -I10 -D1 -St0.3 " + 
		table_file_str + " -G" + grid_file_str;
	// call the greenspline module for gridding
	GMT_Call_Module(API, "greenspline", GMT_MODULE_CMD, (char*) args_gridding.c_str());

	// get grid file from the virtual file for outputting
	struct GMT_GRID *G = reinterpret_cast<GMT_GRID*>(
		GMT_Read_VirtualFile(API, grid_file));

	// write the grid to file
	GMT_Write_Data(API, GMT_IS_GRID, GMT_IS_FILE, GMT_IS_SURFACE, GMT_READ_NORMAL, NULL,
		"table.nc", G);

	// close virtual files
	GMT_Close_VirtualFile (API, table_file);
	GMT_Close_VirtualFile (API, grid_file);

	// end the GMT session
	GMT_Destroy_Session(API);
	return 0;
}