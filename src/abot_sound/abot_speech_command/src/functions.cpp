#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>


VCSetNavigationGoal::VCSetNavigationGoal() {
	_stt_sub = _node.subscribe("/abot/stt/grammar_data", 1, &VCSetNavigationGoal::grammarCallback, this);
	_tts_pub = _node.advertise<std_msgs::String>("/abot/tts/text_to_say", 1);
}



std::vector<std::string> read_record(std::string idstr)
{
	ROS_INFO("%s", idstr.c_str());
	std::ifstream fin;
	//std::ofstream fout("answer.txt");
	fin.open("/home/anya/abot/ros/src/abot_sound/abot_speech_command/src/coordinates.csv", std::ios::in);

	int count = 0;

	std::vector<std::string> row;
	std::string line, word;

	std::getline(fin,line); // headers
	ROS_INFO("headers: %s", line.c_str());

	while (std::getline(fin,line)) {

		row.clear();
		std::stringstream s(line);

		while (getline(s, word, ',')) {
			row.push_back(word);
		}
		ROS_INFO("%s", row[0].c_str());
		
		if (row[0] == idstr) {

			count = 1;
			//fout.write(row[1].data(), row[1].size());
			//fout.close();
			return row;
		}
	}
	if (count == 0){
		row.push_back("Нет такой аудитории\n");
		return row;
	}
}


void VCSetNavigationGoal::grammarCallback(const std_msgs::String::ConstPtr& text_msg) {
	std::ofstream fout("answer.txt");
	//std::stringstream strm = text_msg->data.c_str();
	//const char* grammar_string = text_msg->data.c_str();
	//strm >> grammar_string;
	//ROS_INFO("%s", grammar_string);
	std_msgs::String answer_msg;
	//fout.write(grammar_string.data(), grammar_string.size());
	const char* grammar_string = "dcedee89";
	std::vector<std::string> row = read_record(grammar_string);
	//fout.write(grammar_string.data(), grammar_string.size());
	if(row.size() == 1){
		fout.close();
		answer_msg.data = row[0];
		_tts_pub.publish(answer_msg);
		return ;
	}
	fout << "Details of id " + row[0] + " : \n" + "Name: " + row[1] + "\n" + "x: " + row[2] + "\n" + "y: " + row[3] + "\n";
	fout.close();
	//ROS_INFO("Name: " + row[1] + "\n");
	answer_msg.data = row[1];
	_tts_pub.publish(answer_msg);
	//ROS_INFO("x: " + row[2] + "\n");
	//ROS_INFO("y: " + row[3] + "\n");
}


/*
int main(int argc, char **argv) {
	const char* grammar_string = "dcedee89";
	std::vector<std::string> row = read_record(grammar_string);
	std::cout << row[1] << std::endl;
	return 0;
}
*/
