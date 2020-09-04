#include "gmt.h"
#include "stdio.h"
#include "unistd.h"
#include "iostream"
#include "string"

int main(int argc, char *argv[])
{
	std::string cpt_file = "user.cpt";
	std::string ps_file = "user.ps";

	// Initiate a new GMT session
	// you need to destroy the session later
	void *API = GMT_Create_Session("plot_grid_example", 2U, 0, NULL);

	struct GMT_GRID *G = reinterpret_cast<GMT_GRID*>(
		GMT_Read_Data (API, GMT_IS_GRID, GMT_IS_FILE, GMT_IS_SURFACE, 
			GMT_READ_NORMAL, NULL, "table.nc", NULL));

	// start the plotting
	// 1. set GMT defaults
	std::string args_defaults = "FONT_ANNOT_PRIMARY=10.5p,Times-Roman,black \
MAP_FRAME_PEN=thinnest,black \
MAP_GRID_PEN_PRIMARY=thinnest,black \
MAP_TICK_PEN_PRIMARY=thinnest,black \
MAP_TICK_LENGTH_PRIMARY=1p/0.5p \
MAP_TITLE_OFFSET=7.5p \
MAP_GRID_CROSS_SIZE_PRIMARY=2p \
FONT_LABEL=10.5p,Times-Roman,black \
MAP_LABEL_OFFSET=5p \
MAP_ANNOT_OFFSET_PRIMARY=2.5p";
	// call gmtset
	GMT_Call_Module (API, "gmtset", GMT_MODULE_CMD, (char*) args_defaults.c_str());

	// load the grid to a virtual file for plotting
	char grid_name[GMT_STR16] = {""};
	GMT_Open_VirtualFile (API, GMT_IS_GRID, GMT_IS_SURFACE, GMT_IN, G, grid_name);
	// get string type of grid name
	std::string grid_name_str = grid_name;

	// prepare cpt file
	std::string args_cpt = grid_name_str + " -Crainbow -R0/1000/0/1000 -Z -D ->" + cpt_file;
	// call grd2cpt
	GMT_Call_Module (API, "grd2cpt", GMT_MODULE_CMD, (char*) args_cpt.c_str());

	// plot the image
	std::string args_image = grid_name_str + " -R0/1000/0/1000 -C" + cpt_file + 
		" -Bxag+l\"x (m)\" -Byag+l\"y (m)\" -JX1.5i/1.5i -X1.5i -Y1.5i -K -P --MAP_FRAME_AXES=WesNZ ->" + ps_file;
	// call grdimage
	GMT_Call_Module (API, "grdimage", GMT_MODULE_CMD, (char*) args_image.c_str());

	// plot color bar
	std::string args_bar = "-Dx0.1i/-0.2i+w1.3i/0.05i+h -C" + cpt_file + 
		" -Bxa -By+lm -O -)" + ps_file;
	// call psscale
	GMT_Call_Module (API, "psscale", GMT_MODULE_CMD, (char*) args_bar.c_str());

	// convert ps file to raster formats
	std::string args_pic = ps_file + " -A -TEG -E300";
	// call psconvert
	GMT_Call_Module (API, "psconvert", GMT_MODULE_CMD, (char*) args_pic.c_str());

	// close virtual file
	GMT_Close_VirtualFile (API, grid_name);

	// destroy data container
	GMT_Destroy_Data(API, G);

	// end the GMT session
	GMT_Destroy_Session(API);

	// remove temporary files. including GMT defaults
	remove(cpt_file.c_str());
	remove(ps_file.c_str());
	// if exist, delete
	std::string hist_file = "gmt.history";
	std::string conf_file = "gmt.conf";
	if (!access(hist_file.c_str(), F_OK))
		remove(hist_file.c_str());
	if (!access(conf_file.c_str(), F_OK))
		remove(conf_file.c_str());

	return 0;
}