#include <iostream>
#include <fstream>
#include <json/json.h>

//<!> dependency on JsonCPP https://github.com/open-source-parsers/jsoncpp

using namespace std;

string filename = "player_scores.json";

int main()
{
	//open file
	Json::Value players_data_file;
	ifstream score_file;	
	try {

		score_file.open(filename, ifstream::binary);

		if (score_file.fail())
			cout << endl << "failed" << endl;

		score_file >> players_data_file;
	}catch (Json::Exception e)
	{
		cout << e.what();
	}


	//isolate scores subtree
	Json::Value players_data_scores;
	try
	{
		players_data_scores = players_data_file["all_players"];
	}
	catch (Json::Exception e)
	{
		cout << __LINE__ << _SPACE << e.what() << endl;
	}
	
	//create iterator
	int n;
	try
	{
		if (players_data_scores.isArray())
			n = players_data_scores.size();
		else
			throw string("wrong save file structure");
	} catch (string e)
	{
		cout << e << endl;
		return 1;
	}


	//read scores
	std::vector<std::pair<string, int>> player_scores;
	try
	{
		for (int i = 0; i < n; i++)
		{
			string stemp = players_data_scores[i]["name"].asString();
			int itemp = players_data_scores[i]["score"].asInt();
			player_scores.push_back(std::make_pair(stemp,itemp));
		}
	}
	catch (Json::Exception e)
	{
		cout << __LINE__ << _SPACE << e.what() << endl;
	}
	

	//display scores
	for (int i = 0; i < n; i++)
		cout << player_scores[i].first << "\t" << player_scores[i].second << endl;
	

	//create new entry
	Json::Value new_score;
	new_score["name"] = "new player";
	new_score["score"] = 999;
	

	//add new entry to scores
	try
	{
		players_data_file["all_players"].append(new_score);
	}catch (Json::Exception e)
	{
		cout << __LINE__ << e.what() << endl;
	}
	

	//overwrite file
	ofstream write_scores(filename, ios::binary | ios::trunc);
	write_scores << players_data_file;
	if (write_scores.fail())
		cout << "failed to write" << endl;
	
	return 0;
}