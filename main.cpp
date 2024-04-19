#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Convert Matlab groundtruth data into specified output

// Enable ONLY ONE: front, top, or back view
//std::string cam_view = "front";
std::string cam_view = "back";
//std::string cam_view = "top";

/*  Given the frame number, name the output file. Naming convention :
	frame # --> file name
	frame 1 --> frame_0001
	frame 2 --> frame_0002
	...
	frame n --> frame_000n
 */

// Return index corresponding to class name (hands = 0, face = 1, eyes = 2, mouth = 3) based on index range
std::string get_label_name(int i) {

	if (cam_view == "top") {
		return "hands";
	}

	else if (i < 16) {
		return "hands";
	}

	else if (i < 24) {
		return "face";
	}

	else if (i < 32) {
		return "eyes";
	}

	else {
		return "mouth";
	}
}

// CSV structure is different between the front/back and top views. Set corresponding iteration count
int get_max_iterations() {

	if (cam_view == "front" || cam_view == "back") {
		return 40;
	}

	else {
		return 32;
	}
}

// Read input line, find label cell center, name and export txt file
void export_data(int frame_number, int input_line[40], std::ofstream& out_file, int set_number) {

	// Max number of columns from csv
	int iterations = get_max_iterations();

	/* Name of the label we're looking at. This program is for the front/back views
	* which have the following order: four hands, then two each of face, eyes, mouth */
	std::string class_name = "hands";

	// Vars to operate on.
	int x_input = 0;
	int y_input = 0;
	int length = 0;
	int height = 0;

	// Get label values in chunks of 4
	for (int i = 0; i < iterations; i += 4) {

		// Get input data
		x_input = input_line[i];
		y_input = input_line[i + 1];
		length = input_line[i + 2];
		height = input_line[i + 3];

		// Check which class label to use
		class_name = get_label_name(i);

		if (set_number == 5) {
			//Debug
			std::cout << x_input << "," << y_input << "," << length << "," << height << "," << frame_number << "," << class_name << "," << set_number << "," << cam_view << std::endl;
		}


		/*	Write label data to file. One line = one label.
		Format is: class_name x_center y_center length height
		Example: hands 0.12 0.42 0.36 0.24 */
		out_file << x_input << "," << y_input << "," << length << "," << height << "," << frame_number << "," << class_name << "," << set_number << "," << cam_view << std::endl;
	}
}

int main() {

	// Read input file. Change cam_view at beginning of file (depending on which view csv being converted)
	std::ifstream in_file;
	in_file.open(cam_view + ".csv");
	std::string cell;

	// Frame number (convert to output file name)
	int frame_number = 1;

	// Temp input array. Stores one line at a time
	int input_line[40] = { 0 };

	int set_number = 1;

	std::string file_name = "export.csv";

	// Create output file
	std::ofstream out_file;
	out_file.open(file_name);

	// Parse by line and then each word
	while (std::getline(in_file, cell)) {

		std::stringstream stream(cell);
		//DEBUG: print the whole row from csv
		std::cout << cell;

		// Index for input file
		int current_col = 0;

		while (std::getline(stream, cell, ',')) {
			input_line[current_col] = stoi(cell);
			current_col++;

		} // Input array now stores one full row from input file
		std::cout << std::endl;
		// Export label data to a text file
		export_data(frame_number, input_line, out_file, set_number);

		// Increment frame number (used to name export files)
		frame_number++;

		if (frame_number == 1031) {
			frame_number = 1;
			set_number++;
		}

		for (int i = 0; i < 40; i++) {
			input_line[i] = 0;
		}
	}

	in_file.close();
	out_file.close();
	return 0;
}